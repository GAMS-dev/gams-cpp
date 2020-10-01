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
#include <sstream>
#include "testgamsobject.h"

using namespace gams;

class TestGAMSDatabaseDomainViolation: public TestGAMSObject
{
};

TEST_F(TestGAMSDatabaseDomainViolation, testDefaultConstructor) {
    // when,
    GAMSDatabaseDomainViolation domViolation;
    // then
    ASSERT_TRUE( ! domViolation.isValid() );
    EXPECT_THROW( domViolation.violSym(), GAMSException );
    EXPECT_THROW( domViolation.violRecs(), GAMSException );
}

TEST_F(TestGAMSDatabaseDomainViolation, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    EXPECT_EQ( db.getDatabaseDVs(0, 5).size(), size_t(2) );
    for(GAMSDatabaseDomainViolation domViol : db.getDatabaseDVs(0, 5)){
        GAMSDatabaseDomainViolation dv( domViol );
        EXPECT_EQ( dv.violSym(), domViol.violSym() );
        EXPECT_EQ( dv.violRecs().size(), domViol.violRecs().size() );
        ASSERT_TRUE( dv == domViol );
    }
}

TEST_F(TestGAMSDatabaseDomainViolation, testIsValid) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    EXPECT_EQ( db.getDatabaseDVs(0, 5).size(), size_t(2) );
    for(GAMSDatabaseDomainViolation domViol : db.getDatabaseDVs(0, 5)){
        ASSERT_TRUE( domViol.isValid() );
    }
}

TEST_F(TestGAMSDatabaseDomainViolation, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    for( GAMSDatabaseDomainViolation domViol1 : db.getDatabaseDVs(0, 5) ) {
        // when
        GAMSDatabaseDomainViolation domViol2 = domViol1;
        // then
        EXPECT_EQ( domViol2.violSym(), domViol1.violSym() );
        EXPECT_EQ( domViol2.violRecs().size(), domViol2.violRecs().size() );
        for(size_t i=size_t(0); i< domViol2.violRecs().size(); i++) {
            EXPECT_EQ( domViol2.violRecs()[i], domViol1.violRecs()[i] );
        }
    }
}

TEST_F(TestGAMSDatabaseDomainViolation, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    GAMSDatabaseDomainViolation domViol0 = db.getDatabaseDVs(0, 2)[0];
    GAMSDatabaseDomainViolation domViol1 = db.getDatabaseDVs(0, 2)[1];

    // GAMSDatabaseDomainViolation != GAMSDatabaseDomainViolation is always true
    for(size_t i=size_t(0); i< db.getDatabaseDVs(0, 2).size(); i++) {
        // when, then
        if (i==size_t(0)) {
            ASSERT_TRUE( db.getDatabaseDVs(0, 2)[i] != domViol0 );
        } else if (i==size_t(1)) {
            ASSERT_TRUE( db.getDatabaseDVs(0, 2)[i] != domViol1 );
        } else {
            FAIL() << "do not expect more than 2 database domain violation";
        }
    }
}

TEST_F(TestGAMSDatabaseDomainViolation, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    GAMSDatabaseDomainViolation domViol0 = db.getDatabaseDVs(0, 2)[0];
    GAMSDatabaseDomainViolation domViol1 = db.getDatabaseDVs(0, 2)[1];

    for(size_t i=size_t(0); i< db.getDatabaseDVs(0, 2).size(); i++) {
        // when, then
        if (i==size_t(0)) {
            ASSERT_TRUE( db.getDatabaseDVs(0, 2)[i] != domViol1 );
        } else if (i==size_t(1)) {
            ASSERT_TRUE( db.getDatabaseDVs(0, 2)[i] != domViol0 );
        } else {
            FAIL() << "do not expect more than 2 database domain violation";
        }
    }
}

TEST_F(TestGAMSDatabaseDomainViolation, testViolSym) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    std::map<std::string, size_t> symbolCounter;
    for(GAMSDatabaseDomainViolation domViol : db.getDatabaseDVs(0, 5)){
        symbolCounter[domViol.violSym().name()]++;
    }
    EXPECT_EQ( symbolCounter.size(), size_t(2) );
    EXPECT_EQ( symbolCounter["a"], size_t(1) );
    EXPECT_EQ( symbolCounter["b"], size_t(1) );
    EXPECT_EQ( symbolCounter["d"], size_t(0) );
}

TEST_F(TestGAMSDatabaseDomainViolation, testViolRecs) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    std::map<std::string, size_t> symbolCounter;
    std::map<std::string, size_t> recordCounter;
    for(GAMSDatabaseDomainViolation domViol : db.getDatabaseDVs(0, 5)){
        for(GAMSSymbolDomainViolation rec : domViol.violRecs()) {
            symbolCounter[domViol.violSym().name()]++;
            std::stringstream ss;
            ss << domViol.violSym().name() << "_" << rec.violRec().key(0) ;
            recordCounter[ss.str()]++;
        }
    }
    EXPECT_EQ( symbolCounter.size(), size_t(2) );
    EXPECT_EQ( symbolCounter["a"], size_t(2) );
    EXPECT_EQ( symbolCounter["b"], size_t(1) );
    EXPECT_EQ( symbolCounter["d"], size_t(0) );
    EXPECT_EQ( recordCounter.size(), size_t(3) );
    EXPECT_EQ( recordCounter["a_Alburquerque"], size_t(1) );
    EXPECT_EQ( recordCounter["a_Sanfrancisco"], size_t(1) );
    EXPECT_EQ( recordCounter["b_Braunschweig"], size_t(1) );
}
