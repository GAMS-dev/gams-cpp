/**
 * GAMS C++ API
 *
 * Copyright (c) 2017-2025 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2025 GAMS Development Corp. <support@gams.com>
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
#include "gamsequationrecord.h"
#include "gamsparameter.h"
#include "gamsparameterrecord.h"
#include "gamsset.h"
#include "gamssetrecord.h"
#include "gamsvariable.h"
#include "gamsvariablerecord.h"

using namespace gams;

class TestGAMSVariableRecord: public TestGAMSObject
{
};

TEST_F(TestGAMSVariableRecord, testDefaultConstructor) {
    // when
    GAMSVariableRecord rec;
    // then
    ASSERT_TRUE( ! rec.isValid() );
    EXPECT_THROW( rec.type(), GAMSException );
    EXPECT_THROW( rec.logID(), GAMSException );
    EXPECT_THROW( rec.upper(), GAMSException );
    EXPECT_THROW( rec.setLevel( 0.0 ), GAMSException );
    EXPECT_THROW( rec.keys(), GAMSException );
    EXPECT_THROW( rec.moveNext(), GAMSException );
}

TEST_F(TestGAMSVariableRecord, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
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
    EXPECT_EQ( newRecord, rec );
    ASSERT_TRUE( newRecord == rec );
    EXPECT_EQ( x.numberRecords(), numberOfRecords );
}

TEST_F(TestGAMSVariableRecord, testCopyConstructor_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    EXPECT_THROW( GAMSVariableRecord newRecord( db.getSet("i").firstRecord() ), GAMSException );
    EXPECT_THROW( GAMSVariableRecord newRecord( db.getParameter("d").firstRecord() ), GAMSException );
    EXPECT_THROW( GAMSVariableRecord newRecord( db.getEquation("demand").firstRecord() ), GAMSException );
}

TEST_F(TestGAMSVariableRecord, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariableRecord rec = db.getVariable("x").firstRecord();
    // when
    GAMSVariableRecord newRecord = rec;
    EXPECT_EQ( newRecord, rec );
    ASSERT_TRUE( newRecord == rec );
}

class ParameterizedTestIncorrectType
        : public ::testing::WithParamInterface<std::tuple<std::string, int, std::string>>,
          public TestGAMSVariableRecord {
};

INSTANTIATE_TEST_SUITE_P(testIncorrectType,
                        ParameterizedTestIncorrectType,
                        ::testing::Values (
                            std::make_tuple("plants_i",      0, "i"),
                            std::make_tuple("distance_d",    1, "d"),
                            std::make_tuple("freightcost_f", 1, "f"),
                            std::make_tuple("shipment_x",    2, "x")
                        ));

TEST_P(ParameterizedTestIncorrectType, testIncorrectType) {
    int symbolType = std::get<1>(GetParam());
    std::string symbolID = std::get<2>(GetParam());

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    switch(symbolType) {
      case GAMSEnum::SymbolType::SymTypeSet :  {
          GAMSSet symbol = db.getSet( symbolID );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          EXPECT_THROW( GAMSSymbolRecord rec2 = GAMSVariableRecord( symbol.firstRecord() ), GAMSException );
          EXPECT_THROW( GAMSVariableRecord r = rec1, GAMSException );
          break;
      }
      case GAMSEnum::SymbolType::SymTypePar : {
         GAMSParameter symbol = db.getParameter( symbolID );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          EXPECT_THROW( GAMSSymbolRecord rec2 = GAMSVariableRecord( symbol.firstRecord() ), GAMSException );
          EXPECT_THROW( GAMSVariableRecord r = rec1, GAMSException );
          break;
      }
      case GAMSEnum::SymbolType::SymTypeEqu : {
          GAMSEquation symbol = db.getEquation( symbolID );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          EXPECT_THROW( GAMSSymbolRecord rec2 = GAMSVariableRecord( symbol.firstRecord() ), GAMSException );
          EXPECT_THROW( GAMSVariableRecord r = rec1, GAMSException );
          break;
      }
      default: break;
    }
}

TEST_F(TestGAMSVariableRecord, testGetSetLevel) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    GAMSVariable varx = db.getVariable("x");
    ASSERT_TRUE( equals(varx.findRecord("seattle", "new-york").level(), 50.0) );
    ASSERT_TRUE( equals(varx.findRecord("seattle", "chicago").level(), 300.0) );
    ASSERT_TRUE( equals(varx.findRecord("seattle", "topeka").level(), 0.0) );
    ASSERT_TRUE( equals(varx.findRecord("san-diego", "new-york").level(), 275.0) );
    ASSERT_TRUE( equals(varx.findRecord("san-diego", "chicago").level(), 0.0) );
    ASSERT_TRUE( equals(varx.findRecord("san-diego", "topeka").level(), 275.0) );

    varx.addRecord("alburquerque", "topeka");
    ASSERT_TRUE( equals(varx.findRecord("alburquerque", "topeka").level(), 0.0) );
    // when, then
    GAMSVariable varz = db.getVariable("z");
    varz.firstRecord().setLevel( 123.45 );
    ASSERT_TRUE( equals(varz.firstRecord().level(), 123.45) );
}

TEST_F(TestGAMSVariableRecord, testGetSetMarginal) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    GAMSVariable varx = db.getVariable("x");
    ASSERT_TRUE( equals(varx.findRecord("seattle", "new-york").marginal(), 0.0) );
    ASSERT_TRUE( equals(varx.findRecord("seattle", "chicago").marginal(), 0.0)  );
    ASSERT_TRUE( equals(varx.findRecord("seattle", "topeka").marginal(), 0.036) );
    ASSERT_TRUE( equals(varx.findRecord("san-diego", "new-york").marginal(), 0.0) );
    ASSERT_TRUE( equals(varx.findRecord("san-diego", "chicago").marginal(), 0.00900000000000001) );
    ASSERT_TRUE( equals(varx.findRecord("san-diego", "topeka").marginal(), 0.0) );

    varx.addRecord("alburquerque", "topeka");
    ASSERT_TRUE( equals(varx.findRecord("alburquerque", "topeka").level(), 0.0) );

    // when, then
    GAMSVariable varz = db.getVariable("z");
    varz.firstRecord().setMarginal( 0.036 );
    ASSERT_TRUE( equals(varz.firstRecord().marginal(), 0.036) );
}

TEST_F(TestGAMSVariableRecord, testGetSetUpper) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable varz = db.getVariable("z");
    GAMSVariable varx = db.getVariable("x");
    { // when get default Upper Bound on free variable record, then pinf is expected
      ASSERT_TRUE( std::isinf(varz.firstRecord().upper()) );
      ASSERT_TRUE( std::isinf(varx.firstRecord().upper()) );

      // when get default Lower Bound on positive variable recrod,  then
      ASSERT_TRUE( std::isinf(varx.findRecord("seattle", "new-york").upper()) );
      ASSERT_TRUE( std::isinf(varx.findRecord("seattle", "chicago").upper()) );
      ASSERT_TRUE( std::isinf(varx.findRecord("san-diego", "new-york").upper()) );
      ASSERT_TRUE( std::isinf(varx.findRecord("san-diego", "chicago").upper()) );
      ASSERT_TRUE( std::isinf(varx.findRecord("san-diego", "topeka").upper()) );
    }
    { // when, then
      varz.firstRecord().setUpper( 0.036 );
      ASSERT_TRUE( equals(varz.firstRecord().upper(), 0.036) );
      // when, then
      varx.firstRecord().setUpper( -1 );
      ASSERT_TRUE( equals(varx.firstRecord().upper(), -1) );
    }
}

TEST_F(TestGAMSVariableRecord, testGetSetLower) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSVariable varz = db.getVariable("z");
    GAMSVariable varx = db.getVariable("x");
    { // when get default Lower Bound on free variable record,  then minf is expected
      ASSERT_TRUE( std::isinf(-varz.firstRecord().lower()) );
      // when get default Lower Bound on positive variable record,  then 0.0 is expected
      ASSERT_TRUE( equals(varx.firstRecord().lower(), 0.0) );

      // when get default Lower Bound on positive variable recrod,  then
      ASSERT_TRUE( equals(varx.findRecord("seattle", "new-york").lower(), 0.0) );
      ASSERT_TRUE( equals(varx.findRecord("seattle", "chicago").lower(), 0.0) );
      ASSERT_TRUE( equals(varx.findRecord("san-diego", "new-york").lower(), 0.0) );
      ASSERT_TRUE( equals(varx.findRecord("san-diego", "chicago").lower(), 0.0) );
      ASSERT_TRUE( equals(varx.findRecord("san-diego", "topeka").lower(), 0.0) );
    }
    { // when, then
      varz.firstRecord().setLower( 0.036 );
      ASSERT_TRUE( equals(varz.firstRecord().lower(), 0.036) );
      // when set lower level on free var to minf, then
      varz.firstRecord().setLower( defaultMINF );
      ASSERT_TRUE( std::isinf(-varz.firstRecord().lower()) );
      // when set lower level on positive var to negative value, then
      varx.firstRecord().setLower( -1 );
      ASSERT_TRUE( equals(varx.firstRecord().lower(), -1) );
    }
}

TEST_F(TestGAMSVariableRecord, testGetSetScale) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    GAMSVariable varx = db.getVariable("x");
    ASSERT_TRUE( equals(varx.findRecord("seattle", "new-york").scale(), 1.0) );
    ASSERT_TRUE( equals(varx.findRecord("seattle", "chicago").scale(), 1.0) );
    ASSERT_TRUE( equals(varx.findRecord("seattle", "topeka").scale(), 1.0) );
    ASSERT_TRUE( equals(varx.findRecord("san-diego", "new-york").scale(), 1.0) );
    ASSERT_TRUE( equals(varx.findRecord("san-diego", "chicago").scale(), 1.0) );
    ASSERT_TRUE( equals(varx.findRecord("san-diego", "topeka").scale(), 1.0) );

    varx.addRecord("alburquerque", "topeka");
    ASSERT_TRUE( equals(varx.findRecord("alburquerque", "topeka").scale(), 1.0) );
    // when, then
    GAMSVariable varz = db.getVariable("z");
    varz.firstRecord().setScale( 2.0 );
    ASSERT_TRUE( equals(varz.firstRecord().scale(), 2.0) );
}


