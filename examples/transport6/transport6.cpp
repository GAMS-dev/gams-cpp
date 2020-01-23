/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2020 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2020 GAMS Development Corp. <support@gams.com>
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
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

using namespace gams;
using namespace std;

/// Run the job with a different scenario
void runScenario(GAMSWorkspace* ws, const GAMSCheckpoint& cp, mutex* ioMutex, double b)
{
    auto t6 = ws->addJobFromString("bmult=" + to_string(b) + "; solve transport min z use lp; ms=transport.modelstat; ss=transport.solvestat;", cp);
    t6.run();

    // we need to make the ouput a critical section to avoid messed up report information
    lock_guard<mutex> lck(*ioMutex);
    cout << "Scenario bmult=" << b << ":" << endl;
    cout << "  Modelstatus: " << t6.outDB().getParameter("ms").findRecord().value() << endl;
    cout << "  Solvestatus: " << t6.outDB().getParameter("ss").findRecord().value() << endl;
    cout << "  Obj: " << t6.outDB().getVariable("z").findRecord().level() << endl;
}

/// Get model as string
string getModelText()
{
    return "Sets                                                                       \n"
           "   i   canning plants / seattle, san-diego /                               \n"
           "   j   markets / new-york, chicago, topeka / ;                             \n"
           "                                                                           \n"
           "Parameters                                                                 \n"
           "                                                                           \n"
           "   a(i)  capacity of plant i in cases                                      \n"
           "   / seattle     350                                                       \n"
           "     san-diego   600 /                                                     \n"
           "                                                                           \n"
           "   b(j)  demand at market j in cases                                       \n"
           "   / new-york    325                                                       \n"
           "     chicago       300                                                     \n"
           "     topeka        275 / ;                                                 \n"
           "                                                                           \n"
           "Table d(i, j)  distance in thousands of miles                              \n"
           "                  new-york       chicago      topeka                       \n"
           "   seattle           2.5           1.7          1.8                        \n"
           "   san-diego         2.5           1.8          1.4;                       \n"
           "                                                                           \n"
           "Scalar f      freight in dollars per case per thousand miles / 90 / ;      \n"
           "Scalar bmult  demand multiplier / 1 / ;                                    \n"
           "                                                                           \n"
           "Parameter c(i, j)  transport cost in thousands of dollars per case;        \n"
           "                                                                           \n"
           "c(i, j) = f * d(i, j) / 1000;                                              \n"
           "                                                                           \n"
           "Variables                                                                  \n"
           "   x(i, j)  shipment quantities in cases                                   \n"
           "   z       total transportation costs in thousands of dollars;             \n"
           "                                                                           \n"
           "Positive Variable x;                                                       \n"
           "                                                                           \n"
           "Equations                                                                  \n"
           "   cost        define objective function                                   \n"
           "   supply(i)   observe supply limit at plant i                             \n"
           "   demand(j)   satisfy demand at market j;                                 \n"
           "                                                                           \n"
           "cost ..        z =e= sum((i, j), c(i, j)*x(i, j));                         \n"
           "                                                                           \n"
           "supply(i) ..   sum(j, x(i, j)) =l= a(i);                                   \n"
           "                                                                           \n"
           "demand(j) ..   sum(i, x(i, j)) =g= bmult*b(j);                             \n"
           "                                                                           \n"
           "Model transport / all / ;                                                  \n"
           "Scalar ms 'model status', ss 'solve status'                                \n";
}


/// \file transport6.cpp
/// \brief This is the 6th model in a series of tutorial examples.
///
/// Here we show:
///   - How to run multiple GAMSJobs in parallel using a GAMSCheckpoint
int main(int argc, char* argv[])
{
    cout << "---------- Transport 6 --------------" << endl;

    try {
        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);
        GAMSCheckpoint cp = ws.addCheckpoint();

        // initialize a GAMSCheckpoint by running a GAMSJob
        ws.addJobFromString(getModelText()).run(cp);

        vector<double> bmultlist = { 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3 };

        // run multiple parallel jobs using the created GAMSCheckpoint
        mutex ioMutex;
        vector<thread> v;
        for(double b : bmultlist)
            v.emplace_back([&ws, cp, &ioMutex, b]{runScenario(&ws, cp,&ioMutex,b);});
        for (auto& t : v)
            t.join();

    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
