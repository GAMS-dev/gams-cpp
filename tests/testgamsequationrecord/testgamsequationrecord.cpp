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
#include "gamsparameter.h"
#include "gamsset.h"
#include "gamsvariable.h"
#include "gamsequationrecord.h"
#include "testgamsequationrecord.h"

using namespace gams;

QString TestGAMSEquationRecord::classname()  { return "TestGAMSEquationRecord"; }

void TestGAMSEquationRecord::testDefaultConstructor() {
    // when
    GAMSEquationRecord rec;
    // then
    QVERIFY( ! rec.isValid() );
    QVERIFY_EXCEPTION_THROWN( rec.type(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.logID(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.upper(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.setLevel( 0.0 ), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.keys(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( rec.moveNext(), GAMSException );
}

void TestGAMSEquationRecord::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquation supply =  db.getEquation("supply");
    int numberOfRecords = supply.numberRecords();
    GAMSEquationRecord rec = supply.firstRecord();

    // when
    GAMSEquationRecord newRecord( rec  );
    // then
    QCOMPARE( newRecord, rec );
    QVERIFY( newRecord == rec );
    QCOMPARE( supply.numberRecords(), numberOfRecords );
}


void TestGAMSEquationRecord::testCopyConstructor_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    QVERIFY_EXCEPTION_THROWN( GAMSEquationRecord newRecord( db.getSet("i").firstRecord() ), GAMSException );
    QVERIFY_EXCEPTION_THROWN( GAMSEquationRecord newRecord( db.getParameter("d").firstRecord() ), GAMSException );
    QVERIFY_EXCEPTION_THROWN( GAMSEquationRecord newRecord( db.getVariable("x").firstRecord() ), GAMSException );
}

void TestGAMSEquationRecord::testAssignmentOperator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquationRecord rec = db.getEquation("demand").firstRecord();
    // when
    GAMSEquationRecord newRecord = rec;
    QCOMPARE( newRecord, rec );
    QVERIFY( newRecord == rec );
}

void TestGAMSEquationRecord::testIncorrectType_data() {
    QTest::addColumn<int>("symbolType");
    QTest::addColumn<QString>("symbolID");

    QTest::newRow("plants_i")      << 0 << "i"       ;
    QTest::newRow("distance_d")    << 1 << "d"       ;
    QTest::newRow("freightcost_f") << 1 << "f"       ;
    QTest::newRow("shipment_x")    << 2 << "x"       ;
}

void TestGAMSEquationRecord::testIncorrectType() {
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
      case GAMSEnum::SymbolType::SymTypeSet :
        {
          GAMSSet symbol = db.getSet( symbolID.toStdString() );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSymbolRecord rec2 = GAMSParameterRecord( symbol.firstRecord()), GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSParameterRecord r = rec1, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeVar :
        {
          GAMSVariable symbol = db.getVariable( symbolID.toStdString() );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSymbolRecord rec2 = GAMSParameterRecord( symbol.firstRecord() ), GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSParameterRecord r = rec1, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypePar :
      {
        GAMSParameter symbol = db.getParameter( symbolID.toStdString() );
        GAMSSymbolRecord rec1 = symbol.firstRecord();
        QVERIFY_EXCEPTION_THROWN( GAMSSymbolRecord rec2 = GAMSSetRecord( symbol.firstRecord() ), GAMSException );
        QVERIFY_EXCEPTION_THROWN( GAMSSetRecord r = rec1, GAMSException );
        break;
      }
      default: break;
    }
}

void TestGAMSEquationRecord::testGetSetLevel() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    GAMSEquation supply = db.getEquation("supply");
    QVERIFY( equals(supply.findRecord("seattle").level(), 350.0) );
    QVERIFY( equals(supply.findRecord("san-diego").level(), 550.0) );

    GAMSEquation demand = db.getEquation("demand");
    QVERIFY( equals(demand.findRecord("topeka").level(), 275.0) );
    QVERIFY( equals(demand.findRecord("chicago").level(), 300.0) );
    QVERIFY( equals(demand.findRecord("new-york").level(), 325.0) );

    supply.addRecord("alburquerque");
    // when, then
    QVERIFY( equals(supply.findRecord("alburquerque").level(), 0.0));

    demand.firstRecord().setLevel( 123.45 );
    // when, then
    QVERIFY( equals(demand.firstRecord().level(), 123.45) );

    GAMSEquation cost = db.getEquation("cost");
    // when, then
    QVERIFY( equals(cost.firstRecord().level(), 0.0) );
}

void TestGAMSEquationRecord::testGetSetMarginal() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSEquation supply = db.getEquation("supply");
    QVERIFY( equals( supply.findRecord("seattle").marginal(), defaultEPS ) );
    QVERIFY( equals(supply.findRecord("san-diego").marginal(), 0.0) );

    // when, then
    GAMSEquation demand = db.getEquation("demand");
    QVERIFY( equals(demand.findRecord("topeka").marginal(), 0.126) );
    QVERIFY( equals(demand.findRecord("chicago").marginal(), 0.153) );
    QVERIFY( equals(demand.findRecord("new-york").marginal(), 0.225) );

    // when, then
    GAMSEquation cost = db.getEquation("cost");
    QVERIFY( equals(cost.firstRecord().marginal(), 1.0) );

    // when, then
    cost.firstRecord().setLevel( 1.2345 );
    QVERIFY( equals(cost.firstRecord().level(), 1.2345) );
}

void TestGAMSEquationRecord::testGetSetUpper() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;
    GAMSEquation supply = db.getEquation("supply");
    // when, then
    QVERIFY( equals( supply.findRecord("seattle").upper(), 350.0 ) );
    QVERIFY( equals(supply.findRecord("san-diego").upper(), 600.0) );

    // demand(j) ..   sum(i, x(i,j))  =g=  b(j) ;
    GAMSEquation demand = db.getEquation("demand");
    // when, then
    QVERIFY( std::isinf(demand.findRecord("topeka").upper()) );
    QVERIFY( std::isinf(demand.findRecord("chicago").upper()) );
    QVERIFY( std::isinf(demand.findRecord("new-york").upper()) );

    // cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;
    GAMSEquation cost = db.getEquation("cost");
    // when, then
    QVERIFY( equals(cost.firstRecord().upper(), 0.0) );

    // when, then
    cost.firstRecord().setUpper( 123.45 );
    QVERIFY( equals(cost.firstRecord().upper(), 123.45) );
}

