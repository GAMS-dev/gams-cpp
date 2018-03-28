/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>s
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
#include "optimizer.h"

#include <iostream>
#include <thread>

using namespace gams;
using namespace std;

void run(Optimizer optim, double bmult)
{
    cout << "Scenario bmult=" << bmult << ", Obj:" << optim.solve(bmult) << endl;
}

/// \file transport14.cpp
/// \brief This is the 14th model in a series of tutorial examples.
///
/// Here we show:
///  - How to run multiple GAMSJobs in parallel each using different scenario data
int main(int argc, char* argv[])
{
    cout << "---------- Transport 14 --------------" << endl;

    try {
        vector<double> bmultlist { 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3 };
        const Optimizer &optim = Optimizer(argc, argv);

        vector<thread> threads;
        for (auto bmult : bmultlist)
            threads.push_back(thread(run, optim, bmult));

        for (auto& t : threads)
            t.join();

    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
