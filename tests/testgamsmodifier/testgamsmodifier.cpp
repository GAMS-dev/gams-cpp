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

#include "testgamsobject.h"
#include "gamsmodifier.h"
#include "gamsequation.h"
#include "gamsparameter.h"
#include "gamsvariable.h"


using namespace gams;

class TestGAMSModifier: public TestGAMSObject
{
};

TEST_F(TestGAMSModifier, testDefaultConstructor) {
     // when
     GAMSModifier mod;
     // then
     ASSERT_TRUE( ! mod.isValid() );
     EXPECT_THROW( mod.dataSymbol(), GAMSException );
     EXPECT_THROW( mod.gamsSymbol(), GAMSException );
     EXPECT_THROW( mod.updAction(), GAMSException );
     EXPECT_THROW( mod.updType(), GAMSException );
     EXPECT_THROW( mod.isParameter(), GAMSException );
     EXPECT_THROW( mod.isEquation(), GAMSException );
     EXPECT_THROW( mod.isVariable(), GAMSException );
}

TEST_F(TestGAMSModifier, testConstructor_Par) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    { // when
      GAMSModifier mod( db.getParameter("a") );
      // then
      ASSERT_TRUE( mod.isParameter() );
      EXPECT_EQ( mod.updType(), GAMSEnum::SymbolUpdateType::Inherit );
      ASSERT_TRUE( mod.gamsSymbol() == db.getParameter("a") );
      EXPECT_STRCASEEQ( mod.gamsSymbol().firstRecord().key(0).c_str(), "Seattle");
    }
    { // when
      GAMSModifier mod( db.getParameter("b"), GAMSEnum::SymbolUpdateType::BaseCase );
      // then
      ASSERT_TRUE( mod.isParameter() );
      EXPECT_EQ( mod.updType(), GAMSEnum::SymbolUpdateType::BaseCase );
      ASSERT_TRUE( mod.gamsSymbol() == db.getParameter("b") );
      EXPECT_STRCASEEQ( mod.gamsSymbol().firstRecord().key(0).c_str(), "New-York" );
    }
}

