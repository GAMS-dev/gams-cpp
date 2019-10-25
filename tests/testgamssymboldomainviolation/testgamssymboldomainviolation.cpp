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

#include "gamsparameter.h"
#include "testgamssymboldomainviolation.h"

using namespace gams;

QString TestGAMSSymbolDomainViolation::classname()  { return "TestGAMSDatabaseDomainViolation"; }

void TestGAMSSymbolDomainViolation::testDefaultConstructor() {
    // when,
    GAMSSymbolDomainViolation dv;
    // then
    QVERIFY( ! dv.isValid() );
    QVERIFY_EXCEPTION_THROWN( dv.violInd(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( dv.violRec(), GAMSException );

    GAMSSymbolDomainViolation dv1;
    QVERIFY( dv == dv1 );
}

void TestGAMSSymbolDomainViolation::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    GAMSParameter a = db.getParameter("a");

    // when, then
    QCOMPARE( a.getSymbolDVs(5).size(), size_t(2) );
    for(GAMSSymbolDomainViolation domViol : a.getSymbolDVs( 5)){
        GAMSSymbolDomainViolation dv( domViol );
        QCOMPARE( dv.violInd().size(), domViol.violInd().size() );
        QCOMPARE( dv.violRec(), domViol.violRec() );
        QVERIFY( dv == domViol );
    }
}

void TestGAMSSymbolDomainViolation::testIsValid() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    TestGAMSObject::getTestData_Parameter_freightcost_f( db );
    GAMSParameter f = db.getParameter("f");

    // when, then
    QCOMPARE( f.getSymbolDVs(5).size(), size_t(0) );

}

void TestGAMSSymbolDomainViolation::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    GAMSParameter a = db.getParameter("a");
    QCOMPARE( a.getSymbolDVs(2).size(), size_t(2) );

    GAMSSymbolDomainViolation dv0 = a.getSymbolDVs(2)[0];
    GAMSSymbolDomainViolation dv1 = a.getSymbolDVs(2)[1];

    // GAMSSymbolDomainViolation != GAMSSymbolDomainViolation is always true
    for(size_t i=size_t(0); i<a.getSymbolDVs(2).size(); i++) {
         // when, then
         if (i==size_t(0)) {
             QVERIFY( a.getSymbolDVs(2)[i] != dv0 );
         } else if (i==size_t(1)) {
                    QVERIFY(a.getSymbolDVs(2)[i] != dv1 );
         } else {
             QFAIL("do not expect more than 2 database domain violation");
         }
    }
}

void TestGAMSSymbolDomainViolation::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    GAMSParameter a = db.getParameter("a");
    QCOMPARE( a.getSymbolDVs(2).size(), size_t(2) );

    GAMSSymbolDomainViolation dv0 = a.getSymbolDVs(2)[0];
    GAMSSymbolDomainViolation dv1 = a.getSymbolDVs(2)[1];

    for(size_t i=size_t(0); i<a.getSymbolDVs(2).size(); i++) {
         // when, then
         if (i==size_t(0)) {
             QVERIFY( a.getSymbolDVs(2)[i] != dv1 );
         } else if (i==size_t(1)) {
                    QVERIFY(a.getSymbolDVs(2)[i] != dv0 );
         } else {
             QFAIL("do not expect more than 2 database domain violation");
         }
    }
}

void TestGAMSSymbolDomainViolation::testViolRecs() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    { GAMSParameter a = db.getParameter("a");
      QCOMPARE( a.getSymbolDVs(2).size(), size_t(2) );

      std::map<std::string, size_t> recordCounter;
      for (GAMSSymbolDomainViolation dv : a.getSymbolDVs()) {
          recordCounter[dv.violRec().key(0)]++;
      }
      QCOMPARE( recordCounter.size(), size_t(2) );
      QCOMPARE( recordCounter["Alburquerque"], size_t(1) );
      QCOMPARE( recordCounter["Sanfrancisco"], size_t(1) );
    }

    // when, then
    { GAMSParameter b = db.getParameter("b");
      QCOMPARE( b.getSymbolDVs(2).size(), size_t(1) );

      std::map<std::string, size_t> recordCounter;
      for (GAMSSymbolDomainViolation dv : b.getSymbolDVs()) {
          recordCounter[dv.violRec().key(0)]++;
      }
      QCOMPARE( recordCounter.size(), size_t(1) );
      QCOMPARE( recordCounter["Braunschweig"], size_t(1) );
    }
}

void TestGAMSSymbolDomainViolation::testViolInd() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    { GAMSParameter a = db.getParameter("a");
      QCOMPARE( a.getSymbolDVs(2).size(), size_t(2) );
      for (GAMSSymbolDomainViolation dv : a.getSymbolDVs()) {
          // when, then
          std::vector<bool> vi = dv.violInd();
          QCOMPARE( vi.size(), size_t(1) );
          QVERIFY( vi[0] );
      }
    }

    // given
    {  GAMSParameter newd = db.addParameter("newd", "distance in thousands of miles", GAMSDomain(db.getSet("i")), GAMSDomain(db.getSet("j")));
       newd.addRecord("Seattle", "Chicago");
       newd.addRecord("Seattle", "Alburquerque");
       QCOMPARE( newd.getSymbolDVs(2).size(), size_t(1) );
       for (GAMSSymbolDomainViolation dv : newd.getSymbolDVs()) {
           // when ,then
           std::vector<bool> vi = dv.violInd();
           QCOMPARE( vi.size(), size_t(2) );
           QVERIFY( ! vi[0] );
           QVERIFY( vi[1] );
       }
    }
}

QTEST_MAIN(TestGAMSSymbolDomainViolation)
