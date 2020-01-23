/*
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

#include "transport.h"

#include <ostream>

using namespace gams;
using namespace std;

/// \file transport.cpp
/// \brief Wrapper class implementation for GAMS trnsport model
Transport::Transport(GAMSWorkspace ws)
    : fws(ws),
      job(fws.addJobFromString(getModelSource())),
      fopt(fws.addOptions())
{
    fws = ws;
    fopt = ws.addOptions();

    fDbIn1 = ws.addDatabase("", "dbIn1");
    fDbIn2 = ws.addDatabase("", "dbIn2");

    fopt.setSolveLink(GAMSOptions::ESolveLink::LoadLibrary);
    fopt.setAllModelTypes("Cplex");
    fopt.setDefine("dbOut1", "dbOut1");

    fi = fDbIn1.addSet("i", "canning plants");
    fj = fDbIn1.addSet("j", "markets");
    fa = fDbIn1.addParameter("a", "capacity of plant i in cases", fi);
    fb = fDbIn1.addParameter("b", "demand at market j in cases", fj);
    fd = fDbIn1.addParameter("d", "distance in thousands of miles", fi, fj);
    ff = fDbIn2.addParameter("f", "freight in dollars per case per thousand miles");

    job = ws.addJobFromString(getModelSource());
}

void Transport::run(GAMSCheckpoint checkpoint, ostream &output)
{
    if (!fDbIn1.checkDomains())
        throw GAMSException("Domain Errors in Database 1");
    if (!fDbIn2.checkDomains())
        throw GAMSException("Domain Errors in Database 2");

    vector<GAMSDatabase> dbs {fDbIn1, fDbIn2};
    job.run(fopt, checkpoint, output, false, dbs);

    fDbOut1 = fws.addDatabaseFromGDX(fopt.getDefine("dbOut1") + ".gdx");
    fx = fDbOut1.getVariable("x");
    fz = fDbOut1.getVariable("z");
}

string Transport::getModelSource()
{
    return  "  Sets                                                                     \n"
            "       i   canning plants                                                  \n"
            "       j   markets                                                         \n"
            "                                                                           \n"
            "  Parameters                                                               \n"
            "       a(i)   capacity of plant i in cases                                 \n"
            "       b(j)   demand at market j in cases                                  \n"
            "       d(i,j) distance in thousands of miles                               \n"
            "  Scalar f  freight in dollars per case per thousand miles;                \n"
            "                                                                           \n"
            "$if not set dbIn1 $abort 'no file name for in-database 1 file provided'    \n"
            "$gdxin %dbIn1%                                                             \n"
            "$load i j a b d                                                            \n"
            "                                                                           \n"
            "$if not set dbIn2 $abort 'no file name for in-database 2 file provided'    \n"
            "$gdxin %dbIn2%                                                             \n"
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
            "$if not set dbOut1 $abort 'no file name for out-database 1 file provided'  \n"
            "  execute_unload '%dbOut1%', x, z;                                         \n";
}
