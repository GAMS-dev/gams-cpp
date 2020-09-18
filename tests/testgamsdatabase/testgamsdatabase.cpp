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

#include "gclgms.h"
#include "gamsenum.h"
#include "gamsdatabase.h"
#include "gamsdatabaseiter.h"
#include "gamsdatabasedomainviolation.h"
#include "gamsequation.h"
#include "gamsparameter.h"
#include "gamsset.h"
#include "gamssymboldomainviolation.h"
#include "gamsvariable.h"
#include "testgamsdatabase.h"

#include <sstream>
#include <string>

using namespace gams;

QString TestGAMSDatabase::classname()  { return "TestGAMSDatabase"; }

void TestGAMSDatabase::testDefaultConstructor() {
    // when
    GAMSDatabase db;
    // then
    ASSERT_TRUE(! db.isValid() );
    QVERIFY_EXCEPTION_THROWN( db.name(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.getNrSymbols(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.clear(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.begin(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.end(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.getSet("x"), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.getParameter("x"), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.getEquation("x"), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.getVariable("x"), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.doExport(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.logID(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( db.workspace(), GAMSException );
}

void TestGAMSDatabase::testAssignmentOperator()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);

    GAMSDatabase db1 = ws.addDatabase();
    TestGAMSDatabase::getTestData_Set_plants_i(db1);
    TestGAMSDatabase::getTestData_Parameter_distance_d(db1);
    // when
    GAMSDatabase db2 = db1;
    QCOMPARE( db2.getNrSymbols(), db1.getNrSymbols());
    QCOMPARE( db2.getSet("i").numberRecords(), 2 );
    QCOMPARE( db2.getParameter("d").numberRecords(), 6 );
    QCOMPARE( db2.logID(), db1.logID() );
}

void TestGAMSDatabase::testOutOfScopeDatabaseAssignement()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1;
    // when assignedDatabaseFrom is out of scope
    {
         GAMSDatabase db2 = ws.addDatabase();
         TestGAMSObject::getTestData_Set_plants_i( db2 );
         TestGAMSObject::getTestData_Parameter_capacity_a( db2 );
         QCOMPARE( db2.getNrSymbols(), 2 );
         db1 = db2;
    }
    // then
    ASSERT_TRUE( db1.isValid() );
    QCOMPARE( db1.getNrSymbols(), 2 );
    QCOMPARE( db1.getSet("i").numberRecords(), 2 );
    QCOMPARE( db1.getParameter("a").numberRecords(), 2 );
    QVERIFY_EXCEPTION_THROWN( db1.getSet( "x" ), GAMSException );
}

void TestGAMSDatabase::testIsValid()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    // when, then
    GAMSDatabase db;
    ASSERT_TRUE( ! db.isValid() );
    // when, then
    db = ws.addDatabase();
    ASSERT_TRUE( db.isValid() );
}

void TestGAMSDatabase::testBegin() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    ASSERT_TRUE( db.isValid() );
    GAMSDatabaseIter it = db.begin();
    QCOMPARE((*it).name().c_str(), "i");
}

void TestGAMSDatabase::testEnd()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    GAMSDatabaseIter it = db.begin();
    GAMSDatabaseIter it_end = db.end();
    for(int i=0; i<db.getNrSymbols(); i++) {
        ++it;
    }
    ASSERT_TRUE( it == it_end );
}

void TestGAMSDatabase::testLogID() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    GAMSDatabase db2 = ws.addDatabase();

    // when, then
    QCOMPARE( db1.logID(), db2.logID() );

    GAMSWorkspaceInfo anotherWsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace anotherws(anotherWsInfo);
    GAMSDatabase db3 = anotherws.addDatabase();
    ASSERT_TRUE( db3.logID() != db2.logID());
}

void TestGAMSDatabase::testGetNrSymbols() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    // when, then
    GAMSDatabase db = ws.addDatabase();
    QCOMPARE( db.getNrSymbols(), 0 );
    // when, then
    TestGAMSObject::getTestData_Set_markets_j(db);
    QCOMPARE( db.getNrSymbols(), 1 );
    // when, then
    TestGAMSObject::getTestData_Set_plants_i(db);
    QCOMPARE( db.getNrSymbols(), 2 );
    // when, then
    TestGAMSObject::getTestData_Parameter_freightcost_f(db);
    QCOMPARE( db.getNrSymbols(), 3 );
}

