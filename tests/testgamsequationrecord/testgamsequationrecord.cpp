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

#include <tuple>

#include "gamsequation.h"
#include "gamsparameter.h"
#include "gamsset.h"
#include "gamsvariable.h"
#include "gamsequationrecord.h"
#include "testgamsobject.h"

using namespace gams;

class TestGAMSEquationRecord: public TestGAMSObject
{
};

TEST_F(TestGAMSEquationRecord, testDefaultConstructor) {
    // when
    GAMSEquationRecord rec;
    // then
    ASSERT_TRUE( ! rec.isValid() );
    EXPECT_THROW( rec.type(), GAMSException );
    EXPECT_THROW( rec.logID(), GAMSException );
    EXPECT_THROW( rec.upper(), GAMSException );
    EXPECT_THROW( rec.setLevel( 0.0 ), GAMSException );
    EXPECT_THROW( rec.keys(), GAMSException );
    EXPECT_THROW( rec.moveNext(), GAMSException );
}

TEST_F(TestGAMSEquationRecord, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
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
    EXPECT_EQ( newRecord, rec );
    ASSERT_TRUE( newRecord == rec );
    EXPECT_EQ( supply.numberRecords(), numberOfRecords );
}


TEST_F(TestGAMSEquationRecord, testCopyConstructor_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    EXPECT_THROW( GAMSEquationRecord newRecord( db.getSet("i").firstRecord() ), GAMSException );
    EXPECT_THROW( GAMSEquationRecord newRecord( db.getParameter("d").firstRecord() ), GAMSException );
    EXPECT_THROW( GAMSEquationRecord newRecord( db.getVariable("x").firstRecord() ), GAMSException );
}

TEST_F(TestGAMSEquationRecord, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSEquationRecord rec = db.getEquation("demand").firstRecord();
    // when
    GAMSEquationRecord newRecord = rec;
    EXPECT_EQ( newRecord, rec );
    ASSERT_TRUE( newRecord == rec );
}

TEST_F(TestGAMSEquationRecord, testGetSetLevel) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when, then
    GAMSEquation supply = db.getEquation("supply");
    ASSERT_TRUE( equals(supply.findRecord("seattle").level(), 350.0) );
    ASSERT_TRUE( equals(supply.findRecord("san-diego").level(), 550.0) );

    GAMSEquation demand = db.getEquation("demand");
    ASSERT_TRUE( equals(demand.findRecord("topeka").level(), 275.0) );
    ASSERT_TRUE( equals(demand.findRecord("chicago").level(), 300.0) );
    ASSERT_TRUE( equals(demand.findRecord("new-york").level(), 325.0) );

    supply.addRecord("alburquerque");
    // when, then
    ASSERT_TRUE( equals(supply.findRecord("alburquerque").level(), 0.0));

    demand.firstRecord().setLevel( 123.45 );
    // when, then
    ASSERT_TRUE( equals(demand.firstRecord().level(), 123.45) );

    GAMSEquation cost = db.getEquation("cost");
    // when, then
    ASSERT_TRUE( equals(cost.firstRecord().level(), 0.0) );
}

TEST_F(TestGAMSEquationRecord, testGetSetMarginal) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSEquation supply = db.getEquation("supply");
    ASSERT_TRUE( equals( supply.findRecord("seattle").marginal(), defaultEPS ) );
    ASSERT_TRUE( equals(supply.findRecord("san-diego").marginal(), 0.0) );

    // when, then
    GAMSEquation demand = db.getEquation("demand");
    ASSERT_TRUE( equals(demand.findRecord("topeka").marginal(), 0.126) );
    ASSERT_TRUE( equals(demand.findRecord("chicago").marginal(), 0.153) );
    ASSERT_TRUE( equals(demand.findRecord("new-york").marginal(), 0.225) );

    // when, then
    GAMSEquation cost = db.getEquation("cost");
    ASSERT_TRUE( equals(cost.firstRecord().marginal(), 1.0) );

    // when, then
    cost.firstRecord().setLevel( 1.2345 );
    ASSERT_TRUE( equals(cost.firstRecord().level(), 1.2345) );
}

