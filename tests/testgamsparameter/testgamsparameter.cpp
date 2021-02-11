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
#include "gamsparameter.h"
#include "gamsparameterrecord.h"
#include "gamsset.h"
#include "gamsvariable.h"
#include "gamsworkspace.h"
#include "gamsworkspaceinfo.h"
#include "testgamsparameter.h"

using namespace gams;

QString TestGAMSParameter::classname()  { return "TestGAMSParameter"; }

void TestGAMSParameter::testDefaultConstructor() {
    // when
    GAMSParameter p;
    // then
    QVERIFY( ! p.isValid() );
    QVERIFY_EXCEPTION_THROWN( p.type(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( p.logID(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( p.dim(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( p.checkDomains(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( p.numberRecords(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( p.firstRecord(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( p.addRecord("x"), GAMSException );
}

void TestGAMSParameter::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    int numberOfSymbols = db.getNrSymbols();
    GAMSParameter a = db.getParameter("a");
    // when
    GAMSParameter capacity( a );
    // then
    QCOMPARE( capacity.name(), a.name() );
    QCOMPARE( capacity.numberRecords(), a.numberRecords() );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );
    // when
    a.addRecord("Albuquerque");
    // then
    QCOMPARE( capacity.findRecord("Albuquerque").key(0).c_str(), "Albuquerque" );
    QCOMPARE( capacity.numberRecords(), a.numberRecords() );
}

void TestGAMSParameter::testCopyConstructor_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    QVERIFY_EXCEPTION_THROWN( GAMSParameter param_i( db.getSet("i") ), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSParameter param_z( db.getVariable("z") ), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSParameter param_cost( db.getEquation("cost")), GAMSException);
}

void TestGAMSParameter::testIterator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    // when, then
    GAMSParameter b;
    QVERIFY_EXCEPTION_THROWN( b.begin(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( b.end(), GAMSException);

    TestGAMSObject::getTestData_Parameter_demand_b( db );
    b = db.getParameter("b");
    std::map<std::string, double> symbolMap;
    for (GAMSParameterRecord rec : b) {
        symbolMap[rec.key(0)] = rec.value() ;
    }
    // then
    QCOMPARE( symbolMap.size(), size_t(3) );
    QCOMPARE( symbolMap["New-York"], 325.0 );
    QCOMPARE( symbolMap["Chicago"], 300.0 );
    QCOMPARE( symbolMap["Topeka"], 275.0 );
}

void TestGAMSParameter::testAssignmentOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameter a = db.getParameter("a");
    int numberOfSymbols = db.getNrSymbols();

    // when, then
    GAMSParameter capacity = a;
    QCOMPARE( capacity, a );
    QVERIFY( capacity == a );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );
}

void TestGAMSParameter::testAssignmentOperator_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    QVERIFY_EXCEPTION_THROWN( GAMSParameter i = db.getSet("i"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSParameter x = db.getVariable("x"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSParameter supply = db.getEquation("supply"), GAMSException);
}

void TestGAMSParameter::testGetFirstRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameter a = db.getParameter("a");

    // when, then
    GAMSParameterRecord rec = a.firstRecord();
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "Seattle", Qt::CaseSensitive ), 0 );
    QVERIFY( equals(rec.value(), 350.0) );
}

void TestGAMSParameter::testGetFirstRecordSlice() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b(db);
    GAMSParameter b = db.getParameter("b");

    // when, then
    GAMSParameterRecord rec = b.firstRecord("Chicago");
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "Chicago", Qt::CaseSensitive ), 0 );
    QVERIFY( equals(rec.value(), 300.0) );
}

void TestGAMSParameter::testGetFirstRecordSlice_InvalidKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameter a = db.getParameter("a");

    // when InvalidKeys, then
    QVERIFY_EXCEPTION_THROWN( a.firstRecord("Albuquerque"), GAMSException );
    // when IncorrectDimension, then
    QVERIFY_EXCEPTION_THROWN( a.firstRecord("Seattle", "Albuquerque"), GAMSException);
}

void TestGAMSParameter::testGetLastRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameter a = db.getParameter("a");

    // when, then
    GAMSParameterRecord rec = a.lastRecord();
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "San-Diego", Qt::CaseSensitive ), 0 );
    QCOMPARE( rec.value(), 600.0 );
}

