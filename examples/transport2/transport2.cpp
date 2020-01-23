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
#include <iostream>
#include <fstream>

using namespace gams;
using namespace std;

/// Get data as string
string getDataText()
{
    return "Sets                                                           \n"
           "  i   canning plants   / seattle, san-diego /                  \n"
           "  j   markets          / new-york, chicago, topeka / ;         \n"
           "Parameters                                                     \n"
           "                                                               \n"
           "  a(i)  capacity of plant i in cases                           \n"
           "                     /    seattle     350                      \n"
           "                          san-diego   600  /                   \n"
           "                                                               \n"
           "  b(j)  demand at market j in cases                            \n"
           "                     /    new-york    325                      \n"
           "                          chicago     300                      \n"
           "                          topeka      275  / ;                 \n"
           "                                                               \n"
           "Table d(i,j)  distance in thousands of miles                   \n"
           "               new-york       chicago      topeka              \n"
           "  seattle          2.5           1.7          1.8              \n"
           "  san-diego        2.5           1.8          1.4  ;           \n"
           "                                                               \n"
           "Scalar f  freight in dollars per case per thousand miles  /90/;\n";
}

/// Get model as string
string getModelText()
{
    return "Sets                                                                    \n"
           "      i   canning plants                                                \n"
           "      j   markets                                                       \n"
           "                                                                        \n"
           "Parameters                                                              \n"
           "      a(i)   capacity of plant i in cases                               \n"
           "      b(j)   demand at market j in cases                                \n"
           "      d(i,j) distance in thousands of miles                             \n"
           "Scalar f  freight in dollars per case per thousand miles;               \n"
           "                                                                        \n"
           "$if not set incname $abort 'no include file name for data file provided'\n"
           "$include %incname%                                                      \n"
           "                                                                        \n"
           " Parameter c(i,j)  transport cost in thousands of dollars per case ;    \n"
           "                                                                        \n"
           "            c(i,j) = f * d(i,j) / 1000 ;                                \n"
           "                                                                        \n"
           " Variables                                                              \n"
           "       x(i,j)  shipment quantities in cases                             \n"
           "       z       total transportation costs in thousands of dollars ;     \n"
           "                                                                        \n"
           " Positive Variable x ;                                                  \n"
           "                                                                        \n"
           " Equations                                                              \n"
           "                                                                        \n"
           "      cost        define objective function                             \n"
           "      supply(i)   observe supply limit at plant i                       \n"
           "       demand(j)   satisfy demand at market j ;                         \n"
           "                                                                        \n"
           "  cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;                    \n"
           "                                                                        \n"
           "  supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;                            \n"
           "                                                                        \n"
           "  demand(j) ..   sum(i, x(i,j))  =g=  b(j) ;                            \n"
           "                                                                        \n"
           " Model transport /all/ ;                                                \n"
           "                                                                        \n"
           " Solve transport using lp minimizing z ;                                \n"
           "                                                                        \n"
           "Display x.l, x.m ;                                                      \n";
}

/// \file transport2.cpp
/// \brief This is the 2nd model in a series of tutorial examples.
///
/// Here we show:
///   - How to use include files to separate model and data
int main(int argc, char* argv[])
{
    cout << "---------- Transport 2 --------------" << endl;

    try {
        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);

        // write an include file containing data with GAMS syntax
        ofstream tdata(ws.workingDirectory() + cPathSep + "tdata.gms");
        tdata << getDataText();
        tdata.close();

        // run a job using an instance of GAMSOptions that defines the data include file
        GAMSOptions opt = ws.addOptions();
        GAMSJob t2 = ws.addJobFromString(getModelText());
        opt.setDefine("incname", "tdata");
        t2.run(opt);
        for (GAMSVariableRecord rec : t2.outDB().getVariable("x"))
            cout << "x(" << rec.key(0) << "," << rec.key(1) << "):" << " level=" << rec.level() << " marginal="
                 << rec.marginal() << endl;

    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