void TestGAMSDatabase::testSetSuppressAutoDomainChecking_data() {
    QTest::addColumn<bool>("suppressed");
    QTest::addColumn<QString>("gdxfilename");

    QTest::newRow("SuppressedAutoDomainChecking")   << true  << "suppressed.gdx";
    QTest::newRow("UnsuppressedAutoDomainChecking") << false << "unsuppressed.gdx" ;
}

void TestGAMSDatabase::testSetSuppressAutoDomainChecking() {
    QFETCH(bool, suppressed);
    QFETCH(QString, gdxfilename);

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);

    GAMSDatabase db = ws.addDatabase();
    GAMSSet i = db.addSet("i", "");
    i.addRecord("i1");
    GAMSParameter a = db.addParameter("x", "", i);
    a.addRecord("i1").setValue( 0.5 );
    a.addRecord("i2").setValue( 1.2 );
    // when
    db.setSuppressAutoDomainChecking(suppressed);
    // then
    if (suppressed) {
        db.doExport();
        QFileInfo gdxfile(QDir(ws.workingDirectory().c_str()),
                          QString::fromStdString(db.name())+QString::fromStdString(".gdx"));
        ASSERT_TRUE( gdxfile.exists() );
    }  else {
        QVERIFY_EXCEPTION_THROWN( db.doExport( gdxfilename.toStdString() ), GAMSException );
    }
}

void TestGAMSDatabase::testGetSuppressAutoDomainChecking() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    db.setSuppressAutoDomainChecking( true );
    // then
    QCOMPARE( db.suppressAutoDomainChecking() , true );

    // when
    db.setSuppressAutoDomainChecking( false );
    // then
    QCOMPARE( db.suppressAutoDomainChecking() , false );
}

void TestGAMSDatabase::testGetName() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    // when, then
    ASSERT_TRUE( ! QString::fromStdString(db1.name()).isNull() );
    ASSERT_TRUE( ! QString::fromStdString(db1.name()).isEmpty() );

    // when, then
    std::string dbname = "myDatabase";
    GAMSDatabase db2 = ws.addDatabase(dbname);
    QCOMPARE( db2.name(), dbname );
}

void TestGAMSDatabase::testGetWorkspace() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QCOMPARE( db.workspace(), ws );
}

void TestGAMSDatabase::testDoExport() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    db.doExport();
    // then
    QFileInfo gdxfile(QDir(ws.workingDirectory().c_str()),
                      QString::fromStdString(db.name())+QString::fromStdString(".gdx"));
    ASSERT_TRUE( gdxfile.exists() );
    // @todo more check on gdxfile
}

void TestGAMSDatabase::testDoExportWithName() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSDatabase db = ws.addDatabase("capacity");
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    db.doExport("capacity");
    // then
    QFileInfo gdxfile(QDir(ws.workingDirectory().c_str()),
                      QString::fromStdString(db.name())+QString::fromStdString(".gdx"));
    ASSERT_TRUE( gdxfile.exists() );
    // @todo more check on gdxfile
}

void TestGAMSDatabase::testDoExportWithDomainViolation() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase("domain");
    GAMSSet i = db.addSet("i", "");
    i.addRecord("i1");
    GAMSParameter a = db.addParameter("x", "", i);
    a.addRecord("i1").setValue( 0.5 );

    // when
    a.addRecord("i2").setValue( 0.5 );
    // then
    QVERIFY_EXCEPTION_THROWN( db.doExport("domain.gdx"), GAMSException );
}

void TestGAMSDatabase::testCheckDomains() {
    // when
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    // when, then
    GAMSSet i = db.addSet("i", "");
    ASSERT_TRUE( db.checkDomains() );

    // when, then
    i.addRecord("i1");
    GAMSParameter a = db.addParameter("x", "", i);
    a.addRecord("i1").setValue( 0.5 ); ;
    ASSERT_TRUE( db.checkDomains() );

    // when, then
    a.addRecord("i2").setValue( 0.5 ); ;
    ASSERT_TRUE( ! db.checkDomains() );
}

