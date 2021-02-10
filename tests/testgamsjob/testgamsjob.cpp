/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2021 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2021 GAMS Development Corp. <support@gams.com>
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

QString TestGAMSJob::classname()  { return "TestGAMSJob"; }

void TestGAMSJob::testDefaultConstructor() {
    // when
    GAMSJob job;
    // then
    QVERIFY( ! job.isValid() );
    QVERIFY_EXCEPTION_THROWN( job.run(), GAMSException);
}

void TestGAMSJob::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job;
    GAMSJob job1 = ws.addJobFromGamsLib("trnsport");
    GAMSJob job2 = ws.addJobFromString("scalar x;");
    // when, then
    QVERIFY(job != job1);
    QVERIFY(job1 != job2);
}

void TestGAMSJob::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job1 = ws.addJobFromGamsLib("trnsport");
    GAMSJob job2 = ws.addJobFromGamsLib("trnsport");
    GAMSJob job3 = job1;
    // when, then
    QVERIFY( !( job1 == job2) );
    QVERIFY( job3 == job1 );
    // when
    job1.run();
    // then
    QVERIFY( job3 == job1 );
}

void TestGAMSJob::testIsValid() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job1 = ws.addJobFromGamsLib("trnsport");
    QVERIFY( job1.isValid() );

    GAMSJob job2 = ws.addJobFromString("scalar x;");
    QVERIFY( job2.isValid() );
}

void TestGAMSJob::testRun() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib("trnsport");
    // when
    job.run();
    // then
    QVERIFY( job.outDB().isValid() );
    QCOMPARE( job.outDB().getNrSymbols(), 12 );
    GAMSVariable z = job.outDB().getVariable("z");
    QVERIFY( equals(z.firstRecord().level(), 153.675) );
}

void TestGAMSJob::testRunJobFromEmptyString() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSJob job = ws.addJobFromString("");
    // then
    QVERIFY_EXCEPTION_THROWN( job.run(), GAMSExceptionExecution );
}

void TestGAMSJob::testOutDB() {
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);

    GAMSJob job = ws.addJobFromString(TestGAMSObject::getDataText());
    job.run();

    // when
    GAMSDatabase db = job.outDB();
    // then
    QVERIFY( db.isValid() );
    QCOMPARE( db.getNrSymbols(), 6);
    QVERIFY( db.workspace() == ws );

    QCOMPARE( db.getSet("i").numberRecords(), 2 );
    QCOMPARE( db.getSet("j").numberRecords(), 3 );
    QCOMPARE( db.getParameter("a").numberRecords(), 2 );
    QCOMPARE( db.getParameter("b").numberRecords(), 3 );
    QCOMPARE( db.getParameter("d").numberRecords(), 6 );
    QCOMPARE( db.getParameter("f").numberRecords(), 1 );
}

void TestGAMSJob::testOutDB_BeforeRun() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromString(TestGAMSObject::getDataText());

    // when
    GAMSDatabase db = job.outDB();
    // then
    QVERIFY( ! db.isValid() );
    QVERIFY_EXCEPTION_THROWN( db.getNrSymbols(), GAMSException );
}

void TestGAMSJob::testGetName() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromString(TestGAMSObject::getDataText());
    // when
    QString name = QString::fromStdString(job.name());
    // then
    QVERIFY( ! name.isNull() );
    QVERIFY( ! name.isEmpty() );
    QVERIFY( name.startsWith(defaultScratchFilePrefix.c_str()) );
}

void TestGAMSJob::testGetWorkspace() {
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);

    GAMSJob job = ws.addJobFromGamsLib("zloof");
    QCOMPARE( job.workspace(), ws );
    QCOMPARE( job.workspace().logID(), ws.logID() );
}

void TestGAMSJob::testGetLogID() {
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);

    GAMSJob job1 = ws.addJobFromGamsLib("trnsport");
    GAMSJob job2 = ws.addJobFromGamsLib("whouse");
    GAMSJob job3 = ws.addJobFromGamsLib("trnsport");

    // we have on logID per workspace,
    // so the logID's of all jobs shall be the same
    QVERIFY( job1.logID() == job2.logID() );
    QVERIFY( job2.logID() == job3.logID() );
}

QTEST_MAIN(TestGAMSJob)
