/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2023 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2023 GAMS Development Corp. <support@gams.com>
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
#include <mutex>
#include <thread>
#include <iostream>

using namespace gams;
using namespace std;

/// Solve the model in different scenarios
void scenSolve(GAMSWorkspace* ws, GAMSCheckpoint* cp, vector<double>* bmultVector, std::mutex* vectorMutex, std::mutex* ioMutex)
{
    unique_lock<mutex> vectorLock(*vectorMutex);
    GAMSModelInstance mi = cp->addModelInstance();
    vectorLock.unlock();

    GAMSParameter bmult = mi.syncDb().addParameter("bmult", "demand multiplier");
    GAMSOptions opt = ws->addOptions();
    opt.setAllModelTypes("cplex");
    // instantiate the GAMSModelInstance and pass a model definition and GAMSModifier to declare bmult mutable
    mi.instantiate("transport use lp min z", opt, GAMSModifier(bmult));

    bmult.addRecord().setValue(1.0);

    while (true)
    {
        double b;

        // dynamically get a bmult value from the vector instead of passing it to the different threads at creation time
        vectorLock.lock();
        if (bmultVector->empty())
            return;
        b = bmultVector->back();
        bmultVector->pop_back();
        vectorLock.unlock();
        bmult.firstRecord().setValue(b);
        mi.solve();

        // we need to make the output a critical section to avoid messed up report informations
        unique_lock<mutex> ioLock(*ioMutex);
        cout << "Scenario bmult=" << b << ":" << endl;
        cout << "  Modelstatus: " << mi.modelStatus() << endl;
        cout << "  Solvestatus: " << mi.solveStatus() << endl;
        cout << "  Obj: " << mi.syncDb().getVariable("z").findRecord().level() << endl;
        ioLock.unlock();
    }
}

/// Get model as string
string getModelText()
{
    return "Sets                                                                  \n"
           "   i   canning plants / seattle, san-diego /                          \n"
           "   j   markets / new-york, chicago, topeka / ;                        \n"
           "                                                                      \n"
           "Parameters                                                            \n"
           "                                                                      \n"
           "   a(i)  capacity of plant i in cases                                 \n"
           "   / seattle     350                                                  \n"
           "     san-diego   600 /                                                \n"
           "                                                                      \n"
           "   b(j)  demand at market j in cases                                  \n"
           "   / new-york    325                                                  \n"
           "     chicago     300                                                  \n"
           "     topeka      275 / ;                                              \n"
           "                                                                      \n"
           "Table d(i, j)  distance in thousands of miles                         \n"
           "                   new-york       chicago      topeka                 \n"
           "   seattle            2.5           1.7          1.8                  \n"
           "   san-diego          2.5           1.8          1.4;                 \n"
           "                                                                      \n"
           "Scalar f      freight in dollars per case per thousand miles / 90 / ; \n"
           "Scalar bmult  demand multiplier / 1 / ;                               \n"
           "                                                                      \n"
           "Parameter c(i, j)  transport cost in thousands of dollars per case;   \n"
           "                                                                      \n"
           "c(i, j) = f * d(i, j) / 1000;                                         \n"
           "                                                                      \n"
           "Variables                                                             \n"
           "   x(i, j)  shipment quantities in cases                              \n"
           "   z       total transportation costs in thousands of dollars;        \n"
           "                                                                      \n"
           "Positive Variable x;                                                  \n"
           "                                                                      \n"
           "Equations                                                             \n"
           "   cost        define objective function                              \n"
           "   supply(i)   observe supply limit at plant i                        \n"
           "   demand(j)   satisfy demand at market j;                            \n"
           "                                                                      \n"
           "cost..        z =e= sum((i, j), c(i, j)*x(i, j));                     \n"
           "                                                                      \n"
           "supply(i)..   sum(j, x(i, j)) =l= a(i);                               \n"
           "                                                                      \n"
           "demand(j)..   sum(i, x(i, j)) =g= bmult*b(j);                         \n"
           "                                                                      \n"
           "Model transport / all / ;                                             \n";
}

/// \file transport8.cpp
/// \brief This is the 8th model in a series of tutorial examples.
///
/// Here we show:
///   - How to use a vector to solve multiple GAMSModelInstances in parallel
int main(int argc, char* argv[])
{
    cout << "---------- Transport 8 --------------" << endl;

    try {
        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);
        GAMSCheckpoint cp = ws.addCheckpoint();

        // initialize a GAMSCheckpoint by running a GAMSJob
        ws.addJobFromString(getModelText()).run(cp);

        vector<double> bmultVector = { 1.3, 1.2, 1.1, 1.0, 0.9, 0.8, 0.7, 0.6 };
        int nrThreads = 2;
        // solve multiple model instances in parallel
        std::mutex vectorMutex;
        std::mutex ioMutex;
        vector<thread> v;
        for (int i = 0; i < nrThreads; i++)
            v.emplace_back([&ws, &cp, &bmultVector, &vectorMutex, &ioMutex] {scenSolve(&ws, &cp, &bmultVector, &vectorMutex, &ioMutex);});
        for (auto& t : v)
            t.join();

    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}

