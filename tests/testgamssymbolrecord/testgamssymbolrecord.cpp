/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>
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
#include "gamssymbolrecord.h"
#include "gamsequation.h"
#include "gamsequationrecord.h"
#include "gamsset.h"
#include "gamssetrecord.h"
#include "gamsparameter.h"
#include "gamsparameterrecord.h"
#include "gamsvariable.h"
#include "gamsvariablerecord.h"

#include <sstream>
#include <string>

using namespace gams;

class TestGAMSSymbolRecord: public TestGAMSObject
{
};

TEST_F(TestGAMSSymbolRecord, testDefaultConstructor) {
    // when
    GAMSSymbolRecord rec;
    // then
    ASSERT_TRUE( ! rec.isValid() );
    EXPECT_THROW( rec.moveNext(), GAMSException);
}

TEST_F(TestGAMSSymbolRecord, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i(db);
    GAMSSymbolRecord rec = db.getSet("i").firstRecord();
    // when
    GAMSSymbolRecord newRecord = GAMSSymbolRecord( rec );
    // then
    EXPECT_EQ( newRecord, rec );
}

class ParameterizedTestAssignmentOperator
        : public ::testing::WithParamInterface<std::tuple<std::string, int, std::string>>,
          public TestGAMSSymbolRecord {
};

INSTANTIATE_TEST_SUITE_P(testAssignmentOperator,
                        ParameterizedTestAssignmentOperator,
                        ::testing::Values (
                            std::make_tuple("plants_j",      0, "j"),
                            std::make_tuple("distance_a",    1, "a"),
                            std::make_tuple("obj_x",         2, "x"),
                            std::make_tuple("supply",        3, "supply")
                        ));

TEST_P(ParameterizedTestAssignmentOperator, testAssignmentOperator) {
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
          GAMSSetRecord rec = db.getSet(symbolID).firstRecord();
          GAMSSetRecord newRecord = rec;
          EXPECT_EQ( newRecord.keys().size(), rec.keys().size() );
          for(auto i=0UL; i<newRecord.keys().size(); ++i)
              EXPECT_EQ( newRecord.key(0), rec.key(0) );
          EXPECT_EQ( newRecord, rec );
        }
        break;
      case GAMSEnum::SymbolType::SymTypePar :
        {
          GAMSParameterRecord rec = db.getParameter(symbolID).firstRecord();
          GAMSParameterRecord newRecord = rec;
          EXPECT_EQ( newRecord.keys().size(), rec.keys().size() );
          for(auto i=0UL; i<newRecord.keys().size(); ++i) {
              EXPECT_EQ( newRecord.key(0), rec.key(0) );
          }
          EXPECT_EQ( newRecord, rec );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeVar :
        {
          GAMSVariableRecord rec = db.getVariable(symbolID).firstRecord();
          GAMSVariableRecord newRecord = rec;
          EXPECT_EQ( newRecord.keys().size(), rec.keys().size() );
          for(auto i=0UL; i<newRecord.keys().size(); ++i) {
              EXPECT_EQ( newRecord.key(0), rec.key(0) );
          }
          EXPECT_EQ( newRecord, rec );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeEqu :
        {
          GAMSEquationRecord rec = db.getEquation( symbolID ).firstRecord();;
          GAMSEquationRecord newRecord = rec;
          EXPECT_EQ( newRecord.keys().size(), rec.keys().size() );
          for(auto i=0UL; i<newRecord.keys().size(); ++i) {
              EXPECT_EQ( newRecord.key(0), rec.key(0) );
          }
          EXPECT_EQ( newRecord, rec );
          break;
       }
      default: break;
    }
}

class ParameterizedTestAssignmentOperator_IncorrectType
        : public ::testing::WithParamInterface<std::tuple<std::string, int, std::string>>,
          public TestGAMSSymbolRecord {
};

INSTANTIATE_TEST_SUITE_P(testAssignmentOperator,
                        ParameterizedTestAssignmentOperator_IncorrectType,
                        ::testing::Values (
                            std::make_tuple("markets_i",     0, "i"),
                            std::make_tuple("distance_d",    1, "d"),
                            std::make_tuple("obj_z",         2, "z"),
                            std::make_tuple("supply",        3, "supply")
                        ));

