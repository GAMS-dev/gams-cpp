/*
 *
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

#include "testperformance.h"
#include "gamsworkspaceinfo.h"
#include "gamsworkspace.h"
#include "gamsdatabase.h"
#include "gamssymbol.h"
#include "gamsoptions.h"
#include "gamscheckpoint.h"
#include "gamsmodelinstance.h"
#include "gamsparameter.h"
#include "gamsmodifier.h"
#include "gamsjob.h"

#include <vector>
#include <QDebug>

using namespace std;
using namespace gams;

QString TestPerformance::classname() { return "TestPerformance"; }

// preparing huge GAMSSet with >1e6 entries
void TestPerformance::initTestCase(){

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    // filling hugeTestSet
    GAMSDatabase db = ws.addDatabase();
    hugeTestSet = db.addSet("set", 1, "test set");

    QElapsedTimer timer;
    timer.start();
    for(int i = 0; i < NR_ITEMS_HUGE_TEST; i++){
        hugeTestSet.addRecord(string("rec" + to_string(i)));
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";
}


// test adding many many (>1e6) records to a symbol
void TestPerformance::testAddingRecords(){

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    GAMSSet set = db.addSet("set", 1, "test set");

    QElapsedTimer timer;
    timer.start();
    for(int i = 0; i < NR_ITEMS_HUGE_TEST; i++){
        set.addRecord(string("dim" + to_string(i)));
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";

    QVERIFY(1);
}

// test adding many many symbols to a database
void TestPerformance::testAddingSymbols(){

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    QElapsedTimer timer;
    timer.start();
    for(int i = 0; i < NR_ITEMS_HUGE_TEST; i++){
        GAMSSet set = db.addSet(string("set" + to_string(i)), 1, "test set");
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";

    QVERIFY(1);
}

// test adding many databases to a workspace
void TestPerformance::testAddingDatabases(){

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    QElapsedTimer timer;
    timer.start();
    for(int i = 0; i < NR_ITEMS_BIG_TEST; i++){
        GAMSDatabase db = ws.addDatabase();
        db.addSet("someset", 1).addRecord("rec1");
        QVERIFY(db.getSet("someset").dim());
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";

    QVERIFY(1);
}

// test adding many jobs to a workspace
void TestPerformance::testAddingJobs(){

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    QElapsedTimer timer;
    timer.start();
    for(int i = 0; i < NR_ITEMS_BIG_TEST; i++){
        GAMSJob j = ws.addJobFromGamsLib("diet");
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";

    QVERIFY(1);
}

// test adding many options to a workspace
void TestPerformance::testAddingOptions(){

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    QElapsedTimer timer;
    timer.start();
    for(int i = 0; i < NR_ITEMS_BIG_TEST; i++){
        GAMSOptions o = ws.addOptions();
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";

    QVERIFY(1);
}

// test iterating over all records for a very large symbol
void TestPerformance::testIteratingRecordsPointer(){

    GAMSSymbolRecord record = hugeTestSet.firstRecord();

    QElapsedTimer timer;
    timer.start();
    int i=0;
    while(record.moveNext()){
        i++;
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";

    QVERIFY(1);
}

// test iterating over all records for a very large symbol
void TestPerformance::testIteratingRecordsObject(){

    QElapsedTimer timer;
    timer.start();
    int i = 0;
    for(GAMSSetRecord rec : hugeTestSet){
        i++;
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";

    QVERIFY(1);
}

// running many small (>1e3) GAMSJobs (or running the same job many times)
void TestPerformance::testRunningJobs(){

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib("trnsport");

    QElapsedTimer timer;
    timer.start();
    for (int i = 0; i < NR_ITEMS_BIG_TEST; i++) {
        job.run();
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";

    QVERIFY(1);
}

// test duplicating many GAMSModelInstances and solving them
void TestPerformance::testDuplicatingModelInstances(){

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    GAMSJob job = ws.addJobFromGamsLib("trnsport");
    job.run(cp);

    GAMSModelInstance mi = cp.addModelInstance("");
    mi.instantiate("transport use lp min z");

    mi.solve();

    QElapsedTimer timer;
    timer.start();
    for (int i = 0; i < NR_ITEMS_BIG_TEST; i++) {
        GAMSModelInstance copy = mi.copyModelInstance();
        copy.solve();
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";

    QVERIFY(1);
}

//QTEST_MAIN(TestPerformance)
