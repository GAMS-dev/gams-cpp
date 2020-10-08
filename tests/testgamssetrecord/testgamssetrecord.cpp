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

class TestGAMSSetRecord: public TestGAMSObject
{
};

TEST_F(TestGAMSSetRecord, testDefaultConstructor) {
    // when
    GAMSSetRecord rec;
    // then
    ASSERT_TRUE( ! rec.isValid() );
    EXPECT_THROW( rec.type(), GAMSException );
    EXPECT_THROW( rec.logID(), GAMSException );
    EXPECT_THROW( rec.setText("x"), GAMSException );
    EXPECT_THROW( rec.keys(), GAMSException );
    EXPECT_THROW( rec.moveNext(), GAMSException );
}

TEST_F(TestGAMSSetRecord, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i(db);
    GAMSSetRecord rec = db.getSet("i").firstRecord();
    // when
    GAMSSetRecord newRecord( rec );
    EXPECT_EQ( newRecord, rec );
}

TEST_F(TestGAMSSetRecord, testCopyConstructor_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    EXPECT_THROW( GAMSSetRecord newRecord( db.getParameter("a").firstRecord() ), GAMSException );
    EXPECT_THROW( GAMSSetRecord newRecord( db.getVariable("x").firstRecord() ), GAMSException );
    EXPECT_THROW( GAMSSetRecord newRecord( db.getEquation("demand").firstRecord() ), GAMSException );
}

TEST_F(TestGAMSSetRecord, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSSetRecord rec = db.getSet("i").firstRecord();

    // when, then
    GAMSSetRecord newRecord = rec;
    EXPECT_EQ( newRecord, rec );
    ASSERT_TRUE( newRecord == rec );
}

class ParameterizedTestIncorrectType
        : public ::testing::WithParamInterface<std::tuple<std::string, int, std::string>>,
          public TestGAMSSetRecord {
};

INSTANTIATE_TEST_SUITE_P(testIncorrectType,
                        ParameterizedTestIncorrectType,
                        ::testing::Values (
                            std::make_tuple("distance_d"   , 1, "d"),
                            std::make_tuple("freightcost_f", 1, "f"),
                            std::make_tuple("obj_z"        , 2, "z"),
                            std::make_tuple("supply"       , 3, "supply")
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
      case GAMSEnum::SymbolType::SymTypePar :
        {
          GAMSParameter symbol = db.getParameter( symbolID );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          EXPECT_THROW( GAMSSymbolRecord rec2 = GAMSSetRecord( symbol.firstRecord() ), GAMSException );
          EXPECT_THROW( GAMSSetRecord r = rec1, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeVar :
        {
          GAMSVariable symbol = db.getVariable( symbolID );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          EXPECT_THROW( GAMSSymbolRecord rec2 = GAMSSetRecord( symbol.firstRecord() ), GAMSException );
          EXPECT_THROW( GAMSSetRecord r = rec1, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeEqu :
        {
          GAMSEquation symbol = db.getEquation( symbolID );
          GAMSSymbolRecord rec1 = symbol.firstRecord();
          EXPECT_THROW( GAMSSymbolRecord rec2 = GAMSSetRecord( symbol.firstRecord() ), GAMSException );
          EXPECT_THROW( GAMSSetRecord r = rec1, GAMSException );
          break;
        }
      default: break;
    }
}

TEST_F(TestGAMSSetRecord, testGetSetText) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i(db);
    GAMSSetRecord rec = db.getSet("i").firstRecord();
    rec.setText("seattle record");
    EXPECT_STREQ( rec.text().c_str(), "seattle record" );

    GAMSSet j = db.addSet("j", "set j");
    j.addRecord("j1").setText("");
    ASSERT_TRUE(j.lastRecord().text().empty());
}



