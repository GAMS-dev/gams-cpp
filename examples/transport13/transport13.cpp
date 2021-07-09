/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2021 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2021 GAMS Development Corp. <support@gams.com>
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
#include "transport.h"

#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace gams;
using namespace std;

/// \file transport13.cpp
/// \brief This is the 13th model in a series of tutorial examples.
///
/// Here we show:
///  - How to run a GAMSJob using a wrapper class to package a particular GAMS model
int main(int argc, char* argv[])
{
    cout << "---------- Transport 13 --------------" << endl;

    try {
        vector<string> plants {"Seattle", "San-Diego"};
        vector<string> markets {"New-York", "Chicago", "Topeka"};
        map<string, double> capacity {
            { "Seattle", 350.0 }, { "San-Diego", 600.0 }
        };
        map<string, double> demand {
            { "New-York", 325.0 }, { "Chicago", 300.0 }, { "Topeka", 275.0 }
        };
        map<tuple<string, string>, double> distance {
            { make_tuple("Seattle",   "New-York"), 2.5 },
            { make_tuple("Seattle",   "Chicago"),  1.7 },
            { make_tuple("Seattle",   "Topeka"),   1.8 },
            { make_tuple("San-Diego", "New-York"), 2.5 },
            { make_tuple("San-Diego", "Chicago"),  1.8 },
            { make_tuple("San-Diego", "Topeka"),   1.4 }
        };

        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);

        Transport t(ws);

        for (string p : plants)
            t.i().addRecord(p);

        for (string m : markets)
            t.j().addRecord(m);

        for (string p : plants)
            t.a().addRecord(p).setValue(capacity[p]);

        for (string m : markets)
            t.b().addRecord(m).setValue(demand[m]);

        for (auto dis : distance)
            t.d().addRecord(get<0>(dis.first), get<1>(dis.first)).setValue(distance[dis.first]);

        t.f().addRecord().setValue(90);

        t.opt().setAllModelTypes("cplex");

        t.run(GAMSCheckpoint(), cout);

        cout << "Objective: " << t.z().firstRecord().level() << endl;
        for (GAMSVariableRecord rec : t.x())
            cout << "x(" << rec.key(0) << "," << rec.key(1) << "): level=" << rec.level() <<
                    " marginal=" << rec.marginal() << endl;

    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
