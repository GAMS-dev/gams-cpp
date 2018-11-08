/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
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
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include "gams.h"

using namespace gams;
using namespace std;

static int status;
static string statusString;

/// Get model as string
string getModelText()
{
    return "$title Warehouse.gms                                                                    \n"
           "                                                                                        \n"
           "$eolcom //                                                                              \n"
           "$SetDDList warehouse store fixed disaggregate // acceptable defines                     \n"
           "$if not set warehouse    $set warehouse   10                                            \n"
           "$if not set store        $set store       50                                            \n"
           "$if not set fixed        $set fixed       20                                            \n"
           "$if not set disaggregate $set disaggregate 1 // indicator for tighter bigM constraint   \n"
           "$ife %store%<=%warehouse% $abort Increase number of stores (>%warehouse)                \n"
           "                                                                                        \n"
           "set res respond codes  / 0 Normal                                                       \n"
           "                         1 License Error                                                \n"
           "                         2 No solution                                                  \n"
           "                         3 Other Error   /                                              \n"
           "    ares(res)          / 3 /;                                                           \n"
           "                                                                                        \n"
           "Sets Warehouse  /w1*w%warehouse% /                                                      \n"
           "     Store      /s1*s%store%     /                                                      \n"
           "Alias (Warehouse,w), (Store,s);                                                         \n"
           "Scalar                                                                                  \n"
           "     fixed        fixed cost for opening a warehouse / %fixed% /                        \n"
           "Parameter                                                                               \n"
           "     capacity(WareHouse)                                                                \n"
           "     supplyCost(Store,Warehouse);                                                       \n"
           "                                                                                        \n"
           "$eval storeDIVwarehouse trunc(card(store)/card(warehouse))                              \n"
           "capacity(w)     =   %storeDIVwarehouse% + mod(ord(w),%storeDIVwarehouse%);              \n"
           "supplyCost(s,w) = 1+mod(ord(s)+10*ord(w), 100);                                         \n"
           "                                                                                        \n"
           "Variables                                                                               \n"
           "    open(Warehouse)                                                                     \n"
           "    supply(Store,Warehouse)                                                             \n"
           "    obj;                                                                                \n"
           "Binary variables open, supply;                                                          \n"
           "                                                                                        \n"
           "Equations                                                                               \n"
           "    defobj                                                                              \n"
           "    oneWarehouse(s)                                                                     \n"
           "    defopen(w);                                                                         \n"
           "                                                                                        \n"
           "defobj..  obj =e= sum(w, fixed*open(w)) + sum((w,s), supplyCost(s,w)*supply(s,w));      \n"
           "                                                                                        \n"
           "oneWarehouse(s).. sum(w, supply(s,w)) =e= 1;                                            \n"
           "                                                                                        \n"
           "defopen(w)..      sum(s, supply(s,w)) =l= open(w)*capacity(w);                          \n"
           "                                                                                        \n"
           "$ifthen %disaggregate%==1                                                               \n"
           "Equations                                                                               \n"
           "     defopen2(s,w);                                                                     \n"
           "defopen2(s,w).. supply(s,w) =l= open(w);                                                \n"
           "$endif                                                                                  \n"
           "                                                                                        \n"
           "model distrib /all/;                                                                    \n"
           "solve distrib min obj using mip;                                                        \n"
           "                                                                                        \n"
           "$macro setResult(n) option clear=ares; ares(n) = yes;                                   \n"
           "if (distrib.modelstat=%ModelStat.LicensingProblem% or                                   \n"
           "    distrib.solvestat=%Solvestat.LicensingProblems%,                                    \n"
           "  setResult('1');                                                                       \n"
           "  abort 'License Error';                                                                \n"
           ");                                                                                      \n"
           "if (distrib.solvestat<>%SolveStat.NormalCompletion% or                                  \n"
           "    distrib.modelstat<>%ModelStat.Optimal% and                                          \n"
           "    distrib.modelstat<>%ModelStat.IntegerSolution%,                                     \n"
           "  setResult('2');                                                                       \n"
           "  abort 'No solution';                                                                  \n"
           ");                                                                                      \n"
           "setResult('0');                                                                         \n";
}