void TestGAMSDatabase::testGetSymbol_data() {
    QTest::addColumn<int>("symbolType");
    QTest::addColumn<QString>("symbolID");
    QTest::addColumn<int>("dimension");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("numberOfRecords");

    QTest::newRow("markets_i")     << 0 << "i"       << 1 << "canning plants" << 2 ;
    QTest::newRow("plants_j")      << 0 << "j"       << 1 << "markets"        << 3 ;
    QTest::newRow("capacity_a")    << 1 << "a"       << 1 << "capacity of plant i in cases" << 2 ;
    QTest::newRow("demand_b")      << 1 << "b"       << 1 << "demand at market j in cases"  << 3 ;
    QTest::newRow("distance_d")    << 1 << "d"       << 2 << "distance in thousands of miles"  << 6 ;
    QTest::newRow("freightcost_f") << 1 << "f"       << 0 << "freight in dollars per case per thousand miles"  << 1 ;
    QTest::newRow("cost_c")        << 1 << "c"       << 2 << "transport cost in thousands of dollars per case" << 6 ;
    QTest::newRow("shipment_x")    << 2 << "x"       << 2 << "shipment quantities in cases" << 6 ;
    QTest::newRow("obj_z")         << 2 << "z"       << 0 << "total transportation costs in thousands of dollars" << 1 ;
    QTest::newRow("supply")        << 3 << "supply"  << 1 << "observe supply limit at plant i" << 2 ;
    QTest::newRow("demand")        << 3 << "demand"  << 1 << "satisfy demand at market j" << 3 ;
}

void TestGAMSDatabase::testGetSymbol() {
    QFETCH(int, symbolType);
    QFETCH(QString, symbolID);
    QFETCH(int, dimension);
    QFETCH(QString, text);
    QFETCH(int, numberOfRecords);

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    switch(symbolType) {
      case GAMSEnum::SymbolType::SymTypeSet :
        {
          GAMSSet set = db.getSet( symbolID.toStdString() );
          QCOMPARE( set.dim(), dimension );
          QCOMPARE( set.text(), text.toStdString() );
          QCOMPARE( set.numberRecords(), numberOfRecords );
          QVERIFY_EXCEPTION_THROWN( db.getParameter( symbolID.toStdString() ), GAMSException);
          QVERIFY_EXCEPTION_THROWN( db.getEquation( symbolID.toStdString() ), GAMSException);
          QVERIFY_EXCEPTION_THROWN( db.getVariable( symbolID.toStdString() ), GAMSException);
        }
        break;
      case GAMSEnum::SymbolType::SymTypePar :
        {
          GAMSParameter param = db.getParameter( symbolID.toStdString() );
          QCOMPARE( param.dim(), dimension );
          QCOMPARE( param.text(), text.toStdString() );
          QCOMPARE( param.numberRecords(), numberOfRecords );
          QVERIFY_EXCEPTION_THROWN( db.getSet( symbolID.toStdString() ), GAMSException);
          QVERIFY_EXCEPTION_THROWN( db.getEquation( symbolID.toStdString() ), GAMSException);
          QVERIFY_EXCEPTION_THROWN( db.getVariable( symbolID.toStdString() ), GAMSException);
          break;
        }
      case GAMSEnum::SymbolType::SymTypeVar :
        {
          GAMSVariable var = db.getVariable( symbolID.toStdString() );
          QCOMPARE( var.dim(), dimension );
          QCOMPARE( var.text(), text.toStdString() );
          QCOMPARE( var.numberRecords(), numberOfRecords );
          QVERIFY_EXCEPTION_THROWN( db.getSet( symbolID.toStdString() ), GAMSException);
          QVERIFY_EXCEPTION_THROWN( db.getEquation( symbolID.toStdString() ), GAMSException);
          QVERIFY_EXCEPTION_THROWN( db.getParameter( symbolID.toStdString() ), GAMSException);
          break;
        }
      case GAMSEnum::SymbolType::SymTypeEqu :
        {
          GAMSEquation eq = db.getEquation( symbolID.toStdString() );
          QCOMPARE( eq.dim(),  dimension );
          QCOMPARE( eq.text(), text.toStdString() );
          QCOMPARE( eq.numberRecords(), numberOfRecords );
          QVERIFY_EXCEPTION_THROWN( db.getSet( symbolID.toStdString() ), GAMSException);
          QVERIFY_EXCEPTION_THROWN( db.getParameter( symbolID.toStdString() ), GAMSException);
          QVERIFY_EXCEPTION_THROWN( db.getVariable( symbolID.toStdString() ), GAMSException);
          break;
        }
      default: break;
    }
}

