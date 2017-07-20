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
#include "gamsexception.h"
#include "gamsparameter.h"
#include "gamsset.h"
#include "gamsvariable.h"
#include "gamsvariablerecord.h"
#include "testgamsvariable.h"

#include <sstream>
#include <string>

using namespace gams;

QString TestGAMSVariable::classname()  { return "TestGAMSVariable"; }

void TestGAMSVariable::testDefaultConstructor() {
    // when
    GAMSVariable v;
    // then
    QVERIFY( ! v.isValid() );

    QVERIFY_EXCEPTION_THROWN( v.type(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( v.logID(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( v.varType(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( v.text(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( v.checkDomains(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( v.numberRecords(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( v.firstRecord(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( v.addRecord("x"), GAMSException );
}

void TestGAMSVariable::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();
    GAMSVariable x = db.getVariable("x");

        // when
    GAMSVariable var( x );
    // then
    QCOMPARE( var.name(), x.name() );
    QCOMPARE( var.numberRecords(), x.numberRecords() );
    QCOMPARE( var.varType(), x.varType() );
    QCOMPARE( var.dim(), x.dim() );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );

    // when
    x.addRecord("seattle", "Albuquerque");
    // then
    QVERIFY( equals(var.findRecord("seattle", "Albuquerque").level(), 0.0) );
    QCOMPARE( var.numberRecords(), x.numberRecords() );
}

void TestGAMSVariable::testCopyConstructor_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    QVERIFY_EXCEPTION_THROWN( GAMSVariable var_j( db.getSet("j") ), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSVariable var_d( db.getParameter("d")), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSVariable var_demand( db.getEquation("dmemand") ), GAMSException);
}

void TestGAMSVariable::testIterator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    std::map<std::string, double> symbolMap;
    for (GAMSVariableRecord rec :  db.getVariable("x")) {
        std::stringstream ss;
        ss << rec.key(0) << "_" <<rec.key(1) ;
        symbolMap[ss.str()] = rec.level();
    }
    // then
    QCOMPARE( symbolMap.size(), size_t(6) );
    QVERIFY( equals(symbolMap["seattle_new-york"], 50.0) );
    QVERIFY( equals(symbolMap["seattle_chicago"], 300.0) );
    QVERIFY( equals(symbolMap["seattle_topeka"], 0.0) );
    QVERIFY( equals(symbolMap["san-diego_new-york"], 275.0) );
    QVERIFY( equals(symbolMap["san-diego_chicago"], 0.0) );
    QVERIFY( equals(symbolMap["san-diego_topeka"], 275.0) );
}

void TestGAMSVariable::testAssignmentOperator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable x = db.getVariable("x");
    int numberOfSymbols = db.getNrSymbols();

    // when, then
    GAMSVariable var = x;
    QCOMPARE( var, x );
    QVERIFY( var == x );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );
}

void TestGAMSVariable::testAssignmentOperator_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    QVERIFY_EXCEPTION_THROWN( GAMSVariable i = db.getSet("i"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSVariable d = db.getParameter("d"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSVariable supply = db.getEquation("supply"), GAMSException);
}

void TestGAMSVariable::testGetFirstRecord() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").firstRecord();
      QCOMPARE( rec.keys().size(), size_t(2) );
      QCOMPARE( rec.key(0).c_str(), "seattle" );
      QCOMPARE( rec.key(1).c_str(), "new-york" );
      QVERIFY( equals(rec.level(), 50.0) );
    }
    // when, then
    { GAMSVariableRecord rec = db.getVariable("z").firstRecord();
      QCOMPARE( rec.keys().size(), size_t(0) );
      QVERIFY( equals(rec.level(), 153.675) );
    }
}

void TestGAMSVariable::testGetFirstRecordSlice() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").firstRecord("seattle", "chicago");
      QCOMPARE( rec.keys().size(), size_t(2) );
      QCOMPARE( rec.key(0).c_str(), "seattle" );
      QCOMPARE( rec.key(1).c_str(), "chicago" );
      QVERIFY( equals(rec.level(), 300.0) );
    }
    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").firstRecord("san-diego", "topeka");
      QCOMPARE( rec.keys().size(), size_t(2) );
      QCOMPARE( rec.key(0).c_str(), "san-diego" );
      QCOMPARE( rec.key(1).c_str(), "topeka" );
      QVERIFY( equals(rec.level(), 275.0) );
    }
}

void TestGAMSVariable::testGetFirstRecordSlice_InvalidKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when InvalidKeys, then
    { GAMSVariable x = db.getVariable("x");
      QVERIFY_EXCEPTION_THROWN( x.firstRecord("seattle"), GAMSException );
      QVERIFY_EXCEPTION_THROWN( x.firstRecord("seattle", "topeka", "chicago"), GAMSException);
    }
    // when IncorrectDimension, then
    { GAMSVariable z = db.getVariable("z");
      QVERIFY_EXCEPTION_THROWN( z.firstRecord("seattle"), GAMSException);
      QVERIFY_EXCEPTION_THROWN( z.firstRecord("seattle", "topeka"), GAMSException);
    }
}

void TestGAMSVariable::testGetLastRecord() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").lastRecord();
      QCOMPARE( rec.keys().size(), size_t(2) );
      QCOMPARE( rec.key(0).c_str(), "san-diego" );
      QCOMPARE( rec.key(1).c_str(), "topeka" );
      QVERIFY( equals(rec.level(), 275.0) );
    }
    // when, then
    { GAMSVariableRecord rec = db.getVariable("z").lastRecord();
      QCOMPARE( rec.keys().size(), size_t(0) );
      QVERIFY( equals(rec.level(), 153.675) );
    }
}

void TestGAMSVariable::testGetLastRecordSlice() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();


    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").lastRecord("san-diego", "new-york");
      QCOMPARE( rec.keys().size(), size_t(2) );
      QCOMPARE( rec.key(0).c_str(), "san-diego" );
      QCOMPARE( rec.key(1).c_str(), "new-york" );
      QVERIFY( equals(rec.level(), 275.0) );
    }
    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").lastRecord("seattle", "chicago");
      QCOMPARE( rec.keys().size(), size_t(2) );
      QCOMPARE( rec.key(0).c_str(), "seattle" );
      QCOMPARE( rec.key(1).c_str(), "chicago" );
      QVERIFY( equals(rec.level(), 300.0) );
    }
}

