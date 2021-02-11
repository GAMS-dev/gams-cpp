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

#include "gamsmodifier.h"
#include "gamsequation.h"
#include "gamsparameter.h"
#include "gamsvariable.h"
#include "testgamsmodifier.h"

#include <QTest>

using namespace gams;

QString TestGAMSModifier::classname()  { return "TestGAMSModifier"; }

void TestGAMSModifier::testDefaultConstructor() {
     // when
     GAMSModifier mod;
     // then
     QVERIFY( ! mod.isValid() );
     QVERIFY_EXCEPTION_THROWN( mod.dataSymbol(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mod.gamsSymbol(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mod.updAction(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mod.updType(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mod.isParameter(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mod.isEquation(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mod.isVariable(), GAMSException );
}

void TestGAMSModifier::testConstructor_Par() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    { // when
      GAMSModifier mod( db.getParameter("a") );
      // then
      QVERIFY( mod.isParameter() );
      QCOMPARE( mod.updType(), GAMSEnum::SymbolUpdateType::Inherit );
      QVERIFY( mod.gamsSymbol() == db.getParameter("a") );
      QCOMPARE( QString::compare( QString(mod.gamsSymbol().firstRecord().key(0).c_str()),
                                QString("Seattle"),
                                Qt::CaseInsensitive ), 0 );
    }
    { // when
      GAMSModifier mod( db.getParameter("b"), GAMSEnum::SymbolUpdateType::BaseCase );
      // then
      QVERIFY( mod.isParameter() );
      QCOMPARE( mod.updType(), GAMSEnum::SymbolUpdateType::BaseCase );
      QVERIFY( mod.gamsSymbol() == db.getParameter("b") );
      QCOMPARE( QString::compare( QString(mod.gamsSymbol().firstRecord().key(0).c_str()),
                                QString("New-York"),
                                Qt::CaseInsensitive ), 0 );
    }
}

void TestGAMSModifier::testConstructor_Var() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    { // when
      GAMSModifier mod( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
      // then
      QVERIFY( mod.isVariable() );
      QCOMPARE( mod.updType(), GAMSEnum::SymbolUpdateType::Inherit );
      QVERIFY( mod.gamsSymbol() == db.getVariable("x") );
      QCOMPARE( QString::compare( QString(mod.gamsSymbol().firstRecord().key(0).c_str()),
                                QString("Seattle"),
                                Qt::CaseInsensitive ), 0 );
      QCOMPARE( QString::compare( QString(mod.gamsSymbol().firstRecord().key(1).c_str()),
                                QString("new-york"),
                                Qt::CaseInsensitive ), 0 );
    }
    { // when
      GAMSModifier mod( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Fixed,  db.getParameter("d"),
                        GAMSEnum::SymbolUpdateType::BaseCase );
      // then
      QVERIFY( mod.isVariable() );
      QCOMPARE( mod.updAction(), GAMSEnum::SymbolUpdateAction::Fixed );
      QCOMPARE( mod.updType(), GAMSEnum::SymbolUpdateType::BaseCase );
      QVERIFY( mod.gamsSymbol() == db.getVariable("x") );
      QCOMPARE( QString::compare( QString(mod.gamsSymbol().firstRecord().key(0).c_str()),
                                QString("Seattle"),
                                Qt::CaseInsensitive ), 0 );
      QCOMPARE( QString::compare( QString(mod.gamsSymbol().firstRecord().key(1).c_str()),
                                QString("new-york"),
                                Qt::CaseInsensitive ), 0 );
    }
    { // when data symbol of different dim, then
      QVERIFY_EXCEPTION_THROWN(
                  GAMSModifier( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("a") ),
                  GAMSException );
      QVERIFY_EXCEPTION_THROWN(
                  GAMSModifier( db.getVariable("z"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("a") ),
                  GAMSException );
    }
}

void TestGAMSModifier::testConstructor_Equ() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    { // when
      GAMSModifier mod( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
      // then
      QVERIFY( mod.isEquation());
      QCOMPARE( mod.updType(), GAMSEnum::SymbolUpdateType::Inherit );
      QVERIFY( mod.dataSymbol() == db.getParameter("a") );
      QVERIFY( mod.gamsSymbol() == db.getEquation("supply") );
      QCOMPARE( QString::compare( QString(mod.gamsSymbol().firstRecord().key(0).c_str()),
                                  QString("Seattle"),
                                  Qt::CaseInsensitive ), 0 );
    }
    { // when
      GAMSModifier mod( db.getEquation("demand"), GAMSEnum::SymbolUpdateAction::Dual, db.getParameter("b"),
                        GAMSEnum::SymbolUpdateType::Accumulate );
      // then
      QVERIFY( mod.isEquation());
      QCOMPARE( mod.updType(), GAMSEnum::SymbolUpdateType::Accumulate );
      QCOMPARE( mod.updAction(), GAMSEnum::SymbolUpdateAction::Dual );
      QVERIFY( mod.dataSymbol() == db.getParameter("b") );
      QVERIFY( mod.gamsSymbol() == db.getEquation("demand") );
      QCOMPARE( QString::compare( QString(mod.gamsSymbol().firstRecord().key(0).c_str()),
                                  QString("New-York"),
                                  Qt::CaseInsensitive ), 0 );
    }
    { // when symbolUpdateAction is not allowed on Equation, then
      QVERIFY_EXCEPTION_THROWN(
                 GAMSModifier( db.getEquation("demand"), GAMSEnum::SymbolUpdateAction::Fixed, db.getParameter("b")),
                 GAMSException );
      QVERIFY_EXCEPTION_THROWN(
                 GAMSModifier( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Upper, db.getParameter("b")),
                 GAMSException );
      QVERIFY_EXCEPTION_THROWN(
                 GAMSModifier( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Lower, db.getParameter("b")),
                 GAMSException );
    }
    { // when data symbol of different dim, then
      QVERIFY_EXCEPTION_THROWN(
                  GAMSModifier( db.getEquation("demand"), GAMSEnum::SymbolUpdateAction::Fixed, db.getParameter("d")),
                  GAMSException );
      QVERIFY_EXCEPTION_THROWN(
                  GAMSModifier( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("f") ),
                  GAMSException );
    }
}

void TestGAMSModifier::testAssignmentOperator_Par() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
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
      QVERIFY( mod.isValid() );
      QVERIFY( mod.isParameter() );
      QCOMPARE(mod, parmod_a);
    }
    // when, then
    { parmod_b = parmod_a;
      QVERIFY( parmod_b.gamsSymbol() == db.getParameter("a") );
    }
    // when, then
    { parmod_a = varmod_x;
      QVERIFY( ! varmod_x.isParameter() );
      QVERIFY( varmod_x.isVariable() );
      QVERIFY( varmod_x.gamsSymbol() != db.getParameter("a") );
      QVERIFY( varmod_x.gamsSymbol() == db.getVariable("x") );
    }
    // when, then
    { parmod_a = eqmod_supply;
      QVERIFY( ! parmod_a.isParameter() );
      QVERIFY( parmod_a.isEquation() );
      QVERIFY( parmod_a.gamsSymbol() != db.getParameter("a") );
      QVERIFY( parmod_a.gamsSymbol() == db.getEquation("supply") );
    }
}

void TestGAMSModifier::testAssignmentOperator_Var() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
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
      QVERIFY( mod.isValid() );
      QVERIFY( mod.isVariable() );
      QCOMPARE(mod, varmod_x1);
    }
    // when, then
    { varmod_x1 = varmod_x2;
      QVERIFY( varmod_x1.gamsSymbol() == db.getVariable("x") );
    }
    // when, then
    { varmod_x1 = parmod_a;
      QVERIFY( varmod_x1.isParameter() );
      QVERIFY( ! varmod_x1.isVariable() );
      QVERIFY( varmod_x1.gamsSymbol() == db.getParameter("a") );
      QVERIFY( varmod_x1.gamsSymbol() != db.getVariable("x") );
    }
    // when, then
    { varmod_x1 = eqmod_supply;
      QVERIFY( varmod_x1.isEquation() );
      QVERIFY( ! varmod_x1.isVariable() );
      QVERIFY( varmod_x1.gamsSymbol() == db.getEquation("supply") );
      QVERIFY( varmod_x1.gamsSymbol() != db.getVariable("x") );
    }
}

void TestGAMSModifier::testAssignmentOperator_Equ() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
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
      QVERIFY( mod.isValid() );
      QVERIFY( mod.isEquation() );
      QCOMPARE(mod, eqmod_supply);
    }
    // when, then
    { eqmod_supply = eqmod_demand;
      QVERIFY( eqmod_supply.gamsSymbol() == db.getEquation("demand") );
    }
    // when, then
    { eqmod_supply = parmod_a;
      QVERIFY( eqmod_supply.isParameter() );
      QVERIFY( ! eqmod_supply.isEquation() );
      QVERIFY( eqmod_supply.gamsSymbol() == db.getParameter("a") );
      QVERIFY( eqmod_supply.gamsSymbol() != db.getEquation("supply") );
    }
    // when, then
    { eqmod_supply = varmod_x ;
      QVERIFY( eqmod_supply.isVariable() );
      QVERIFY( ! eqmod_supply.isEquation() );
      QVERIFY( eqmod_supply.gamsSymbol() == db.getVariable("x") );
      QVERIFY( eqmod_supply.gamsSymbol() != db.getEquation("supply") );
    }
}

