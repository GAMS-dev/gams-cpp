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

class TestGAMSParameterRecord: public TestGAMSObject
{
};

TEST_F(TestGAMSParameterRecord, testDefaultConstructor) {
    // when
    GAMSParameterRecord rec;
    // then
    ASSERT_TRUE( ! rec.isValid() );
    EXPECT_THROW( rec.type(), GAMSException );
    EXPECT_THROW( rec.logID(), GAMSException );
    EXPECT_THROW( rec.setValue( 0.0 ), GAMSException );
    EXPECT_THROW( rec.keys(), GAMSException );
    EXPECT_THROW( rec.moveNext(), GAMSException );
}

TEST_F(TestGAMSParameterRecord, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameterRecord rec = db.getParameter("a").firstRecord();
    // when
    GAMSParameterRecord newRecord( rec );
    EXPECT_EQ( newRecord, rec );
}

TEST_F(TestGAMSParameterRecord, testCopyConstructor_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    EXPECT_THROW( GAMSParameterRecord newRecord( db.getSet("i").firstRecord() ), GAMSException );
    EXPECT_THROW( GAMSParameterRecord newRecord( db.getVariable("x").firstRecord() ), GAMSException );
    EXPECT_THROW( GAMSParameterRecord newRecord( db.getEquation("demand").firstRecord() ), GAMSException );
}

TEST_F(TestGAMSParameterRecord, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameterRecord rec = db.getParameter("a").firstRecord();
    // when
    GAMSParameterRecord newRecord = rec;
    EXPECT_EQ( newRecord, rec );
    ASSERT_TRUE( newRecord == rec );
}
class ParameterizedTtestIncorrectType
        : public ::testing::WithParamInterface<std::tuple<std::string, int, std::string>>,
          public TestGAMSParameterRecord {
};

INSTANTIATE_TEST_SUITE_P(testIncorrectType,
                        ParameterizedTtestIncorrectType,
                        ::testing::Values (
                            std::make_tuple("markets_i",  0, "i"),
                            std::make_tuple("shipment_x", 2, "x"),
                            std::make_tuple("obj_z",      2, "z"),
                            std::make_tuple("supply",     3, "supply")
                        ));

TEST_P(ParameterizedTtestIncorrectType, testIncorrectType) {
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
      case GAMSEnum::SymbolType::SymTypeEqu :
        {
          GAMSEquation symbol = db.getEquation( symbolID );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          EXPECT_THROW( GAMSSymbolRecord rec2 = GAMSParameterRecord( symbol.firstRecord() ), GAMSException );
          EXPECT_THROW( GAMSParameterRecord r = rec1, GAMSException );
          break;
        }
      default: break;
    }
}

TEST_F(TestGAMSParameterRecord, testGetValue) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_distance_d( db );

    GAMSParameter param = db.getParameter("d");
    ASSERT_TRUE( equals(param.findRecord("seattle", "new-york").value(), 2.5) );
    ASSERT_TRUE( equals(param.findRecord("seattle", "chicago").value(), 1.7) );
    ASSERT_TRUE( equals(param.findRecord("seattle", "topeka").value(), 1.8) );
    ASSERT_TRUE( equals(param.findRecord("san-diego", "new-york").value(), 2.5) );
    ASSERT_TRUE( equals(param.findRecord("san-diego", "chicago").value(), 1.8) );
    ASSERT_TRUE( equals(param.findRecord("san-diego", "topeka").value(), 1.4) );

    param.addRecord("alburquerque", "topeka");
    ASSERT_TRUE( equals(param.findRecord("alburquerque", "topeka").value(), 0.0) );
}

TEST_F(TestGAMSParameterRecord, testSetValue) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );

    GAMSParameter param = db.getParameter("b");
    param.findRecord("new-york").setValue( 3.2 );
    ASSERT_TRUE( equals(param.findRecord("new-york").value(), 3.2) );

    param.addRecord("alburquerque").setValue( 4.2 );
    ASSERT_TRUE( equals(param.findRecord("alburquerque").value(), 4.2) );
}


