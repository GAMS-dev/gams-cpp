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

#include "gamsworkspace.h"
#include "gamsexceptionexecution.h"
#include "gamsset.h"
#include "gamsparameter.h"
#include "gamsvariable.h"
#include "gamsworkspaceinfo.h"
#include "testgamsjob.h"

using namespace gams;

class TestGAMSJob: public TestGAMSObject
{
};

TEST_F(TestGAMSJob, testDefaultConstructor) {
    // when
    GAMSJob job;
    // then
    ASSERT_TRUE( ! job.isValid() );
    EXPECT_THROW( job.run(), GAMSException);
}

TEST_F(TestGAMSJob, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job;
    GAMSJob job1 = ws.addJobFromGamsLib("trnsport");
    GAMSJob job2 = ws.addJobFromString("scalar x;");
    // when, then
    ASSERT_TRUE(job != job1);
    ASSERT_TRUE(job1 != job2);
}

TEST_F(TestGAMSJob, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job1 = ws.addJobFromGamsLib("trnsport");
    GAMSJob job2 = ws.addJobFromGamsLib("trnsport");
    GAMSJob job3 = job1;
    // when, then
    ASSERT_TRUE( !( job1 == job2) );
    ASSERT_TRUE( job3 == job1 );
    // when
    job1.run();
    // then
    ASSERT_TRUE( job3 == job1 );
}

TEST_F(TestGAMSJob, testIsValid) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job1 = ws.addJobFromGamsLib("trnsport");
    ASSERT_TRUE( job1.isValid() );

    GAMSJob job2 = ws.addJobFromString("scalar x;");
    ASSERT_TRUE( job2.isValid() );
}

TEST_F(TestGAMSJob, testRun) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib("trnsport");
    // when
    job.run();
    // then
    ASSERT_TRUE( job.outDB().isValid() );
    EXPECT_EQ( job.outDB().getNrSymbols(), 12 );
    GAMSVariable z = job.outDB().getVariable("z");
    ASSERT_TRUE( equals(z.firstRecord().level(), 153.675) );
}

TEST_F(TestGAMSJob, testRunJobFromEmptyString) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSJob job = ws.addJobFromString("");
    // then
    EXPECT_THROW( job.run(), GAMSExceptionExecution );
}

TEST_F(TestGAMSJob, testOutDB) {
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSJob job = ws.addJobFromString(TestGAMSObject::getDataText());
    job.run();

    // when
    GAMSDatabase db = job.outDB();
    // then
    ASSERT_TRUE( db.isValid() );
    EXPECT_EQ( db.getNrSymbols(), 6);
    ASSERT_TRUE( db.workspace() == ws );

    EXPECT_EQ( db.getSet("i").numberRecords(), 2 );
    EXPECT_EQ( db.getSet("j").numberRecords(), 3 );
    EXPECT_EQ( db.getParameter("a").numberRecords(), 2 );
    EXPECT_EQ( db.getParameter("b").numberRecords(), 3 );
    EXPECT_EQ( db.getParameter("d").numberRecords(), 6 );
    EXPECT_EQ( db.getParameter("f").numberRecords(), 1 );
}

TEST_F(TestGAMSJob, testOutDB_BeforeRun) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromString(TestGAMSObject::getDataText());

    // when
    GAMSDatabase db = job.outDB();
    // then
    ASSERT_TRUE( ! db.isValid() );
    EXPECT_THROW( db.getNrSymbols(), GAMSException );
}

TEST_F(TestGAMSJob, testGetName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromString(TestGAMSObject::getDataText());
    // when
    QString name = QString::fromStdString(job.name());
    // then
    ASSERT_TRUE( ! name.isNull() );
    ASSERT_TRUE( ! name.isEmpty() );
    ASSERT_TRUE( name.startsWith(defaultScratchFilePrefix.c_str()) );
}

TEST_F(TestGAMSJob, testGetWorkspace) {
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSJob job = ws.addJobFromGamsLib("zloof");
    EXPECT_EQ( job.workspace(), ws );
    EXPECT_EQ( job.workspace().logID(), ws.logID() );
}

TEST_F(TestGAMSJob, testGetLogID) {
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSJob job1 = ws.addJobFromGamsLib("trnsport");
    GAMSJob job2 = ws.addJobFromGamsLib("whouse");
    GAMSJob job3 = ws.addJobFromGamsLib("trnsport");

    // we have on logID per workspace,
    // so the logID's of all jobs shall be the same
    ASSERT_TRUE( job1.logID() == job2.logID() );
    ASSERT_TRUE( job2.logID() == job3.logID() );
}


