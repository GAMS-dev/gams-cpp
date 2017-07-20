/**
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
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
#include "testgamsvariablerecord.h"

using namespace gams;

QString TestGAMSVariableRecord::classname()  { return "TestGAMSVariableRecord"; }

void TestGAMSVariableRecord::testDefaultConstructor() {
    // when
    GAMSVariableRecord rec;
    // then
    QVERIFY( ! rec.isValid() );
    QVERIFY_EXCEPTION_THROWN( rec.type(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.logID(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.upper(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.setLevel( 0.0 ), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.keys(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.moveNext(), GAMSException );
}

void TestGAMSVariableRecord::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable x =  db.getVariable("x");
    int numberOfRecords = x.numberRecords();
    GAMSVariableRecord rec = x.firstRecord();

    // when
    GAMSVariableRecord newRecord( rec );
    // then
    QCOMPARE( newRecord, rec );
    QVERIFY( newRecord == rec );
    QCOMPARE( x.numberRecords(), numberOfRecords );
}

void TestGAMSVariableRecord::testCopyConstructor_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord newRecord( db.getSet("i").firstRecord() ), GAMSException );
    QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord newRecord( db.getParameter("d").firstRecord() ), GAMSException );
    QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord newRecord( db.getEquation("demand").firstRecord() ), GAMSException );
}

void TestGAMSVariableRecord::testAssignmentOperator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariableRecord rec = db.getVariable("x").firstRecord();
    // when
    GAMSVariableRecord newRecord = rec;
    QCOMPARE( newRecord, rec );
    QVERIFY( newRecord == rec );
}

void TestGAMSVariableRecord::testIncorrectType_data() {
    QTest::addColumn<int>("symbolType");
    QTest::addColumn<QString>("symbolID");

    QTest::newRow("plants_i")      << 0 << "i"       ;
    QTest::newRow("distance_d")    << 1 << "d"       ;
    QTest::newRow("freightcost_f") << 1 << "f"       ;
    QTest::newRow("supply")        << 3 << "supply"  ;
}

void TestGAMSVariableRecord::testIncorrectType() {
    QFETCH(int, symbolType);
    QFETCH(QString, symbolID);

    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    switch(symbolType) {
      case GAMSEnum::SymbolType::SymTypeSet :  {
          GAMSSet symbol = db.getSet( symbolID.toStdString() );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSymbolRecord rec2 = GAMSVariableRecord( symbol.firstRecord() ), GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord r = rec1, GAMSException );
          break;
      }
      case GAMSEnum::SymbolType::SymTypePar : {
         GAMSParameter symbol = db.getParameter( symbolID.toStdString() );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSymbolRecord rec2 = GAMSVariableRecord( symbol.firstRecord() ), GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord r = rec1, GAMSException );
          break;
      }
      case GAMSEnum::SymbolType::SymTypeEqu : {
          GAMSEquation symbol = db.getEquation( symbolID.toStdString() );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSymbolRecord rec2 = GAMSVariableRecord( symbol.firstRecord() ), GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord r = rec1, GAMSException );
          break;
      }
      default: break;
    }
}

void TestGAMSVariableRecord::testGetSetLevel() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    GAMSVariable varx = db.getVariable("x");
    QVERIFY( equals(varx.findRecord("seattle", "new-york").level(), 50.0) );
    QVERIFY( equals(varx.findRecord("seattle", "chicago").level(), 300.0) );
    QVERIFY( equals(varx.findRecord("seattle", "topeka").level(), 0.0) );
    QVERIFY( equals(varx.findRecord("san-diego", "new-york").level(), 275.0) );
    QVERIFY( equals(varx.findRecord("san-diego", "chicago").level(), 0.0) );
    QVERIFY( equals(varx.findRecord("san-diego", "topeka").level(), 275.0) );

    varx.addRecord("alburquerque", "topeka");
    QVERIFY( equals(varx.findRecord("alburquerque", "topeka").level(), 0.0) );
    // when, then
    GAMSVariable varz = db.getVariable("z");
    varz.firstRecord().setLevel( 123.45 );
    QVERIFY( equals(varz.firstRecord().level(), 123.45) );
}

void TestGAMSVariableRecord::testGetSetMarginal() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    GAMSVariable varx = db.getVariable("x");
    QVERIFY( equals(varx.findRecord("seattle", "new-york").marginal(), 0.0) );
    QVERIFY( equals(varx.findRecord("seattle", "chicago").marginal(), 0.0)  );
    QVERIFY( equals(varx.findRecord("seattle", "topeka").marginal(), 0.036) );
    QVERIFY( equals(varx.findRecord("san-diego", "new-york").marginal(), 0.0) );
    QVERIFY( equals(varx.findRecord("san-diego", "chicago").marginal(), 0.00900000000000001) );
    QVERIFY( equals(varx.findRecord("san-diego", "topeka").marginal(), 0.0) );

    varx.addRecord("alburquerque", "topeka");
    QVERIFY( equals(varx.findRecord("alburquerque", "topeka").level(), 0.0) );

    // when, then
    GAMSVariable varz = db.getVariable("z");
    varz.firstRecord().setMarginal( 0.036 );
    QVERIFY( equals(varz.firstRecord().marginal(), 0.036) );
}

void TestGAMSVariableRecord::testGetSetUpper() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable varz = db.getVariable("z");
    GAMSVariable varx = db.getVariable("x");
    { // when get default Upper Bound on free variable record, then pinf is expected
      QVERIFY( std::isinf(varz.firstRecord().upper()) );
      QVERIFY( std::isinf(varx.firstRecord().upper()) );

      // when get default Lower Bound on positive variable recrod,  then
      QVERIFY( std::isinf(varx.findRecord("seattle", "new-york").upper()) );
      QVERIFY( std::isinf(varx.findRecord("seattle", "chicago").upper()) );
      QVERIFY( std::isinf(varx.findRecord("san-diego", "new-york").upper()) );
      QVERIFY( std::isinf(varx.findRecord("san-diego", "chicago").upper()) );
      QVERIFY( std::isinf(varx.findRecord("san-diego", "topeka").upper()) );
    }
    { // when, then
      varz.firstRecord().setUpper( 0.036 );
      QVERIFY( equals(varz.firstRecord().upper(), 0.036) );
      // when, then
      varx.firstRecord().setUpper( -1 );
      QVERIFY( equals(varx.firstRecord().upper(), -1) );
    }
}

void TestGAMSVariableRecord::testGetSetLower() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable varz = db.getVariable("z");
    GAMSVariable varx = db.getVariable("x");
    { // when get default Lower Bound on free variable record,  then minf is expected
      QVERIFY( std::isinf(-varz.firstRecord().lower()) );
      // when get default Lower Bound on positive variable record,  then 0.0 is expected
      QVERIFY( equals(varx.firstRecord().lower(), 0.0) );

      // when get default Lower Bound on positive variable recrod,  then
      QVERIFY( equals(varx.findRecord("seattle", "new-york").lower(), 0.0) );
      QVERIFY( equals(varx.findRecord("seattle", "chicago").lower(), 0.0) );
      QVERIFY( equals(varx.findRecord("san-diego", "new-york").lower(), 0.0) );
      QVERIFY( equals(varx.findRecord("san-diego", "chicago").lower(), 0.0) );
      QVERIFY( equals(varx.findRecord("san-diego", "topeka").lower(), 0.0) );
    }
    { // when, then
      varz.firstRecord().setLower( 0.036 );
      QVERIFY( equals(varz.firstRecord().lower(), 0.036) );
      // when set lower level on free var to minf, then
      varz.firstRecord().setLower( defaultMINF );
      QVERIFY( std::isinf(-varz.firstRecord().lower()) );
      // when set lower level on positive var to negative value, then
      varx.firstRecord().setLower( -1 );
      QVERIFY( equals(varx.firstRecord().lower(), -1) );
    }
}

void TestGAMSVariableRecord::testGetSetScale() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    GAMSVariable varx = db.getVariable("x");
    QVERIFY( equals(varx.findRecord("seattle", "new-york").scale(), 1.0) );
    QVERIFY( equals(varx.findRecord("seattle", "chicago").scale(), 1.0) );
    QVERIFY( equals(varx.findRecord("seattle", "topeka").scale(), 1.0) );
    QVERIFY( equals(varx.findRecord("san-diego", "new-york").scale(), 1.0) );
    QVERIFY( equals(varx.findRecord("san-diego", "chicago").scale(), 1.0) );
    QVERIFY( equals(varx.findRecord("san-diego", "topeka").scale(), 1.0) );

    varx.addRecord("alburquerque", "topeka");
    QVERIFY( equals(varx.findRecord("alburquerque", "topeka").scale(), 1.0) );
    // when, then
    GAMSVariable varz = db.getVariable("z");
    varz.firstRecord().setScale( 2.0 );
    QVERIFY( equals(varz.firstRecord().scale(), 2.0) );
}

QTEST_MAIN(TestGAMSVariableRecord)
