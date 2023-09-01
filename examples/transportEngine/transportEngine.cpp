/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "gams.h"
#include "gamsengineconfiguration.h"
#include "gamsvariablerecord.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include <gamspath.h>

using namespace gams;
using namespace std;
using namespace std::string_literals;

static string getDataText()
{
    string data {
            R"(
Sets
        i   canning plants   / seattle, san-diego /
        j   markets          / new-york, chicago, topeka / ;

Parameters

        a(i)  capacity of plant i in cases
         /    seattle     350
              san-diego   600  /

        b(j)  demand at market j in cases
         /    new-york    325
              chicago     300
              topeka      275  / ;

Table d(i,j)  distance in thousands of miles
                      new-york       chicago      topeka
        seattle          2.5           1.7          1.8
        san-diego        2.5           1.8          1.4  ;

Scalar f      freight in dollars per case per thousand miles  / 90 /
       bmult  demand multiplier                               /  1 /;
            )"s
    };
    return data;
}

static string getModelText()
{
    string model {
            R"(
Sets
    i   canning plants
    j   markets;

Parameters
    a(i)   capacity of plant i in cases
    b(j)   demand at market j in cases
    d(i,j) distance in thousands of miles
Scalar f      freight in dollars per case per thousand miles
       bmult  demand multiplier;

$if not set gdxincname $abort 'no include file name for data file provided'
$gdxLoad %gdxincname% i j a b d f bmult

$echo test > test.txt

Parameter c(i,j)  transport cost in thousands of dollars per case ;
          c(i,j) = f * d(i,j) / 1000 ;

Variables
     x(i,j)  shipment quantities in cases
     z       total transportation costs in thousands of dollars ;

Positive Variable x ;

Equations
     cost        define objective function
     supply(i)   observe supply limit at plant i
     demand(j)   satisfy demand at market j ;

cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;
supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;
demand(j) ..   sum(i, x(i,j))  =g=  bmult*b(j) ;

Model transport /all/ ;

Solve transport using lp minimizing z ;

Scalar ms 'model status', ss 'solve status';

Display x.l, x.m ;
            )"s
    };
    return model;
}

