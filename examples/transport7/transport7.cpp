/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
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

using namespace gams;
using namespace std;

string getModelText()
{
    return " Sets                                                               \n"
           "     i   canning plants   / seattle, san-diego /                    \n"
           "     j   markets          / new-york, chicago, topeka / ;           \n"
           "                                                                    \n"
           "Parameters                                                          \n"
           "     a(i)  capacity of plant i in cases                             \n"
           "       /    seattle     350                                         \n"
           "            san-diego   600  /                                      \n"
           "                                                                    \n"
           "     b(j)  demand at market j in cases                              \n"
           "       /    new-york    325                                         \n"
           "            chicago     300                                         \n"
           "            topeka      275  / ;                                    \n"
           "                                                                    \n"
           "Table d(i,j)  distance in thousands of miles                        \n"
           "                  new-york       chicago      topeka                \n"
           "    seattle          2.5           1.7          1.8                 \n"
           "    san-diego        2.5           1.8          1.4  ;              \n"
           "                                                                    \n"
           "Scalar f      freight in dollars per case per thousand miles  /90/ ;\n"
           "Scalar bmult  demand multiplier /1/;                                \n"
           "                                                                    \n"
           "Parameter c(i,j)  transport cost in thousands of dollars per case ; \n"
           "                                                                    \n"
           "          c(i,j) = f * d(i,j) / 1000 ;                              \n"
           "                                                                    \n"
           "Variables                                                           \n"
           "     x(i,j)  shipment quantities in cases                           \n"
           "     z       total transportation costs in thousands of dollars ;   \n"
           "                                                                    \n"
           "Positive Variable x ;                                               \n"
           "                                                                    \n"
           "Equations                                                           \n"
           "     cost        define objective function                          \n"
           "     supply(i)   observe supply limit at plant i                    \n"
           "     demand(j)   satisfy demand at market j ;                       \n"
           "                                                                    \n"
           "cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;                  \n"
           "                                                                    \n"
           "supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;                          \n"
           "                                                                    \n"
           "demand(j) ..   sum(i, x(i,j))  =g=  bmult*b(j) ;                    \n"
           "                                                                    \n"
           "Model transport /all/ ;                                             \n";
}

/// This is the 7th model in a series of tutorial examples.
///
/// Here we show:
///  - How to create a GAMSModelInstance from a GAMSCheckpoint
///  - How to modify a parameter of a GAMSModelInstance using GAMSModifier
///  - How to modify a variable of a GAMSModelInstance using GAMSModifier
int main(int argc, char* argv[])
{
    cout << "---------- Transport 7 --------------" << endl;

    GAMSWorkspaceInfo wsInfo;
    if (argc > 1)
        wsInfo.setSystemDirectory(argv[1]);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();

    // initialize a GAMSCheckpoint by running a GAMSJob
    GAMSJob t7 = ws.addJobFromString(getModelText());
    t7.run(cp);

    // create a GAMSModelInstance and solve it multiple times with different scalar bmult
    GAMSModelInstance mi = cp.addModelInstance();
    GAMSParameter bmult = mi.syncDb().addParameter("bmult", 0, "demand multiplier");
    GAMSOptions opt = ws.addOptions();
    opt.setAllModelTypes("cplexd");

    // instantiate the GAMSModelInstance and pass a model definition and GAMSModifier to declare bmult mutable
    mi.instantiate("transport use lp min z", opt, GAMSModifier(bmult));

    bmult.addRecord().setValue(1.0);
    vector<double> bmultlist = { 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3 };

    for (double b : bmultlist) {
        bmult.firstRecord().setValue(b);
        mi.solve();
        cout << "Scenario bmult=" << b << ":" << endl;
        cout << "  Modelstatus: " << mi.modelStatusAsString() << endl;
        cout << "  Solvestatus: " << mi.solveStatusAsString() << endl;
        cout << "  Obj: " << mi.syncDb().getVariable("z").findRecord().level() << endl;
    }

    // create a GAMSModelInstance and solve it with single links in the network blocked
    mi = cp.addModelInstance();

    GAMSVariable x = mi.syncDb().addVariable("x", 2, GAMSEnum::VarType::Positive, "");
    GAMSParameter xup = mi.syncDb().addParameter("xup", 2, "upper bound on x");

    // instantiate the GAMSModelInstance and pass a model definition and GAMSModifier to declare upper bound of X mutable
    mi.instantiate("transport use lp min z", GAMSModifier (x, GAMSEnum::SymbolUpdateAction::Upper, xup));

    for (GAMSSetRecord i : t7.outDB().getSet("i")) {
        for (GAMSSetRecord j : t7.outDB().getSet("j")) {
            xup.clear();
            xup.addRecord(i.key(0), j.key(0)).setValue(0);
            mi.solve();
            cout << "Scenario link blocked=" << i.key(0) << "-" << j.key(0) << endl;
            cout << "  Modelstatus: " << mi.modelStatusAsString() << endl;
            cout << "  Solvestatus: " << mi.solveStatusAsString() << endl;
            cout << "  Obj: " << mi.syncDb().getVariable("z").findRecord().level() << endl;
        }
    }

    return 0;
}
