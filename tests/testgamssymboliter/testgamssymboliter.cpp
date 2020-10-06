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

#include "gamsequation.h"
#include "gamsset.h"
#include "gamsparameter.h"
#include "gamssymboliter.h"
#include "gamsvariable.h"
#include "testgamssymboliter.h"

#include <sstream>

using namespace gams;

class TestGAMSSymbolIter: public TestGAMSObject
{
};

TEST_F(TestGAMSSymbolIter, testConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
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
        EXPECT_EQ( recordCounter.size(), size_t(2) );
        EXPECT_EQ( recordCounter["seattle"], size_t(1) );
        EXPECT_EQ( recordCounter["san-diego"], size_t(1) );
    }
    { std::map<std::string, double> recordMap;
        // when
        for(GAMSParameterRecord rec : db.getParameter("d")) {
            std::stringstream ss;
            ss << rec.key(0) << "_" << rec.key(1) ;
            recordMap[ss.str()] = rec.value();
        }
        // then
        EXPECT_EQ( recordMap.size(), size_t(6) );
        ASSERT_TRUE( equals( recordMap["seattle_new-york"], 2.5) );
        ASSERT_TRUE( equals( recordMap["seattle_chicago"], 1.7) );
        ASSERT_TRUE( equals( recordMap["seattle_topeka"], 1.8) );
        ASSERT_TRUE( equals( recordMap["san-diego_new-york"], 2.5) );
        ASSERT_TRUE( equals( recordMap["san-diego_chicago"], 1.8) );
        ASSERT_TRUE( equals( recordMap["san-diego_topeka"], 1.4) );
    }
    { std::map<std::string, size_t> recordCounter;
        // when
        for(GAMSEquationRecord rec : db.getEquation("demand")) {
            recordCounter[rec.key(0)]++;
        }
        // then
        EXPECT_EQ( recordCounter.size(), size_t(3) );
        EXPECT_EQ( recordCounter["new-york"], size_t(1) );
        EXPECT_EQ( recordCounter["chicago"], size_t(1) );
        EXPECT_EQ( recordCounter["topeka"], size_t(1) );
        recordCounter.clear();
    }
}

TEST_F(TestGAMSSymbolIter, testConstructor_InvalidPosition) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    GAMSSet i = db.getSet("i");
    GAMSSymbolIter<GAMSSet> it_i(i, 1);
    try {
        GAMSSymbolIter<GAMSSet> it_i(i, i.numberRecords()+1);
    } catch(GAMSException&) { ASSERT_TRUE(true); }

    GAMSParameter b = db.getParameter("b");
    GAMSSymbolIter<GAMSParameter> it_b(b, 1);
    try {
        GAMSSymbolIter<GAMSParameter> it_b(b, b.numberRecords()+1);
    } catch(GAMSException&) { ASSERT_TRUE(true); }

    GAMSEquation supply = db.getEquation("supply");
    GAMSSymbolIter<GAMSEquation> it_supply(supply, 1);
    try {
        GAMSSymbolIter<GAMSEquation> it_supply(supply, supply.numberRecords()+1);
    } catch(GAMSException&) { ASSERT_TRUE(true); }

    GAMSVariable x = db.getVariable("x");
    GAMSSymbolIter<GAMSVariable> it_x(x, 1);
    try {
        GAMSSymbolIter<GAMSVariable> it_x(x, x.numberRecords()+1);
    } catch(GAMSException&) { ASSERT_TRUE(true); }
}

TEST_F(TestGAMSSymbolIter, testConstructor_InvalidSymbol) {
    GAMSSet set;
    EXPECT_THROW( GAMSSymbolIter<GAMSSet> it(set, 0), GAMSException);
    EXPECT_THROW( GAMSSymbolIter<GAMSSet> it(set, -1), GAMSException);
    EXPECT_THROW( GAMSSymbolIter<GAMSSet> it(set, 100), GAMSException);
    try {
        for(GAMSSetRecord rec : set) { QFAIL("Invalid GAMSSet contains no record!");  }
    } catch(GAMSException&) { ASSERT_TRUE(true); }

    GAMSParameter par;
    EXPECT_THROW( GAMSSymbolIter<GAMSParameter> it(par, 0), GAMSException);
    EXPECT_THROW( GAMSSymbolIter<GAMSParameter> it(par, -1), GAMSException);
    EXPECT_THROW( GAMSSymbolIter<GAMSParameter> it(par, 100), GAMSException);
    try {
        for(GAMSParameterRecord rec : par) { QFAIL("Invalid GAMSParameter contains no record!");  }
    } catch(GAMSException&) { ASSERT_TRUE(true); }

    GAMSEquation eq;
    EXPECT_THROW( GAMSSymbolIter<GAMSEquation> it(eq, 0), GAMSException);
    EXPECT_THROW( GAMSSymbolIter<GAMSEquation> it(eq, -1), GAMSException);
    EXPECT_THROW( GAMSSymbolIter<GAMSEquation> it(eq, 100), GAMSException);
    try {
        for(GAMSEquationRecord rec : eq) { QFAIL("Invalid GAMSEquation contains no record!");  }
    } catch(GAMSException&) { ASSERT_TRUE(true); }

    GAMSVariable var;
    EXPECT_THROW( GAMSSymbolIter<GAMSVariable> it(var, 0), GAMSException);
    EXPECT_THROW( GAMSSymbolIter<GAMSVariable> it(var, -1), GAMSException);
    EXPECT_THROW( GAMSSymbolIter<GAMSVariable> it(var, 100), GAMSException);
    try {
        for(GAMSVariableRecord rec : var) { QFAIL("Invalid GAMSVariable contains no record!");  }
    } catch(GAMSException&) { ASSERT_TRUE(true); }

}