void TestGAMSModifier::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
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
    {   QVERIFY( parmod_a1 == parmod_a2 );
        GAMSModifier mod = parmod_a1;
        QVERIFY( mod == parmod_a2 );
    }
    // when, then
    {   QVERIFY( varmod_x1 == varmod_x2 );
        GAMSModifier mod = varmod_x1;
        QVERIFY( mod == varmod_x2 );
    }
    // when, then
    {   QVERIFY( eqmod_supply1 == eqmod_supply2 );
        GAMSModifier mod = eqmod_supply1;
        QVERIFY( mod == eqmod_supply2 );
    }
}

void TestGAMSModifier::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
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
    {   QVERIFY( parmod_a1 != varmod_x1 );
        QVERIFY( parmod_a1 != varmod_x2 );
        QVERIFY( parmod_a1 != parmod_a2 );
        QVERIFY( parmod_a1 != eqmod_supply1 );
        QVERIFY( parmod_a1 != eqmod_supply2 );

        QVERIFY( parmod_a2 != varmod_x1 );
        QVERIFY( parmod_a2 != varmod_x2 );
        QVERIFY( parmod_a2 != eqmod_supply1 );
        QVERIFY( parmod_a2 != eqmod_supply2 );

        QVERIFY( varmod_x1 != varmod_x2 );
        QVERIFY( varmod_x1 != eqmod_supply1 );
        QVERIFY( varmod_x1 != eqmod_supply2 );

        QVERIFY( varmod_x2 != eqmod_supply1 );
        QVERIFY( varmod_x2 != eqmod_supply2 );

        QVERIFY( eqmod_supply1 != eqmod_supply2 );
    }
}

