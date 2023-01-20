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

#include "testgamsobject.h"
#include "gamsequation.h"
#include "gamsexception.h"
#include "gamsparameter.h"
#include "gamsset.h"
#include "gamsvariable.h"
#include "gamsvariablerecord.h"

#include <sstream>
#include <string>

using namespace gams;

class TestGAMSVariable: public TestGAMSObject
{
};

TEST_F(TestGAMSVariable, testDefaultConstructor) {
    // when
    GAMSVariable v;
    // then
    ASSERT_TRUE( ! v.isValid() );

    EXPECT_THROW( v.type(), GAMSException );
    EXPECT_THROW( v.logID(), GAMSException);
    EXPECT_THROW( v.varType(), GAMSException );
    EXPECT_THROW( v.text(), GAMSException );
    EXPECT_THROW( v.checkDomains(), GAMSException );
    EXPECT_THROW( v.numberRecords(), GAMSException );
    EXPECT_THROW( v.firstRecord(), GAMSException );
    EXPECT_THROW( v.addRecord("x"), GAMSException );
}

TEST_F(TestGAMSVariable, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();
    GAMSVariable x = db.getVariable("x");

        // when
    GAMSVariable var( x );
    // then
    EXPECT_EQ( var.name(), x.name() );
    EXPECT_EQ( var.numberRecords(), x.numberRecords() );
    EXPECT_EQ( var.varType(), x.varType() );
    EXPECT_EQ( var.dim(), x.dim() );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );

    // when
    x.addRecord("seattle", "Albuquerque");
    // then
    ASSERT_TRUE( equals(var.findRecord("seattle", "Albuquerque").level(), 0.0) );
    EXPECT_EQ( var.numberRecords(), x.numberRecords() );
}

TEST_F(TestGAMSVariable, testCopyConstructor_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    EXPECT_THROW( GAMSVariable var_j( db.getSet("j") ), GAMSException);
    EXPECT_THROW( GAMSVariable var_d( db.getParameter("d")), GAMSException);
    EXPECT_THROW( GAMSVariable var_demand( db.getEquation("dmemand") ), GAMSException);
}

TEST_F(TestGAMSVariable, testIterator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
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
    EXPECT_EQ( symbolMap.size(), size_t(6) );
    ASSERT_TRUE( equals(symbolMap["seattle_new-york"], 50.0) );
    ASSERT_TRUE( equals(symbolMap["seattle_chicago"], 300.0) );
    ASSERT_TRUE( equals(symbolMap["seattle_topeka"], 0.0) );
    ASSERT_TRUE( equals(symbolMap["san-diego_new-york"], 275.0) );
    ASSERT_TRUE( equals(symbolMap["san-diego_chicago"], 0.0) );
    ASSERT_TRUE( equals(symbolMap["san-diego_topeka"], 275.0) );
}

TEST_F(TestGAMSVariable, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable x = db.getVariable("x");
    int numberOfSymbols = db.getNrSymbols();

    // when, then
    GAMSVariable var = x;
    EXPECT_EQ( var, x );
    ASSERT_TRUE( var == x );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
}

TEST_F(TestGAMSVariable, testAssignmentOperator_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    EXPECT_THROW( GAMSVariable i = db.getSet("i"), GAMSException);
    EXPECT_THROW( GAMSVariable d = db.getParameter("d"), GAMSException);
    EXPECT_THROW( GAMSVariable supply = db.getEquation("supply"), GAMSException);
}

TEST_F(TestGAMSVariable, testGetFirstRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").firstRecord();
      EXPECT_EQ( rec.keys().size(), size_t(2) );
      EXPECT_STREQ( rec.key(0).c_str(), "seattle" );
      EXPECT_STREQ( rec.key(1).c_str(), "new-york" );
      ASSERT_TRUE( equals(rec.level(), 50.0) );
    }
    // when, then
    { GAMSVariableRecord rec = db.getVariable("z").firstRecord();
      EXPECT_EQ( rec.keys().size(), size_t(0) );
      ASSERT_TRUE( equals(rec.level(), 153.675) );
    }
}

TEST_F(TestGAMSVariable, testGetFirstRecordSlice) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").firstRecord("seattle", "chicago");
      EXPECT_EQ( rec.keys().size(), size_t(2) );
      EXPECT_STREQ( rec.key(0).c_str(), "seattle" );
      EXPECT_STREQ( rec.key(1).c_str(), "chicago" );
      ASSERT_TRUE( equals(rec.level(), 300.0) );
    }
    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").firstRecord("san-diego", "topeka");
      EXPECT_EQ( rec.keys().size(), size_t(2) );
      EXPECT_STREQ( rec.key(0).c_str(), "san-diego" );
      EXPECT_STREQ( rec.key(1).c_str(), "topeka" );
      ASSERT_TRUE( equals(rec.level(), 275.0) );
    }
}

TEST_F(TestGAMSVariable, testGetFirstRecordSlice_InvalidKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when InvalidKeys, then
    { GAMSVariable x = db.getVariable("x");
      EXPECT_THROW( x.firstRecord("seattle"), GAMSException );
      EXPECT_THROW( x.firstRecord("seattle", "topeka", "chicago"), GAMSException);
    }
    // when IncorrectDimension, then
    { GAMSVariable z = db.getVariable("z");
      EXPECT_THROW( z.firstRecord("seattle"), GAMSException);
      EXPECT_THROW( z.firstRecord("seattle", "topeka"), GAMSException);
    }
}