void TestGAMSDatabase::testAddSet() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    db.addSet("j", 1, "markets");
    // then
    QCOMPARE( db.getSet("j").name().c_str(), "j" );
    QCOMPARE( db.getSet("j").dim(), 1 );
    QCOMPARE( db.getSet("j").text().c_str(), "markets" );
    QVERIFY_EXCEPTION_THROWN( db.addSet("j", 2), GAMSException);
}

void TestGAMSDatabase::testAddSet_NonPositiveDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addSet("i", 0, "set i"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addSet("i", -1, "set i"), GAMSException);
}

void TestGAMSDatabase::testAddSet_ExceedingMaxDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addSet("i", GLOBAL_MAX_INDEX_DIM + 1, "set i"), GAMSException);

    // when
    int max_dim = GLOBAL_MAX_INDEX_DIM;
    GAMSSet max = db.addSet("max", max_dim, "set with maximum dimension");
    // then
    QCOMPARE( max.dim(), max_dim ) ;
}

void TestGAMSDatabase::testAddSet_DuplicateIdentifier() {
     GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
     GAMSWorkspace ws(wsInfo);

     GAMSDatabase db = ws.addDatabase();
     GAMSSet i = db.addSet("i", 1, "canning plants");

     QVERIFY_EXCEPTION_THROWN( db.addSet(i.name(), i.dim(),  i.text()), GAMSException);
     QVERIFY_EXCEPTION_THROWN( db.addSet(i.name(), 2, i.text()), GAMSException);
     QVERIFY_EXCEPTION_THROWN( db.addSet(i.name(), i.dim(),  "new canning plants"), GAMSException);
 }

void TestGAMSDatabase::testAddSet_InvalidStringIdentifier() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addSet("", 1, "empty set"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addSet("*", 1, "star/univerise"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addSet("i j", 1, "white space"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addSet("i*j", 1, "star/universe"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addSet("n!", 1, "exclamation marks"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addSet("n/", 1, "slash"), GAMSException);
}

void TestGAMSDatabase::testAddParameter() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    db.addParameter("b", 1, "demand at market j in cases");
    // then
    QCOMPARE( db.getParameter("b").name().c_str(), "b" );
    QCOMPARE( db.getParameter("b").dim(), 1 );
    QCOMPARE( db.getParameter("b").text().c_str(),"demand at market j in cases" );
}

void TestGAMSDatabase::testAddParameter_NegativeDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addParameter("p", -1, "parameter p"), GAMSException);

    // when
    GAMSParameter p = db.addParameter("p", 0, "parameter p");
    // then
    QCOMPARE( p.dim(), 0 ) ;
}

void TestGAMSDatabase::testAddParameter_ExceedingMaxDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addParameter("p2", GLOBAL_MAX_INDEX_DIM + 1, "parameter p2"), GAMSException);

    // when
    int max_dim = GLOBAL_MAX_INDEX_DIM;
    GAMSParameter max = db.addParameter("max", max_dim, "parameter with maximum dimension");
    QCOMPARE( max.dim(), max_dim );
}

void TestGAMSDatabase::testAddParameter_DuplicateIdentifier() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    db.addParameter("p", 1, "parameter p");
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addParameter("p", 2, "parameter p1"), GAMSException);
}