void TestGAMSModifier::testIsValid() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    {  GAMSModifier mod( db.getParameter("a") );
       QVERIFY( mod.isValid() );
    }
    // when, then
    {  GAMSModifier mod( db.getParameter("a") );
       QVERIFY( mod.isValid() );
    }
    // when, then
    {  GAMSModifier mod( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
       QVERIFY( mod.isValid() );
    }
}

void TestGAMSModifier::testGetGamsSymbol() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a( db.getParameter("a") );
    GAMSModifier varmod_x( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
    GAMSModifier eqmod_supply( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
    // when, then
    QVERIFY( parmod_a.gamsSymbol() == db.getParameter("a") );
    QVERIFY( varmod_x.gamsSymbol() == db.getVariable("x") );
    QVERIFY( eqmod_supply.gamsSymbol() == db.getEquation("supply") );
}

void TestGAMSModifier::testGetDataSymbol() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSModifier parmod_a( db.getParameter("a") );
    GAMSModifier varmod_x( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
    GAMSModifier eqmod_supply( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
    // when, then
    QVERIFY( parmod_a.dataSymbol() == db.getParameter("a") );
    QVERIFY( varmod_x.dataSymbol() == db.getParameter("d"));
    QVERIFY( eqmod_supply.dataSymbol() == db.getParameter("a") );
}

void TestGAMSModifier::testGetUpdAction() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
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
    QCOMPARE( parmod_a.updAction(), GAMSEnum::SymbolUpdateAction::Param );
    QCOMPARE( varmod_x1.updAction(), GAMSEnum::SymbolUpdateAction::Fixed );
    QCOMPARE( varmod_x2.updAction(), GAMSEnum::SymbolUpdateAction::Upper );
    QCOMPARE( eqmod_demand.updAction(), GAMSEnum::SymbolUpdateAction::Primal );
    QCOMPARE( eqmod_supply.updAction(), GAMSEnum::SymbolUpdateAction::Dual );
}

void TestGAMSModifier::testGetUpdType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
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
    QCOMPARE( parmod_a.updType(), GAMSEnum::SymbolUpdateType::Default );
    QCOMPARE( varmod_x.updType(), GAMSEnum::SymbolUpdateType::Inherit );
    QCOMPARE( eqmod_demand.updType(), GAMSEnum::SymbolUpdateType::Accumulate );
    QCOMPARE( eqmod_supply.updType(), GAMSEnum::SymbolUpdateType::BaseCase );
}

void TestGAMSModifier::testIsParameter_Variable_Equation() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    { GAMSModifier mod( db.getParameter("d") );
      QVERIFY( mod.isParameter() );
      QVERIFY( ! mod.isVariable() );
      QVERIFY( ! mod.isEquation() );
    }
    // when, then
    { GAMSModifier mod( db.getVariable("x"), GAMSEnum::SymbolUpdateAction::Param,  db.getParameter("d") );
      QVERIFY( mod.isVariable() );
      QVERIFY( ! mod.isParameter() );
      QVERIFY( ! mod.isEquation() );
    }
    // when, then
    { GAMSModifier mod( db.getEquation("supply"), GAMSEnum::SymbolUpdateAction::Param, db.getParameter("a") );
      QVERIFY( mod.isEquation() );
      QVERIFY( ! mod.isVariable() );
      QVERIFY( ! mod.isParameter() );
    }
}

QTEST_MAIN(TestGAMSModifier)
