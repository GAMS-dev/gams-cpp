/**
 * GAMS C++ API
 *
 * Copyright (c) 2017-2024 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2024 GAMS Development Corp. <support@gams.com>
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
#include "gamsparameter.h"
#include "gamsset.h"
#include "gamsvariable.h"
#include "gamssetrecord.h"

using namespace gams;

class TestGAMSSet: public TestGAMSObject
{
};

TEST_F(TestGAMSSet, testDefaultConstructor) {
    // when
    GAMSSet s;

    // then
    ASSERT_TRUE( ! s.isValid() );
    EXPECT_THROW( s.type(), GAMSException );
    EXPECT_THROW( s.logID(), GAMSException);
    EXPECT_THROW( s.dim(), GAMSException);
    EXPECT_THROW( s.checkDomains(), GAMSException );
    EXPECT_THROW( s.numberRecords(), GAMSException);
    EXPECT_THROW( s.firstRecord(), GAMSException );
    EXPECT_THROW( s.addRecord("x"), GAMSException );
}

TEST_F(TestGAMSSet, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    int numberOfSymbols = db.getNrSymbols();
    GAMSSet i = db.getSet("i");
    // when
    GAMSSet plant( i );
    // then
    EXPECT_EQ( plant.name(), i.name() );
    EXPECT_EQ( plant.numberRecords(), i.numberRecords() );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
    // when
    i.addRecord("Albuquerque");
    // then
    EXPECT_STREQ( plant.findRecord("Albuquerque").key(0).c_str(), "Albuquerque" );
    EXPECT_EQ( plant.numberRecords(), i.numberRecords() );
}

TEST_F(TestGAMSSet, testCopyConstructor_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    EXPECT_THROW( GAMSSet set_a( db.getParameter("a") ), GAMSException);
    EXPECT_THROW( GAMSSet set_z( db.getVariable("z") ), GAMSException);
    EXPECT_THROW( GAMSSet set_cost( db.getEquation("cost")), GAMSException);
}

TEST_F(TestGAMSSet, testIterator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    // when, then
    GAMSSet j;
    EXPECT_THROW( j.begin(), GAMSException);
    EXPECT_THROW( j.end(), GAMSException);

    TestGAMSObject::getTestData_Set_markets_j( db );
    j = db.getSet("j");
    std::map<std::string, int> symbolCounter;
    for (GAMSSetRecord rec : j) {
        symbolCounter[rec.key(0)]++;
    }
    // then
    EXPECT_EQ( symbolCounter.size(), size_t(3) );
    EXPECT_EQ( symbolCounter["New-York"], 1 );
    EXPECT_EQ( symbolCounter["Chicago"], 1 );
    EXPECT_EQ( symbolCounter["Topeka"], 1 );
}

TEST_F(TestGAMSSet, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    int numberOfSymbols = db.getNrSymbols();

    GAMSSet j = db.getSet("j");
    GAMSSet market = j;

    // when, then
    EXPECT_EQ( market, j );
    ASSERT_TRUE( market == j );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
}

TEST_F(TestGAMSSet, testAssignmentOperator_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    EXPECT_THROW( GAMSSet a = db.getParameter("a"), GAMSException);
    EXPECT_THROW( GAMSSet x = db.getVariable("x"), GAMSException);
    EXPECT_THROW( GAMSSet supply = db.getEquation("supply"), GAMSException);
}

TEST_F(TestGAMSSet, testGetFirstRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    GAMSSet i = db.getSet("i");

    // when, then
    GAMSSetRecord rec = i.firstRecord();
    EXPECT_STRCASEEQ( rec.key(0).c_str(), "seattle" );
}

TEST_F(TestGAMSSet, testGetFirstRecordSlice) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");

    // when, then
    GAMSSetRecord rec = j.firstRecord("topeka");
    EXPECT_STRCASEEQ( rec.key(0).c_str(), "topeka" );
}

TEST_F(TestGAMSSet, testGetFirstRecordSlice_InvalidKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");

    // when InvalidKeys, then
    EXPECT_THROW( j.firstRecord("Albuquerque"), GAMSException );
    // when IncorrectDimension, then
    EXPECT_THROW( j.firstRecord("topeka", "Albuquerque"), GAMSException );
}

TEST_F(TestGAMSSet, testGetLastRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    GAMSSet i = db.getSet("i");

    // when, then
    GAMSSetRecord rec = i.lastRecord();
    EXPECT_STRCASEEQ( rec.key(0).c_str(), "san-diego" );
}

TEST_F(TestGAMSSet, testGetLastRecordSlice) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");

    // when, then
    GAMSSetRecord rec = j.lastRecord("chicago");
    EXPECT_STRCASEEQ( rec.key(0).c_str(), "chicago" );
}

TEST_F(TestGAMSSet, testGetLastRecordSlice_InvalidKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");

    // when InvalidKeys, then
    EXPECT_THROW( j.lastRecord("Albuquerque"), GAMSException );
    // when IncorrectDimension, then
    EXPECT_THROW( j.lastRecord("topeka", "Albuquerque"), GAMSException );
}

TEST_F(TestGAMSSet, testFindRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );

    // when, then
    GAMSSetRecord rec = db.getSet("j").findRecord("Topeka");
    ASSERT_TRUE( rec.isValid() );
    EXPECT_STRCASEEQ( rec.key(0).c_str(), "topeka" );
}

TEST_F(TestGAMSSet, testFindRecord_InsensitiveCaseKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );

    // when
    GAMSSetRecord rec = db.getSet("j").findRecord("topeka");
    // then
    EXPECT_STRCASEEQ( rec.key(0).c_str(), "topeka" );
}

TEST_F(TestGAMSSet, testAddRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    {
      TestGAMSObject::getTestData_Set_plants_i( db );
      GAMSSet i = db.getSet("i");
      int numberOfRecords = i.numberRecords();
      // when
      i.addRecord("Albuquerque");
      // then
      EXPECT_EQ( i.numberRecords(), numberOfRecords+1);
      ASSERT_TRUE( i.findRecord("Albuquerque").isValid() );
    }
    {
      TestGAMSObject::getTestData_Set_markets_j( db );
      GAMSSet j = db.getSet("j");
      int numberOfRecords = j.numberRecords();
      // when, then
      EXPECT_THROW( GAMSParameterRecord rec = j.addRecord("Albuquerque"), GAMSException);
      EXPECT_EQ( j.numberRecords(), ++numberOfRecords);
      EXPECT_THROW( GAMSVariableRecord rec = j.addRecord("Austin"), GAMSException);
      EXPECT_EQ( j.numberRecords(), ++numberOfRecords);
      EXPECT_THROW( GAMSEquationRecord rec = j.addRecord("LasVegas"), GAMSException);
      EXPECT_EQ( j.numberRecords(), ++numberOfRecords);
    }
}

TEST_F(TestGAMSSet, testAddRecord_DuplicatedKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");
    int numberOfRecords = j.numberRecords();

    // when DuplicatedKeys, then
    EXPECT_THROW( db.getSet("j").addRecord("topeka"), GAMSException);

    // when IncorrectDimension, then
    EXPECT_THROW( j.addRecord(), GAMSException );
    EXPECT_THROW( j.addRecord("i1", "j1"), GAMSException );
    EXPECT_EQ( j.numberRecords(), numberOfRecords );
}

TEST_F(TestGAMSSet, testMergeExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");
    int numberOfRecords = j.numberRecords();

    // when, then
    GAMSSetRecord rec = j.mergeRecord("Chicago");
    EXPECT_EQ( j.numberRecords(), numberOfRecords );
    EXPECT_STREQ( rec.key(0).c_str(), "Chicago" );
}

TEST_F(TestGAMSSet, testMergeNonExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSet j = db.getSet("j");
    int numberOfRecords = j.numberRecords();

    // when, then
    GAMSSetRecord rec = j.mergeRecord("Albuquerque");
    EXPECT_EQ( j.numberRecords(), numberOfRecords+1 );
    EXPECT_STREQ( rec.key(0).c_str(), "Albuquerque" );
}