void solveWarehouse(GAMSWorkspace* ws, int numberOfWarehouses, GAMSDatabase* resultDB, mutex* dbMutex)
{
    GAMSJob gmsJ = ws->addJobFromString(getModelText());
    try{
        // instantiate GAMSOptions and define some scalars
        GAMSOptions gmsOpt = ws->addOptions();
        gmsOpt.setAllModelTypes("cplex");
        gmsOpt.setDefine("Warehouse", to_string(numberOfWarehouses));
        gmsOpt.setDefine("Store", "65");
        gmsOpt.setDefine("fixed", "22");
        gmsOpt.setDefine("disaggregate", "0");
        gmsOpt.setOptCR(0.0);

        // create a GAMSJob from string and write results to the result database
        gmsJ.run(gmsOpt);

        // need to lock database write operations
        {
            lock_guard<mutex> dbLock(*dbMutex);
            resultDB->getParameter("objrep").addRecord(to_string(numberOfWarehouses)).setValue(gmsJ.outDB().getVariable("obj").findRecord().level());
        }

        for(GAMSVariableRecord supplyRec : gmsJ.outDB().getVariable("supply"))
        {
            if (supplyRec.level() > 0.5)
            {
                lock_guard<mutex> dbLock(*dbMutex);
                resultDB->getSet("supplyMap").addRecord(to_string(numberOfWarehouses), supplyRec.key(0), supplyRec.key(1));
            }
        }
    }
    catch(GAMSExceptionExecution& e)
    {
        if(e.rc() == GAMSEnum::ExecutionError)
        {
            lock_guard<mutex> dbLock(*dbMutex);
            statusString = gmsJ.outDB().getSet("res").findRecord(gmsJ.outDB().getSet("ares").firstRecord().key(0)).text();
        }
        {
            lock_guard<mutex> dbLock(*dbMutex);
            status = e.rc();
        }
    }
    catch (GAMSException& e)
    {
        cout << e.what() << endl;
        lock_guard<mutex> dbLock(*dbMutex);
        status = -1;
    }
    catch (exception& e)
    {
        cout << e.what() << endl;
        lock_guard<mutex> dbLock(*dbMutex);
        status = -2;
    }
}

/// \file warehouse.cpp
/// \brief This example demonstrates how to solve a simple GAMS model to assign
/// stores to warehouses for different data sets in parallel.
///
/// The model has been parameterized. The data can be derived from a few numbers namely
/// the number of warehouses, stores, and some fixed cost scalar. The
/// results of the model are written into a single result database that is
/// protected across the parallel threads via a mutex.
int main(int argc, char* argv[])
{
    cout << "---------- Warehouse --------------" << endl;

    try{
        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);

        // create a GAMSDatabase for the results
        GAMSDatabase resultDB = ws.addDatabase();

        resultDB.addParameter("objrep", 1, "Objective value");
        resultDB.addSet("supplyMap", 3, "Supply connection with level");
        // run multiple parallel jobs
        mutex dbMutex;
        vector<thread> v;
        for(int nrWarehouses=10; nrWarehouses<22; nrWarehouses++)
            v.emplace_back([&ws, nrWarehouses, &resultDB, &dbMutex]{solveWarehouse(&ws, nrWarehouses, &resultDB, &dbMutex);});
        for (auto& t : v)
            t.join();
        if (status > 0)
            throw GAMSExceptionExecution("Error when running GAMS: " + GAMSEnum::text((GAMSEnum::GAMSExitCode) status) + " " + statusString, status);
        else if (status == -1)
            throw GAMSException("Error in GAMS API");
        else if (status == -2)
            throw exception();
        // export the result database to a GDX file
        resultDB.doExport("\\tmp\\resultCpp.gdx");
    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
    return status;
}
