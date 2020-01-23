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
#include "gamsparameter.h"
#include "gamsset.h"
#include "gamsvariable.h"
#include "gamssetrecord.h"
#include "testgamsset.h"

using namespace gams;

QString TestGAMSSet::classname()  { return "TestGAMSSet"; }

void TestGAMSSet::testDefaultConstructor() {
    // when
    GAMSSet s;

    // then
    QVERIFY( ! s.isValid() );
    QVERIFY_EXCEPTION_THROWN( s.type(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( s.logID(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( s.dim(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( s.checkDomains(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( s.numberRecords(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( s.firstRecord(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( s.addRecord("x"), GAMSException );
}

void TestGAMSSet::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    int numberOfSymbols = db.getNrSymbols();
    GAMSSet i = db.getSet("i");
    // when
    GAMSSet plant( i );
    // then
    QCOMPARE( plant.name(), i.name() );
    QCOMPARE( plant.numberRecords(), i.numberRecords() );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );
    // when
    i.addRecord("Albuquerque");
    // then
    QCOMPARE( plant.findRecord("Albuquerque").key(0).c_str(), "Albuquerque" );
    QCOMPARE( plant.numberRecords(), i.numberRecords() );
}

void TestGAMSSet::testCopyConstructor_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    QVERIFY_EXCEPTION_THROWN( GAMSSet set_a( db.getParameter("a") ), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSet set_z( db.getVariable("z") ), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSet set_cost( db.getEquation("cost")), GAMSException);
}

void TestGAMSSet::testIterator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    // when, then
    GAMSSet j;
    QVERIFY_EXCEPTION_THROWN( j.begin(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( j.end(), GAMSException);

    TestGAMSObject::getTestData_Set_markets_j( db );
    j = db.getSet("j");
    std::map<std::string, int> symbolCounter;
    for (GAMSSetRecord rec : j) {
        symbolCounter[rec.key(0)]++;
    }
    // then
    QCOMPARE( symbolCounter.size(), size_t(3) );
    QCOMPARE( symbolCounter["New-York"], 1 );
    QCOMPARE( symbolCounter["Chicago"], 1 );
    QCOMPARE( symbolCounter["Topeka"], 1 );
}

void TestGAMSSet::testAssignmentOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    int numberOfSymbols = db.getNrSymbols();

    GAMSSet j = db.getSet("j");
    GAMSSet market = j;

    // when, then
    QCOMPARE( market, j );
    QVERIFY( market == j );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );
}

void TestGAMSSet::testAssignmentOperator_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    QVERIFY_EXCEPTION_THROWN( GAMSSet a = db.getParameter("a"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSet x = db.getVariable("x"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSSet supply = db.getEquation("supply"), GAMSException);
}

void TestGAMSSet::testGetFirstRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    GAMSSet i = db.getSet("i");

    // when, then
    GAMSSetRecord rec = i.firstRecord();
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "seattle", Qt::CaseInsensitive ), 0 );
}

void TestGAMSSet::testGetFirstRecordSlice() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");

    // when, then
    GAMSSetRecord rec = j.firstRecord("topeka");
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "topeka", Qt::CaseInsensitive ), 0 );
}

void TestGAMSSet::testGetFirstRecordSlice_InvalidKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");

    // when InvalidKeys, then
    QVERIFY_EXCEPTION_THROWN( j.firstRecord("Albuquerque"), GAMSException );
    // when IncorrectDimension, then
    QVERIFY_EXCEPTION_THROWN( j.firstRecord("topeka", "Albuquerque"), GAMSException );
}

void TestGAMSSet::testGetLastRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    GAMSSet i = db.getSet("i");

    // when, then
    GAMSSetRecord rec = i.lastRecord();
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "san-diego", Qt::CaseInsensitive ), 0 );
}

void TestGAMSSet::testGetLastRecordSlice() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");

    // when, then
    GAMSSetRecord rec = j.lastRecord("chicago");
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "chicago", Qt::CaseInsensitive ), 0 );
}

void TestGAMSSet::testGetLastRecordSlice_InvalidKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");

    // when InvalidKeys, then
    QVERIFY_EXCEPTION_THROWN( j.lastRecord("Albuquerque"), GAMSException );
    // when IncorrectDimension, then
    QVERIFY_EXCEPTION_THROWN( j.lastRecord("topeka", "Albuquerque"), GAMSException );
}

void TestGAMSSet::testFindRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );

    // when, then
    GAMSSetRecord rec = db.getSet("j").findRecord("Topeka");
    QVERIFY( rec.isValid() );
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "topeka", Qt::CaseInsensitive ), 0 );
}

void TestGAMSSet::testFindRecord_InsensitiveCaseKeys() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );

    // when
    GAMSSetRecord rec = db.getSet("j").findRecord("topeka");
    // then
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "topeka", Qt::CaseInsensitive ), 0 );
}

void TestGAMSSet::testAddRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    {
      TestGAMSObject::getTestData_Set_plants_i( db );
      GAMSSet i = db.getSet("i");
      int numberOfRecords = i.numberRecords();
      // when
      i.addRecord("Albuquerque");
      // then
      QCOMPARE( i.numberRecords(), numberOfRecords+1);
      QVERIFY( i.findRecord("Albuquerque").isValid() );
    }
    {
      TestGAMSObject::getTestData_Set_markets_j( db );
      GAMSSet j = db.getSet("j");
      int numberOfRecords = j.numberRecords();
      // when, then
      QVERIFY_EXCEPTION_THROWN( GAMSParameterRecord rec = j.addRecord("Albuquerque"), GAMSException);
      QCOMPARE( j.numberRecords(), ++numberOfRecords);
      QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord rec = j.addRecord("Austin"), GAMSException);
      QCOMPARE( j.numberRecords(), ++numberOfRecords);
      QVERIFY_EXCEPTION_THROWN( GAMSEquationRecord rec = j.addRecord("LasVegas"), GAMSException);
      QCOMPARE( j.numberRecords(), ++numberOfRecords);
    }
}

void TestGAMSSet::testAddRecord_DuplicatedKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");
    int numberOfRecords = j.numberRecords();

    // when DuplicatedKeys, then
    QVERIFY_EXCEPTION_THROWN( db.getSet("j").addRecord("topeka"), GAMSException);

    // when IncorrectDimension, then
    QVERIFY_EXCEPTION_THROWN( j.addRecord(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( j.addRecord("i1", "j1"), GAMSException );
    QCOMPARE( j.numberRecords(), numberOfRecords );
}

void TestGAMSSet::testMergeExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");
    int numberOfRecords = j.numberRecords();

    // when, then
    GAMSSetRecord rec = j.mergeRecord("Chicago");
    QCOMPARE( j.numberRecords(), numberOfRecords );
    QCOMPARE( rec.key(0).c_str(), "Chicago" );
}

void TestGAMSSet::testMergeNonExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");
    int numberOfRecords = j.numberRecords();

    // when, then
    GAMSSetRecord rec = j.mergeRecord("Albuquerque");
    QCOMPARE( j.numberRecords(), numberOfRecords+1 );
    QCOMPARE( rec.key(0).c_str(), "Albuquerque" );
}

QTEST_MAIN(TestGAMSSet)