void TestGAMSParameter::testGetLastRecordSlice() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");

    // when, then
    GAMSParameterRecord rec = b.lastRecord("New-York");
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "New-York", Qt::CaseSensitive ), 0 );
    QCOMPARE( rec.value(), 325.0 );
}

void TestGAMSParameter::testGetLastRecordSlice_InvalidKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");

    // when InvalidKeys, then
    QVERIFY_EXCEPTION_THROWN( b.lastRecord("Albuquerque"), GAMSException);
    // when IncorrectDimension, then
    QVERIFY_EXCEPTION_THROWN( b.lastRecord("seattle", "Albuquerque"), GAMSException);
}

void TestGAMSParameter::testFindRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");

    // when, then
    GAMSParameterRecord rec = db.getParameter("b").findRecord("Topeka");
    QVERIFY( rec.isValid() );
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "Topeka", Qt::CaseSensitive ), 0 );
    QVERIFY( equals(rec.value(), 275.0) );
}

void TestGAMSParameter::testFindRecord_InsensitiveCaseKeys() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );

    // when
    GAMSParameterRecord rec = db.getParameter("b").findRecord("CHICAGO");
    // then
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "chicago", Qt::CaseInsensitive ), 0 );
    QVERIFY( equals(rec.value(), 300.0) );
}

void TestGAMSParameter::testAddRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");
    int numberOfRecords = b.numberRecords();

    // when
    b.addRecord("Albuquerque");

    // then
    QCOMPARE( b.numberRecords(), numberOfRecords+ 1);
    QVERIFY( b.findRecord("Albuquerque").isValid() );
    QVERIFY( b.findRecord("Albuquerque").value() - 0.0 <= defaultEPS );

    // when
    { TestGAMSObject::getTestData_Parameter_freightcost_f( db );
      // when, then
      QVERIFY_EXCEPTION_THROWN( db.getParameter("f").addRecord(), GAMSException );
    }
    {
      TestGAMSObject::getTestData_Parameter_capacity_a( db );
      GAMSParameter a = db.getParameter("a");
      int numberOfRecords = a.numberRecords();
      // when, then
      QVERIFY_EXCEPTION_THROWN( GAMSSetRecord rec = a.addRecord("Albuquerque"), GAMSException);
      QCOMPARE( a.numberRecords(), ++numberOfRecords);
      QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord rec = a.addRecord("Austin"), GAMSException);
      QCOMPARE( a.numberRecords(), ++numberOfRecords);
      QVERIFY_EXCEPTION_THROWN( GAMSEquationRecord rec = a.addRecord("LasVegas"), GAMSException);
      QCOMPARE( a.numberRecords(), ++numberOfRecords);
    }
}

void TestGAMSParameter::testAddRecord_DuplicatedKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");
    int numberOfRecords = b.numberRecords();

    // when DuplicatedKeys, then
    QVERIFY_EXCEPTION_THROWN( db.getParameter("b").addRecord("Topeka"), GAMSException);

    // when IncorrectDimension, then
    QVERIFY_EXCEPTION_THROWN( b.addRecord(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( b.addRecord("i1", "j1"), GAMSException );
    QCOMPARE( b.numberRecords(), numberOfRecords );
}

void TestGAMSParameter::testMergeExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");
    int numberOfRecords = b.numberRecords();

    // when, then
    GAMSParameterRecord rec = b.mergeRecord("Chicago");
    QCOMPARE( b.numberRecords(), numberOfRecords );
    QCOMPARE( rec.key(0).c_str(), "Chicago" );
}

void TestGAMSParameter::testMergeNonExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameter a = db.getParameter("a");
    int numberOfRecords = a.numberRecords();

    // when, then
    GAMSParameterRecord rec = a.mergeRecord("Albuquerque");
    QCOMPARE( a.numberRecords(), numberOfRecords+1 );
    QCOMPARE( rec.key(0).c_str(), "Albuquerque" );
}

QTEST_MAIN(TestGAMSParameter)