void TestGAMSVariable::testGetLastRecordSlice_InvalidKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable x = db.getVariable("x");

    // when InvalidKeys, then
    QVERIFY_EXCEPTION_THROWN( x.lastRecord("chicago", "seattle"), GAMSException );
    // when IncorrectDimension, then
    QVERIFY_EXCEPTION_THROWN( x.lastRecord("chicago"), GAMSException );
    QVERIFY_EXCEPTION_THROWN( x.lastRecord("seattle", "chicago", "alburquerque"), GAMSException );
}

void TestGAMSVariable::testFindRecord() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSVariableRecord rec = db.getVariable("x").findRecord("seattle", "chicago");
    QVERIFY( rec.isValid() );
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "seattle", Qt::CaseSensitive ), 0 );
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(1)), "chicago", Qt::CaseSensitive ), 0 );
    QVERIFY( equals(rec.level(), 300.0) );
}

void TestGAMSVariable::testFindRecord_InsensitiveCaseKeys() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSVariableRecord rec = db.getVariable("x").findRecord("SAN-DIEGO", "TOPEKA");
    QVERIFY( rec.isValid() );
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "san-diego", Qt::CaseInsensitive ), 0 );
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(1)), "topeka", Qt::CaseInsensitive ), 0 );
    QVERIFY( equals(rec.level(), 275.0) );

}

void TestGAMSVariable::testAddRecord() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable x = db.getVariable("x");
    int numberOfRecords = x.numberRecords();

    // when
    GAMSVariableRecord rec = x.addRecord("seattle", "alburquerque");
    QCOMPARE( x.numberRecords(), numberOfRecords+1 );
    QVERIFY( rec.isValid() );
    QVERIFY( equals(rec.level(), 0.0) );

}

void TestGAMSVariable::testAddRecord_DuplicatedKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when DuplicatedKeys, then
    QVERIFY_EXCEPTION_THROWN( db.getVariable("z").addRecord(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.getVariable("x").addRecord("seattle","topeka"), GAMSException);

    // when IncorrectDimension, then
    GAMSVariable x = db.getVariable("x");
    int numberOfRecords = x.numberRecords();
    QVERIFY_EXCEPTION_THROWN( x.addRecord(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( x.addRecord("alburquerque"), GAMSException );
    QCOMPARE( x.numberRecords(), numberOfRecords );
}

void TestGAMSVariable::testMergeExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    {
      GAMSVariable z = db.getVariable("z");
      int numberOfRecords = z.numberRecords();
      GAMSVariableRecord z_rec = z.firstRecord();
      // when
      GAMSVariableRecord rec = z.mergeRecord();
      // then
      QCOMPARE( z.numberRecords(), numberOfRecords );
      QCOMPARE( rec.keys().size(), size_t(0) );
      QVERIFY( equals(rec.level(), z_rec.level()) );
    }
    {
      GAMSVariable x = db.getVariable("x");
      int numberOfRecords = x.numberRecords();
      GAMSVariableRecord x_rec = x.findRecord("seattle", "topeka");
      // when
      GAMSVariableRecord rec = x.mergeRecord("seattle", "topeka");
      // then
      QCOMPARE( x.numberRecords(), numberOfRecords );
      QCOMPARE( rec, x_rec );
      QCOMPARE( rec.keys().size(), size_t(2) );
      QCOMPARE( rec.key(0).c_str(), "seattle" );
      QCOMPARE( rec.key(1).c_str(), "topeka" );
      QVERIFY( equals(rec.level(), x_rec.level()) );
    }
}

void TestGAMSVariable::testMergeNonExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    GAMSVariable x = db.getVariable("x");
    int numberOfRecords = x.numberRecords();
    QVERIFY_EXCEPTION_THROWN( x.findRecord("seattle", "alburquerque"), GAMSException );
    // when
    GAMSVariableRecord rec = x.mergeRecord("seattle", "alburquerque");
    // then
    QCOMPARE( x.numberRecords(), numberOfRecords+1 );
    QCOMPARE( rec.keys().size(), size_t(2) );
    QCOMPARE( rec.key(0).c_str(), "seattle" );
    QCOMPARE( rec.key(1).c_str(), "alburquerque" );
    QVERIFY( equals(rec.level(), 0.0) );
}


void TestGAMSVariable::testGetVarType() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    QCOMPARE( db.getVariable("x").varType(), GAMSEnum::VarType::Positive );
    QCOMPARE( db.getVariable("z").varType(), GAMSEnum::VarType::Free );
}

QTEST_MAIN(TestGAMSVariable)
