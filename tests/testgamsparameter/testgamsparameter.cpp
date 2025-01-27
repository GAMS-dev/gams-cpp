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
#include "gamsparameter.h"
#include "gamsparameterrecord.h"
#include "gamsset.h"
#include "gamsvariable.h"
#include "gamsworkspace.h"
#include "gamsworkspaceinfo.h"

using namespace gams;

class TestGAMSParameter: public TestGAMSObject
{
};

TEST_F(TestGAMSParameter, testDefaultConstructor) {
    // when
    GAMSParameter p;
    // then
    ASSERT_TRUE( ! p.isValid() );
    EXPECT_THROW( p.type(), GAMSException );
    EXPECT_THROW( p.logID(), GAMSException);
    EXPECT_THROW( p.dim(), GAMSException);
    EXPECT_THROW( p.checkDomains(), GAMSException);
    EXPECT_THROW( p.numberRecords(), GAMSException);
    EXPECT_THROW( p.firstRecord(), GAMSException );
    EXPECT_THROW( p.addRecord("x"), GAMSException );
}

TEST_F(TestGAMSParameter, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    int numberOfSymbols = db.getNrSymbols();
    GAMSParameter a = db.getParameter("a");
    // when
    GAMSParameter capacity( a );
    // then
    EXPECT_EQ( capacity.name(), a.name() );
    EXPECT_EQ( capacity.numberRecords(), a.numberRecords() );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
    // when
    a.addRecord("Albuquerque");
    // then
    EXPECT_STREQ( capacity.findRecord("Albuquerque").key(0).c_str(), "Albuquerque" );
    EXPECT_EQ( capacity.numberRecords(), a.numberRecords() );
}

TEST_F(TestGAMSParameter, testCopyConstructor_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    EXPECT_THROW( GAMSParameter param_i( db.getSet("i") ), GAMSException);
    EXPECT_THROW( GAMSParameter param_z( db.getVariable("z") ), GAMSException);
    EXPECT_THROW( GAMSParameter param_cost( db.getEquation("cost")), GAMSException);
}

TEST_F(TestGAMSParameter, testIterator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    // when, then
    GAMSParameter b;
    EXPECT_THROW( b.begin(), GAMSException);
    EXPECT_THROW( b.end(), GAMSException);

    TestGAMSObject::getTestData_Parameter_demand_b( db );
    b = db.getParameter("b");
    std::map<std::string, double> symbolMap;
    for (GAMSParameterRecord rec : b) {
        symbolMap[rec.key(0)] = rec.value() ;
    }
    // then
    EXPECT_EQ( symbolMap.size(), size_t(3) );
    EXPECT_EQ( symbolMap["New-York"], 325.0 );
    EXPECT_EQ( symbolMap["Chicago"], 300.0 );
    EXPECT_EQ( symbolMap["Topeka"], 275.0 );
}

TEST_F(TestGAMSParameter, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameter a = db.getParameter("a");
    int numberOfSymbols = db.getNrSymbols();

    // when, then
    GAMSParameter capacity = a;
    EXPECT_EQ( capacity, a );
    ASSERT_TRUE( capacity == a );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
}

TEST_F(TestGAMSParameter, testAssignmentOperator_IncorrectType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    EXPECT_THROW( GAMSParameter i = db.getSet("i"), GAMSException);
    EXPECT_THROW( GAMSParameter x = db.getVariable("x"), GAMSException);
    EXPECT_THROW( GAMSParameter supply = db.getEquation("supply"), GAMSException);
}

TEST_F(TestGAMSParameter, testGetFirstRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameter a = db.getParameter("a");

    // when, then
    GAMSParameterRecord rec = a.firstRecord();
    EXPECT_STREQ( rec.key(0).c_str(), "Seattle" );
    ASSERT_TRUE( equals(rec.value(), 350.0) );
}

TEST_F(TestGAMSParameter, testGetFirstRecordSlice) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b(db);
    GAMSParameter b = db.getParameter("b");

    // when, then
    GAMSParameterRecord rec = b.firstRecord("Chicago");
    EXPECT_STREQ( rec.key(0).c_str(), "Chicago" );
    ASSERT_TRUE( equals(rec.value(), 300.0) );
}

TEST_F(TestGAMSParameter, testGetFirstRecordSlice_InvalidKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameter a = db.getParameter("a");

    // when InvalidKeys, then
    EXPECT_THROW( a.firstRecord("Albuquerque"), GAMSException );
    // when IncorrectDimension, then
    EXPECT_THROW( a.firstRecord("Seattle", "Albuquerque"), GAMSException);
}

