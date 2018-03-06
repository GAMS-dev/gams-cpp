/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
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

    try {
        int amount = NR_ITEMS_HUGE_TEST;
        GAMSWorkspace ws("", qgetenv("GTESTDIR").toStdString());

        // filling hugeTestSet
        GAMSDatabase db = ws.addDatabase();
        hugeTestSet = db.addSet("set", 1, "test set");

        QElapsedTimer timer;
        timer.start();
        for(int i = 0; i < amount; i++){
            hugeTestSet.addRecord(string("rec" + to_string(i)));
        }
        qInfo() << "[" << amount << " iterations ]  Elapsed time:" << timer.elapsed() << "ms";

    } catch (GAMSException &e) {
        qInfo() << "Crashed: " << e.what();
    }
}


// test adding many many (>1e6) records to a symbol
void TestPerformance::testAddingRecords(){

    try {
        int amount = NR_ITEMS_HUGE_TEST;
        GAMSWorkspace ws("", qgetenv("GTESTDIR").toStdString());
        GAMSDatabase db = ws.addDatabase();

        GAMSSet set = db.addSet("set", 1, "test set");

        QElapsedTimer timer;
        timer.start();
        for(int i = 0; i < amount; i++){
            set.addRecord(string("dim" + to_string(i)));
        }
        qInfo() << "[" << amount << " iterations ]  Elapsed time:" << timer.elapsed() << "ms";

        QVERIFY(1);
    } catch (GAMSException &e) {
        qInfo() << "Crashed: " << e.what();
    }
}

// test adding many many symbols to a database
void TestPerformance::testAddingSymbols(){

    try {
        int amount = NR_ITEMS_HUGE_TEST;
        GAMSWorkspace ws("", qgetenv("GTESTDIR").toStdString());
        GAMSDatabase db = ws.addDatabase();

        QElapsedTimer timer;
        timer.start();
        for(int i = 0; i < amount; i++){
            GAMSSet set = db.addSet(string("set" + to_string(i)), 1, "test set");
        }
        qInfo() << "[" << amount << " iterations ]  Elapsed time:" << timer.elapsed() << "ms";

        QVERIFY(1);
    } catch (GAMSException &e) {
        qInfo() << "Crashed: " << e.what();
    }
}

// test adding many databases to a workspace
void TestPerformance::testAddingDatabases(){

    try {
        int amount = NR_ITEMS_BIG_TEST;
        GAMSWorkspace ws("", qgetenv("GTESTDIR").toStdString());

        QElapsedTimer timer;
        timer.start();
        for(int i = 0; i < amount; i++){
            GAMSDatabase db = ws.addDatabase();
            db.addSet("someset", 1).addRecord("rec1");
            QVERIFY(db.getSet("someset").dim());
        }
        qInfo() << "[" << amount << " iterations ]  Elapsed time:" << timer.elapsed() << "ms";

        QVERIFY(1);
    } catch (GAMSException &e) {
        qInfo() << "Crashed: " << e.what();
    }
}

// test adding many jobs to a workspace
void TestPerformance::testAddingJobs(){

    try {
        int amount = NR_ITEMS_BIG_TEST;
        GAMSWorkspace ws("", qgetenv("GTESTDIR").toStdString());

        QElapsedTimer timer;
        timer.start();
        for(int i = 0; i < amount; i++){
            GAMSJob j = ws.addJobFromGamsLib("diet");
        }
        qInfo() << "[" << amount << " iterations ]  Elapsed time:" << timer.elapsed() << "ms";

        QVERIFY(1);
    } catch (GAMSException &e) {
        qInfo() << "Crashed: " << e.what();
    }
}

// test adding many options to a workspace
void TestPerformance::testAddingOptions(){

    try {
        int amount = NR_ITEMS_BIG_TEST;
        GAMSWorkspace ws("", qgetenv("GTESTDIR").toStdString());

        QElapsedTimer timer;
        timer.start();
        for(int i = 0; i < amount; i++){
            GAMSOptions o = ws.addOptions();
        }
        qInfo() << "[" << amount << " iterations ]  Elapsed time:" << timer.elapsed() << "ms";

        QVERIFY(1);
    } catch (GAMSException &e) {
        qInfo() << "Crashed: " << e.what();
    }
}

// test iterating over all records for a very large symbol
void TestPerformance::testIteratingRecordsPointer(){

    try {
        int amount = NR_ITEMS_HUGE_TEST;
        GAMSSymbolRecord record = hugeTestSet.firstRecord();

        QElapsedTimer timer;
        timer.start();
        int i=0;
        while(record.moveNext()){
            i++;
        }
        qInfo() << "[" << amount << " iterations ]  Elapsed time:" << timer.elapsed() << "ms";

        QVERIFY(1);
    } catch (GAMSException &e) {
        qInfo() << "Crashed: " << e.what();
    }
}

// test iterating over all records for a very large symbol
void TestPerformance::testIteratingRecordsObject(){

    try {
        int amount = NR_ITEMS_HUGE_TEST;
        QElapsedTimer timer;
        timer.start();
        int i = 0;
        for(GAMSSetRecord rec : hugeTestSet){
            i++;
        }
        qInfo() << "[" << amount << " iterations ]  Elapsed time:" << timer.elapsed() << "ms";

        QVERIFY(1);
    } catch (GAMSException &e) {
        qInfo() << "Crashed: " << e.what();
    }
}

// running many small (>1e3) GAMSJobs (or running the same job many times)
void TestPerformance::testRunningJobs(){

    try {
        int amount = NR_ITEMS_BIG_TEST;
        GAMSWorkspace ws("", qgetenv("GTESTDIR").toStdString());
        GAMSJob job = ws.addJobFromGamsLib("trnsport");

        QElapsedTimer timer;
        timer.start();
        for (int i = 0; i < amount; i++) {
            job.run();
        }
        qInfo() << "[" << amount << " iterations ]  Elapsed time:" << timer.elapsed() << "ms";

        QVERIFY(1);
    } catch (GAMSException &e) {
        qInfo() << "Crashed: " << e.what();
    }
}

// test duplicating many GAMSModelInstances and solving them
void TestPerformance::testDuplicatingModelInstances(){

    try {
        int amount = NR_ITEMS_BIG_TEST;
        GAMSWorkspace ws("", qgetenv("GTESTDIR").toStdString());
        GAMSCheckpoint cp = ws.addCheckpoint();
        GAMSJob job = ws.addJobFromGamsLib("trnsport");
        job.run(cp);

        GAMSModelInstance mi = cp.addModelInstance("");
        mi.instantiate("transport use lp min z");

        mi.solve();

        QElapsedTimer timer;
        timer.start();
        for (int i = 0; i < amount; i++) {
            GAMSModelInstance copy = mi.copyModelInstance();
            copy.solve();
        }
        qInfo() << "[" << amount << " iterations ]  Elapsed time:" << timer.elapsed() << "ms";

        QVERIFY(1);
    } catch (GAMSException &e) {
        qInfo() << "Crashed: " << e.what();
    }
}

//QTEST_MAIN(TestPerformance)
