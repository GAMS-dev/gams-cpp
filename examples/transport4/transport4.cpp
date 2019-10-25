/*
 *
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
#include <vector>
#include <map>

using namespace gams;
using namespace std;

/// Get model as string
string getModelText()
{
    return "Sets                                                                       \n"
           "      i   canning plants                                                   \n"
           "      j   markets                                                          \n"
           "                                                                           \n"
           "Parameters                                                                 \n"
           "      a(i)   capacity of plant i in cases                                  \n"
           "      b(j)   demand at market j in cases                                   \n"
           "      d(i,j) distance in thousands of miles                                \n"
           "Scalar f  freight in dollars per case per thousand miles;                  \n"
           "                                                                           \n"
           "$if not set gdxincname $abort 'no include file name for data file provided'\n"
           "$gdxin %gdxincname%                                                        \n"
           "$load i j a b d f                                                          \n"
           "$gdxin                                                                     \n"
           "                                                                           \n"
           " Parameter c(i,j)  transport cost in thousands of dollars per case ;       \n"
           "                                                                           \n"
           "            c(i,j) = f * d(i,j) / 1000 ;                                   \n"
           "                                                                           \n"
           " Variables                                                                 \n"
           "       x(i,j)  shipment quantities in cases                                \n"
           "       z       total transportation costs in thousands of dollars ;        \n"
           "                                                                           \n"
           " Positive Variable x ;                                                     \n"
           "                                                                           \n"
           " Equations                                                                 \n"
           "                                                                           \n"
           "      cost        define objective function                                \n"
           "      supply(i)   observe supply limit at plant i                          \n"
           "       demand(j)   satisfy demand at market j ;                            \n"
           "                                                                           \n"
           "  cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;                       \n"
           "                                                                           \n"
           "  supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;                               \n"
           "                                                                           \n"
           "  demand(j) ..   sum(i, x(i,j))  =g=  b(j) ;                               \n"
           "                                                                           \n"
           " Model transport /all/ ;                                                   \n"
           "                                                                           \n"
           " Solve transport using lp minimizing z ;                                   \n"
           "                                                                           \n"
           "Display x.l, x.m ;                                                         \n";
}

/// \file transport4.cpp
/// \brief This is the 4th model in a series of tutorial examples.
///
/// Here we show:
///   - How to define data using C++ data structures
///   - How to prepare a GAMSDatabase from C++ data structures
int main(int argc, char* argv[])
{
    cout << "---------- Transport 4 --------------" << endl;

    try {
        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);

        // define some data by using C++ data structures
        vector<string> plants = {
            "Seattle", "San-Diego"
        };
        vector<string> markets = {
            "New-York", "Chicago", "Topeka"
        };
        map<string, double> capacity = {
            { "Seattle", 350.0 }, { "San-Diego", 600.0 }
        };
        map<string, double> demand = {
            { "New-York", 325.0 }, { "Chicago", 300.0 }, { "Topeka", 275.0 }
        };
        map<tuple<string, string>, double> distance = {
            { make_tuple("Seattle", "New-York"), 2.5 },
            { make_tuple("Seattle", "Chicago"), 1.7 },
            { make_tuple("Seattle", "Topeka"), 1.8 },
            { make_tuple("San-Diego", "New-York"), 2.5 },
            { make_tuple("San-Diego", "Chicago"), 1.8 },
            { make_tuple("San-Diego", "Topeka"), 1.4 }
        };

        // prepare a GAMSDatabase with data from the C++ data structures
        GAMSDatabase db = ws.addDatabase();

        GAMSSet i = db.addSet("i", 1, "canning plants");
        for (string p: plants)
            i.addRecord(p);

        GAMSSet j = db.addSet("j", 1, "markets");
        for (string m: markets)
            j.addRecord(m);

        GAMSParameter a = db.addParameter("a", "capacity of plant i in cases", i);
        for (string p: plants)
            a.addRecord(p).setValue(capacity[p]);

        GAMSParameter b = db.addParameter("b", "demand at market j in cases", j);
        for (string m: markets)
            b.addRecord(m).setValue(demand[m]);

        GAMSParameter d = db.addParameter("d", "distance in thousands of miles", i, j);
        for (auto t : distance)
            d.addRecord(get<0>(t.first), get<1>(t.first)).setValue(t.second);

        GAMSParameter f = db.addParameter("f", "freight in dollars per case per thousand miles");
        f.addRecord().setValue(90);

        // run a job using data from the created GAMSDatabase
        GAMSJob t4 = ws.addJobFromString(getModelText());
        GAMSOptions opt = ws.addOptions();
        opt.setDefine("gdxincname", db.name());
        opt.setAllModelTypes("xpress");
        t4.run(opt, db);

        for (GAMSVariableRecord rec : t4.outDB().getVariable("x"))
            cout << "x(" << rec.key(0) << "," << rec.key(1) << "):" << " level=" << rec.level() << " marginal="
                 << rec.marginal() << endl;

    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