void TestGAMSEquationRecord::testGetSetLower() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;
    GAMSEquation supply = db.getEquation("supply");
    //when, then
    QVERIFY( std::isinf( - supply.findRecord("seattle").lower()) );
    QVERIFY( std::isinf( - supply.findRecord("san-diego").lower()) );

    // demand(j) ..   sum(i, x(i,j))  =g=  b(j) ;
    GAMSEquation demand = db.getEquation("demand");
    // when, then
    QVERIFY( equals(demand.findRecord("new-york").lower(), 325.0) );
    QVERIFY( equals(demand.findRecord("chicago").lower(), 300.0) );
    QVERIFY( equals(demand.findRecord("topeka").lower(), 275.0) );

    // cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;
    GAMSEquation cost = db.getEquation("cost");
    // when , then
    QVERIFY( equals(cost.firstRecord().lower(), 0.0) );
}

void TestGAMSEquationRecord::testGetSetScale() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSEquation supply = db.getEquation("supply");
    QVERIFY( equals( supply.findRecord("seattle").scale(), 1.0 ) );
    QVERIFY( equals(supply.findRecord("san-diego").scale(), 1.0) );

    // when, then
    GAMSEquation demand = db.getEquation("demand");
    QVERIFY( equals(demand.findRecord("topeka").scale(), 1.0) );
    QVERIFY( equals(demand.findRecord("chicago").scale(), 1.0) );
    QVERIFY( equals(demand.findRecord("new-york").scale(), 1.0) );

    // when, then
    GAMSEquation cost = db.getEquation("cost");
    QVERIFY( equals(cost.firstRecord().scale(), 1.0) );

    // when, then
    cost.firstRecord().setScale( 1.2345 );
    QVERIFY( equals(cost.firstRecord().scale(), 1.2345) );
}

QTEST_MAIN(TestGAMSEquationRecord)
