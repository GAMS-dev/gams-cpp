/**
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

#include "testgmd.h"
#include <QDebug>

using namespace std;
using namespace gams;

QString TestGMD::classname() { return "TestGMD"; }

// IMPORTANT (TODO)
// These tests need the libraries libgmdcclib64.so and libgdxdclib64.so to be in $BUILD/bin.
// The files can be found in the gams root directory and - for now - needs to be copied manually.

void TestGMD::initTestCase() {

    GDXSTRINDEXPTRS_INIT( strIndex, strIndexPtrs );
    if (!gmdCreate(&gmd, msg, sizeof(msg))) {
        QFAIL(msg);
    }

    void* symIterPtr;
    QVERIFY(gmdAddSymbol(gmd, "myset", 1, GMS_DT_SET, 0, "test set", &hugeTestSet));

    QElapsedTimer timer;
    timer.start();
    for(int i = 0; i < NR_ITEMS_HUGE_TEST; i++) {
        sprintf(strIndexPtrs[0],"rec%d",i);
        QVERIFY(gmdAddRecord(gmd, hugeTestSet, const_cast<const char**>(strIndexPtrs), &symIterPtr));
        QVERIFY(gmdFreeSymbolIterator(gmd, symIterPtr));
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";
}

void TestGMD::testAddingSymbols() {

    char setname[GMS_SSSIZE];
    void* symPtr = 0;
    if (!gmdCreate(&gmd, msg, sizeof(msg))) {
        QFAIL(msg);
    }

    QElapsedTimer timer;
    timer.start();
    for(int i = 0; i < NR_ITEMS_HUGE_TEST; i++) {
        sprintf(setname,"set%d",i);
        QVERIFY(gmdAddSymbol(gmd, setname, 1, GMS_DT_SET, 0, "test set", &symPtr));
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";
    QVERIFY(gmdFree(&gmd));
}

void TestGMD::testAddingDatabases() {
    void* symPtr = 0;
    void* symItrPtr = 0;

    QElapsedTimer timer;
    timer.start();
    for(int i = 0; i < NR_ITEMS_BIG_TEST; i++){
        if (!gmdCreate(&gmd, msg, sizeof(msg))) {
            QFAIL(msg);
        }
        QVERIFY(gmdAddSymbol(gmd, "someset", 1, GMS_DT_SET, 0, "test set", &symPtr));

        sprintf(strIndexPtrs[0],"rec1");
        QVERIFY(gmdAddRecord(gmd, symPtr, const_cast<const char**>(strIndexPtrs), &symItrPtr));
        QVERIFY(gmdFree(&gmd));
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";
}

void TestGMD::testIteratingRecordsPointer() {

    if (!gmdCreate(&gmd, msg, sizeof(msg))) {
        QFAIL(msg);
    }

    void* symIterPtr;

    QElapsedTimer timer;
    timer.start();
    QVERIFY(gmdFindFirstRecord(gmd, hugeTestSet, &symIterPtr));
    while(gmdRecordMoveNext(gmd,symIterPtr)) {
        ;
    }
    qInfo() << "Elapsed time:" << timer.elapsed() << "ms";

    QVERIFY(gmdFree(&gmd));
}

QTEST_MAIN(TestGMD)
