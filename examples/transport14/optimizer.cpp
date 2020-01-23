/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2020 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2020 GAMS Development Corp. <support@gams.com>s
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

/// \file optimizer.cpp
/// \brief Wrapper class implementation for executing transport14
Optimizer::Optimizer(int argc, char *argv[])
{
    GAMSWorkspaceInfo wsInfo;
    if (argc > 1)
        wsInfo.setSystemDirectory(argv[1]);
    ws = GAMSWorkspace(wsInfo);
}

double Optimizer::solve(double mult)
{
    GAMSDatabase gDb = ws.addDatabase();

    GAMSParameter f = gDb.addParameter("f", "freight in dollars per case per thousand miles");
    f.addRecord().setValue(90 * mult);

    GAMSJob gModJob = ws.addJobFromString(getModelText());

    GAMSOptions gOption = ws.addOptions();
    gOption.setDefine("gdxincname", gDb.name());
    gModJob.run(gOption, gDb);

    return gModJob.outDB().getVariable("z").firstRecord().level();
}

string Optimizer::getModelText()
{
    return "  Sets                                                                     \n"
           "       i   canning plants   / seattle, san-diego /                         \n"
           "       j   markets          / new-york, chicago, topeka / ;                \n"
           "                                                                           \n"
           "  Parameters                                                               \n"
           "                                                                           \n"
           "       a(i)  capacity of plant i in cases                                  \n"
           "         /    seattle     350                                              \n"
           "              san-diego   600  /                                           \n"
           "                                                                           \n"
           "       b(j)  demand at market j in cases                                   \n"
           "         /    new-york    325                                              \n"
           "              chicago     300                                              \n"
           "              topeka      275  / ;                                         \n"
           "                                                                           \n"
           "  Table d(i,j)  distance in thousands of miles                             \n"
           "                    new-york       chicago      topeka                     \n"
           "      seattle          2.5           1.7          1.8                      \n"
           "      san-diego        2.5           1.8          1.4  ;                   \n"
           "                                                                           \n"
           "  Scalar f  freight in dollars per case per thousand miles;                \n"
           "                                                                           \n"
           "$if not set gdxincname $abort 'no include file name for data file provided'\n"
           "$gdxin %gdxincname%                                                        \n"
           "$load f                                                                    \n"
           "$gdxin                                                                     \n"
           "                                                                           \n"
           "  Parameter c(i,j)  transport cost in thousands of dollars per case ;      \n"
           "                                                                           \n"
           "            c(i,j) = f * d(i,j) / 1000 ;                                   \n"
           "                                                                           \n"
           "  Variables                                                                \n"
           "       x(i,j)  shipment quantities in cases                                \n"
           "       z       total transportation costs in thousands of dollars ;        \n"
           "                                                                           \n"
           "  Positive Variable x ;                                                    \n"
           "                                                                           \n"
           "  Equations                                                                \n"
           "       cost        define objective function                               \n"
           "       supply(i)   observe supply limit at plant i                         \n"
           "       demand(j)   satisfy demand at market j ;                            \n"
           "                                                                           \n"
           "  cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;                       \n"
           "                                                                           \n"
           "  supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;                               \n"
           "                                                                           \n"
           "  demand(j) ..   sum(i, x(i,j))  =g=  b(j) ;                               \n"
           "                                                                           \n"
           "  Model transport /all/ ;                                                  \n"
           "                                                                           \n"
           "  Solve transport using lp minimizing z ;                                  \n"
           "                                                                           \n"
           "Display x.l, x.m ;                                                         \n";
}
