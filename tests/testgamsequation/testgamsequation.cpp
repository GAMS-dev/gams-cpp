/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2019 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2019 GAMS Development Corp. <support@gams.com>
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
#include "testgamsequation.h"

using namespace gams;

QString TestGAMSEquation::classname()  { return "TestGAMSEquation"; }

void  TestGAMSEquation::testDefaultConstructor() {
    // when
    GAMSEquation eq;
    // then
    QVERIFY( ! eq.isValid() );
    QVERIFY_EXCEPTION_THROWN( eq.type(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( eq.logID(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( eq.equType(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( eq.text(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( eq.checkDomains(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( eq.numberRecords(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( eq.firstRecord(), GAMSException );
}

void TestGAMSEquation::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();
    GAMSEquation supply = db.getEquation("supply");

    // when
    GAMSEquation eq( supply );
    // then
    QCOMPARE( eq.name(), supply.name() );
    QCOMPARE( eq.numberRecords(), supply.numberRecords() );
    QCOMPARE( eq.equType(), supply.equType() );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );

    // when
    supply.addRecord("Albuquerque");
    // then
    QVERIFY( equals(eq.findRecord("Albuquerque").level(), 0.0) );
    QCOMPARE( eq.findRecord("Albuquerque").key(0).c_str(), "Albuquerque" );
    QCOMPARE( eq.numberRecords(), supply.numberRecords() );
}

void TestGAMSEquation::testCopyConstructor_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    QVERIFY_EXCEPTION_THROWN( GAMSEquation eq_j( db.getSet("j") ), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSEquation eq_d( db.getParameter("d")), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSEquation eq_x( db.getVariable("x") ), GAMSException);
}

void TestGAMSEquation::testIterator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSEquation eq;
    QVERIFY_EXCEPTION_THROWN( eq.begin(), GAMSException);
    QVERIFY_EXCEPTION_THROWN( eq.end(), GAMSException);

    std::map<std::string, double> symbolMap;
    for (GAMSEquationRecord rec :   db.getEquation("demand")) {
       symbolMap[rec.key(0)] = rec.level();
    }
    // then
    QCOMPARE( symbolMap.size(), size_t(3) );
    QVERIFY( equals(symbolMap["new-york"], 325.0) );
    QVERIFY( equals(symbolMap["chicago"], 300.0) );
    QVERIFY( equals(symbolMap["topeka"], 275.0) );
}

void TestGAMSEquation::testAssignmentOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquation supply = db.getEquation("supply");
    int numberOfSymbols = db.getNrSymbols();

    // when, then
    GAMSEquation eq = supply;
    QCOMPARE( eq.name(), supply.name() );
    QCOMPARE( eq.dim(), supply.dim() );
    QCOMPARE( eq.numberRecords(), supply.numberRecords() );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );
}

void TestGAMSEquation::testAssignmentOperator_IncorrectType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    QVERIFY_EXCEPTION_THROWN( GAMSEquation j = db.getSet("j"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSEquation d = db.getParameter("d"), GAMSException);
    QVERIFY_EXCEPTION_THROWN( GAMSEquation v = db.getVariable("x"), GAMSException);
}

void TestGAMSEquation::testGetFirstRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSEquationRecord rec = db.getEquation("cost").firstRecord();
      QVERIFY( equals(rec.marginal(), 1.0) );
    }
    // when, then
    { GAMSEquationRecord rec = db.getEquation("demand").firstRecord();
      QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "new-york", Qt::CaseSensitive ), 0 );
      QVERIFY( equals(rec.level(), 325.0) );
    }
}

void TestGAMSEquation::testGetFirstRecordSlice() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSEquationRecord rec = db.getEquation("supply").firstRecord("san-diego");
      QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "san-diego", Qt::CaseSensitive ), 0 );
      QVERIFY( equals(rec.level(), 550.0) );
    }
    // when, then
    { GAMSEquationRecord rec = db.getEquation("demand").firstRecord("topeka");
      QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "topeka", Qt::CaseSensitive ), 0 );
      QVERIFY( equals(rec.level(), 275.0) );
    }
}

void TestGAMSEquation::testGetFirstRecordSlice_InvalidKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when InvalidKeys, then
    { GAMSEquation cost = db.getEquation("cost");
      QVERIFY_EXCEPTION_THROWN( cost.firstRecord("Albuquerque"), GAMSException );
    }
    // when IncorrectDimension, then
    { GAMSEquation supply = db.getEquation("supply");
      QVERIFY_EXCEPTION_THROWN( supply.firstRecord("seattle", "topeka"), GAMSException);
    }
}

