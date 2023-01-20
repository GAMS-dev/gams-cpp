/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2023 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2023 GAMS Development Corp. <support@gams.com>
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
#include "testgamsobject.h"

using namespace gams;

class TestGAMSEquation: public TestGAMSObject
{
};

TEST_F(TestGAMSEquation, testDefaultConstructor) {
    // when
    GAMSEquation eq;
    // then
    ASSERT_TRUE( ! eq.isValid() );
    EXPECT_THROW( eq.type(), GAMSException );
    EXPECT_THROW( eq.logID(), GAMSException);
    EXPECT_THROW( eq.equType(), GAMSException );
    EXPECT_THROW( eq.text(), GAMSException );
    EXPECT_THROW( eq.checkDomains(), GAMSException );
    EXPECT_THROW( eq.numberRecords(), GAMSException );
    EXPECT_THROW( eq.firstRecord(), GAMSException );
}

TEST_F(TestGAMSEquation, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();
    GAMSEquation supply = db.getEquation("supply");

    // when
    GAMSEquation eq( supply );
    // then
    EXPECT_EQ( eq.name(), supply.name() );
    EXPECT_EQ( eq.numberRecords(), supply.numberRecords() );
    EXPECT_EQ( eq.equType(), supply.equType() );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );

    // when
    supply.addRecord("Albuquerque");
    // then
    ASSERT_TRUE( equals(eq.findRecord("Albuquerque").level(), 0.0) );
    EXPECT_STREQ( eq.findRecord("Albuquerque").key(0).c_str(), "Albuquerque" );
    EXPECT_EQ( eq.numberRecords(), supply.numberRecords() );
}

TEST_F(TestGAMSEquation, testCopyConstructor_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    EXPECT_THROW( GAMSEquation eq_j( db.getSet("j") ), GAMSException);
    EXPECT_THROW( GAMSEquation eq_d( db.getParameter("d")), GAMSException);
    EXPECT_THROW( GAMSEquation eq_x( db.getVariable("x") ), GAMSException);
}

TEST_F(TestGAMSEquation, testIterator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSEquation eq;
    EXPECT_THROW( eq.begin(), GAMSException);
    EXPECT_THROW( eq.end(), GAMSException);

    std::map<std::string, double> symbolMap;
    for (GAMSEquationRecord rec :   db.getEquation("demand")) {
       symbolMap[rec.key(0)] = rec.level();
    }
    // then
    EXPECT_EQ( symbolMap.size(), size_t(3) );
    ASSERT_TRUE( equals(symbolMap["new-york"], 325.0) );
    ASSERT_TRUE( equals(symbolMap["chicago"], 300.0) );
    ASSERT_TRUE( equals(symbolMap["topeka"], 275.0) );
}

TEST_F(TestGAMSEquation, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquation supply = db.getEquation("supply");
    int numberOfSymbols = db.getNrSymbols();

    // when, then
    GAMSEquation eq = supply;
    EXPECT_EQ( eq.name(), supply.name() );
    EXPECT_EQ( eq.dim(), supply.dim() );
    EXPECT_EQ( eq.numberRecords(), supply.numberRecords() );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
}

TEST_F(TestGAMSEquation, testAssignmentOperator_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    EXPECT_THROW( GAMSEquation j = db.getSet("j"), GAMSException);
    EXPECT_THROW( GAMSEquation d = db.getParameter("d"), GAMSException);
    EXPECT_THROW( GAMSEquation v = db.getVariable("x"), GAMSException);
}

TEST_F(TestGAMSEquation, testGetFirstRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSEquationRecord rec = db.getEquation("cost").firstRecord();
      ASSERT_TRUE( equals(rec.marginal(), 1.0) );
    }
    // when, then
    { GAMSEquationRecord rec = db.getEquation("demand").firstRecord();
      EXPECT_STREQ( rec.key(0).c_str(), "new-york" );
      ASSERT_TRUE( equals(rec.level(), 325.0) );
    }
}

TEST_F(TestGAMSEquation, testGetFirstRecordSlice) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSEquationRecord rec = db.getEquation("supply").firstRecord("san-diego");
      EXPECT_STREQ( rec.key(0).c_str(), "san-diego" );
      ASSERT_TRUE( equals(rec.level(), 550.0) );
    }
    // when, then
    { GAMSEquationRecord rec = db.getEquation("demand").firstRecord("topeka");
      EXPECT_STREQ( rec.key(0).c_str(), "topeka" );
      ASSERT_TRUE( equals(rec.level(), 275.0) );
    }
}

TEST_F(TestGAMSEquation, testGetFirstRecordSlice_InvalidKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when InvalidKeys, then
    { GAMSEquation cost = db.getEquation("cost");
      EXPECT_THROW( cost.firstRecord("Albuquerque"), GAMSException );
    }
    // when IncorrectDimension, then
    { GAMSEquation supply = db.getEquation("supply");
      EXPECT_THROW( supply.firstRecord("seattle", "topeka"), GAMSException);
    }
}

