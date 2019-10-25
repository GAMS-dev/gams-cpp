/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2019 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2019 GAMS Development Corp. <support@gams.com>
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
#include <iostream>
#include <fstream>

using namespace gams;
using namespace std;

/// \file transport1.cpp
/// \brief This is the 1st model in a series of tutorial examples.
///
/// This is the 1st model in a series of tutorial examples.
///
/// Here we show:
///   - How to run a GAMSJob from file
///   - How to specify the solver
///   - How to run a job with a solver option file
///
/// The example runs the [trnsport] model from the GAMS Model Library.
int main(int argc, char* argv[])
{
    cout << "---------- Transport 1 --------------" << endl;

    try {
        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);
        // Retrieves [trnsport] model from GAMS Model Library
        ws.gamsLib("trnsport");

        // create a GAMSJob from file and run it with default settings
        GAMSJob t1 = ws.addJobFromFile("trnsport.gms");

        // Default run
        t1.run();
        cout << "Ran with Defaults:" << endl;
        for (GAMSVariableRecord rec : t1.outDB().getVariable("x"))
            cout << "x(" << rec.key(0) << "," << rec.key(1) << "):" << " level=" << rec.level() << " marginal="
                 << rec.marginal() << endl;

        // Run the job again with another solver
        GAMSOptions opt = ws.addOptions();
        opt.setAllModelTypes("xpress");
        t1.run(opt);
        cout << "Ran with XPRESS:" << endl;
        for (GAMSVariableRecord rec : t1.outDB().getVariable("x"))
            cout << "x(" << rec.key(0) << "," << rec.key(1) << "):" << " level=" << rec.level() << " marginal="
                 << rec.marginal() << endl;

        // Run the job with a solver option file
        ofstream xpressopt(ws.workingDirectory() + cPathSep + "xpress.opt");
        xpressopt << "algorithm=barrier" << endl;
        xpressopt.close();

        opt.setOptFile(1);
        t1.run(opt);
        cout << "Ran with XPRESS with non-default option:" << endl;
        for (GAMSVariableRecord rec : t1.outDB().getVariable("x"))
            cout << "x(" << rec.key(0) << "," << rec.key(1) << "):" << " level=" << rec.level() << " marginal="
                 << rec.marginal() << endl;

    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
