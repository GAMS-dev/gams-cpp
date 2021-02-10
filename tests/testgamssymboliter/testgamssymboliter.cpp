/*
 *
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

#include "gamsequation.h"
#include "gamsset.h"
#include "gamsparameter.h"
#include "gamssymboliter.h"
#include "gamsvariable.h"
#include "testgamssymboliter.h"

#include <sstream>

using namespace gams;

QString TestGAMSSymbolIter::classname()  { return "TestGAMSSymbolIter"; }

void TestGAMSSymbolIter::testConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    { std::map<std::string, size_t> recordCounter;
        // when
        for(GAMSSetRecord rec : db.getSet("i")) {
            recordCounter[rec.key(0)]++;
        }
        // then
        QCOMPARE( recordCounter.size(), size_t(2) );
        QCOMPARE( recordCounter["seattle"], size_t(1) );
        QCOMPARE( recordCounter["san-diego"], size_t(1) );
    }
    { std::map<std::string, double> recordMap;
        // when
        for(GAMSParameterRecord rec : db.getParameter("d")) {
            std::stringstream ss;
            ss << rec.key(0) << "_" << rec.key(1) ;
            recordMap[ss.str()] = rec.value();
        }
        // then
        QCOMPARE( recordMap.size(), size_t(6) );
        QVERIFY( equals( recordMap["seattle_new-york"], 2.5) );
        QVERIFY( equals( recordMap["seattle_chicago"], 1.7) );
        QVERIFY( equals( recordMap["seattle_topeka"], 1.8) );
        QVERIFY( equals( recordMap["san-diego_new-york"], 2.5) );
        QVERIFY( equals( recordMap["san-diego_chicago"], 1.8) );
        QVERIFY( equals( recordMap["san-diego_topeka"], 1.4) );
    }
    { std::map<std::string, size_t> recordCounter;
        // when
        for(GAMSEquationRecord rec : db.getEquation("demand")) {
            recordCounter[rec.key(0)]++;
        }
        // then
        QCOMPARE( recordCounter.size(), size_t(3) );
        QCOMPARE( recordCounter["new-york"], size_t(1) );
        QCOMPARE( recordCounter["chicago"], size_t(1) );
        QCOMPARE( recordCounter["topeka"], size_t(1) );
        recordCounter.clear();
    }
}

void TestGAMSSymbolIter::testConstructor_InvalidPosition() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    GAMSSet i = db.getSet("i");
    GAMSSymbolIter<GAMSSet> it_i(i, 1);
    try {
        GAMSSymbolIter<GAMSSet> it_i(i, i.numberRecords()+1);
    } catch(GAMSException&) { QVERIFY(true); }

    GAMSParameter b = db.getParameter("b");
    GAMSSymbolIter<GAMSParameter> it_b(b, 1);
    try {
        GAMSSymbolIter<GAMSParameter> it_b(b, b.numberRecords()+1);
    } catch(GAMSException&) { QVERIFY(true); }

    GAMSEquation supply = db.getEquation("supply");
    GAMSSymbolIter<GAMSEquation> it_supply(supply, 1);
    try {
        GAMSSymbolIter<GAMSEquation> it_supply(supply, supply.numberRecords()+1);
    } catch(GAMSException&) { QVERIFY(true); }

    GAMSVariable x = db.getVariable("x");
    GAMSSymbolIter<GAMSVariable> it_x(x, 1);
    try {
        GAMSSymbolIter<GAMSVariable> it_x(x, x.numberRecords()+1);
    } catch(GAMSException&) { QVERIFY(true); }
}

void TestGAMSSymbolIter::testConstructor_InvalidSymbol() {
    GAMSSet set;
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSSet> it(set, 0), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSSet> it(set, -1), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSSet> it(set, 100), GAMSException);
    try {
        for(GAMSSetRecord rec : set) { QFAIL("Invalid GAMSSet contains no record!");  }
    } catch(GAMSException&) { QVERIFY(true); }

    GAMSParameter par;
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSParameter> it(par, 0), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSParameter> it(par, -1), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSParameter> it(par, 100), GAMSException);
    try {
        for(GAMSParameterRecord rec : par) { QFAIL("Invalid GAMSParameter contains no record!");  }
    } catch(GAMSException&) { QVERIFY(true); }

    GAMSEquation eq;
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSEquation> it(eq, 0), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSEquation> it(eq, -1), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSEquation> it(eq, 100), GAMSException);
    try {
        for(GAMSEquationRecord rec : eq) { QFAIL("Invalid GAMSEquation contains no record!");  }
    } catch(GAMSException&) { QVERIFY(true); }

    GAMSVariable var;
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSVariable> it(var, 0), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSVariable> it(var, -1), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSymbolIter<GAMSVariable> it(var, 100), GAMSException);
    try {
        for(GAMSVariableRecord rec : var) { QFAIL("Invalid GAMSVariable contains no record!");  }
    } catch(GAMSException&) { QVERIFY(true); }

}

void TestGAMSSymbolIter::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    {  GAMSSet set_i = db.getSet("i");
        // when
        GAMSSymbolIter<GAMSSet> it_i1(set_i, 0);
        GAMSSymbolIter<GAMSSet> it_i2(set_i, 0);
        // then
        QVERIFY( it_i1 == it_i2 );

        GAMSSet i = db.getSet("i");
        // when
        GAMSSymbolIter<GAMSSet> it_i3(i, 0);
        // then
        QVERIFY( it_i1 == it_i3 );

        ++it_i3;
        // then
        QVERIFY( !(it_i1 == it_i3) );
        // when
        GAMSSymbolIter<GAMSSet> it_i4(i, i.numberRecords());
        // then
        QVERIFY( !(it_i1 == it_i4) );
        QVERIFY( !(it_i3 == it_i4) );
    }
    {  GAMSParameter par_d = db.getParameter("d");
        // when
        GAMSSymbolIter<GAMSParameter> it_d1(par_d, 0);
        GAMSSymbolIter<GAMSParameter> it_d2(par_d, 0);
        // then
        QVERIFY( it_d1 == it_d2 );

        GAMSParameter d = db.getParameter("d");
        // when
        GAMSSymbolIter<GAMSParameter> it_d3(d, 0);
        // then
        QVERIFY( it_d1 == it_d3 );

        ++it_d3;
        // then
        QVERIFY( !(it_d1 == it_d3) );
        // when
        GAMSSymbolIter<GAMSParameter> it_d4(d, d.numberRecords());
        // then
        QVERIFY( !(it_d1 == it_d4) );
        QVERIFY( !(it_d3 == it_d4) );
    }
    {  GAMSEquation eq_supply = db.getEquation("supply");
        // when
        GAMSSymbolIter<GAMSEquation> it_s1(eq_supply, 0);
        GAMSSymbolIter<GAMSEquation> it_s2(eq_supply, 0);
        // then
        QVERIFY( it_s1 == it_s2 );

        GAMSEquation supply = db.getEquation("supply");
        // when
        GAMSSymbolIter<GAMSEquation> it_s3(supply, 0);
        // then
        QVERIFY( it_s1 == it_s3 );

        ++it_s3;
        // then
        QVERIFY( !(it_s1 == it_s3) );
        // when
        GAMSSymbolIter<GAMSEquation> it_s4(supply, supply.numberRecords());
        // then
        QVERIFY( !(it_s1 == it_s4) );
        QVERIFY( !(it_s3 == it_s4) );
    }
    {  GAMSVariable var_x = db.getVariable("x");
        // when
        GAMSSymbolIter<GAMSVariable> it_x1(var_x, 0);
        GAMSSymbolIter<GAMSVariable> it_x2(var_x, 0);
        // then
        QVERIFY( it_x1 == it_x2 );

        GAMSVariable x = db.getVariable("x");
        // when
        GAMSSymbolIter<GAMSVariable> it_x3(x, 0);
        // then
        QVERIFY( it_x1 == it_x3 );

        ++it_x3;
        // then
        QVERIFY( !(it_x1 == it_x3) );
        // when
        GAMSSymbolIter<GAMSVariable> it_x4(x, x.numberRecords());
        // then
        QVERIFY( !(it_x1 == it_x4) );
        QVERIFY( !(it_x3 == it_x4) );
    }
}

void TestGAMSSymbolIter::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    { GAMSSet set_i = db.getSet("i");
        GAMSSet set_j = db.getSet("j");
        // when
        GAMSSymbolIter<GAMSSet> it_i(set_i, 0);
        GAMSSymbolIter<GAMSSet> it_j1(set_j, 0);
        // then
        QVERIFY( it_i != it_j1 );

        ++it_i;
        ++it_j1;
        // then
        QVERIFY( it_i != it_j1 );
        // when

        GAMSSymbolIter<GAMSSet> it_j2(set_j, set_j.numberRecords());
        // then
        QVERIFY( it_j1 != it_j2 );
    }
    { GAMSParameter par_a = db.getParameter("a");
        GAMSParameter par_b = db.getParameter("b");
        // when
        GAMSSymbolIter<GAMSParameter> it_a(par_a, 0);
        GAMSSymbolIter<GAMSParameter> it_b1(par_b, 0);
        // then
        QVERIFY( it_a != it_b1 );

        ++it_a;
        ++it_b1;
        // then
        QVERIFY( it_a != it_b1 );
        // when

        GAMSSymbolIter<GAMSParameter> it_b2(par_b, par_b.numberRecords());
        // then
        QVERIFY( it_b1 != it_b2 );
    }
    { GAMSVariable var_z= db.getVariable("z");
        GAMSVariable var_x = db.getVariable("x");
        // when
        GAMSSymbolIter<GAMSVariable> it_z(var_z, 0);
        GAMSSymbolIter<GAMSVariable> it_x1(var_x, 0);
        // then
        QVERIFY( it_z != it_x1 );

        ++it_x1;
        // then
        QVERIFY( it_z != it_x1 );
        // when

        GAMSSymbolIter<GAMSVariable> it_x2(var_x, var_x.numberRecords());
        // then
        QVERIFY( it_x1 != it_x2 );
    }
}

void TestGAMSSymbolIter::testPointerOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    { GAMSSet set_i = db.getSet("i");
        // when
        GAMSSymbolIter<GAMSSet> it_i(set_i, 0);
        // then
        QCOMPARE( (*it_i).key(0).c_str(), "seattle");
    }

    { GAMSParameter par_d = db.getParameter("d");
        // when
        GAMSSymbolIter<GAMSParameter> it_d(par_d, 0);
        ++it_d;
        // then
        QCOMPARE( (*it_d).key(0).c_str(), "seattle");
        QCOMPARE( (*it_d).key(1).c_str(), "chicago");
        // when
        ++it_d;
        ++it_d;
        // then
        QCOMPARE( (*it_d).key(0).c_str(), "san-diego");
        QCOMPARE( (*it_d).key(1).c_str(), "new-york");
    }

    { GAMSVariable var_z = db.getVariable("z");
        // when
        GAMSSymbolIter<GAMSVariable> it_z(var_z, 0);
        // then
        QVERIFY( equals((*it_z).level(), 153.675) );
    }

    { GAMSEquation eq_demand = db.getEquation("demand");
        // when
        GAMSSymbolIter<GAMSEquation> it_demand(eq_demand, 0);
        // then
        QVERIFY( equals((*it_demand).level(), 325.0) );
    }
}


void TestGAMSSymbolIter::testPointerOperator_BeyondLastRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    {
        GAMSSet set_i = db.getSet("i");
        GAMSSymbolIter<GAMSSet> it_i(set_i, set_i.numberRecords());
        // when, then
        QVERIFY_EXCEPTION_THROWN( (*it_i).key(0), GAMSException);
    }

    {
        GAMSParameter par_f = db.getParameter("f");
        GAMSSymbolIter<GAMSParameter> it_f(par_f, par_f.numberRecords());
        // when, then
        QVERIFY_EXCEPTION_THROWN( (*it_f).value(), GAMSException);
        // when
        ++it_f;
        // then
        QVERIFY_EXCEPTION_THROWN( (*it_f).value(), GAMSException);
    }

    {
        GAMSEquation eq_demand = db.getEquation("demand");
        GAMSSymbolIter<GAMSEquation> it_demand(eq_demand, eq_demand.numberRecords());
        // when, then
        QVERIFY_EXCEPTION_THROWN( (*it_demand).upper(), GAMSException);
    }

    {
        GAMSVariable var_z = db.getVariable("z");
        GAMSSymbolIter<GAMSVariable> it_z(var_z, var_z.numberRecords());
        // when, then
        QVERIFY_EXCEPTION_THROWN( (*it_z).marginal(), GAMSException);
    }
}

void TestGAMSSymbolIter::testIncrementOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    { GAMSSet set_i = db.getSet("i");
        GAMSSymbolIter<GAMSSet> it_i(set_i, 0);
        // when
        ++it_i;
        // then
        QCOMPARE( (*it_i).key(0).c_str(), "san-diego");
    }
    { GAMSParameter par_d = db.getParameter("d");
        GAMSSymbolIter<GAMSParameter> it_d(par_d, 0);
        // when
        ++it_d;
        // then
        QCOMPARE( (*it_d).key(0).c_str(), "seattle");
        QCOMPARE( (*it_d).key(1).c_str(), "chicago");
        // when
        ++it_d;
        ++it_d;
        // then
        QCOMPARE( (*it_d).key(0).c_str(), "san-diego");
        QCOMPARE( (*it_d).key(1).c_str(), "new-york");
    }
    { GAMSVariable var_x = db.getVariable("x");
        GAMSSymbolIter<GAMSVariable> it_x(var_x, 0);
        // when
        ++it_x;
        ++it_x;
        ++it_x;
        ++it_x;
        // then
        QCOMPARE( (*it_x).key(0).c_str(), "san-diego");
        QCOMPARE( (*it_x).key(1).c_str(), "chicago");
    }
    { GAMSEquation eq_demand = db.getEquation("demand");
        GAMSSymbolIter<GAMSEquation> it_demand(eq_demand, 0);
        ++it_demand;
        QCOMPARE( (*it_demand).key(0).c_str(), "chicago");
    }
}

QTEST_MAIN(TestGAMSSymbolIter)