void TestGAMSEquation::testGetLastRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSEquationRecord rec = db.getEquation("cost").lastRecord();
      QVERIFY( equals(rec.marginal(), 1.0) );
    }
    // when, then
    { GAMSEquationRecord rec = db.getEquation("demand").lastRecord();
      QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "topeka", Qt::CaseSensitive ), 0 );
      QVERIFY( equals(rec.level(), 275.0) );
    }
}

void TestGAMSEquation::testGetLastRecordSlice() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSEquationRecord rec = db.getEquation("supply").lastRecord("seattle");
      QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "seattle", Qt::CaseSensitive ), 0 );
      QVERIFY( equals(rec.level(), 350.0) );
    }
    // when, then
    { GAMSEquationRecord rec = db.getEquation("demand").lastRecord("chicago");
      QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "chicago", Qt::CaseSensitive ), 0 );
      QVERIFY( equals(rec.level(), 300.0) );
    }
}

void TestGAMSEquation::testGetLastRecordSlice_InvalidKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when InvalidKeys, then
    { GAMSEquation cost = db.getEquation("cost");
      QVERIFY_EXCEPTION_THROWN( cost.lastRecord("Albuquerque"), GAMSException );
    }
    // when IncorrectDimension, then
    { GAMSEquation supply = db.getEquation("demand");
      QVERIFY_EXCEPTION_THROWN( supply.lastRecord("topeka", "seattle"), GAMSException);
    }
}

void TestGAMSEquation::testFindRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSEquationRecord rec = db.getEquation("supply").findRecord("san-diego");
    QVERIFY( rec.isValid() );
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "san-diego", Qt::CaseSensitive ), 0 );
    QVERIFY( equals(rec.level(), 550.0) );
}

void TestGAMSEquation::testFindRecord_InsensitiveCaseKeys() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when
    GAMSEquationRecord rec = db.getEquation("demand").findRecord("TOPEKA");
    // then
    QCOMPARE( QString::compare( QString::fromStdString(rec.key(0)), "topeka", Qt::CaseInsensitive ), 0 );
    QVERIFY( equals(rec.level(), 275.0) );
}

void TestGAMSEquation::testAddRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquation demand = db.getEquation("demand");
    int numberOfRecords = demand.numberRecords();

    // when
    GAMSEquationRecord rec = demand.addRecord("Albuquerque");
    QCOMPARE( demand.numberRecords(), numberOfRecords+1 );
    QVERIFY( rec.isValid() );
    QVERIFY( equals(rec.level(), 0.0)  );
}

void TestGAMSEquation::testAddRecord_DuplicatedKeys_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when DuplicatedKeys, then
    QVERIFY_EXCEPTION_THROWN( db.getEquation("cost").addRecord("chicago"), GAMSException);

    // when IncorrectDimension, then
    GAMSEquation demand = db.getEquation("demand");
    int numberOfRecords = demand.numberRecords();
    QVERIFY_EXCEPTION_THROWN( demand.addRecord(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( demand.addRecord("chicago", "seattle"), GAMSException );
    QCOMPARE( demand.numberRecords(), numberOfRecords );
}

void TestGAMSEquation::testMergeExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquation eq = db.getEquation("demand");
    int numberOfRecords = eq.numberRecords();
    GAMSEquationRecord chicago_rec = eq.findRecord("chicago");
    // when, then
    GAMSEquationRecord rec = eq.mergeRecord("chicago");
    QCOMPARE( eq.numberRecords(), numberOfRecords );
    QCOMPARE( rec.key(0), chicago_rec.key(0) );
    QVERIFY( equals(rec.level(), chicago_rec.level()) );
}

void TestGAMSEquation::testMergeNonExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquation eq = db.getEquation("supply");
    int numberOfRecords = eq.numberRecords();
    // when
    GAMSEquationRecord rec = eq.mergeRecord("Albuquerque");
    // then
    QCOMPARE( eq.numberRecords(), numberOfRecords+1 );
    QCOMPARE( rec.key(0).c_str(), "Albuquerque" );
    QVERIFY( equals(rec.level(), 0.0) );
}

void TestGAMSEquation::testGetEquType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    QCOMPARE( db.getEquation("cost").equType(), GAMSEnum::EquType::E );
    QCOMPARE( db.getEquation("supply").equType(), GAMSEnum::EquType::L );
    QCOMPARE( db.getEquation("demand").equType(), GAMSEnum::EquType::G );
}
QTEST_MAIN(TestGAMSEquation)