/// \file transportEngine.cpp
/// \brief This is an example how to run a GAMSJob on GAMS Engine using the run_engine method.
///
/// Here we show:
///   - How to send along additional model files(besides the main file)
///   - How to provide additional Engine options
///   - How to use checkpoints with run_engine
///   - How to interrupt a GAMSJob running on GAMS Engine
///
int main(int argc, char* argv[])
{
    cout << "---------- Transport Engine --------------" << endl;

    GAMSWorkspaceInfo wsInfo;
    if (argc > 1)
        wsInfo.setSystemDirectory(argv[1]);
    GAMSWorkspace ws(wsInfo);

    filesystem::remove(ws.workingDirectory().append("/test.txt"));

    string envPrefix("ENGINE_");
    for (const string &id : {"URL", "USER", "PASSWORD", "NAMESPACE"}) {
        if(!getenv((envPrefix + id).c_str())) {
            cerr << "No ENGINE_" << id << " set" << endl;
            return -1;
        }
    }

    GAMSEngineConfiguration engineConf(getenv("ENGINE_URL"), getenv("ENGINE_USER"),
                                       getenv("ENGINE_PASSWORD"), getenv("ENGINE_NAMESPACE"));

    // run with data from a string with GAMS syntax with explicit export to GDX file
    GAMSJob jobData = ws.addJobFromString(getDataText());
    GAMSOptions defaultOptions = ws.addOptions();

    cout << "Run 1" << endl;
    jobData.runEngine(engineConf, &defaultOptions, nullptr, &cout);
    jobData.outDB().doExport(filesystem::absolute(ws.workingDirectory().append("/tdata.gdx")).string());

    map<string, double> expectedLevels = {  { "seattle.new-york", 0.0 },
                                            { "seattle.chicago", 300.0 },
                                            { "seattle.topeka", 0.0 },
                                            { "san-diego.new-york", 325.0 },
                                            { "san-diego.chicago", 0.0 },
                                            { "san-diego.topeka", 275.0 }
                                         };


    GAMSJob jobModel = ws.addJobFromString(getModelText());

    GAMSOptions opt = ws.addOptions();
    opt.setDefine("gdxincname", "tdata.gdx");
    opt.setAllModelTypes("xpress");

    // run a job using an instance of GAMSOptions that defines the data include file
    cout << "Run 2" << endl;
    jobModel.runEngine(engineConf, &opt, nullptr, &cout, {}, { "tdata.gdx" },
                            {
                                { "inex_string", "{\"type\": \"include\", \"files\": [\"*.gdx\"]}" }
                            });

    for (GAMSVariableRecord &rec : jobModel.outDB().getVariable("x")) {
        cout << "x(" << rec.key(0) << "," + rec.key(1) << "): level=" << rec.level()
             << "marginal=" << rec.marginal() << "\n";

        if (expectedLevels.at(rec.key(0).append(".").append(rec.key(1))) != rec.level()) {
            cout << "Unexpected result, expected level: "
                 << expectedLevels.at(rec.key(0).append(".").append(rec.key(1)))
                 << endl;
            return 1;
        }
    }

    if (filesystem::exists(filesystem::path(ws.workingDirectory()).append("test.txt"))) {
        cout << "Test.txt should not have sent back - inex_string failed" << endl;
        return 1;
    }

    // same but with implicit database communication
    GAMSCheckpoint cp = ws.addCheckpoint();
    {
        GAMSOptions opt = ws.addOptions();
        GAMSJob jobA = ws.addJobFromString(getDataText());
        GAMSJob jobB = ws.addJobFromString(getModelText());

        cout << "Run 3" << endl;
        jobA.runEngine(engineConf, &defaultOptions, nullptr, &cout);

        opt.setDefine("gdxincname", jobA.outDB().name());
        opt.setAllModelTypes("xpress");

        cout << "Run 4" << endl;
        jobB.runEngine(engineConf, &opt, &cp, &cout, vector<GAMSDatabase>{jobA.outDB()});

        for (GAMSVariableRecord &rec : jobB.outDB().getVariable("x")) {
            cout << "x(" << rec.key(0) << "," + rec.key(1) << "): level=" << rec.level()
                 << "marginal=" << rec.marginal() << "\n";

            if (expectedLevels.at(rec.key(0).append(".").append(rec.key(1))) != rec.level()) {
                cout << "Unexpected result, expected level: "
                     << expectedLevels.at(rec.key(0).append(".").append(rec.key(1)))
                     << endl;
                return 1;
            }
        }
        vector<map<string, double>> bmultExpected {
            {{ "bmult", 0.9 }, { "ms", 1 }, { "ss", 1 }, { "obj", 138.31 } },
            {{ "bmult", 1.2 }, { "ms", 4 }, { "ss", 1 }, { "obj", 184.41 } }
        };

        for (map<string, double> &m : bmultExpected) {
            GAMSJob tEbmult = ws.addJobFromString("bmult=" + to_string(m["bmult"])
                    + "; solve transport min z use lp; ms=transport.modelstat; "
                      "ss=transport.solvestat;", cp);
            try {
                cout << "Run 5" << endl;
                tEbmult.runEngine(engineConf, &defaultOptions, nullptr, &cout);
                cout << "Scenario bmult=" << to_string(m["bmult"]) << ":" << endl;
                cout << "  Modelstatus: " << tEbmult.outDB().getParameter("ms").firstRecord().value() << endl;
                cout << "  Solvestatus: " << tEbmult.outDB().getParameter("ss").firstRecord().value() << endl;
                cout << "  Obj        : " << tEbmult.outDB().getVariable("z").firstRecord().level() << endl;
                if (tEbmult.outDB().getParameter("bmult").firstRecord().value() != m["bmult"]) {
                    cout << "Unexpected input, expected bmult: " + to_string(m["bmult"]) << endl;
                    return 1;
                }
                if (tEbmult.outDB().getParameter("ms").firstRecord().value() != m["ms"]) {
                    cout << "Unexpected result, expected ms: " + to_string(m["ms"]);
                    return 1;
                }
                if (tEbmult.outDB().getParameter("ss").firstRecord().value() != m["ss"]) {
                    cout << "Unexpected result, expected ss: " + to_string(m["ss"]);
                    return 1;
                }
                if (fabs(tEbmult.outDB().getVariable("z").firstRecord().level() - m["obj"]) > 0.01) {
                    cout << "Unexpected result, expected obj: " + to_string(m["obj"]);
                    return 1;
                }

            }
            catch (exception ex) {
                cout << "Exception caught:" << ex.what() << endl;
                return 1;
            }
        }
    }

    // Example how to interrupt Engine Job
    GAMSJob jc = ws.addJobFromGamsLib("clad");

    // Define an option file for the solver to be used
    string optFile1Path = filesystem::path(ws.workingDirectory().append("/cplex.opt")).string();
    {
        ofstream optFile1(optFile1Path);

        // Set relative stopping tolerance to 0 initially
        optFile1 << "epgap 0" << endl;
        // Activate interactive option setting on interrupt
        optFile1 << "interactive 1" << endl;
        // Define new option file to read on interrupt
        optFile1 << "iafile cplex.op2" << endl;
    }

    // Write new Cplex option file
    string optFile2Path = filesystem::path(ws.workingDirectory().append("/cplex.op2")).string();
    {
        ofstream optFile2(filesystem::path(ws.workingDirectory().append("/cplex.op2")));
        optFile2.open(optFile2Path);
        optFile2 << "epgap 0.1" << endl;
        optFile2.close();
    }

    opt = ws.addOptions();

    opt.setMIP("cplex");
    opt.setOptFile(1);
    opt.setSolveLink(GAMSOptions::ESolveLink::LoadLibrary);

    cout << "Run 6" << endl;

    string logPath = filesystem::path(ws.workingDirectory().append("/ej.log")).string();
    {
        ofstream logFile(logPath);
        thread optThread(&GAMSJob::runEngine, &jc, engineConf, &opt, nullptr, &logFile,
                         vector<GAMSDatabase>(), set<string> {optFile1Path, optFile2Path, "claddat.gdx"},
                         unordered_map<string, string>(), true, true);

        while (filesystem::exists(logPath) && filesystem::is_empty(logPath))
            this_thread::sleep_for(500ms);

        jc.interrupt();
        cout << "Interrupted Cplex to continue with new option" << endl;

        if (optThread.joinable())
            optThread.join();
    }

    {
        string line;
        ifstream inLog(logPath);
        while (getline(inLog, line)) {
            if (line.find("Interrupted") != string::npos) {
                cout << "Interrupted succesfully" << endl;
                return 0;
            }
        }
        cout << "Expected the solver to be interrupted at least once." << endl;
        return 1;
    }

    cout << "Interrupted succesfully" << endl;

    return 0;
}