TEST_P(ParameterizedTestAssignmentOperator_IncorrectType, testAssignmentOperator) {
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
          GAMSSymbolRecord rec = symbol.firstRecord();
          EXPECT_THROW( GAMSParameterRecord r = rec, GAMSException );
          EXPECT_THROW( GAMSEquationRecord r = rec, GAMSException );
          EXPECT_THROW( GAMSVariableRecord r = rec, GAMSException );
        }
        break;
      case GAMSEnum::SymbolType::SymTypePar :
        {
          GAMSParameter symbol = db.getParameter( symbolID );
          GAMSSymbolRecord rec = symbol.firstRecord();
          EXPECT_THROW( GAMSSetRecord r = rec, GAMSException );
          EXPECT_THROW( GAMSEquationRecord r = rec, GAMSException );
          EXPECT_THROW( GAMSVariableRecord r = rec, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeVar :
        {
          GAMSVariable symbol = db.getVariable( symbolID );
          GAMSSymbolRecord rec = symbol.firstRecord();
          EXPECT_THROW( GAMSSetRecord r = rec, GAMSException );
          EXPECT_THROW( GAMSEquationRecord r = rec, GAMSException );
          EXPECT_THROW( GAMSParameterRecord r = rec, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeEqu :
        {
          GAMSEquation symbol = db.getEquation( symbolID );
          GAMSSymbolRecord rec = symbol.firstRecord();
          EXPECT_THROW( GAMSSetRecord r = rec, GAMSException );
          EXPECT_THROW( GAMSVariableRecord r = rec, GAMSException );
          EXPECT_THROW( GAMSParameterRecord r = rec, GAMSException );
          break;
        }
      default: break;
    }
}

TEST_F(TestGAMSSymbolRecord, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    { GAMSSymbol symbol = db.getSet( "i" );
      GAMSSymbolRecord rec1 = symbol.firstRecord();
      GAMSSymbolRecord rec2 = symbol.firstRecord();
      GAMSSymbolRecord rec3 = symbol.findRecord("seattle");
      // then
      ASSERT_TRUE( rec1 == rec2 );
      ASSERT_TRUE( rec1 == rec3 );
    }
    // when
    { GAMSSymbol symbol = db.getParameter( "f" );
      GAMSSymbolRecord rec1 = symbol.firstRecord();
      GAMSSymbolRecord rec2 = symbol.lastRecord();
      // then
      ASSERT_TRUE( rec1 == rec2 );
    }
}

TEST_F(TestGAMSSymbolRecord, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    { GAMSSymbol symbol = db.getSet( "i" );
      GAMSSymbolRecord rec1 = symbol.firstRecord();
      GAMSSymbolRecord rec2 = symbol.lastRecord();
      GAMSSymbolRecord rec3 = symbol.findRecord("seattle");
      // then
      ASSERT_TRUE( rec1 != rec2 );
      ASSERT_TRUE( rec1 == rec3 );
      ASSERT_TRUE( rec2 != rec3 );
    }
}

TEST_F(TestGAMSSymbolRecord, testIsValid) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSSymbolRecord rec;
      ASSERT_TRUE( ! rec.isValid() );
    }
    // when, then
    for(GAMSSymbolRecord rec : db.getSet("j")) {
        ASSERT_TRUE( rec.isValid() );
    }

    // when, then
    for(GAMSSymbolRecord rec : db.getParameter("b")) {
       ASSERT_TRUE( rec.isValid() );
    }

    // when, then
    for(GAMSSymbolRecord rec : db.getParameter("f")) {
       ASSERT_TRUE( rec.isValid() );
    }

    // when, then
    for(GAMSSymbolRecord rec : db.getVariable("x")) {
       ASSERT_TRUE( rec.isValid() );
    }

    // when, then
    for(GAMSSymbolRecord rec : db.getEquation("demand")) {
       ASSERT_TRUE( rec.isValid() );
    }
}