TEST_F(TestGAMSParameter, testGetLastRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameter a = db.getParameter("a");

    // when, then
    GAMSParameterRecord rec = a.lastRecord();
    EXPECT_STREQ( rec.key(0).c_str(), "San-Diego" );
    EXPECT_EQ( rec.value(), 600.0 );
}

TEST_F(TestGAMSParameter, testGetLastRecordSlice) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");

    // when, then
    GAMSParameterRecord rec = b.lastRecord("New-York");
    EXPECT_STREQ( rec.key(0).c_str(), "New-York" );
    EXPECT_EQ( rec.value(), 325.0 );
}

TEST_F(TestGAMSParameter, testGetLastRecordSlice_InvalidKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");

    // when InvalidKeys, then
    EXPECT_THROW( b.lastRecord("Albuquerque"), GAMSException);
    // when IncorrectDimension, then
    EXPECT_THROW( b.lastRecord("seattle", "Albuquerque"), GAMSException);
}

TEST_F(TestGAMSParameter, testFindRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");

    // when, then
    GAMSParameterRecord rec = db.getParameter("b").findRecord("Topeka");
    ASSERT_TRUE( rec.isValid() );
    EXPECT_STREQ( rec.key(0).c_str(), "Topeka");
    ASSERT_TRUE( equals(rec.value(), 275.0) );
}

TEST_F(TestGAMSParameter, testFindRecord_InsensitiveCaseKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );

    // when
    GAMSParameterRecord rec = db.getParameter("b").findRecord("CHICAGO");
    // then
    EXPECT_STRCASEEQ( rec.key(0).c_str(), "chicago" );
    ASSERT_TRUE( equals(rec.value(), 300.0) );
}

TEST_F(TestGAMSParameter, testAddRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");
    int numberOfRecords = b.numberRecords();

    // when
    b.addRecord("Albuquerque");

    // then
    EXPECT_EQ( b.numberRecords(), numberOfRecords+ 1);
    ASSERT_TRUE( b.findRecord("Albuquerque").isValid() );
    ASSERT_TRUE( b.findRecord("Albuquerque").value() - 0.0 <= defaultEPS );

    // when
    { TestGAMSObject::getTestData_Parameter_freightcost_f( db );
      // when, then
      EXPECT_THROW( db.getParameter("f").addRecord(), GAMSException );
    }
    {
      TestGAMSObject::getTestData_Parameter_capacity_a( db );
      GAMSParameter a = db.getParameter("a");
      int numberOfRecords = a.numberRecords();
      // when, then
      EXPECT_THROW( GAMSSetRecord rec = a.addRecord("Albuquerque"), GAMSException);
      EXPECT_EQ( a.numberRecords(), ++numberOfRecords);
      EXPECT_THROW( GAMSVariableRecord rec = a.addRecord("Austin"), GAMSException);
      EXPECT_EQ( a.numberRecords(), ++numberOfRecords);
      EXPECT_THROW( GAMSEquationRecord rec = a.addRecord("LasVegas"), GAMSException);
      EXPECT_EQ( a.numberRecords(), ++numberOfRecords);
    }
}

TEST_F(TestGAMSParameter, testAddRecord_DuplicatedKeys_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");
    int numberOfRecords = b.numberRecords();

    // when DuplicatedKeys, then
    EXPECT_THROW( db.getParameter("b").addRecord("Topeka"), GAMSException);

    // when IncorrectDimension, then
    EXPECT_THROW( b.addRecord(), GAMSException );
    EXPECT_THROW( b.addRecord("i1", "j1"), GAMSException );
    EXPECT_EQ( b.numberRecords(), numberOfRecords );
}

TEST_F(TestGAMSParameter, testMergeExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    GAMSParameter b = db.getParameter("b");
    int numberOfRecords = b.numberRecords();

    // when, then
    GAMSParameterRecord rec = b.mergeRecord("Chicago");
    EXPECT_EQ( b.numberRecords(), numberOfRecords );
    EXPECT_STREQ( rec.key(0).c_str(), "Chicago" );
}

TEST_F(TestGAMSParameter, testMergeNonExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSParameter a = db.getParameter("a");
    int numberOfRecords = a.numberRecords();

    // when, then
    GAMSParameterRecord rec = a.mergeRecord("Albuquerque");
    EXPECT_EQ( a.numberRecords(), numberOfRecords+1 );
    EXPECT_STREQ( rec.key(0).c_str(), "Albuquerque" );
}