TEST_F(TestGAMSEquation, testGetLastRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSEquationRecord rec = db.getEquation("cost").lastRecord();
      ASSERT_TRUE( equals(rec.marginal(), 1.0) );
    }
    // when, then
    { GAMSEquationRecord rec = db.getEquation("demand").lastRecord();
      EXPECT_STREQ( rec.key(0).c_str(), "topeka" );
      ASSERT_TRUE( equals(rec.level(), 275.0) );
    }
}

TEST_F(TestGAMSEquation, testGetLastRecordSlice) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSEquationRecord rec = db.getEquation("supply").lastRecord("seattle");
      EXPECT_STREQ( rec.key(0).c_str(), "seattle" );
      ASSERT_TRUE( equals(rec.level(), 350.0) );
    }
    // when, then
    { GAMSEquationRecord rec = db.getEquation("demand").lastRecord("chicago");
      EXPECT_STREQ( rec.key(0).c_str(), "chicago" );
      ASSERT_TRUE( equals(rec.level(), 300.0) );
    }
}

TEST_F(TestGAMSEquation, testGetLastRecordSlice_InvalidKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when InvalidKeys, then
    { GAMSEquation cost = db.getEquation("cost");
      EXPECT_THROW( cost.lastRecord("Albuquerque"), GAMSException );
    }
    // when IncorrectDimension, then
    { GAMSEquation supply = db.getEquation("demand");
      EXPECT_THROW( supply.lastRecord("topeka", "seattle"), GAMSException);
    }
}

TEST_F(TestGAMSEquation, testFindRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSEquationRecord rec = db.getEquation("supply").findRecord("san-diego");
    ASSERT_TRUE( rec.isValid() );
    EXPECT_STREQ( rec.key(0).c_str(), "san-diego" );
    ASSERT_TRUE( equals(rec.level(), 550.0) );
}

TEST_F(TestGAMSEquation, testFindRecord_InsensitiveCaseKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when
    GAMSEquationRecord rec = db.getEquation("demand").findRecord("TOPEKA");
    // then
    EXPECT_STREQ( rec.key(0).c_str(), "topeka" );
    ASSERT_TRUE( equals(rec.level(), 275.0) );
}

TEST_F(TestGAMSEquation, testAddRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquation demand = db.getEquation("demand");
    int numberOfRecords = demand.numberRecords();

    // when
    GAMSEquationRecord rec = demand.addRecord("Albuquerque");
    EXPECT_EQ( demand.numberRecords(), numberOfRecords+1 );
    ASSERT_TRUE( rec.isValid() );
    ASSERT_TRUE( equals(rec.level(), 0.0)  );
}

TEST_F(TestGAMSEquation, testAddRecord_DuplicatedKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when DuplicatedKeys, then
    EXPECT_THROW( db.getEquation("cost").addRecord("chicago"), GAMSException);

    // when IncorrectDimension, then
    GAMSEquation demand = db.getEquation("demand");
    int numberOfRecords = demand.numberRecords();
    EXPECT_THROW( demand.addRecord(), GAMSException );
    EXPECT_THROW( demand.addRecord("chicago", "seattle"), GAMSException );
    EXPECT_EQ( demand.numberRecords(), numberOfRecords );
}

TEST_F(TestGAMSEquation, testMergeExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquation eq = db.getEquation("demand");
    int numberOfRecords = eq.numberRecords();
    GAMSEquationRecord chicago_rec = eq.findRecord("chicago");
    // when, then
    GAMSEquationRecord rec = eq.mergeRecord("chicago");
    EXPECT_EQ( eq.numberRecords(), numberOfRecords );
    EXPECT_EQ( rec.key(0), chicago_rec.key(0) );
    ASSERT_TRUE( equals(rec.level(), chicago_rec.level()) );
}

TEST_F(TestGAMSEquation, testMergeNonExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquation eq = db.getEquation("supply");
    int numberOfRecords = eq.numberRecords();
    // when
    GAMSEquationRecord rec = eq.mergeRecord("Albuquerque");
    // then
    EXPECT_EQ( eq.numberRecords(), numberOfRecords+1 );
    EXPECT_STREQ( rec.key(0).c_str(), "Albuquerque" );
    ASSERT_TRUE( equals(rec.level(), 0.0) );
}

TEST_F(TestGAMSEquation, testGetEquType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    EXPECT_EQ( db.getEquation("cost").equType(), GAMSEnum::EquType::E );
    EXPECT_EQ( db.getEquation("supply").equType(), GAMSEnum::EquType::L );
    EXPECT_EQ( db.getEquation("demand").equType(), GAMSEnum::EquType::G );
}
