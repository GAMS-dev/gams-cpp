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
#include "gamsequationrecord.h"
#include "gamsparameter.h"
#include "gamsparameterrecord.h"
#include "gamsset.h"
#include "gamssetrecord.h"
#include "gamsvariable.h"
#include "gamsvariablerecord.h"
#include "testgamssetrecord.h"

using namespace gams;

QString TestGAMSSetRecord::classname()  { return "TestGAMSSetRecord"; }

void TestGAMSSetRecord::testDefaultConstructor() {
    // when
    GAMSSetRecord rec;
    // then
    ASSERT_TRUE( ! rec.isValid() );
    QVERIFY_EXCEPTION_THROWN( rec.type(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.logID(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.setText("x"), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.keys(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.moveNext(), GAMSException );
}

void TestGAMSSetRecord::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i(db);
    GAMSSetRecord rec = db.getSet("i").firstRecord();
    // when
    GAMSSetRecord newRecord( rec );
    QCOMPARE( newRecord, rec );
}

void TestGAMSSetRecord::testCopyConstructor_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    QVERIFY_EXCEPTION_THROWN( GAMSSetRecord newRecord( db.getParameter("a").firstRecord() ), GAMSException );
    QVERIFY_EXCEPTION_THROWN( GAMSSetRecord newRecord( db.getVariable("x").firstRecord() ), GAMSException );
    QVERIFY_EXCEPTION_THROWN( GAMSSetRecord newRecord( db.getEquation("demand").firstRecord() ), GAMSException );
}

void TestGAMSSetRecord::testAssignmentOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSSetRecord rec = db.getSet("i").firstRecord();

    // when, then
    GAMSSetRecord newRecord = rec;
    QCOMPARE( newRecord, rec );
    ASSERT_TRUE( newRecord == rec );
}

void TestGAMSSetRecord::testIncorrectType_data() {
    QTest::addColumn<int>("symbolType");
    QTest::addColumn<QString>("symbolID");

    QTest::newRow("distance_d")    << 1 << "d"       ;
    QTest::newRow("freightcost_f") << 1 << "f"       ;
    QTest::newRow("obj_z")         << 2 << "z"       ;
    QTest::newRow("supply")        << 3 << "supply"  ;
}

void TestGAMSSetRecord::testIncorrectType() {
    QFETCH(int, symbolType);
    QFETCH(QString, symbolID);

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    switch(symbolType) {
      case GAMSEnum::SymbolType::SymTypePar :
        {
          GAMSParameter symbol = db.getParameter( symbolID.toStdString() );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSymbolRecord rec2 = GAMSSetRecord( symbol.firstRecord() ), GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSSetRecord r = rec1, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeVar :
        {
          GAMSVariable symbol = db.getVariable( symbolID.toStdString() );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSymbolRecord rec2 = GAMSSetRecord( symbol.firstRecord() ), GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSSetRecord r = rec1, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeEqu :
        {
          GAMSEquation symbol = db.getEquation( symbolID.toStdString() );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSymbolRecord rec2 = GAMSSetRecord( symbol.firstRecord() ), GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSSetRecord r = rec1, GAMSException );
          break;
        }
      default: break;
    }
}

void TestGAMSSetRecord::testGetSetText() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i(db);
    GAMSSetRecord rec = db.getSet("i").firstRecord();
    rec.setText("seattle record");
    QCOMPARE( rec.text().c_str(), "seattle record" );

    GAMSSet j = db.addSet("j", "set j");
    j.addRecord("j1").setText("");
    ASSERT_TRUE(j.lastRecord().text().empty());
}

QTEST_MAIN(TestGAMSSetRecord)

