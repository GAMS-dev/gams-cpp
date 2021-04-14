/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2021 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2021 GAMS Development Corp. <support@gams.com>
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
#include "testgamsobject.h"

using namespace gams;

class TestGAMSDomain: public TestGAMSObject
{
};

TEST_F(TestGAMSDomain, testConstructor_Set) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    // when, then
    { GAMSSet i;
      EXPECT_THROW( GAMSDomain domain( i ), GAMSException );
    }
    { GAMSSet i = db.getSet("i");
      GAMSDomain domain( i );
      EXPECT_EQ( domain.name(), i.name() );
      ASSERT_TRUE( ! domain.isRelaxed() );
      ASSERT_TRUE( domain.getSet() == i );
    }
}

TEST_F(TestGAMSDomain, testConstructor_StringRelaxedName) {
   // given
   std::string setName = "x";
   // when, then
   GAMSDomain domain( setName );
   ASSERT_TRUE( domain.isRelaxed() );
   EXPECT_EQ( domain.name(), setName );
   EXPECT_THROW( domain.getSet(), GAMSException );
}

TEST_F(TestGAMSDomain, testConstructor_CharPtrRelaxedName) {
    // given
    GAMSDomain domain( "x" );
    // when, then
    ASSERT_TRUE( domain.isRelaxed() );
    EXPECT_STREQ( domain.name().c_str(), "x" );
    EXPECT_THROW( domain.getSet(), GAMSException );
}

TEST_F(TestGAMSDomain, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    TestGAMSObject::getTestData_Set_plants_i( db );
    GAMSDomain domain_i( db.getSet("i") );

    std::string setName = "j";
    GAMSDomain domain_j( setName );

    GAMSDomain domain_k( "k");

    // when, then
    { GAMSDomain domain( domain_i );
      ASSERT_TRUE( ! domain.isRelaxed() );
      ASSERT_TRUE( domain.getSet() == domain_i.getSet() );
    }
    { GAMSDomain domain( domain_j );
      ASSERT_TRUE( domain.isRelaxed() );
      EXPECT_THROW( domain.getSet(), GAMSException );
    }
    { GAMSDomain domain( domain_k );
      ASSERT_TRUE( domain.isRelaxed() );
      EXPECT_THROW( domain.getSet(), GAMSException );
    }
}

TEST_F(TestGAMSDomain, testAssignmentOperator_Set) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    GAMSSet i = db.getSet( "i" );
    // when, then
    { GAMSDomain domain_i = i;
      ASSERT_TRUE( domain_i.getSet() == i );
      EXPECT_EQ( domain_i.name(), i.name() );
      ASSERT_TRUE( ! domain_i.isRelaxed() );
    }
    { GAMSSet j;
      EXPECT_THROW( GAMSDomain domain_j = j, GAMSException);
    }
}

TEST_F(TestGAMSDomain, testAssignmentOperator_StringReleaxedName) {
    // given, when, then
    std::string setName = "i";
    GAMSDomain domain = setName;
    ASSERT_TRUE( domain.isRelaxed() );
    EXPECT_EQ( domain.name(), setName );
    EXPECT_THROW( domain.getSet(), GAMSException );
}

TEST_F(TestGAMSDomain, testAssignmentOperator_CharPtrReleaxedName) {
    // given, when, then
    GAMSDomain domain = "i";
    ASSERT_TRUE( domain.isRelaxed() );
    EXPECT_STREQ( domain.name().c_str(), "i" );
    EXPECT_THROW( domain.getSet(), GAMSException );
}

TEST_F(TestGAMSDomain, testAssignmentOperator_GAMSDomain) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    // when,then
    { GAMSSet i = db.getSet( "i" );
      GAMSDomain domain_i( i );
      GAMSDomain domain = domain_i;
      ASSERT_TRUE( domain.getSet() == i );
      ASSERT_TRUE( domain.getSet() == domain_i.getSet() );
      ASSERT_TRUE( ! domain.isRelaxed() );
    }
    { std::string setName = "i";
      GAMSDomain domain_i( setName );
      GAMSDomain domain = domain_i;
      EXPECT_EQ( domain.name(), setName );
      EXPECT_THROW( domain.getSet(), GAMSException );
      ASSERT_TRUE( domain.isRelaxed() );
    }
    { GAMSDomain domain_i( "i" );
      GAMSDomain domain = domain_i;
      EXPECT_STREQ( domain.name().c_str(), "i" );
      EXPECT_THROW( domain.getSet(), GAMSException );
      ASSERT_TRUE( domain.isRelaxed() );
    }
}

TEST_F(TestGAMSDomain, testGetName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_markets_j( db );
    // when, then
    { GAMSDomain domain( db.getSet("j") );
      EXPECT_STREQ( domain.name().c_str(), "j" );
    }
    { std::string setName = "SetX";;
      GAMSDomain domain( setName );
      EXPECT_EQ( domain.name(), setName );
    }
    { GAMSDomain domain( "SetX" );
      EXPECT_STREQ( domain.name().c_str(), "SetX" );
    }
}

TEST_F(TestGAMSDomain, testGetSet) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );

    // when, then
    { GAMSSet i = db.getSet("i");
      GAMSDomain domain( i );
      EXPECT_EQ( domain.getSet().name(), i.name() );
      EXPECT_EQ( domain.getSet().numberRecords(), i.numberRecords() );
      ASSERT_TRUE( domain.getSet() == i );
    }
    { std::string dom = "i";
      GAMSDomain domain( dom );
      EXPECT_THROW( domain.getSet(), GAMSException );
    }
    { GAMSDomain domain("i");
      EXPECT_THROW( domain.getSet(), GAMSException );
    }
}

TEST_F(TestGAMSDomain, testIsRelaxed) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );

    // when, then
    { GAMSSet i = db.getSet("i");
      GAMSDomain domain( i );
      ASSERT_TRUE( ! domain.isRelaxed() );
    }
    { std::string dom = "i";
      GAMSDomain domain( dom );
      ASSERT_TRUE( domain.isRelaxed() );
    }
    { GAMSDomain domain("i");
      ASSERT_TRUE( domain.isRelaxed() );
    }
}