void TestGAMSDatabase::testAddParameter_InvalidStringIdentifier() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addParameter("", 1, "empty string"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addParameter("*", 1, "star/univerise"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addParameter("i*j", 1, "star/univerise"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addParameter("n!", 1, "exclamation mark"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addParameter("n/", 1, "slash"), GAMSException);
}

void TestGAMSDatabase::testAddVariable() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    db.addVariable("x", 2, GAMSEnum::VarType::Positive, "shipment quantities in cases");
    // then
    QCOMPARE( db.getVariable("x").name().c_str(), "x" );
    QCOMPARE( db.getVariable("x").dim(), 2 );
    QCOMPARE( db.getVariable("x").varType(),  GAMSEnum::VarType::Positive);
    QCOMPARE( db.getVariable("x").text().c_str(),"shipment quantities in cases" );

    // when
    int max_dimension = GLOBAL_MAX_INDEX_DIM;
    db.addVariable("y", max_dimension, GAMSEnum::VarType::Positive, "variable y");
    QCOMPARE( db.getVariable("y").dim(), max_dimension );
}

void TestGAMSDatabase::testAddVariable_NegativeDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addVariable("x",  -1, GAMSEnum::VarType::Positive, "variable x"), GAMSException);

    // when
    GAMSVariable x = db.addVariable("x",  0, GAMSEnum::VarType::Positive, "variable x");
    // then
    QCOMPARE( x.dim(), 0 ) ;
}

void TestGAMSDatabase::testAddVariable_ExceedingMaxDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addVariable("x",  GLOBAL_MAX_INDEX_DIM + 1, GAMSEnum::VarType::Positive, "variable x")
                              , GAMSException);

    // when
    int max_dim = GLOBAL_MAX_INDEX_DIM;
    GAMSVariable max = db.addVariable("max", max_dim, GAMSEnum::VarType::Positive,"variable with maximum dimension");
    QCOMPARE( max.dim(), max_dim ) ;
}

void TestGAMSDatabase::testAddVariable_DuplicateIdentifier() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    db.addVariable("x", 1,  GAMSEnum::VarType::Positive, "variable x");
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addVariable("x", 2, GAMSEnum::VarType::Free, "new variable x"), GAMSException);
}

void TestGAMSDatabase::testAddVariable_InvalidStringIdentifier() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addVariable("", 1, GAMSEnum::VarType::Free, "empty string"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addVariable("*", 1, GAMSEnum::VarType::Free, "whitespace"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addVariable("i j", 1, GAMSEnum::VarType::Free, "variable x"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addVariable("n!", 1, GAMSEnum::VarType::Free, "exclamation mark"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addVariable("n/", 1, GAMSEnum::VarType::Free, "slash"), GAMSException);
}

void TestGAMSDatabase::testAddEquation() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    auto equation = db.addEquation("s", 1, GAMSEnum::EquType::L, "observe supply limit at plant i");
    // then
    QCOMPARE( db.getNrSymbols(), 1);
    try {
        QCOMPARE(equation.name().c_str(), "s");
        QCOMPARE(equation.dim(), 1);
        QCOMPARE(equation.equType(), GAMSEnum::EquType::L);
        QCOMPARE(equation.text().c_str(), "observe supply limit at plant i");
    } catch(GAMSException& e) {
        QFAIL(qPrintable( "Unexpected GAMSException raised by: "+ QString::fromStdString(e.what()) ));
    }
}

void TestGAMSDatabase::testGetEquation() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    auto initEquation = db.addEquation("s", 1, GAMSEnum::EquType::L, "observe supply limit at plant i");
    // then
    QCOMPARE( db.getNrSymbols(), 1);
    try {
        auto getEquation = db.getEquation("s");
        QCOMPARE(initEquation, getEquation);
    } catch(GAMSException& e) {
        QFAIL(qPrintable( "Unexpected GAMSException raised by: "+ QString::fromStdString(e.what()) ));
    }
}

