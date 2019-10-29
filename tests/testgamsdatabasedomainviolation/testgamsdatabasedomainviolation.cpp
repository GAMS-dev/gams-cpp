/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2019 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2019 GAMS Development Corp. <support@gams.com>
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

#include "testgamsdatabasedomainviolation.h"

#include <sstream>
#include <QtTest>

using namespace gams;

QString TestGAMSDatabaseDomainViolation::classname()  { return "TestGAMSDatabaseDomainViolation"; }

void TestGAMSDatabaseDomainViolation::testDefaultConstructor() {
    // when,
    GAMSDatabaseDomainViolation domViolation;
    // then
    QVERIFY( ! domViolation.isValid() );
    QVERIFY_EXCEPTION_THROWN( domViolation.violSym(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( domViolation.violRecs(), GAMSException );
}

void TestGAMSDatabaseDomainViolation::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    QCOMPARE( db.getDatabaseDVs(0, 5).size(), size_t(2) );
    for(GAMSDatabaseDomainViolation domViol : db.getDatabaseDVs(0, 5)){
        GAMSDatabaseDomainViolation dv( domViol );
        QCOMPARE( dv.violSym(), domViol.violSym() );
        QCOMPARE( dv.violRecs().size(), domViol.violRecs().size() );
        QVERIFY( dv == domViol );

    }
}

void TestGAMSDatabaseDomainViolation::testIsValid() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    QCOMPARE( db.getDatabaseDVs(0, 5).size(), size_t(2) );
    for(GAMSDatabaseDomainViolation domViol : db.getDatabaseDVs(0, 5)){
        QVERIFY( domViol.isValid() );
    }
}

void TestGAMSDatabaseDomainViolation::testAssignmentOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    for( GAMSDatabaseDomainViolation domViol1 : db.getDatabaseDVs(0, 5) ) {
        // when
        GAMSDatabaseDomainViolation domViol2 = domViol1;
        // then
        QCOMPARE( domViol2.violSym(), domViol1.violSym() );
        QCOMPARE( domViol2.violRecs().size(), domViol2.violRecs().size() );
        for(size_t i=size_t(0); i< domViol2.violRecs().size(); i++) {
            QCOMPARE( domViol2.violRecs()[i], domViol1.violRecs()[i] );
        }
    }
}

void TestGAMSDatabaseDomainViolation::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    GAMSDatabaseDomainViolation domViol0 = db.getDatabaseDVs(0, 2)[0];
    GAMSDatabaseDomainViolation domViol1 = db.getDatabaseDVs(0, 2)[1];

    // GAMSDatabaseDomainViolation != GAMSDatabaseDomainViolation is always true
    for(size_t i=size_t(0); i< db.getDatabaseDVs(0, 2).size(); i++) {
        // when, then
        if (i==size_t(0)) {
            QVERIFY( db.getDatabaseDVs(0, 2)[i] != domViol0 );
        } else if (i==size_t(1)) {
            QVERIFY( db.getDatabaseDVs(0, 2)[i] != domViol1 );
        } else {
            QFAIL("do not expect more than 2 database domain violation");
        }
    }
}

void TestGAMSDatabaseDomainViolation::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    GAMSDatabaseDomainViolation domViol0 = db.getDatabaseDVs(0, 2)[0];
    GAMSDatabaseDomainViolation domViol1 = db.getDatabaseDVs(0, 2)[1];

    for(size_t i=size_t(0); i< db.getDatabaseDVs(0, 2).size(); i++) {
        // when, then
        if (i==size_t(0)) {
            QVERIFY( db.getDatabaseDVs(0, 2)[i] != domViol1 );
        } else if (i==size_t(1)) {
            QVERIFY( db.getDatabaseDVs(0, 2)[i] != domViol0 );
        } else {
            QFAIL("do not expect more than 2 database domain violation");
        }
    }
}

void TestGAMSDatabaseDomainViolation::testViolSym() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    std::map<std::string, size_t> symbolCounter;
    for(GAMSDatabaseDomainViolation domViol : db.getDatabaseDVs(0, 5)){
        symbolCounter[domViol.violSym().name()]++;
    }
    QCOMPARE( symbolCounter.size(), size_t(2) );
    QCOMPARE( symbolCounter["a"], size_t(1) );
    QCOMPARE( symbolCounter["b"], size_t(1) );
    QCOMPARE( symbolCounter["d"], size_t(0) );
}

void TestGAMSDatabaseDomainViolation::testViolRecs() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
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
    QCOMPARE( symbolCounter.size(), size_t(2) );
    QCOMPARE( symbolCounter["a"], size_t(2) );
    QCOMPARE( symbolCounter["b"], size_t(1) );
    QCOMPARE( symbolCounter["d"], size_t(0) );
    QCOMPARE( recordCounter.size(), size_t(3) );
    QCOMPARE( recordCounter["a_Alburquerque"], size_t(1) );
    QCOMPARE( recordCounter["a_Sanfrancisco"], size_t(1) );
    QCOMPARE( recordCounter["b_Braunschweig"], size_t(1) );
}

QTEST_MAIN(TestGAMSDatabaseDomainViolation)