TEST_F(TestGAMSSymbolRecord, testIterator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    { std::map<std::string, int> symbolCounter;
      for(GAMSSymbolRecord rec : db.getSet("i"))
          symbolCounter[rec.key(0)]++;
      EXPECT_EQ( symbolCounter.size(), size_t(2) );
      EXPECT_EQ( symbolCounter["seattle"], 1);
      EXPECT_EQ( symbolCounter["san-diego"], 1);
    }

    { std::map<std::string, int> symbolCounter;
      for(GAMSSymbolRecord rec : db.getParameter("b"))
          symbolCounter[rec.key(0)]++;
      EXPECT_EQ( symbolCounter.size(), size_t(3) );
      EXPECT_EQ( symbolCounter["new-york"], 1);
      EXPECT_EQ( symbolCounter["chicago"], 1);
      EXPECT_EQ( symbolCounter["topeka"], 1);
    }

    {  int i = 0;
       for(GAMSSymbolRecord rec : db.getParameter("f"))
           ++i;
       EXPECT_EQ( i, 1 );
    }

    { std::map<std::string, int> symbolCounter;
      for(GAMSSymbolRecord rec : db.getParameter("d")) {
          std::stringstream ss;
          ss << rec.key(0) << "_" <<rec.key(1) ;
          symbolCounter[ss.str()]++;
      }
      EXPECT_EQ( symbolCounter.size(), size_t(6) );
      EXPECT_EQ( symbolCounter["seattle_new-york"], 1);
      EXPECT_EQ( symbolCounter["seattle_chicago"], 1);
      EXPECT_EQ( symbolCounter["seattle_topeka"], 1);
      EXPECT_EQ( symbolCounter["san-diego_new-york"], 1);
      EXPECT_EQ( symbolCounter["san-diego_chicago"], 1);
      EXPECT_EQ( symbolCounter["san-diego_topeka"], 1);
    }

    { std::map<std::string, int> symbolCounter;
      for(GAMSSymbolRecord rec : db.getVariable("x")) {
          std::stringstream ss;
          ss << rec.key(0) << "_" <<rec.key(1) ;
          symbolCounter[ss.str()]++;
      }
      EXPECT_EQ( symbolCounter.size(), size_t(6) );
      EXPECT_EQ( symbolCounter["seattle_new-york"], 1);
      EXPECT_EQ( symbolCounter["seattle_chicago"], 1);
      EXPECT_EQ( symbolCounter["seattle_topeka"], 1);
      EXPECT_EQ( symbolCounter["san-diego_new-york"], 1);
      EXPECT_EQ( symbolCounter["san-diego_chicago"], 1);
      EXPECT_EQ( symbolCounter["san-diego_topeka"], 1);
    }

    { std::map<std::string, int> symbolCounter;
      for(GAMSSymbolRecord rec : db.getEquation("supply"))
         symbolCounter[rec.key(0)]++;
      EXPECT_EQ( symbolCounter.size(), size_t(2) );
      EXPECT_EQ( symbolCounter["seattle"], 1);
      EXPECT_EQ( symbolCounter["san-diego"], 1);
    }
}

TEST_F(TestGAMSSymbolRecord, testGetKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbolRecord rec_j = db.getSet("j").lastRecord();
    EXPECT_EQ( rec_j.keys().size(), size_t(1));

    GAMSSymbolRecord rec_d = db.getParameter("d").lastRecord();
    EXPECT_EQ( rec_d.keys().size(), size_t(2));

    GAMSSymbolRecord rec_f = db.getParameter("f").lastRecord();
    EXPECT_EQ( rec_f.keys().size(), size_t(0));

    GAMSSymbolRecord rec_x = db.getVariable("x").lastRecord();
    EXPECT_EQ( rec_x.keys().size(), size_t(2));

    GAMSSymbolRecord rec_cost = db.getEquation("cost").lastRecord();
    EXPECT_EQ( rec_cost.keys().size(), size_t(0));

    GAMSSymbolRecord rec_demand = db.getEquation("demand").lastRecord();
    EXPECT_EQ( rec_demand.keys().size(), size_t(1));
}

