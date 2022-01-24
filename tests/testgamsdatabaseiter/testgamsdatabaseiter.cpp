/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>
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
#include "testgamsobject.h"

using namespace gams;

class TestGAMSDatabaseIter: public TestGAMSObject
{
};

TEST_F(TestGAMSDatabaseIter, testConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    { // when
        GAMSDatabaseIter it = db.begin();
        // then
        ASSERT_TRUE( (*it) == db.getSet("i") );
    }
}

TEST_F(TestGAMSDatabaseIter, testInvalidIterator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    {
        // when
        GAMSDatabaseIter it = db.end();
        // then
        EXPECT_THROW( (*it), GAMSException);
        // when
        ++it;
        // then
        EXPECT_THROW( (*it), GAMSException);
        // when
        ++it;
        // then
        EXPECT_THROW( (*it), GAMSException);
    }

    {
        // given
        GAMSDatabase emptydb = ws.addDatabase();
        // when
        GAMSDatabaseIter it = emptydb.begin();
        // then
        EXPECT_THROW( (*it), GAMSException);
        // when
        ++it;
        // then
        EXPECT_THROW( (*it), GAMSException);
    }
}

TEST_F(TestGAMSDatabaseIter, testEqualToOperator_EmptyDatabase) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase emptydb1 = ws.addDatabase();
    GAMSDatabase emptydb2 = ws.addDatabase();

    {
        // when
        GAMSDatabaseIter it1 = emptydb1.begin();
        GAMSDatabaseIter it2 = emptydb1.begin();
        ASSERT_TRUE( it1 == it2 );
    }

    {
        // when
        GAMSDatabaseIter it1 = emptydb1.end();
        GAMSDatabaseIter it2 = emptydb1.end();
        // then
        ASSERT_TRUE( it1 == it2 );
    }

    {
        // when
        GAMSDatabaseIter it1 = emptydb1.begin();
        GAMSDatabaseIter it2 = emptydb2.begin();
        // then, they are different because ws.addDatabase() creates a new object
        ASSERT_TRUE( it1 != it2 );
    }
}

TEST_F(TestGAMSDatabaseIter, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    {
        // when
        GAMSDatabaseIter it1 = db.begin();
        GAMSDatabaseIter it2 = db.begin();
        // then
        ASSERT_TRUE( it1 == it2 );
        while( it1 != db.end() || it2 !=db.end() ) {
            ++it1;
            ++it2;
            ASSERT_TRUE( it1 == it2 );
        }
    }

    {
        // when
        GAMSDatabaseIter it1 = db.end();
        GAMSDatabaseIter it2 = db.end();
        // then
        ASSERT_TRUE( it1 == it2 );

        // when
        ++it1;
        // then
        ASSERT_TRUE( it1 != it2 );

        // when
        ++it2;
        // then
        ASSERT_TRUE( it1 == it2 );
    }
}

TEST_F(TestGAMSDatabaseIter, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    GAMSDatabase emptydb = ws.addDatabase();

    {  // when
        GAMSDatabaseIter it1 = db.begin();
        GAMSDatabaseIter it2 = emptydb.begin();
        // then
        ASSERT_TRUE( it1 != it2 );
    }
    {  // when
        GAMSDatabaseIter it1 = db.end();
        GAMSDatabaseIter it2 = emptydb.end();
        // then
        ASSERT_TRUE( it1 != it2 );
    }
    {  // when
        GAMSDatabaseIter it1 = db.begin();
        GAMSDatabaseIter it2 = db.begin();
        while( it1 != db.end() ) {
            ++it1;
            // then
            ASSERT_TRUE( it1 != it2 );
        }
        // when
        ++it2;
        while( it2 != db.end() ) {
            // then
            ASSERT_TRUE( it1 != it2 );
            ++it2;
        }
    }
}

TEST_F(TestGAMSDatabaseIter, testPointerAndIncrementOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
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
    EXPECT_EQ( symbolCounter.size(), size_t(12) );
    EXPECT_EQ( symbolCounter["i"], GAMSEnum::SymbolType::SymTypeSet );
    EXPECT_EQ( symbolCounter["j"], GAMSEnum::SymbolType::SymTypeSet );
    EXPECT_EQ( symbolCounter["a"], GAMSEnum::SymbolType::SymTypePar );
    EXPECT_EQ( symbolCounter["b"], GAMSEnum::SymbolType::SymTypePar );
    EXPECT_EQ( symbolCounter["d"], GAMSEnum::SymbolType::SymTypePar );
    EXPECT_EQ( symbolCounter["f"], GAMSEnum::SymbolType::SymTypePar );
    EXPECT_EQ( symbolCounter["c"], GAMSEnum::SymbolType::SymTypePar );
    EXPECT_EQ( symbolCounter["x"], GAMSEnum::SymbolType::SymTypeVar );
    EXPECT_EQ( symbolCounter["z"], GAMSEnum::SymbolType::SymTypeVar );
    EXPECT_EQ( symbolCounter["cost"], GAMSEnum::SymbolType::SymTypeEqu );
    EXPECT_EQ( symbolCounter["supply"], GAMSEnum::SymbolType::SymTypeEqu );
    EXPECT_EQ( symbolCounter["demand"], GAMSEnum::SymbolType::SymTypeEqu );
}