TEST_F(TestGAMSVariable, testGetLastRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").lastRecord();
      EXPECT_EQ( rec.keys().size(), size_t(2) );
      EXPECT_STREQ( rec.key(0).c_str(), "san-diego" );
      EXPECT_STREQ( rec.key(1).c_str(), "topeka" );
      ASSERT_TRUE( equals(rec.level(), 275.0) );
    }
    // when, then
    { GAMSVariableRecord rec = db.getVariable("z").lastRecord();
      EXPECT_EQ( rec.keys().size(), size_t(0) );
      ASSERT_TRUE( equals(rec.level(), 153.675) );
    }
}

TEST_F(TestGAMSVariable, testGetLastRecordSlice) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();


    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").lastRecord("san-diego", "new-york");
      EXPECT_EQ( rec.keys().size(), size_t(2) );
      EXPECT_STREQ( rec.key(0).c_str(), "san-diego" );
      EXPECT_STREQ( rec.key(1).c_str(), "new-york" );
      ASSERT_TRUE( equals(rec.level(), 275.0) );
    }
    // when, then
    { GAMSVariableRecord rec = db.getVariable("x").lastRecord("seattle", "chicago");
      EXPECT_EQ( rec.keys().size(), size_t(2) );
      EXPECT_STREQ( rec.key(0).c_str(), "seattle" );
      EXPECT_STREQ( rec.key(1).c_str(), "chicago" );
      ASSERT_TRUE( equals(rec.level(), 300.0) );
    }
}

TEST_F(TestGAMSVariable, testGetLastRecordSlice_InvalidKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable x = db.getVariable("x");

    // when InvalidKeys, then
    EXPECT_THROW( x.lastRecord("chicago", "seattle"), GAMSException );
    // when IncorrectDimension, then
    EXPECT_THROW( x.lastRecord("chicago"), GAMSException );
    EXPECT_THROW( x.lastRecord("seattle", "chicago", "alburquerque"), GAMSException );
}

TEST_F(TestGAMSVariable, testFindRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSVariableRecord rec = db.getVariable("x").findRecord("seattle", "chicago");
    ASSERT_TRUE( rec.isValid() );
    EXPECT_STREQ( rec.key(0).c_str(), "seattle" );
    EXPECT_STREQ( rec.key(1).c_str(), "chicago" );
    ASSERT_TRUE( equals(rec.level(), 300.0) );
}

TEST_F(TestGAMSVariable, testFindRecord_InsensitiveCaseKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSVariableRecord rec = db.getVariable("x").findRecord("SAN-DIEGO", "TOPEKA");
    ASSERT_TRUE( rec.isValid() );
    EXPECT_STREQ( rec.key(0).c_str(), "san-diego" );
    EXPECT_STREQ( rec.key(1).c_str(), "topeka" );
    ASSERT_TRUE( equals(rec.level(), 275.0) );

}

TEST_F(TestGAMSVariable, testAddRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable x = db.getVariable("x");
    int numberOfRecords = x.numberRecords();

    // when
    GAMSVariableRecord rec = x.addRecord("seattle", "alburquerque");
    EXPECT_EQ( x.numberRecords(), numberOfRecords+1 );
    ASSERT_TRUE( rec.isValid() );
    ASSERT_TRUE( equals(rec.level(), 0.0) );

}

TEST_F(TestGAMSVariable, testAddRecord_DuplicatedKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when DuplicatedKeys, then
    EXPECT_THROW( db.getVariable("z").addRecord(), GAMSException);
    EXPECT_THROW( db.getVariable("x").addRecord("seattle","topeka"), GAMSException);

    // when IncorrectDimension, then
    GAMSVariable x = db.getVariable("x");
    int numberOfRecords = x.numberRecords();
    EXPECT_THROW( x.addRecord(), GAMSException );
    EXPECT_THROW( x.addRecord("alburquerque"), GAMSException );
    EXPECT_EQ( x.numberRecords(), numberOfRecords );
}

TEST_F(TestGAMSVariable, testMergeExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
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
      EXPECT_EQ( z.numberRecords(), numberOfRecords );
      EXPECT_EQ( rec.keys().size(), size_t(0) );
      ASSERT_TRUE( equals(rec.level(), z_rec.level()) );
    }
    {
      GAMSVariable x = db.getVariable("x");
      int numberOfRecords = x.numberRecords();
      GAMSVariableRecord x_rec = x.findRecord("seattle", "topeka");
      // when
      GAMSVariableRecord rec = x.mergeRecord("seattle", "topeka");
      // then
      EXPECT_EQ( x.numberRecords(), numberOfRecords );
      EXPECT_EQ( rec, x_rec );
      EXPECT_EQ( rec.keys().size(), size_t(2) );
      EXPECT_STREQ( rec.key(0).c_str(), "seattle" );
      EXPECT_STREQ( rec.key(1).c_str(), "topeka" );
      ASSERT_TRUE( equals(rec.level(), x_rec.level()) );
    }
}

TEST_F(TestGAMSVariable, testMergeNonExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    GAMSVariable x = db.getVariable("x");
    int numberOfRecords = x.numberRecords();
    EXPECT_THROW( x.findRecord("seattle", "alburquerque"), GAMSException );
    // when
    GAMSVariableRecord rec = x.mergeRecord("seattle", "alburquerque");
    // then
    EXPECT_EQ( x.numberRecords(), numberOfRecords+1 );
    EXPECT_EQ( rec.keys().size(), size_t(2) );
    EXPECT_STREQ( rec.key(0).c_str(), "seattle" );
    EXPECT_STREQ( rec.key(1).c_str(), "alburquerque" );
    ASSERT_TRUE( equals(rec.level(), 0.0) );
}


TEST_F(TestGAMSVariable, testGetVarType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    EXPECT_EQ( db.getVariable("x").varType(), GAMSEnum::VarType::Positive );
    EXPECT_EQ( db.getVariable("z").varType(), GAMSEnum::VarType::Free );
}