TEST_F(TestGAMSSymbolIter, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    {  GAMSSet set_i = db.getSet("i");
        // when
        GAMSSymbolIter<GAMSSet> it_i1(set_i, 0);
        GAMSSymbolIter<GAMSSet> it_i2(set_i, 0);
        // then
        ASSERT_TRUE( it_i1 == it_i2 );

        GAMSSet i = db.getSet("i");
        // when
        GAMSSymbolIter<GAMSSet> it_i3(i, 0);
        // then
        ASSERT_TRUE( it_i1 == it_i3 );

        ++it_i3;
        // then
        ASSERT_TRUE( !(it_i1 == it_i3) );
        // when
        GAMSSymbolIter<GAMSSet> it_i4(i, i.numberRecords());
        // then
        ASSERT_TRUE( !(it_i1 == it_i4) );
        ASSERT_TRUE( !(it_i3 == it_i4) );
    }
    {  GAMSParameter par_d = db.getParameter("d");
        // when
        GAMSSymbolIter<GAMSParameter> it_d1(par_d, 0);
        GAMSSymbolIter<GAMSParameter> it_d2(par_d, 0);
        // then
        ASSERT_TRUE( it_d1 == it_d2 );

        GAMSParameter d = db.getParameter("d");
        // when
        GAMSSymbolIter<GAMSParameter> it_d3(d, 0);
        // then
        ASSERT_TRUE( it_d1 == it_d3 );

        ++it_d3;
        // then
        ASSERT_TRUE( !(it_d1 == it_d3) );
        // when
        GAMSSymbolIter<GAMSParameter> it_d4(d, d.numberRecords());
        // then
        ASSERT_TRUE( !(it_d1 == it_d4) );
        ASSERT_TRUE( !(it_d3 == it_d4) );
    }
    {  GAMSEquation eq_supply = db.getEquation("supply");
        // when
        GAMSSymbolIter<GAMSEquation> it_s1(eq_supply, 0);
        GAMSSymbolIter<GAMSEquation> it_s2(eq_supply, 0);
        // then
        ASSERT_TRUE( it_s1 == it_s2 );

        GAMSEquation supply = db.getEquation("supply");
        // when
        GAMSSymbolIter<GAMSEquation> it_s3(supply, 0);
        // then
        ASSERT_TRUE( it_s1 == it_s3 );

        ++it_s3;
        // then
        ASSERT_TRUE( !(it_s1 == it_s3) );
        // when
        GAMSSymbolIter<GAMSEquation> it_s4(supply, supply.numberRecords());
        // then
        ASSERT_TRUE( !(it_s1 == it_s4) );
        ASSERT_TRUE( !(it_s3 == it_s4) );
    }
    {  GAMSVariable var_x = db.getVariable("x");
        // when
        GAMSSymbolIter<GAMSVariable> it_x1(var_x, 0);
        GAMSSymbolIter<GAMSVariable> it_x2(var_x, 0);
        // then
        ASSERT_TRUE( it_x1 == it_x2 );

        GAMSVariable x = db.getVariable("x");
        // when
        GAMSSymbolIter<GAMSVariable> it_x3(x, 0);
        // then
        ASSERT_TRUE( it_x1 == it_x3 );

        ++it_x3;
        // then
        ASSERT_TRUE( !(it_x1 == it_x3) );
        // when
        GAMSSymbolIter<GAMSVariable> it_x4(x, x.numberRecords());
        // then
        ASSERT_TRUE( !(it_x1 == it_x4) );
        ASSERT_TRUE( !(it_x3 == it_x4) );
    }
}

