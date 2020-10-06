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

#include "gamsparameter.h"
#include "testgamssymboldomainviolation.h"

using namespace gams;

class TestGAMSDatabaseDomainViolation: public TestGAMSObject
{
};

void TestGAMSSymbolDomainViolation::testDefaultConstructor() {
    // when,
    GAMSSymbolDomainViolation dv;
    // then
    ASSERT_TRUE( ! dv.isValid() );
    EXPECT_THROW( dv.violInd(), GAMSException );
    EXPECT_THROW( dv.violRec(), GAMSException );

    GAMSSymbolDomainViolation dv1;
    ASSERT_TRUE( dv == dv1 );
}

void TestGAMSSymbolDomainViolation::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    GAMSParameter a = db.getParameter("a");

    // when, then
    EXPECT_EQ( a.getSymbolDVs(5).size(), size_t(2) );
    for(GAMSSymbolDomainViolation domViol : a.getSymbolDVs( 5)){
        GAMSSymbolDomainViolation dv( domViol );
        EXPECT_EQ( dv.violInd().size(), domViol.violInd().size() );
        EXPECT_EQ( dv.violRec(), domViol.violRec() );
        ASSERT_TRUE( dv == domViol );
    }
}

void TestGAMSSymbolDomainViolation::testIsValid() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    TestGAMSObject::getTestData_Parameter_freightcost_f( db );
    GAMSParameter f = db.getParameter("f");

    // when, then
    EXPECT_EQ( f.getSymbolDVs(5).size(), size_t(0) );

}

void TestGAMSSymbolDomainViolation::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    GAMSParameter a = db.getParameter("a");
    EXPECT_EQ( a.getSymbolDVs(2).size(), size_t(2) );

    GAMSSymbolDomainViolation dv0 = a.getSymbolDVs(2)[0];
    GAMSSymbolDomainViolation dv1 = a.getSymbolDVs(2)[1];

    // GAMSSymbolDomainViolation != GAMSSymbolDomainViolation is always true
    for(size_t i=size_t(0); i<a.getSymbolDVs(2).size(); i++) {
         // when, then
         if (i==size_t(0)) {
             ASSERT_TRUE( a.getSymbolDVs(2)[i] != dv0 );
         } else if (i==size_t(1)) {
                    ASSERT_TRUE(a.getSymbolDVs(2)[i] != dv1 );
         } else {
             QFAIL("do not expect more than 2 database domain violation");
         }
    }
}

void TestGAMSSymbolDomainViolation::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    GAMSParameter a = db.getParameter("a");
    EXPECT_EQ( a.getSymbolDVs(2).size(), size_t(2) );

    GAMSSymbolDomainViolation dv0 = a.getSymbolDVs(2)[0];
    GAMSSymbolDomainViolation dv1 = a.getSymbolDVs(2)[1];

    for(size_t i=size_t(0); i<a.getSymbolDVs(2).size(); i++) {
         // when, then
         if (i==size_t(0)) {
             ASSERT_TRUE( a.getSymbolDVs(2)[i] != dv1 );
         } else if (i==size_t(1)) {
                    ASSERT_TRUE(a.getSymbolDVs(2)[i] != dv0 );
         } else {
             QFAIL("do not expect more than 2 database domain violation");
         }
    }
}

void TestGAMSSymbolDomainViolation::testViolRecs() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    { GAMSParameter a = db.getParameter("a");
      EXPECT_EQ( a.getSymbolDVs(2).size(), size_t(2) );

      std::map<std::string, size_t> recordCounter;
      for (GAMSSymbolDomainViolation dv : a.getSymbolDVs()) {
          recordCounter[dv.violRec().key(0)]++;
      }
      EXPECT_EQ( recordCounter.size(), size_t(2) );
      EXPECT_EQ( recordCounter["Alburquerque"], size_t(1) );
      EXPECT_EQ( recordCounter["Sanfrancisco"], size_t(1) );
    }

    // when, then
    { GAMSParameter b = db.getParameter("b");
      EXPECT_EQ( b.getSymbolDVs(2).size(), size_t(1) );

      std::map<std::string, size_t> recordCounter;
      for (GAMSSymbolDomainViolation dv : b.getSymbolDVs()) {
          recordCounter[dv.violRec().key(0)]++;
      }
      EXPECT_EQ( recordCounter.size(), size_t(1) );
      EXPECT_EQ( recordCounter["Braunschweig"], size_t(1) );
    }
}

void TestGAMSSymbolDomainViolation::testViolInd() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    { GAMSParameter a = db.getParameter("a");
      EXPECT_EQ( a.getSymbolDVs(2).size(), size_t(2) );
      for (GAMSSymbolDomainViolation dv : a.getSymbolDVs()) {
          // when, then
          std::vector<bool> vi = dv.violInd();
          EXPECT_EQ( vi.size(), size_t(1) );
          ASSERT_TRUE( vi[0] );
      }
    }

    // given
    {  GAMSParameter newd = db.addParameter("newd", "distance in thousands of miles", GAMSDomain(db.getSet("i")), GAMSDomain(db.getSet("j")));
       newd.addRecord("Seattle", "Chicago");
       newd.addRecord("Seattle", "Alburquerque");
       EXPECT_EQ( newd.getSymbolDVs(2).size(), size_t(1) );
       for (GAMSSymbolDomainViolation dv : newd.getSymbolDVs()) {
           // when ,then
           std::vector<bool> vi = dv.violInd();
           EXPECT_EQ( vi.size(), size_t(2) );
           ASSERT_TRUE( ! vi[0] );
           ASSERT_TRUE( vi[1] );
       }
    }
}


