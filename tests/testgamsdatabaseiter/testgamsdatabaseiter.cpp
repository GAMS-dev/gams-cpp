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

#include "gamsdatabase.h"
#include "gamsequation.h"
#include "gamsexception.h"
#include "gamsset.h"
#include "gamsdatabaseiter.h"
#include "testgamsdatabaseiter.h"

using namespace gams;

QString TestGAMSDatabaseIter::classname()  { return "TestGAMSDatabaseIter"; }

void TestGAMSDatabaseIter::testConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    { // when
        GAMSDatabaseIter it = db.begin();
        // then
        QVERIFY( (*it) == db.getSet("i") );
    }
}

void TestGAMSDatabaseIter::testInvalidIterator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    {
        // when
        GAMSDatabaseIter it = db.end();
        // then
        QVERIFY_EXCEPTION_THROWN( (*it), GAMSException);
        // when
        ++it;
        // then
        QVERIFY_EXCEPTION_THROWN( (*it), GAMSException);
        // when
        ++it;
        // then
        QVERIFY_EXCEPTION_THROWN( (*it), GAMSException);
    }

    {
        // given
        GAMSDatabase emptydb = ws.addDatabase();
        // when
        GAMSDatabaseIter it = emptydb.begin();
        // then
        QVERIFY_EXCEPTION_THROWN( (*it), GAMSException);
        // when
        ++it;
        // then
        QVERIFY_EXCEPTION_THROWN( (*it), GAMSException);
    }
}

void TestGAMSDatabaseIter::testEqualToOperator_EmptyDatabase() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase emptydb1 = ws.addDatabase();
    GAMSDatabase emptydb2 = ws.addDatabase();

    {
        // when
        GAMSDatabaseIter it1 = emptydb1.begin();
        GAMSDatabaseIter it2 = emptydb1.begin();
        QVERIFY( it1 == it2 );
    }

    {
        // when
        GAMSDatabaseIter it1 = emptydb1.end();
        GAMSDatabaseIter it2 = emptydb1.end();
        // then
        QVERIFY( it1 == it2 );
    }

    {
        // when
        GAMSDatabaseIter it1 = emptydb1.begin();
        GAMSDatabaseIter it2 = emptydb2.begin();
        // then, they are different because ws.addDatabase() creates a new object
        QVERIFY( it1 != it2 );
    }
}

void TestGAMSDatabaseIter::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    {
        // when
        GAMSDatabaseIter it1 = db.begin();
        GAMSDatabaseIter it2 = db.begin();
        // then
        QVERIFY( it1 == it2 );
        while( it1 != db.end() || it2 !=db.end() ) {
            ++it1;
            ++it2;
            QVERIFY( it1 == it2 );
        }
    }

    {
        // when
        GAMSDatabaseIter it1 = db.end();
        GAMSDatabaseIter it2 = db.end();
        // then
        QVERIFY( it1 == it2 );

        // when
        ++it1;
        // then
        QVERIFY( it1 != it2 );

        // when
        ++it2;
        // then
        QVERIFY( it1 == it2 );
    }
}

void TestGAMSDatabaseIter::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSDatabase emptydb = ws.addDatabase();

    {  // when
        GAMSDatabaseIter it1 = db.begin();
        GAMSDatabaseIter it2 = emptydb.begin();
        // then
        QVERIFY( it1 != it2 );
    }
    {  // when
        GAMSDatabaseIter it1 = db.end();
        GAMSDatabaseIter it2 = emptydb.end();
        // then
        QVERIFY( it1 != it2 );
    }
    {  // when
        GAMSDatabaseIter it1 = db.begin();
        GAMSDatabaseIter it2 = db.begin();
        while( it1 != db.end() ) {
            ++it1;
            // then
            QVERIFY( it1 != it2 );
        }
        // when
        ++it2;
        while( it2 != db.end() ) {
            // then
            QVERIFY( it1 != it2 );
            ++it2;
        }
    }
}

void TestGAMSDatabaseIter::testPointerAndIncrementOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    std::map<std::string, GAMSEnum::SymbolType> symbolCounter;

    GAMSDatabaseIter it = db.begin();
    while( it != db.end() ) {
        symbolCounter[(*it).name()] = (*it).type();
        ++it;
    }
    QCOMPARE( symbolCounter.size(), size_t(12) );
    QCOMPARE( symbolCounter["i"], GAMSEnum::SymbolType::SymTypeSet );
    QCOMPARE( symbolCounter["j"], GAMSEnum::SymbolType::SymTypeSet );
    QCOMPARE( symbolCounter["a"], GAMSEnum::SymbolType::SymTypePar );
    QCOMPARE( symbolCounter["b"], GAMSEnum::SymbolType::SymTypePar );
    QCOMPARE( symbolCounter["d"], GAMSEnum::SymbolType::SymTypePar );
    QCOMPARE( symbolCounter["f"], GAMSEnum::SymbolType::SymTypePar );
    QCOMPARE( symbolCounter["c"], GAMSEnum::SymbolType::SymTypePar );
    QCOMPARE( symbolCounter["x"], GAMSEnum::SymbolType::SymTypeVar );
    QCOMPARE( symbolCounter["z"], GAMSEnum::SymbolType::SymTypeVar );
    QCOMPARE( symbolCounter["cost"], GAMSEnum::SymbolType::SymTypeEqu );
    QCOMPARE( symbolCounter["supply"], GAMSEnum::SymbolType::SymTypeEqu );
    QCOMPARE( symbolCounter["demand"], GAMSEnum::SymbolType::SymTypeEqu );
}

QTEST_MAIN(TestGAMSDatabaseIter)