void TestGAMSDatabase::testAddEquation_NegativeDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addEquation("eq",  -1, GAMSEnum::EquType::E, "equation eq")
                              , GAMSException);

    // when
    GAMSEquation eq = db.addEquation("eq", 0, GAMSEnum::EquType::E, "equation eq");
    // then
    QCOMPARE( eq.dim(), 0 ) ;
}

void TestGAMSDatabase::testAddEquation_ExceedingMaxDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addEquation("eq",  GLOBAL_MAX_INDEX_DIM+1, GAMSEnum::EquType::E, "equation eq")
                              , GAMSException);
    // when
    int max_dim = GLOBAL_MAX_INDEX_DIM;
    GAMSEquation max = db.addEquation("max", max_dim, GAMSEnum::EquType::E, "equation with maximum dimension");
    QCOMPARE( max.dim(), max_dim ) ;
}

void TestGAMSDatabase::testAddEquation_DuplicateIdentifier() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    db.addEquation("s", 1, GAMSEnum::EquType::L, "observe supply limit at plant i");
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addEquation("s", 2, GAMSEnum::EquType::E, "new equation s"), GAMSException);
}

void TestGAMSDatabase::testAddEquation_InvalidStringIdentifier() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    QVERIFY_EXCEPTION_THROWN( db.addEquation("", 1, GAMSEnum::EquType::E, "new equation"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addEquation("*", 1, GAMSEnum::EquType::L, "whitespace"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addEquation("i j", 1, GAMSEnum::EquType::G, "variable x"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addEquation("n!", 1, GAMSEnum::EquType::E, "exclamation mark"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.addEquation("n/", 1, GAMSEnum::EquType::L, "slash"), GAMSException);
}

void TestGAMSDatabase::testIterator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    for (GAMSSymbol symbol : db) {
        QFAIL("unexpected symbol from empty database");
        symbol.name();
    }

    // given
    TestGAMSObject::getTestData_TransportModel(db);
    // when
    std::map<std::string, int> symbolCounter;
    for (GAMSSymbol sym : db) {
        switch(sym.type()) {
           case GAMSEnum::SymbolType::SymTypeSet :
               symbolCounter["GAMSSet"]++;
               break;
           case GAMSEnum::SymbolType::SymTypePar :
               symbolCounter["GAMSParameter"]++;
               break;
           default:
               QFAIL("unexpected symbol type of a symbol from transport database");
               break;
       }
    }
    // then
    QCOMPARE( symbolCounter["GAMSSet"], 2 );
    QCOMPARE( symbolCounter["GAMSParameter"], 4 );
    // cleanup
    try {
        db.clear();
    } catch(GAMSException&) {
        QFAIL("Do not expect exception when clearing database");
    }
}

void TestGAMSDatabase::testGetDatabaseDVs() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    QCOMPARE( db.getDatabaseDVs(0, 5).size(), size_t(2) );

    std::map<std::string, size_t> symbolCounter;
    std::map<std::string, size_t> recordCounter;
    for(GAMSDatabaseDomainViolation domViol : db.getDatabaseDVs(0, 5)){
        for(GAMSSymbolDomainViolation rec : domViol.violRecs()) {
            symbolCounter[domViol.violSym().name()]++;
            std::stringstream ss;
            ss << domViol.violSym().name() << "_" << rec.violRec().key(0) ;
            recordCounter[ss.str()]++;
        }
    }
    QCOMPARE( symbolCounter.size(), size_t(2) );
    QCOMPARE( symbolCounter["a"], size_t(2) );
    QCOMPARE( symbolCounter["b"], size_t(1) );
    QCOMPARE( recordCounter.size(), size_t(3) );
    QCOMPARE( recordCounter["a_Alburquerque"], size_t(1) );
    QCOMPARE( recordCounter["a_Sanfrancisco"], size_t(1) );
    QCOMPARE( recordCounter["b_Braunschweig"], size_t(1) );
}

void TestGAMSDatabase::testGetDatabaseDVs_MaxViolation() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    { QCOMPARE( db.getDatabaseDVs(1, 1).size(), size_t(1) );
      std::map<std::string, size_t> symbolCounter;
      std::map<std::string, size_t> recordCounter;
      for(GAMSDatabaseDomainViolation domViol : db.getDatabaseDVs(1, 1)){
          for(GAMSSymbolDomainViolation rec : domViol.violRecs()) {
              symbolCounter[domViol.violSym().name()]++;
              recordCounter[rec.violRec().key(0)]++;
          }
      }
      QCOMPARE( symbolCounter.size(), size_t(1) );
      QCOMPARE( symbolCounter["a"] + symbolCounter["b"], size_t(1) );
      QCOMPARE( recordCounter.size(), size_t(1) );
    }

    // when, then
    for(auto &item : db.getDatabaseDVs(3, 1)){
       QCOMPARE(item.violRecs().size(), size_t(1));
    }
    QCOMPARE(db.getDatabaseDVs(3, 1).size(), size_t(2));


    for(auto &item : db.getDatabaseDVs(2, 1)){
       QCOMPARE(item.violRecs().size(), size_t(1));
    }
    QCOMPARE(db.getDatabaseDVs(2, 1).size(), size_t(2));


    int counter = 0;
    for(auto &item : db.getDatabaseDVs(0, 5)){
        switch (counter) {
            case 0:
                QCOMPARE(item.violRecs().size(), size_t(2));
            break;
            case 1:
                QCOMPARE(item.violRecs().size(), size_t(1));
            break;
        }
        counter++;
    }
    QCOMPARE(db.getDatabaseDVs(0, 5).size(), size_t(2));


    for(auto &item : db.getDatabaseDVs(2, 0)){
       QCOMPARE(item.violRecs().size(), size_t(2));
    }
    QCOMPARE(db.getDatabaseDVs(2, 0).size(), size_t(1));
}