TEST_F(TestGAMSModifier, testConstructor_Var) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    { // when
      GAMSModifier mod( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
      // then
      ASSERT_TRUE( mod.isVariable() );
      EXPECT_EQ( mod.updType(), GAMSEnum::SymbolUpdateType::Inherit );
      ASSERT_TRUE( mod.gamsSymbol() == db.getVariable("x") );
      EXPECT_STRCASEEQ( mod.gamsSymbol().firstRecord().key(0).c_str(), "seattle");
      EXPECT_STRCASEEQ( mod.gamsSymbol().firstRecord().key(1).c_str(), "New-York" );
    }
    { // when
      GAMSModifier mod( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Fixed,  db.getParameter("d"),
                        GAMSEnum::SymbolUpdateType::BaseCase );
      // then
      ASSERT_TRUE( mod.isVariable() );
      EXPECT_EQ( mod.updAction(), GAMSEnum::SymbolUpdateAction::Fixed );
      EXPECT_EQ( mod.updType(), GAMSEnum::SymbolUpdateType::BaseCase );
      ASSERT_TRUE( mod.gamsSymbol() == db.getVariable("x") );
      EXPECT_STRCASEEQ( mod.gamsSymbol().firstRecord().key(0).c_str(), "seattle");
      EXPECT_STRCASEEQ( mod.gamsSymbol().firstRecord().key(1).c_str(), "New-York" );
    }
    { // when data symbol of different dim, then
      EXPECT_THROW(
                  GAMSModifier( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("a") ),
                  GAMSException );
      EXPECT_THROW(
                  GAMSModifier( db.getVariable("z"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("a") ),
                  GAMSException );
    }
}

TEST_F(TestGAMSModifier, testConstructor_Equ) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    { // when
      GAMSModifier mod( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
      // then
      ASSERT_TRUE( mod.isEquation());
      EXPECT_EQ( mod.updType(), GAMSEnum::SymbolUpdateType::Inherit );
      ASSERT_TRUE( mod.dataSymbol() == db.getParameter("a") );
      ASSERT_TRUE( mod.gamsSymbol() == db.getEquation("supply") );
      EXPECT_STRCASEEQ( mod.gamsSymbol().firstRecord().key(0).c_str(), "seattle");
    }
    { // when
      GAMSModifier mod( db.getEquation("demand"), GAMSEnum::SymbolUpdateAction::Dual, db.getParameter("b"),
                        GAMSEnum::SymbolUpdateType::Accumulate );
      // then
      ASSERT_TRUE( mod.isEquation());
      EXPECT_EQ( mod.updType(), GAMSEnum::SymbolUpdateType::Accumulate );
      EXPECT_EQ( mod.updAction(), GAMSEnum::SymbolUpdateAction::Dual );
      ASSERT_TRUE( mod.dataSymbol() == db.getParameter("b") );
      ASSERT_TRUE( mod.gamsSymbol() == db.getEquation("demand") );
      EXPECT_STRCASEEQ( mod.gamsSymbol().firstRecord().key(0).c_str(), "New-York" );
    }
    { // when symbolUpdateAction is not allowed on Equation, then
      EXPECT_THROW(
                 GAMSModifier( db.getEquation("demand"), GAMSEnum::SymbolUpdateAction::Fixed, db.getParameter("b")),
                 GAMSException );
      EXPECT_THROW(
                 GAMSModifier( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Upper, db.getParameter("b")),
                 GAMSException );
      EXPECT_THROW(
                 GAMSModifier( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Lower, db.getParameter("b")),
                 GAMSException );
    }
    { // when data symbol of different dim, then
      EXPECT_THROW(
                  GAMSModifier( db.getEquation("demand"), GAMSEnum::SymbolUpdateAction::Fixed, db.getParameter("d")),
                  GAMSException );
      EXPECT_THROW(
                  GAMSModifier( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("f") ),
                  GAMSException );
    }
}

TEST_F(TestGAMSModifier, testAssignmentOperator_Par) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a( db.getParameter("a") );
    GAMSModifier parmod_b( db.getParameter("b") );
    GAMSModifier varmod_x( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
    GAMSModifier eqmod_supply( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
    // when, then
    { GAMSModifier mod = parmod_a;
      ASSERT_TRUE( mod.isValid() );
      ASSERT_TRUE( mod.isParameter() );
      EXPECT_EQ(mod, parmod_a);
    }
    // when, then
    { parmod_b = parmod_a;
      ASSERT_TRUE( parmod_b.gamsSymbol() == db.getParameter("a") );
    }
    // when, then
    { parmod_a = varmod_x;
      ASSERT_TRUE( ! varmod_x.isParameter() );
      ASSERT_TRUE( varmod_x.isVariable() );
      ASSERT_TRUE( varmod_x.gamsSymbol() != db.getParameter("a") );
      ASSERT_TRUE( varmod_x.gamsSymbol() == db.getVariable("x") );
    }
    // when, then
    { parmod_a = eqmod_supply;
      ASSERT_TRUE( ! parmod_a.isParameter() );
      ASSERT_TRUE( parmod_a.isEquation() );
      ASSERT_TRUE( parmod_a.gamsSymbol() != db.getParameter("a") );
      ASSERT_TRUE( parmod_a.gamsSymbol() == db.getEquation("supply") );
    }
}

TEST_F(TestGAMSModifier, testAssignmentOperator_Var) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a( db.getParameter("a") );
    GAMSModifier varmod_x1( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
    GAMSModifier varmod_x2( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
    GAMSModifier eqmod_supply( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
    // when, then
    { GAMSModifier mod = varmod_x1;
      ASSERT_TRUE( mod.isValid() );
      ASSERT_TRUE( mod.isVariable() );
      EXPECT_EQ(mod, varmod_x1);
    }
    // when, then
    { varmod_x1 = varmod_x2;
      ASSERT_TRUE( varmod_x1.gamsSymbol() == db.getVariable("x") );
    }
    // when, then
    { varmod_x1 = parmod_a;
      ASSERT_TRUE( varmod_x1.isParameter() );
      ASSERT_TRUE( ! varmod_x1.isVariable() );
      ASSERT_TRUE( varmod_x1.gamsSymbol() == db.getParameter("a") );
      ASSERT_TRUE( varmod_x1.gamsSymbol() != db.getVariable("x") );
    }
    // when, then
    { varmod_x1 = eqmod_supply;
      ASSERT_TRUE( varmod_x1.isEquation() );
      ASSERT_TRUE( ! varmod_x1.isVariable() );
      ASSERT_TRUE( varmod_x1.gamsSymbol() == db.getEquation("supply") );
      ASSERT_TRUE( varmod_x1.gamsSymbol() != db.getVariable("x") );
    }
}

TEST_F(TestGAMSModifier, testAssignmentOperator_Equ) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a( db.getParameter("a") );
    GAMSModifier varmod_x( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
    GAMSModifier eqmod_supply( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
    GAMSModifier eqmod_demand( db.getEquation("demand"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("b") );
    // when, then
    { GAMSModifier mod = eqmod_supply;
      ASSERT_TRUE( mod.isValid() );
      ASSERT_TRUE( mod.isEquation() );
      EXPECT_EQ(mod, eqmod_supply);
    }
    // when, then
    { eqmod_supply = eqmod_demand;
      ASSERT_TRUE( eqmod_supply.gamsSymbol() == db.getEquation("demand") );
    }
    // when, then
    { eqmod_supply = parmod_a;
      ASSERT_TRUE( eqmod_supply.isParameter() );
      ASSERT_TRUE( ! eqmod_supply.isEquation() );
      ASSERT_TRUE( eqmod_supply.gamsSymbol() == db.getParameter("a") );
      ASSERT_TRUE( eqmod_supply.gamsSymbol() != db.getEquation("supply") );
    }
    // when, then
    { eqmod_supply = varmod_x ;
      ASSERT_TRUE( eqmod_supply.isVariable() );
      ASSERT_TRUE( ! eqmod_supply.isEquation() );
      ASSERT_TRUE( eqmod_supply.gamsSymbol() == db.getVariable("x") );
      ASSERT_TRUE( eqmod_supply.gamsSymbol() != db.getEquation("supply") );
    }
}

TEST_F(TestGAMSModifier, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a1( db.getParameter("a"), GAMSEnum::SymbolUpdateType::BaseCase );
    GAMSModifier parmod_a2( db.getParameter("a"), GAMSEnum::SymbolUpdateType::BaseCase );
    GAMSModifier varmod_x1( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Fixed,  db.getParameter("d") );
    GAMSModifier varmod_x2( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Fixed,  db.getParameter("d") );
    GAMSModifier eqmod_supply1( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
    GAMSModifier eqmod_supply2( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
    // when, then
    {   ASSERT_TRUE( parmod_a1 == parmod_a2 );
        GAMSModifier mod = parmod_a1;
        ASSERT_TRUE( mod == parmod_a2 );
    }
    // when, then
    {   ASSERT_TRUE( varmod_x1 == varmod_x2 );
        GAMSModifier mod = varmod_x1;
        ASSERT_TRUE( mod == varmod_x2 );
    }
    // when, then
    {   ASSERT_TRUE( eqmod_supply1 == eqmod_supply2 );
        GAMSModifier mod = eqmod_supply1;
        ASSERT_TRUE( mod == eqmod_supply2 );
    }
}

TEST_F(TestGAMSModifier, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a1( db.getParameter("a")  );
    GAMSModifier parmod_a2( db.getParameter("a"), GAMSEnum::SymbolUpdateType::BaseCase  );
    GAMSModifier varmod_x1( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Fixed,  db.getParameter("d") );
    GAMSModifier varmod_x2( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Upper,  db.getParameter("d") );
    GAMSModifier eqmod_supply1( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Primal, db.getParameter("a") );
    GAMSModifier eqmod_supply2( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Dual, db.getParameter("a") );
    // when, then
    {   ASSERT_TRUE( parmod_a1 != varmod_x1 );
        ASSERT_TRUE( parmod_a1 != varmod_x2 );
        ASSERT_TRUE( parmod_a1 != parmod_a2 );
        ASSERT_TRUE( parmod_a1 != eqmod_supply1 );
        ASSERT_TRUE( parmod_a1 != eqmod_supply2 );

        ASSERT_TRUE( parmod_a2 != varmod_x1 );
        ASSERT_TRUE( parmod_a2 != varmod_x2 );
        ASSERT_TRUE( parmod_a2 != eqmod_supply1 );
        ASSERT_TRUE( parmod_a2 != eqmod_supply2 );

        ASSERT_TRUE( varmod_x1 != varmod_x2 );
        ASSERT_TRUE( varmod_x1 != eqmod_supply1 );
        ASSERT_TRUE( varmod_x1 != eqmod_supply2 );

        ASSERT_TRUE( varmod_x2 != eqmod_supply1 );
        ASSERT_TRUE( varmod_x2 != eqmod_supply2 );

        ASSERT_TRUE( eqmod_supply1 != eqmod_supply2 );
    }
}

TEST_F(TestGAMSModifier, testIsValid) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    {  GAMSModifier mod( db.getParameter("a") );
       ASSERT_TRUE( mod.isValid() );
    }
    // when, then
    {  GAMSModifier mod( db.getParameter("a") );
       ASSERT_TRUE( mod.isValid() );
    }
    // when, then
    {  GAMSModifier mod( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
       ASSERT_TRUE( mod.isValid() );
    }
}

TEST_F(TestGAMSModifier, testGetGamsSymbol) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a( db.getParameter("a") );
    GAMSModifier varmod_x( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
    GAMSModifier eqmod_supply( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
    // when, then
    ASSERT_TRUE( parmod_a.gamsSymbol() == db.getParameter("a") );
    ASSERT_TRUE( varmod_x.gamsSymbol() == db.getVariable("x") );
    ASSERT_TRUE( eqmod_supply.gamsSymbol() == db.getEquation("supply") );
}

TEST_F(TestGAMSModifier, testGetDataSymbol) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a( db.getParameter("a") );
    GAMSModifier varmod_x( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
    GAMSModifier eqmod_supply( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
    // when, then
    ASSERT_TRUE( parmod_a.dataSymbol() == db.getParameter("a") );
    ASSERT_TRUE( varmod_x.dataSymbol() == db.getParameter("d"));
    ASSERT_TRUE( eqmod_supply.dataSymbol() == db.getParameter("a") );
}

TEST_F(TestGAMSModifier, testGetUpdAction) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a( db.getParameter("a")  );
    GAMSModifier varmod_x1( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Fixed,  db.getParameter("d") );
    GAMSModifier varmod_x2( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Upper,  db.getParameter("d") );
    GAMSModifier eqmod_demand( db.getEquation("demand"), GAMSEnum::SymbolUpdateAction::Primal, db.getParameter("a") );
    GAMSModifier eqmod_supply( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Dual, db.getParameter("a") );
    // when, then
    EXPECT_EQ( parmod_a.updAction(), GAMSEnum::SymbolUpdateAction::Param );
    EXPECT_EQ( varmod_x1.updAction(), GAMSEnum::SymbolUpdateAction::Fixed );
    EXPECT_EQ( varmod_x2.updAction(), GAMSEnum::SymbolUpdateAction::Upper );
    EXPECT_EQ( eqmod_demand.updAction(), GAMSEnum::SymbolUpdateAction::Primal );
    EXPECT_EQ( eqmod_supply.updAction(), GAMSEnum::SymbolUpdateAction::Dual );
}

TEST_F(TestGAMSModifier, testGetUpdType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a( db.getParameter("a"), GAMSEnum::SymbolUpdateType::Default );
    GAMSModifier varmod_x( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Fixed,  db.getParameter("d"),
                            GAMSEnum::SymbolUpdateType::Inherit );
    GAMSModifier eqmod_demand( db.getEquation("demand"), GAMSEnum::SymbolUpdateAction::Primal, db.getParameter("a"),
                            GAMSEnum::SymbolUpdateType::Accumulate );
    GAMSModifier eqmod_supply( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Dual, db.getParameter("a"),
                             GAMSEnum::SymbolUpdateType::BaseCase );
    // when, then
    EXPECT_EQ( parmod_a.updType(), GAMSEnum::SymbolUpdateType::Default );
    EXPECT_EQ( varmod_x.updType(), GAMSEnum::SymbolUpdateType::Inherit );
    EXPECT_EQ( eqmod_demand.updType(), GAMSEnum::SymbolUpdateType::Accumulate );
    EXPECT_EQ( eqmod_supply.updType(), GAMSEnum::SymbolUpdateType::BaseCase );
}

TEST_F(TestGAMSModifier, testIsParameter_Variable_Equation) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    { GAMSModifier mod( db.getParameter("d") );
      ASSERT_TRUE( mod.isParameter() );
      ASSERT_TRUE( ! mod.isVariable() );
      ASSERT_TRUE( ! mod.isEquation() );
    }
    // when, then
    { GAMSModifier mod( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
      ASSERT_TRUE( mod.isVariable() );
      ASSERT_TRUE( ! mod.isParameter() );
      ASSERT_TRUE( ! mod.isEquation() );
    }
    // when, then
    { GAMSModifier mod( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
      ASSERT_TRUE( mod.isEquation() );
      ASSERT_TRUE( ! mod.isVariable() );
      ASSERT_TRUE( ! mod.isParameter() );
    }
}


