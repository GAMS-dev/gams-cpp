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

#include "gamsdomain.h"
#include "testgamsdomain.h"

using namespace gams;

QString TestGAMSDomain::classname()  { return "TestGAMSDomain"; }

void TestGAMSDomain::testConstructor_Set() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    // when, then
    { GAMSSet i;
      QVERIFY_EXCEPTION_THROWN( GAMSDomain domain( i ), GAMSException );
    }
    { GAMSSet i = db.getSet("i");
      GAMSDomain domain( i );
      QCOMPARE( domain.name(), i.name() );
      QVERIFY( ! domain.isRelaxed() );
      QVERIFY( domain.getSet() == i );
    }
}

void TestGAMSDomain::testConstructor_StringRelaxedName() {
   // given
   std::string setName = "x";
   // when, then
   GAMSDomain domain( setName );
   QVERIFY( domain.isRelaxed() );
   QCOMPARE( domain.name(), setName );
   QVERIFY_EXCEPTION_THROWN( domain.getSet(), GAMSException );
}

void TestGAMSDomain::testConstructor_CharPtrRelaxedName() {
    // given
    GAMSDomain domain( "x" );
    // when, then
    QVERIFY( domain.isRelaxed() );
    QCOMPARE( domain.name().c_str(), "x" );
    QVERIFY_EXCEPTION_THROWN( domain.getSet(), GAMSException );
}

void TestGAMSDomain::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    TestGAMSObject::getTestData_Set_plants_i( db );
    GAMSDomain domain_i( db.getSet("i") );

    std::string setName = "j";
    GAMSDomain domain_j( setName );

    GAMSDomain domain_k( "k");

    // when, then
    { GAMSDomain domain( domain_i );
      QVERIFY( ! domain.isRelaxed() );
      QVERIFY( domain.getSet() == domain_i.getSet() );
    }
    { GAMSDomain domain( domain_j );
      QVERIFY( domain.isRelaxed() );
      QVERIFY_EXCEPTION_THROWN( domain.getSet(), GAMSException );
    }
    { GAMSDomain domain( domain_k );
      QVERIFY( domain.isRelaxed() );
      QVERIFY_EXCEPTION_THROWN( domain.getSet(), GAMSException );
    }
}

void TestGAMSDomain::testAssignmentOperator_Set() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    GAMSSet i = db.getSet( "i" );
    // when, then
    { GAMSDomain domain_i = i;
      QVERIFY( domain_i.getSet() == i );
      QCOMPARE( domain_i.name(), i.name() );
      QVERIFY( ! domain_i.isRelaxed() );
    }
    { GAMSSet j;
      QVERIFY_EXCEPTION_THROWN( GAMSDomain domain_j = j, GAMSException);
    }
}

void TestGAMSDomain::testAssignmentOperator_StringReleaxedName() {
    // given, when, then
    std::string setName = "i";
    GAMSDomain domain = setName;
    QVERIFY( domain.isRelaxed() );
    QCOMPARE( domain.name(), setName );
    QVERIFY_EXCEPTION_THROWN( domain.getSet(), GAMSException );
}

void TestGAMSDomain::testAssignmentOperator_CharPtrReleaxedName() {
    // given, when, then
    GAMSDomain domain = "i";
    QVERIFY( domain.isRelaxed() );
    QCOMPARE( domain.name().c_str(), "i" );
    QVERIFY_EXCEPTION_THROWN( domain.getSet(), GAMSException );
}

void TestGAMSDomain::testAssignmentOperator_GAMSDomain() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    // when,then
    { GAMSSet i = db.getSet( "i" );
      GAMSDomain domain_i( i );
      GAMSDomain domain = domain_i;
      QVERIFY( domain.getSet() == i );
      QVERIFY( domain.getSet() == domain_i.getSet() );
      QVERIFY( ! domain.isRelaxed() );
    }
    { std::string setName = "i";
      GAMSDomain domain_i( setName );
      GAMSDomain domain = domain_i;
      QCOMPARE( domain.name(), setName );
      QVERIFY_EXCEPTION_THROWN( domain.getSet(), GAMSException );
      QVERIFY( domain.isRelaxed() );
    }
    { GAMSDomain domain_i( "i" );
      GAMSDomain domain = domain_i;
      QCOMPARE( domain.name().c_str(), "i" );
      QVERIFY_EXCEPTION_THROWN( domain.getSet(), GAMSException );
      QVERIFY( domain.isRelaxed() );
    }
}

void TestGAMSDomain::testGetName() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    // when, then
    { GAMSDomain domain( db.getSet("j") );
      QCOMPARE( domain.name().c_str(), "j" );
    }
    { std::string setName = "SetX";;
      GAMSDomain domain( setName );
      QCOMPARE( domain.name(), setName );
    }
    { GAMSDomain domain( "SetX" );
      QCOMPARE( domain.name().c_str(), "SetX" );
    }
}

void TestGAMSDomain::testGetSet() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );

    // when, then
    { GAMSSet i = db.getSet("i");
      GAMSDomain domain( i );
      QCOMPARE( domain.getSet().name(), i.name() );
      QCOMPARE( domain.getSet().numberRecords(), i.numberRecords() );
      QVERIFY( domain.getSet() == i );
    }
    { std::string dom = "i";
      GAMSDomain domain( dom );
      QVERIFY_EXCEPTION_THROWN( domain.getSet(), GAMSException );
    }
    { GAMSDomain domain("i");
      QVERIFY_EXCEPTION_THROWN( domain.getSet(), GAMSException );
    }
}

void TestGAMSDomain::testIsRelaxed() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );

    // when, then
    { GAMSSet i = db.getSet("i");
      GAMSDomain domain( i );
      QVERIFY( ! domain.isRelaxed() );
    }
    { std::string dom = "i";
      GAMSDomain domain( dom );
      QVERIFY( domain.isRelaxed() );
    }
    { GAMSDomain domain("i");
      QVERIFY( domain.isRelaxed() );
    }
}

QTEST_MAIN(TestGAMSDomain)
