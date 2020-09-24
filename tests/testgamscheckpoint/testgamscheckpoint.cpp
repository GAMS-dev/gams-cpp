/*
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
#include "gamscheckpoint.h"
#include "gamsdatabase.h"
#include "gamsmodelinstance.h"
#include "gamsworkspace.h"
#include "gamsjob.h"
#include "gamsplatform.h"
#include "gamsoptions.h"
#include "gamsversion.h"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
using namespace gams;

class TestGAMSCheckpoint: public TestGAMSObject
{
};

TEST_F(TestGAMSCheckpoint, testDefaultConstructor) {
    GAMSCheckpoint cp;
    ASSERT_TRUE( ! cp.isValid() );
    QVERIFY_EXCEPTION_THROWN( cp.name(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( cp.logID(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( cp.fileName(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( cp.workspace(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( cp.addModelInstance(), GAMSException );

    GAMSCheckpoint anothercp = cp;
    ASSERT_TRUE( ! anothercp.isValid() );
    ASSERT_TRUE( anothercp == cp );
}

TEST_F(TestGAMSCheckpoint, testConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);

    std::string cpname = "mycp";
    GAMSCheckpoint cp(ws, cpname);
    ASSERT_TRUE( cp.isValid() );
    QCOMPARE( cp.workspace(), ws);
    QCOMPARE( cp.name(), cpname);
}

TEST_F(TestGAMSCheckpoint, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp1 = ws.addCheckpoint();

    GAMSCheckpoint cp2 = cp1;
    QCOMPARE( cp2, cp1 );
}

TEST_F(TestGAMSCheckpoint, testAddModelInstance) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
        GAMSWorkspace ws(wsInfo);
        GAMSCheckpoint cp = ws.addCheckpoint();
        // when
        GAMSModelInstance mi = cp.addModelInstance();
        // then
        GAMSDatabase db = mi.syncDb();
        ASSERT_TRUE( db.isValid() );
        QCOMPARE( db.getNrSymbols(), 0 );
}

TEST_F(TestGAMSCheckpoint, testGetWorkspace) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    // when, then
    QCOMPARE(cp.workspace(), ws);
}

TEST_F(TestGAMSCheckpoint, testGetName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    // when, then
    ASSERT_TRUE( ! QString::fromStdString(cp.name()).isEmpty() );
    ASSERT_TRUE( QString::fromStdString(cp.name()).startsWith(defaultScratchFilePrefix.c_str()) );
}

TEST_F(TestGAMSCheckpoint, testGetLogID) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp1 = ws.addCheckpoint();
    GAMSCheckpoint cp2(cp1);
    // when, then
    QCOMPARE( cp1.logID(), cp2.logID() );
    ASSERT_TRUE( cp1.logID()== cp2.logID() );

    GAMSCheckpoint cp3 = ws.addCheckpoint("mycp");
    ASSERT_TRUE( cp3.logID() == cp1.logID());

    GAMSWorkspace anotherws(wsInfo);
    GAMSCheckpoint cp4 = anotherws.addCheckpoint("mycp");
    ASSERT_TRUE( cp4.logID() == anotherws.logID());
    ASSERT_TRUE( cp4.logID() != cp3.logID());
}

TEST_F(TestGAMSCheckpoint, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp1 = ws.addCheckpoint();
    GAMSCheckpoint cp2(cp1);
    GAMSCheckpoint cp3 = ws.addCheckpoint();
    // when, then
    ASSERT_TRUE( cp1 == cp2 );
    ASSERT_TRUE( !(cp1 == cp3) );
}

TEST_F(TestGAMSCheckpoint, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp1 = ws.addCheckpoint();
    GAMSCheckpoint cp2(cp1);
    GAMSCheckpoint cp3 = ws.addCheckpoint();
    // when, then
    ASSERT_TRUE( ! (cp1 != cp2) );
    ASSERT_TRUE( cp1 != cp3 );
    ASSERT_TRUE( cp2 != cp3 );
}

TEST_F(TestGAMSCheckpoint, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);

    GAMSCheckpoint cp1 = ws.addCheckpoint();
    // when
    GAMSCheckpoint cp2(cp1);
    // then
    QCOMPARE(cp2, cp1);
}

TEST_F(TestGAMSCheckpoint, testUninitializedCheckpoint) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();

    // when not initialized cp, then GAMSException raised
    QVERIFY_EXCEPTION_THROWN( ws.addJobFromString("Scalar s 'my scalar' /1/;", cp), GAMSException);

    // when
    ws.addJobFromString("Scalar s 'my scalar' /1/;").run(cp);
    // then
    GAMSJob job = ws.addJobFromString("display s;", cp);
    testJobBeforeRun(job, ws);
}