TEST_F(TestGAMSSymbolIter, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
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
        ASSERT_TRUE( it_i != it_j1 );

        ++it_i;
        ++it_j1;
        // then
        ASSERT_TRUE( it_i != it_j1 );
        // when

        GAMSSymbolIter<GAMSSet> it_j2(set_j, set_j.numberRecords());
        // then
        ASSERT_TRUE( it_j1 != it_j2 );
    }
    { GAMSParameter par_a = db.getParameter("a");
        GAMSParameter par_b = db.getParameter("b");
        // when
        GAMSSymbolIter<GAMSParameter> it_a(par_a, 0);
        GAMSSymbolIter<GAMSParameter> it_b1(par_b, 0);
        // then
        ASSERT_TRUE( it_a != it_b1 );

        ++it_a;
        ++it_b1;
        // then
        ASSERT_TRUE( it_a != it_b1 );
        // when

        GAMSSymbolIter<GAMSParameter> it_b2(par_b, par_b.numberRecords());
        // then
        ASSERT_TRUE( it_b1 != it_b2 );
    }
    { GAMSVariable var_z= db.getVariable("z");
        GAMSVariable var_x = db.getVariable("x");
        // when
        GAMSSymbolIter<GAMSVariable> it_z(var_z, 0);
        GAMSSymbolIter<GAMSVariable> it_x1(var_x, 0);
        // then
        ASSERT_TRUE( it_z != it_x1 );

        ++it_x1;
        // then
        ASSERT_TRUE( it_z != it_x1 );
        // when

        GAMSSymbolIter<GAMSVariable> it_x2(var_x, var_x.numberRecords());
        // then
        ASSERT_TRUE( it_x1 != it_x2 );
    }
}

TEST_F(TestGAMSSymbolIter, testPointerOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    { GAMSSet set_i = db.getSet("i");
        // when
        GAMSSymbolIter<GAMSSet> it_i(set_i, 0);
        // then
        EXPECT_EQ( (*it_i).key(0).c_str(), "seattle");
    }

    { GAMSParameter par_d = db.getParameter("d");
        // when
        GAMSSymbolIter<GAMSParameter> it_d(par_d, 0);
        ++it_d;
        // then
        EXPECT_EQ( (*it_d).key(0).c_str(), "seattle");
        EXPECT_EQ( (*it_d).key(1).c_str(), "chicago");
        // when
        ++it_d;
        ++it_d;
        // then
        EXPECT_EQ( (*it_d).key(0).c_str(), "san-diego");
        EXPECT_EQ( (*it_d).key(1).c_str(), "new-york");
    }

    { GAMSVariable var_z = db.getVariable("z");
        // when
        GAMSSymbolIter<GAMSVariable> it_z(var_z, 0);
        // then
        ASSERT_TRUE( equals((*it_z).level(), 153.675) );
    }

    { GAMSEquation eq_demand = db.getEquation("demand");
        // when
        GAMSSymbolIter<GAMSEquation> it_demand(eq_demand, 0);
        // then
        ASSERT_TRUE( equals((*it_demand).level(), 325.0) );
    }
}


TEST_F(TestGAMSSymbolIter, testPointerOperator_BeyondLastRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    {
        GAMSSet set_i = db.getSet("i");
        GAMSSymbolIter<GAMSSet> it_i(set_i, set_i.numberRecords());
        // when, then
        EXPECT_THROW( (*it_i).key(0), GAMSException);
    }

    {
        GAMSParameter par_f = db.getParameter("f");
        GAMSSymbolIter<GAMSParameter> it_f(par_f, par_f.numberRecords());
        // when, then
        EXPECT_THROW( (*it_f).value(), GAMSException);
        // when
        ++it_f;
        // then
        EXPECT_THROW( (*it_f).value(), GAMSException);
    }

    {
        GAMSEquation eq_demand = db.getEquation("demand");
        GAMSSymbolIter<GAMSEquation> it_demand(eq_demand, eq_demand.numberRecords());
        // when, then
        EXPECT_THROW( (*it_demand).upper(), GAMSException);
    }

    {
        GAMSVariable var_z = db.getVariable("z");
        GAMSSymbolIter<GAMSVariable> it_z(var_z, var_z.numberRecords());
        // when, then
        EXPECT_THROW( (*it_z).marginal(), GAMSException);
    }
}

TEST_F(TestGAMSSymbolIter, testIncrementOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    { GAMSSet set_i = db.getSet("i");
        GAMSSymbolIter<GAMSSet> it_i(set_i, 0);
        // when
        ++it_i;
        // then
        EXPECT_EQ( (*it_i).key(0).c_str(), "san-diego");
    }
    { GAMSParameter par_d = db.getParameter("d");
        GAMSSymbolIter<GAMSParameter> it_d(par_d, 0);
        // when
        ++it_d;
        // then
        EXPECT_EQ( (*it_d).key(0).c_str(), "seattle");
        EXPECT_EQ( (*it_d).key(1).c_str(), "chicago");
        // when
        ++it_d;
        ++it_d;
        // then
        EXPECT_EQ( (*it_d).key(0).c_str(), "san-diego");
        EXPECT_EQ( (*it_d).key(1).c_str(), "new-york");
    }
    { GAMSVariable var_x = db.getVariable("x");
        GAMSSymbolIter<GAMSVariable> it_x(var_x, 0);
        // when
        ++it_x;
        ++it_x;
        ++it_x;
        ++it_x;
        // then
        EXPECT_EQ( (*it_x).key(0).c_str(), "san-diego");
        EXPECT_EQ( (*it_x).key(1).c_str(), "chicago");
    }
    { GAMSEquation eq_demand = db.getEquation("demand");
        GAMSSymbolIter<GAMSEquation> it_demand(eq_demand, 0);
        ++it_demand;
        EXPECT_EQ( (*it_demand).key(0).c_str(), "chicago");
    }
}