void TestGAMSDatabase::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    TestGAMSObject::getTestData_TransportModel(db1);

    // when, then
    GAMSDatabase db2 = db1;
    ASSERT_TRUE( db2 == db1 );

    // when, then
    GAMSDatabase db3 = ws.addDatabase();
    ASSERT_TRUE( ! (db3 == db1) );

    // when, then
    GAMSDatabase db4 = ws.addDatabase(db1);
    ASSERT_TRUE( ! (db4 == db1) );

}

void TestGAMSDatabase::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    getTestData_TransportModel(db1);

    // when, then
    GAMSDatabase db2 = db1;
    ASSERT_TRUE( !( db2 != db1) );

    // when, then
    GAMSDatabase db3 = ws.addDatabase();
    ASSERT_TRUE( db3 != db1 );

    // when, then
    GAMSDatabase db4 = ws.addDatabase(db1);
    ASSERT_TRUE( db4 != db1 );
}

void TestGAMSDatabase::testClear() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_TransportModel(db);
    int numberOfSymbols = db.getNrSymbols();
    // when
    db.clear();
    // then
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );
    QCOMPARE( db.getSet("i").numberRecords(), 0 );
    QCOMPARE( db.getSet("j").numberRecords(), 0 );
    QCOMPARE( db.getParameter("a").numberRecords(), 0 );
    QCOMPARE( db.getParameter("b").numberRecords(), 0 );
    QCOMPARE( db.getParameter("d").numberRecords(), 0 );
    QCOMPARE( db.getParameter("f").numberRecords(), 0 );
}

void TestGAMSDatabase::testClearOutOfScopedDatabase() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    TestGAMSObject::getTestData_TransportModel( db1 );
    int numberOfSymbols = db1.getNrSymbols();
    // when
    {
       GAMSDatabase db2 = db1;
       db2.clear();
    }
    // then
    QCOMPARE( db1.getNrSymbols(), numberOfSymbols );
    QCOMPARE( db1.getSet("i").numberRecords(), 0 );
    QCOMPARE( db1.getSet("j").numberRecords(), 0 );
    QCOMPARE( db1.getParameter("a").numberRecords(), 0 );
    QCOMPARE( db1.getParameter("b").numberRecords(), 0 );
    QCOMPARE( db1.getParameter("d").numberRecords(), 0 );
    QCOMPARE( db1.getParameter("f").numberRecords(), 0 );
}
QTEST_MAIN(TestGAMSDatabase)