TEST_F(TestGAMSEquationRecord, testGetSetUpper) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;
    GAMSEquation supply = db.getEquation("supply");
    // when, then
    ASSERT_TRUE( equals( supply.findRecord("seattle").upper(), 350.0 ) );
    ASSERT_TRUE( equals(supply.findRecord("san-diego").upper(), 600.0) );

    // demand(j) ..   sum(i, x(i,j))  =g=  b(j) ;
    GAMSEquation demand = db.getEquation("demand");
    // when, then
    ASSERT_TRUE( std::isinf(demand.findRecord("topeka").upper()) );
    ASSERT_TRUE( std::isinf(demand.findRecord("chicago").upper()) );
    ASSERT_TRUE( std::isinf(demand.findRecord("new-york").upper()) );

    // cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;
    GAMSEquation cost = db.getEquation("cost");
    // when, then
    ASSERT_TRUE( equals(cost.firstRecord().upper(), 0.0) );

    // when, then
    cost.firstRecord().setUpper( 123.45 );
    ASSERT_TRUE( equals(cost.firstRecord().upper(), 123.45) );
}

TEST_F(TestGAMSEquationRecord, testGetSetLower) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;
    GAMSEquation supply = db.getEquation("supply");
    //when, then
    ASSERT_TRUE( std::isinf( - supply.findRecord("seattle").lower()) );
    ASSERT_TRUE( std::isinf( - supply.findRecord("san-diego").lower()) );

    // demand(j) ..   sum(i, x(i,j))  =g=  b(j) ;
    GAMSEquation demand = db.getEquation("demand");
    // when, then
    ASSERT_TRUE( equals(demand.findRecord("new-york").lower(), 325.0) );
    ASSERT_TRUE( equals(demand.findRecord("chicago").lower(), 300.0) );
    ASSERT_TRUE( equals(demand.findRecord("topeka").lower(), 275.0) );

    // cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;
    GAMSEquation cost = db.getEquation("cost");
    // when , then
    ASSERT_TRUE( equals(cost.firstRecord().lower(), 0.0) );
}

TEST_F(TestGAMSEquationRecord, testGetSetScale) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSEquation supply = db.getEquation("supply");
    ASSERT_TRUE( equals( supply.findRecord("seattle").scale(), 1.0 ) );
    ASSERT_TRUE( equals(supply.findRecord("san-diego").scale(), 1.0) );

    // when, then
    GAMSEquation demand = db.getEquation("demand");
    ASSERT_TRUE( equals(demand.findRecord("topeka").scale(), 1.0) );
    ASSERT_TRUE( equals(demand.findRecord("chicago").scale(), 1.0) );
    ASSERT_TRUE( equals(demand.findRecord("new-york").scale(), 1.0) );

    // when, then
    GAMSEquation cost = db.getEquation("cost");
    ASSERT_TRUE( equals(cost.firstRecord().scale(), 1.0) );

    // when, then
    cost.firstRecord().setScale( 1.2345 );
    ASSERT_TRUE( equals(cost.firstRecord().scale(), 1.2345) );
}

class ParameterizedTestGAMSEquationRecord
        : public ::testing::WithParamInterface<std::tuple<std::string, int, std::string>>,
          public TestGAMSEquationRecord {
};

INSTANTIATE_TEST_SUITE_P(testIncorrectType,
                        ParameterizedTestGAMSEquationRecord,
                        ::testing::Values (
                            std::make_tuple("plants_i",      0, "i"),
                            std::make_tuple("distance_d",    1, "d"),
                            std::make_tuple("freightcost_f", 1, "f"),
                            std::make_tuple("shipment_x",    2, "x")
                        ));

TEST_P(ParameterizedTestGAMSEquationRecord, testIncorrectType) {
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
      case GAMSEnum::SymbolType::SymTypeSet :
        {
          GAMSSet symbol = db.getSet( symbolID );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          EXPECT_THROW( GAMSSymbolRecord rec2 = GAMSParameterRecord( symbol.firstRecord()), GAMSException );
          EXPECT_THROW( GAMSParameterRecord r = rec1, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeVar :
        {
          GAMSVariable symbol = db.getVariable( symbolID );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          EXPECT_THROW( GAMSSymbolRecord rec2 = GAMSParameterRecord( symbol.firstRecord() ), GAMSException );
          EXPECT_THROW( GAMSParameterRecord r = rec1, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypePar :
      {
        GAMSParameter symbol = db.getParameter( symbolID );
        GAMSSymbolRecord rec1 = symbol.firstRecord();
        EXPECT_THROW( GAMSSymbolRecord rec2 = GAMSSetRecord( symbol.firstRecord() ), GAMSException );
        EXPECT_THROW( GAMSSetRecord r = rec1, GAMSException );
        break;
      }
      default: break;
    }
}
