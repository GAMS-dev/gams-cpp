/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
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

#include "gamscheckpoint.h"
#include "gamsdatabase.h"
#include "gamsmodelinstance.h"
#include "gamsworkspace.h"
#include "testgamscheckpoint.h"

using namespace gams;

QString TestGAMSCheckpoint::classname()  { return "TestGAMSCheckpoint"; }

void TestGAMSCheckpoint::testDefaultConstructor()  {
    GAMSCheckpoint cp;
    QVERIFY( ! cp.isValid() );
    QVERIFY_EXCEPTION_THROWN( cp.name(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( cp.logID(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( cp.fileName(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( cp.workspace(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( cp.addModelInstance(), GAMSException );

    GAMSCheckpoint anothercp = cp;
    QVERIFY( ! anothercp.isValid() );
    QVERIFY( anothercp == cp );
}

void TestGAMSCheckpoint::testConstructor()  {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    std::string cpname = "mycp";
    GAMSCheckpoint cp(ws, cpname);
    QVERIFY( cp.isValid() );
    QCOMPARE( cp.workspace(), ws);
    QCOMPARE( cp.name(), cpname);
}

void TestGAMSCheckpoint::testAssignmentOperator()  {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp1 = ws.addCheckpoint();

    GAMSCheckpoint cp2 = cp1;
    QCOMPARE( cp2, cp1 );
}

void TestGAMSCheckpoint::testAddModelInstance() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    // when
    GAMSModelInstance mi = cp.addModelInstance();
    // then
    GAMSDatabase db = mi.syncDb();
    QVERIFY( db.isValid() );
    QCOMPARE( db.getNrSymbols(), 0 );
}

void TestGAMSCheckpoint::testGetWorkspace() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    // when, then
    QCOMPARE(cp.workspace(), ws);
}

void TestGAMSCheckpoint::testGetName() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    // when, then
    QVERIFY( ! QString::fromStdString(cp.name()).isEmpty() );
    QVERIFY( QString::fromStdString(cp.name()).startsWith(defaultScratchFilePrefix.c_str()) );
}

void TestGAMSCheckpoint::testGetLogID() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp1 = ws.addCheckpoint();
    GAMSCheckpoint cp2(cp1);
    // when, then
    QCOMPARE( cp1.logID(), cp2.logID() );
    QVERIFY( cp1.logID()== cp2.logID() );

    GAMSCheckpoint cp3 = ws.addCheckpoint("mycp");
    QVERIFY( cp3.logID() == cp1.logID());

    GAMSWorkspace anotherws(wsInfo);
    GAMSCheckpoint cp4 = anotherws.addCheckpoint("mycp");
    QVERIFY( cp4.logID() == anotherws.logID());
    QVERIFY( cp4.logID() != cp3.logID());
}

void TestGAMSCheckpoint::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp1 = ws.addCheckpoint();
    GAMSCheckpoint cp2(cp1);
    GAMSCheckpoint cp3 = ws.addCheckpoint();
    // when, then
    QVERIFY( cp1 == cp2 );
    QVERIFY( !(cp1 == cp3) );
}

void TestGAMSCheckpoint::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp1 = ws.addCheckpoint();
    GAMSCheckpoint cp2(cp1);
    GAMSCheckpoint cp3 = ws.addCheckpoint();
    // when, then
    QVERIFY( ! (cp1 != cp2) );
    QVERIFY( cp1 != cp3 );
    QVERIFY( cp2 != cp3 );
}

void TestGAMSCheckpoint::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    GAMSCheckpoint cp1 = ws.addCheckpoint();
    // when
    GAMSCheckpoint cp2(cp1);
    // then
    QCOMPARE(cp2, cp1);
}

void TestGAMSCheckpoint::testUninitializedCheckpoint() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();

    // when not initialized cp, then GAMSException raised
    QVERIFY_EXCEPTION_THROWN( ws.addJobFromString("Scalar s 'my scalar' /1/;", cp), GAMSException);

    // when
    ws.addJobFromString("Scalar s 'my scalar' /1/;").run(cp);
    // then
    GAMSJob job = ws.addJobFromString("display s;", cp);
    TestGAMSObject::testJobBeforeRun(job, ws);
}

QTEST_MAIN(TestGAMSCheckpoint)