TEST_F(TestGAMSSymbolRecord, testGetKey_index) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbolRecord rec_i = db.getSet("i").lastRecord();
    EXPECT_STREQ( rec_i.key(0).c_str(), "san-diego");
    EXPECT_THROW( rec_i.key(1), GAMSException );

    GAMSSymbolRecord rec_d = db.getParameter("d").lastRecord();
    EXPECT_STREQ( rec_d.key(0).c_str(), "san-diego");
    EXPECT_STREQ( rec_d.key(1).c_str(), "topeka");
    EXPECT_THROW( rec_d.key(2), GAMSException );

    GAMSSymbolRecord rec_f = db.getParameter("f").lastRecord();
    EXPECT_THROW( rec_f.key(0), GAMSException );

    GAMSSymbolRecord rec_x = db.getVariable("x").lastRecord();
    EXPECT_STREQ( rec_x.key(0).c_str(), "san-diego");
    EXPECT_STREQ( rec_x.key(1).c_str(), "topeka");
    EXPECT_THROW( rec_x.key(2), GAMSException );

    GAMSSymbolRecord rec_cost = db.getEquation("cost").lastRecord();
    EXPECT_THROW( rec_cost.key(0), GAMSException );

    GAMSSymbolRecord rec_supply = db.getEquation("supply").lastRecord();
    EXPECT_STREQ( rec_supply.key(0).c_str(), "san-diego");
    EXPECT_THROW( rec_supply.key(1), GAMSException );
}

TEST_F(TestGAMSSymbolRecord, testIndexOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbolRecord rec_i = db.getSet("i").lastRecord();
    EXPECT_STREQ( rec_i[0].c_str(), "san-diego");
    EXPECT_THROW( rec_i[1], GAMSException );

    GAMSSymbolRecord rec_d = db.getParameter("d").lastRecord();
    EXPECT_STREQ( rec_d[0].c_str(), "san-diego");
    EXPECT_STREQ( rec_d[1].c_str(), "topeka");
    EXPECT_THROW( rec_d[2], GAMSException );

    GAMSSymbolRecord rec_f = db.getParameter("f").lastRecord();
    EXPECT_THROW( rec_f[0], GAMSException );

    GAMSSymbolRecord rec_x = db.getVariable("x").lastRecord();
    EXPECT_STREQ( rec_x[0].c_str(), "san-diego");
    EXPECT_STREQ( rec_x[1].c_str(), "topeka");
    EXPECT_THROW( rec_x[2], GAMSException );

    GAMSSymbolRecord rec_cost = db.getEquation("cost").lastRecord();
    EXPECT_THROW( rec_cost[0], GAMSException );

    GAMSSymbolRecord rec_supply = db.getEquation("supply").lastRecord();
    EXPECT_STREQ( rec_supply[0].c_str(), "san-diego");
    EXPECT_THROW( rec_supply[1], GAMSException );
}

TEST_F(TestGAMSSymbolRecord, testGetType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbolRecord rec_i( db.getSet("i").firstRecord() );
    EXPECT_EQ( rec_i.type(), GAMSEnum::SymbolType::SymTypeSet );
    // when, then
    GAMSSymbolRecord rec_j( db.getSet("j").lastRecord() );
    EXPECT_EQ( rec_j.type(), GAMSEnum::SymbolType::SymTypeSet );
    // when, then
    GAMSSymbolRecord rec_a( db.getParameter("a").firstRecord() );
    EXPECT_EQ( rec_a.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbolRecord rec_b( db.getParameter("b").lastRecord() );
    EXPECT_EQ( rec_b.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbolRecord rec_d( db.getParameter("d").firstRecord() );
    EXPECT_EQ( rec_d.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbolRecord rec_f( db.getParameter("f").lastRecord() );
    EXPECT_EQ( rec_f.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbolRecord rec_c( db.getParameter("c").firstRecord() );
    EXPECT_EQ( rec_c.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbolRecord rec_cost( db.getEquation("cost").lastRecord() );
    EXPECT_EQ( rec_cost.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbolRecord rec_supply( db.getEquation("supply").firstRecord() );
    EXPECT_EQ( rec_supply.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbolRecord rec_demand( db.getEquation("demand").lastRecord() );
    EXPECT_EQ( rec_demand.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbolRecord rec_x( db.getVariable("x").firstRecord() );
    EXPECT_EQ( rec_x.type(), GAMSEnum::SymbolType::SymTypeVar );
    // when, then
    GAMSSymbolRecord rec_z( db.getVariable("z").lastRecord() );
    EXPECT_EQ( rec_z.type(), GAMSEnum::SymbolType::SymTypeVar );
}


