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

#include "gamsequation.h"
#include "gamsparameter.h"
#include "gamssymbol.h"
#include "gamsvariable.h"
#include "gamsworkspace.h"
#include "gamsworkspaceinfo.h"
#include "testgamssymbol.h"

#include <sstream>

using namespace gams;

class TestGAMSSymbol: public TestGAMSObject
{
};

TEST_F(TestGAMSSymbol, testDefaultConstructor) {
    // when
    GAMSSymbol symbol;
    // then
    ASSERT_TRUE( ! symbol.isValid() );
    EXPECT_THROW( symbol.clear(), GAMSException);
}

TEST_F(TestGAMSSymbol, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    try {
        GAMSWorkspace ws(wsInfo);
        GAMSDatabase db = ws.addDatabase();
        TestGAMSObject::getTestData_Set_plants_i( db );
        // when
        GAMSSymbol symbol( db.getSet("i") );
        // then
        ASSERT_TRUE( symbol.isValid() );
        EXPECT_EQ( symbol.name(), db.getSet("i").name() );
        EXPECT_EQ( symbol.numberRecords(), db.getSet("i").numberRecords() );
    } catch (GAMSException &e) {
        EXPECT_EQ(e.what(), "");
    }
}

TEST_F(TestGAMSSymbol, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();

    // when, then
    GAMSSet i = db.getSet("i");
    GAMSSymbol symbol_i = i;
    EXPECT_EQ( symbol_i.name(), i.name() );
    EXPECT_EQ( symbol_i.dim(), i.dim() );
    EXPECT_EQ( symbol_i.text(), i.text() );
    EXPECT_EQ( symbol_i.numberRecords(), i.numberRecords() );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSParameter b = db.getParameter("b");
    GAMSSymbol symbol_b = b;
    EXPECT_EQ( symbol_b.name(), b.name() );
    EXPECT_EQ( symbol_b.dim(), b.dim() );
    EXPECT_EQ( symbol_b.text(), b.text() );
    EXPECT_EQ( symbol_b.numberRecords(), b.numberRecords() );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSParameter f = db.getParameter("f");
    GAMSSymbol symbol_f = f;
    EXPECT_EQ( symbol_f.name(), f.name() );
    EXPECT_EQ( symbol_f.dim(), f.dim() );
    EXPECT_EQ( symbol_f.text(), f.text() );
    EXPECT_EQ( symbol_f.numberRecords(), f.numberRecords() );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSVariable x = db.getVariable("x");
    GAMSSymbol symbol_x = x;
    EXPECT_EQ( symbol_x.name(), x.name() );
    EXPECT_EQ( symbol_x.dim(), x.dim() );
    EXPECT_EQ( symbol_x.text(), x.text() );
    EXPECT_EQ( symbol_x.numberRecords(), x.numberRecords() );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSEquation cost = db.getEquation("cost");
    GAMSSymbol symbol_cost = cost;
    EXPECT_EQ( symbol_cost.name(), cost.name() );
    EXPECT_EQ( symbol_cost.dim(), cost.dim() );
    EXPECT_EQ( symbol_cost.text(), cost.text() );
    EXPECT_EQ( symbol_cost.numberRecords(), cost.numberRecords() );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols);
}

TEST_F(TestGAMSSymbol, testAssignmentOperator_IncorrectSymbolType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();

    // when, then
    GAMSSymbol i = db.getSet("i");
    EXPECT_THROW( GAMSParameter p = i, GAMSException) ;
    EXPECT_THROW( GAMSVariable v = i, GAMSException) ;
    EXPECT_THROW( GAMSEquation eq = i, GAMSException) ;
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSSymbol a = db.getParameter("a");
    EXPECT_THROW( GAMSSet s = a, GAMSException) ;
    EXPECT_THROW( GAMSVariable v = a, GAMSException) ;
    EXPECT_THROW( GAMSEquation eq = a, GAMSException) ;
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSSymbol x = db.getVariable("x");
    EXPECT_THROW( GAMSSet s = x, GAMSException) ;
    EXPECT_THROW( GAMSParameter p = x, GAMSException) ;
    EXPECT_THROW( GAMSEquation eq = x, GAMSException) ;
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    EXPECT_THROW( GAMSSet s = supply, GAMSException) ;
    EXPECT_THROW( GAMSParameter p = supply, GAMSException) ;
    EXPECT_THROW( GAMSVariable v = supply, GAMSException) ;
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols);
}

TEST_F(TestGAMSSymbol, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();

    // given
    GAMSSet j = db.getSet("j");
    GAMSSymbol symbol_j = j;
    ASSERT_TRUE( ! (symbol_j != j) );
    // when, then
    ASSERT_TRUE(  symbol_j != (GAMSSymbol)db.getSet("i") );
    ASSERT_TRUE(  symbol_j != (GAMSSymbol)db.getParameter("a") );
    ASSERT_TRUE(  symbol_j != (GAMSSymbol)db.getVariable("x") );
    ASSERT_TRUE(  symbol_j != (GAMSSymbol)db.getEquation("cost") );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSParameter a = db.getParameter("a");
    GAMSSymbol symbol_a = a;
    // when, then
    ASSERT_TRUE( !( symbol_a != a ) );
    ASSERT_TRUE( symbol_a != (GAMSSymbol)db.getSet("j") );
    ASSERT_TRUE( symbol_a != (GAMSSymbol)db.getParameter("b") );
    ASSERT_TRUE( symbol_a != (GAMSSymbol)db.getVariable("x") );
    ASSERT_TRUE( symbol_a != (GAMSSymbol)db.getEquation("cost") );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSVariable z = db.getVariable("z");
    GAMSSymbol symbol_z = z;
    // when, then
    ASSERT_TRUE( !( symbol_z != z) );
    ASSERT_TRUE( symbol_z != (GAMSSymbol)db.getSet("i") );
    ASSERT_TRUE( symbol_z != (GAMSSymbol)db.getParameter("b") );
    ASSERT_TRUE( symbol_z != (GAMSSymbol)db.getVariable("x") );
    ASSERT_TRUE( symbol_z != (GAMSSymbol)db.getEquation("cost") );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSEquation cost = db.getEquation("cost");
    GAMSSymbol symbol_cost = cost;
    // when, then
    ASSERT_TRUE( !( symbol_cost != cost) );
    ASSERT_TRUE( symbol_cost != (GAMSSymbol)db.getSet("i") );
    ASSERT_TRUE( symbol_cost != (GAMSSymbol)db.getParameter("b") );
    ASSERT_TRUE( symbol_cost != (GAMSSymbol)db.getVariable("x") );
    ASSERT_TRUE( symbol_cost != (GAMSSymbol)db.getEquation("demand") );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
}

TEST_F(TestGAMSSymbol, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();

    // given
    GAMSSet j = db.getSet("j");
    GAMSSymbol symbol_j = j;
    ASSERT_TRUE( symbol_j == j );
    // when, then
    ASSERT_TRUE( symbol_j == db.getSet("j") );
    ASSERT_TRUE( !( symbol_j == (GAMSSymbol)db.getSet("i")) );
    ASSERT_TRUE( !( symbol_j == (GAMSSymbol)db.getParameter("a")) );
    ASSERT_TRUE( !( symbol_j == (GAMSSymbol)db.getVariable("x")) );
    ASSERT_TRUE( !( symbol_j == (GAMSSymbol)db.getEquation("cost")) );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSParameter a = db.getParameter("a");
    GAMSSymbol symbol_a = a;
    // when, then
    ASSERT_TRUE( symbol_a == a );
    ASSERT_TRUE( symbol_a == db.getParameter("a") );
    ASSERT_TRUE( !( symbol_a == (GAMSSymbol)db.getSet("j")) );
    ASSERT_TRUE( !( symbol_a == (GAMSSymbol)db.getParameter("b")) );
    ASSERT_TRUE( !( symbol_a == (GAMSSymbol)db.getVariable("x")) );
    ASSERT_TRUE( !( symbol_a == (GAMSSymbol)db.getEquation("cost")) );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSVariable z = db.getVariable("z");
    GAMSSymbol symbol_z = z;
    // when, then
    ASSERT_TRUE( symbol_z == z );
    ASSERT_TRUE( symbol_z == db.getVariable("z") );
    ASSERT_TRUE( !( symbol_z == (GAMSSymbol)db.getSet("i")) );
    ASSERT_TRUE( !( symbol_z == (GAMSSymbol)db.getParameter("b")) );
    ASSERT_TRUE( !( symbol_z == (GAMSSymbol)db.getVariable("x")) );
    ASSERT_TRUE( !( symbol_z == (GAMSSymbol)db.getEquation("cost")) );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSEquation cost = db.getEquation("cost");
    GAMSSymbol symbol_cost = cost;
    // when, then
    ASSERT_TRUE( symbol_cost == cost );
    ASSERT_TRUE( symbol_cost == db.getEquation("cost") );
    ASSERT_TRUE( !( symbol_cost == (GAMSSymbol)db.getSet("i")) );
    ASSERT_TRUE( !( symbol_cost == (GAMSSymbol)db.getParameter("b")) );
    ASSERT_TRUE( !( symbol_cost == (GAMSSymbol)db.getVariable("x")) );
    ASSERT_TRUE( !( symbol_cost == (GAMSSymbol)db.getEquation("demand")) );
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
}

TEST_F(TestGAMSSymbol, testIsValid) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    int numberOfSymbols = 0;
    for(GAMSSymbol symbol : db) {
        ASSERT_TRUE( symbol.isValid() );
        ++numberOfSymbols;
    }
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
}

TEST_F(TestGAMSSymbol, testBegin) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    GAMSSet i = db.getSet("i");
    // when
    GAMSSymbolIter<GAMSSet> it_i = i.begin();
    // then
    ASSERT_TRUE( (*it_i) == i.firstRecord() );

    GAMSParameter d = db.getParameter("d");
    // when
    GAMSSymbolIter<GAMSParameter> it_d = d.begin();
    // then
    ASSERT_TRUE( (*it_d) == d.firstRecord() );

    GAMSEquation supply = db.getEquation("supply");
    // when
    GAMSSymbolIter<GAMSEquation> it_supply = supply.begin();
    // then
    ASSERT_TRUE( (*it_supply) == supply.firstRecord() );

    GAMSVariable z = db.getVariable("z");
    // when
    GAMSSymbolIter<GAMSVariable> it_z = z.begin();
    // then
    ASSERT_TRUE( (*it_z) == z.firstRecord() );
}

TEST_F(TestGAMSSymbol, testEnd) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    GAMSSet i = db.getSet("i");
    // when
    GAMSSymbolIter<GAMSSet> it_i = i.end();
    // then
    ASSERT_TRUE(it_i == i.end());

    GAMSParameter d = db.getParameter("d");
    // when
    GAMSSymbolIter<GAMSParameter> it_d = d.end();
    // then
    ASSERT_TRUE( it_d == d.end() );

    GAMSEquation supply = db.getEquation("supply");
    // when
    GAMSSymbolIter<GAMSEquation> it_supply = supply.end();
    // then
    ASSERT_TRUE( it_supply == supply.end() );

    GAMSVariable z = db.getVariable("z");
    // when
    GAMSSymbolIter<GAMSVariable> it_z = z.end();
    // then
    ASSERT_TRUE( it_z == z.end() );
}

TEST_F(TestGAMSSymbol, testAddRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    int numberOfRecords = i.numberRecords();
    i.addRecord("Albuquerque");
    EXPECT_EQ( i.numberRecords(), numberOfRecords+1);
    ASSERT_TRUE( i.findRecord("Albuquerque").isValid() );

    // when, then
    GAMSSymbol a = db.getParameter("a");
    numberOfRecords = a.numberRecords();
    a.addRecord("Florida");
    EXPECT_EQ( a.numberRecords(), numberOfRecords+1);
    ASSERT_TRUE( a.findRecord("Florida").isValid() );

    // when, then
    GAMSSymbol d = db.getParameter("d");
    numberOfRecords = d.numberRecords();
    d.addRecord("Albuquerque", "Florida");
    EXPECT_EQ( d.numberRecords(), numberOfRecords+1);
    ASSERT_TRUE( d.findRecord("Albuquerque", "Florida").isValid() );

    // when, then
    GAMSSymbol f = db.getParameter("f");
    numberOfRecords = f.numberRecords();
    EXPECT_THROW( f.addRecord(), GAMSException );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    numberOfRecords = supply.numberRecords();
    supply.addRecord("Albuquerque");
    EXPECT_EQ( supply.numberRecords(), numberOfRecords+1);
    ASSERT_TRUE( supply.findRecord("Albuquerque").isValid() );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    numberOfRecords = x.numberRecords();
    x.addRecord("Albuquerque", "Florida");
    EXPECT_EQ( x.numberRecords(), numberOfRecords+1);
    ASSERT_TRUE( x.findRecord("Albuquerque", "Florida").isValid() );
}

TEST_F(TestGAMSSymbol, testAddRecord_DuplicatedKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    int numberOfRecords = i.numberRecords();
    EXPECT_THROW( i.addRecord("seattle"), GAMSException );
    EXPECT_EQ( i.numberRecords(), numberOfRecords );

    // when
    GAMSSymbol d = db.getParameter("d");
    numberOfRecords = d.numberRecords();
    EXPECT_THROW( d.addRecord("san-diego", "new-york"), GAMSException );
    EXPECT_THROW( d.addRecord("seattle", "topeka"), GAMSException );
    EXPECT_EQ( d.numberRecords(), numberOfRecords );

    // when, then
    GAMSSymbol demand =  db.getEquation("demand");
    numberOfRecords = demand.numberRecords();
    EXPECT_THROW( demand.addRecord("chicago"), GAMSException );
    EXPECT_EQ( demand.numberRecords(), numberOfRecords );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    numberOfRecords = x.numberRecords();
    EXPECT_THROW( x.addRecord("seattle", "new-york"), GAMSException );
    EXPECT_EQ( x.numberRecords(), numberOfRecords );
}

TEST_F(TestGAMSSymbol, testAddRecord_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    int numberOfRecords = i.numberRecords();
    EXPECT_THROW( i.addRecord(), GAMSException );
    EXPECT_THROW( i.addRecord("i1", "j1"), GAMSException );
    EXPECT_EQ( i.numberRecords(), numberOfRecords );

    // when
    GAMSSymbol f = db.getParameter("f");
    numberOfRecords = f.numberRecords();
    EXPECT_THROW( f.addRecord("f1"), GAMSException );
    EXPECT_THROW( f.addRecord("f1", "f2"), GAMSException );
    EXPECT_EQ( f.numberRecords(), numberOfRecords );

    // when, then
    GAMSSymbol demand =  db.getEquation("demand");
    numberOfRecords = demand.numberRecords();
    EXPECT_THROW( demand.addRecord("seattle", "Albuquerque"), GAMSException );
    EXPECT_EQ( demand.numberRecords(), numberOfRecords );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    numberOfRecords = x.numberRecords();
    EXPECT_THROW( x.addRecord("Albuquerque"), GAMSException );
    EXPECT_EQ( x.numberRecords(), numberOfRecords );
}

TEST_F(TestGAMSSymbol, testDeleteRecord_Set_InvalidKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );

    // when, then
    GAMSSymbol i = db.getSet("i");
    std::vector<std::string> plant = { "Albuquerque"  };
    EXPECT_THROW( i.deleteRecord(plant), GAMSException);
    EXPECT_EQ( i.numberRecords(), 2 );
}

TEST_F(TestGAMSSymbol, testDeleteRecord_Parameter_InvalidKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    TestGAMSObject::getTestData_Parameter_distance_d( db );

    //when, then
    GAMSSymbol b = db.getParameter("b");
    std::vector<std::string> market = { "Albuquerque"  };
    EXPECT_THROW( b.deleteRecord( market ), GAMSException);
    EXPECT_EQ( b.numberRecords(), 3 );

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    std::vector<std::string> distance = { "san-diego", "Albuquerque"  };
    EXPECT_THROW( d.deleteRecord( distance ), GAMSException);
    EXPECT_EQ( d.numberRecords(), 6 );

}

TEST_F(TestGAMSSymbol, testDeleteRecord_InvalidKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    std::vector<std::string> supply_plants = { "seattle", "Albuquerque" };
    EXPECT_THROW( supply.deleteRecord( supply_plants ), GAMSException);
    EXPECT_EQ( supply.numberRecords(), 2 );

    // when, then
    GAMSSymbol x  = db.getVariable("x");
    std::vector<std::string> shipment = { "Albuquerque"  };
    EXPECT_THROW( x.deleteRecord( shipment ), GAMSException);
    EXPECT_EQ( x.numberRecords(), 6 );
}

TEST_F(TestGAMSSymbol, testDeleteRecord_IncorrectDimension) {
        // given
        GAMSWorkspaceInfo wsInfo("", testSystemDir);
        GAMSWorkspace ws(wsInfo);
        GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
        job.run();
        GAMSDatabase db = job.outDB();

        // when, then
        GAMSSymbol i = db.getSet("i");
        std::vector<std::string> plant = { };
        EXPECT_THROW( i.deleteRecord(plant), GAMSException);
        EXPECT_EQ( i.numberRecords(), 2 );

        // when, then
        GAMSSymbol b = db.getParameter("b");
        std::vector<std::string> market = { "san-diego", "chicago" };
        EXPECT_THROW( b.deleteRecord( market ), GAMSException);
        EXPECT_EQ( b.numberRecords(), 3 );

        // when, then
        GAMSSymbol d =  db.getParameter("d");
        std::vector<std::string> distance = { "san-diego", "chicago", "Albuquerque" };
        EXPECT_THROW( d.deleteRecord( distance ), GAMSException);
        EXPECT_EQ( d.numberRecords(), 6 );

        // when, then
        GAMSSymbol f = db.getParameter("f");
        std::vector<std::string> freight_cost = { "chicago" };
        EXPECT_THROW( f.deleteRecord( freight_cost ), GAMSException);
        EXPECT_EQ( f.numberRecords(), 1 );

        // when, then
        GAMSSymbol supply = db.getEquation("supply");
        std::vector<std::string> supply_plants = { "seattle", "topeka" };
        EXPECT_THROW( supply.deleteRecord( supply_plants ), GAMSException);
        EXPECT_EQ( supply.numberRecords(), 2 );

        // when, then
        GAMSSymbol x  = db.getVariable("x");
        std::vector<std::string> shipment = { "san-diego" };
        EXPECT_THROW( x.deleteRecord( shipment ), GAMSException);
        EXPECT_EQ( x.numberRecords(), 6 );
}

TEST_F(TestGAMSSymbol, testDeleteRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    std::vector<std::string> plant = { "San-Diego" };
    i.deleteRecord(plant);
    EXPECT_EQ( i.numberRecords(), 1 );
    EXPECT_THROW( i.findRecord( plant ), GAMSException);

    // when, then
    GAMSSymbol b = db.getParameter("b");
    std::vector<std::string> market = { "chicago" };
    b.deleteRecord( market );
    EXPECT_EQ( b.numberRecords(), 2 );
    EXPECT_THROW( b.findRecord( market ), GAMSException);

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    std::vector<std::string> distance = { "san-diego", "chicago" };
    d.deleteRecord( distance );
    EXPECT_EQ( d.numberRecords(), 5 );
    EXPECT_THROW( d.findRecord( distance ), GAMSException);

    // when, then
    GAMSSymbol f = db.getParameter("f");
    std::vector<std::string> freight_cost = { };
    f.deleteRecord( freight_cost );
    EXPECT_EQ( f.numberRecords(), 0 );
    EXPECT_THROW( f.findRecord( freight_cost ), GAMSException);

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    std::vector<std::string> supply_plants = { "seattle" };
    supply.deleteRecord( supply_plants );
    EXPECT_EQ( supply.numberRecords(), 1 );
    EXPECT_THROW( supply.findRecord( supply_plants ), GAMSException);

    // when, then
    GAMSSymbol x  = db.getVariable("x");
    std::vector<std::string> shipment = { "san-diego", "chicago" };
    x.deleteRecord( shipment );
    EXPECT_EQ( x.numberRecords(), 5 );
    EXPECT_THROW( x.findRecord( shipment ), GAMSException);
}

TEST_F(TestGAMSSymbol, testClear) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i ( db.getSet("i") );
    i.clear();
    EXPECT_EQ( i.numberRecords(), 0 );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    j.clear();
    EXPECT_EQ( j.numberRecords(), 0 );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    a.clear();
    EXPECT_EQ( a.numberRecords(), 0 );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    b.clear();
    EXPECT_EQ( b.numberRecords(), 0 );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    d.clear();
    EXPECT_EQ( d.numberRecords(), 0 );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    f.clear();
    EXPECT_EQ( f.numberRecords(), 0 );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    c.clear();
    EXPECT_EQ( c.numberRecords(), 0 );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    cost.clear();
    EXPECT_EQ( cost.numberRecords(), 0 );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    supply.clear();
    EXPECT_EQ( supply.numberRecords(), 0 );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    demand.clear();
    EXPECT_EQ( demand.numberRecords(), 0 );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    x.clear();
    EXPECT_EQ( x.numberRecords(), 0 );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    z.clear();
    EXPECT_EQ( z.numberRecords(), 0 );
}

TEST_F(TestGAMSSymbol, testGetDomains_set) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSDatabase db = ws.addDatabase();

    GAMSSet i = db.addSet("i", 1, "canning plants");
    EXPECT_EQ(i.domains().size(), size_t(1)); // there is always the universe ("*") domain.

    for(GAMSDomain dom : i.domains()) {
        ASSERT_TRUE( dom.isValid() );
        EXPECT_THROW( dom.getSet().name(), GAMSException);
    }

    GAMSSet ii = db.addSet("ii", "local plants", i);
    std::map<std::string, bool> domainMap;
    for(GAMSDomain dom : ii.domains()) {
        domainMap[dom.getSet().name()] = dom.isRelaxed();
        ASSERT_TRUE( dom.isValid() );
    }
    // then
    EXPECT_EQ( domainMap.size(), size_t(1) );
    EXPECT_EQ( domainMap["i"], false );
    domainMap.clear();
}

TEST_F(TestGAMSSymbol, testGetDomains) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();

    GAMSDatabase db = job.outDB();

    std::map<std::string, bool> domainMap;

    GAMSSet i = db.getSet("i");
    GAMSSet ii = db.addSet("ii",  "canning plants", i);
    for(GAMSDomain dom : ii.domains()) {
        domainMap[dom.getSet().name()] = dom.isRelaxed();
        ASSERT_TRUE( dom.isValid() );
    }
    // then
    EXPECT_EQ( domainMap.size(), size_t(1) );
    EXPECT_EQ( domainMap["i"], false );
    domainMap.clear();

    GAMSParameter d = db.getParameter("d");
    for(GAMSDomain dom : d.domains()) {
        domainMap[dom.getSet().name()] = dom.isRelaxed();
        ASSERT_TRUE( dom.isValid() );
    }
    // then
    EXPECT_EQ( domainMap.size(), size_t(2) );
    EXPECT_EQ( domainMap["i"], false );
    EXPECT_EQ( domainMap["j"], false );
    domainMap.clear();

    GAMSParameter f = db.getParameter("f");
    for(GAMSDomain dom : f.domains()) {
       ASSERT_TRUE(false);
    }

    GAMSVariable x = db.getVariable("x");
    for(GAMSDomain dom : x.domains()) {
        domainMap[dom.getSet().name()] = dom.isRelaxed();
        ASSERT_TRUE( dom.isValid() );
    }
    // then
    EXPECT_EQ( domainMap.size(), size_t(2) );
    EXPECT_EQ( domainMap["i"], false );
    EXPECT_EQ( domainMap["j"], false );
    domainMap.clear();

    GAMSEquation supply = db.getEquation("supply");
    for(GAMSDomain dom : supply.domains()) {
        domainMap[dom.getSet().name()] = dom.isRelaxed();
        ASSERT_TRUE( dom.isValid() );
    }
    // then
    EXPECT_EQ( domainMap.size(), size_t(1) );
    EXPECT_EQ( domainMap["i"], false );
    domainMap.clear();

    GAMSVariable z = db.getVariable("z");
    for(GAMSDomain dom : z.domains()) {
       ASSERT_TRUE(false);
    }
}

TEST_F(TestGAMSSymbol, testCheckDomains) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    TestGAMSObject::getTestData_Set_markets_j( db );

    { // strong domain
      GAMSParameter a = db.addParameter("a", "capacity of plant i in cases", GAMSDomain(db.getSet("i")));
      a.addRecord("Seattle").setValue(350.0);
      a.addRecord("San-Diego").setValue(600.0);
      // when, then
      ASSERT_TRUE( a.checkDomains() );
      EXPECT_EQ( a.getSymbolDVs(0).size(), size_t(0));

      a.addRecord("Alburqurque").setValue(123.45);
      // when, then
      ASSERT_TRUE( ! a.checkDomains() );
      EXPECT_EQ( a.getSymbolDVs(0).size(), size_t(1));
    }
    { // relaxed domain
      GAMSParameter b = db.addParameter("b", "demand at market j in cases", GAMSDomain("j"));
      b.addRecord("New-York").setValue(325.0);
      // when, then
      ASSERT_TRUE( b.checkDomains() );
      EXPECT_EQ( b.getSymbolDVs(0).size(), size_t(0));

      b.addRecord("Alburqurque").setValue(123.45);
      // when, then
      ASSERT_TRUE(  b.checkDomains() );
      EXPECT_EQ( b.getSymbolDVs(0).size(), size_t(0));
    }
    { // no domain info
      GAMSParameter d = db.addParameter("c", 2, "demand at market j in cases");
      d.addRecord("Alburqurque","New-York").setValue(123.45);

      // when, then
      ASSERT_TRUE( d.checkDomains() );
      EXPECT_EQ( d.getSymbolDVs(0).size(), size_t(0));
    }
}



TEST_F(TestGAMSSymbol, testGetSymbolDVs) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

   {  GAMSParameter a = db.getParameter("a");
      a.addRecord("Alburquerque").setValue(123.45);
      a.addRecord("SantaFe").setValue(123.45);
      ASSERT_TRUE( ! a.checkDomains() );
      // when, then
      EXPECT_EQ( a.getSymbolDVs(0).size(), size_t(2) );
      std::map<std::string, size_t> recordCounter;
      for(GAMSSymbolDomainViolation dom : a.getSymbolDVs(0)) {
          ASSERT_TRUE( dom.isValid() );
          recordCounter[dom.violRec().key(0)]++;
          EXPECT_EQ( dom.violInd().size(), size_t(1) );
          ASSERT_TRUE( dom.violInd().at(0) );
      }
      EXPECT_EQ( recordCounter.size(), size_t(2) );
      EXPECT_EQ( recordCounter["Alburquerque"], size_t(1) );
      EXPECT_EQ( recordCounter["SantaFe"], size_t(1) );
      recordCounter.clear();

      a.addRecord("SanFrancisco").setValue(123.45);

      // when, then
      EXPECT_EQ( a.getSymbolDVs(0).size(), size_t(3) );
      for(GAMSSymbolDomainViolation dom : a.getSymbolDVs(2)) {
          ASSERT_TRUE( dom.isValid() );
          EXPECT_EQ( dom.violRec().type(), GAMSEnum::SymbolType::SymTypePar );
          recordCounter[dom.violRec().key(0)]++;
          EXPECT_EQ( dom.violInd().size(), size_t(1) );
          ASSERT_TRUE( dom.violInd().at(0) );
      }
      EXPECT_EQ( recordCounter.size(), size_t(2) );
      EXPECT_EQ( recordCounter["Alburquerque"], size_t(1) );
      EXPECT_EQ( recordCounter["SantaFe"], size_t(1) );
    }
    {  GAMSEquation supply = db.getEquation("supply");
       supply.addRecord("Alburquerque").setMarginal(123.45);
       supply.addRecord("Florida").setScale(1.2345);

       // when, then
       EXPECT_EQ( supply.getSymbolDVs(0).size(), size_t(2) );
       std::map<std::string, size_t> recordCounter;
       for(GAMSSymbolDomainViolation dom : supply.getSymbolDVs(5)) {
           ASSERT_TRUE( dom.isValid() );
           EXPECT_EQ( dom.violRec().type(), GAMSEnum::SymbolType::SymTypeEqu );
           recordCounter[dom.violRec().key(0)]++;
           EXPECT_EQ( dom.violInd().size(), size_t(1) );
           ASSERT_TRUE( dom.violInd().at(0) );
       }
       EXPECT_EQ( recordCounter.size(), size_t(2) );
       EXPECT_EQ( recordCounter["Alburquerque"], size_t(1) );
       EXPECT_EQ( recordCounter["Florida"], size_t(1) );
       recordCounter.clear();
    }
    {  GAMSVariable x = db.getVariable("x");
       x.addRecord("seattle", "Alburquerque").setMarginal(123.45);
       x.addRecord("Alburquerque", "topeka").setMarginal(54.321);

       // when, then
       EXPECT_EQ( x.getSymbolDVs(0).size(), size_t(2) );
       std::map<std::string, int> recordMap;
       for(GAMSSymbolDomainViolation dom : x.getSymbolDVs(5)) {
           ASSERT_TRUE( dom.isValid() );
           EXPECT_EQ( dom.violRec().type(), GAMSEnum::SymbolType::SymTypeVar );

           if (dom.violRec().key(0).compare("seattle")==0 ) {
               ASSERT_TRUE( dom.violRec().key(1).compare("Alburquerque")==0 );
               std::vector<bool> vi = dom.violInd();
               EXPECT_EQ( vi.size(), size_t(2) );
               ASSERT_TRUE( ! vi[0] );
               ASSERT_TRUE( vi[1] );
           } else if (dom.violRec().key(0).compare("Alburquerque")==0 ) {
               ASSERT_TRUE(dom.violRec().key(1).compare("topeka")==0);
               std::vector<bool> vi = dom.violInd();
               EXPECT_EQ( vi.size(), size_t(2) );
               ASSERT_TRUE( vi[0] );
               ASSERT_TRUE( ! vi[1] );
           }
           std::stringstream ss;
           ss << dom.violRec().key(0) << "_" << dom.violRec().key(1) ;
           recordMap[ss.str()]++;

       }
       EXPECT_EQ( recordMap.size(), size_t(2) );
       ASSERT_TRUE( equals( recordMap["seattle_Alburquerque"], size_t(1)) );
       ASSERT_TRUE( equals( recordMap["Alburquerque_topeka"], size_t(1)) );
    }
}

TEST_F(TestGAMSSymbol, testGetFirstRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    EXPECT_EQ( i.firstRecord().key(0).c_str(), "seattle" );

    // when, then
    GAMSSymbol b( db.getParameter("b") );
    EXPECT_EQ( b.firstRecord().key(0).c_str(), "new-york");

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_EQ( d.firstRecord().key(1).c_str(), "new-york");
    EXPECT_EQ( d.firstRecord().key(0).c_str(), "seattle" );

    // when, then
    GAMSSymbol f( db.getParameter("f") );
    ASSERT_TRUE( f.firstRecord().isValid() );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_EQ( supply.firstRecord().key(0).c_str(), "seattle" );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_EQ( x.firstRecord().key(1).c_str(), "new-york");
    EXPECT_EQ( x.firstRecord().key(0).c_str(), "seattle" );
}

TEST_F(TestGAMSSymbol, testGetFirstRecordSlice) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol j = db.getSet("j");
    EXPECT_EQ( j.firstRecord("chicago").key(0).c_str(), "chicago" );

    // when, then
    GAMSSymbol b( db.getParameter("b") );
    EXPECT_EQ( b.firstRecord("topeka").key(0).c_str(), "topeka");

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_EQ( d.firstRecord("seattle", "topeka").key(0).c_str(), "seattle" );
    EXPECT_EQ( d.firstRecord("seattle", "topeka").key(1).c_str(), "topeka");

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_EQ( supply.firstRecord("san-diego").key(0).c_str(), "san-diego" );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_EQ( x.firstRecord("san-diego", "new-york").key(0).c_str(), "san-diego" );
    EXPECT_EQ( x.firstRecord("san-diego", "new-york").key(1).c_str(), "new-york");
}

TEST_F(TestGAMSSymbol, testGetFirstRecordSlice_InvalidKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol j = db.getSet("j");
    EXPECT_THROW( j.firstRecord("Albuquerque"), GAMSException );

    // when, then
    GAMSSymbol a( db.getParameter("a") );
    EXPECT_THROW( a.firstRecord("Boston"), GAMSException );

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_THROW( d.firstRecord("seattle", "Omaha"), GAMSException );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_THROW( supply.firstRecord("Atlanta"), GAMSException );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_THROW( x.firstRecord("seattle", "Dallas"), GAMSException );
}

TEST_F(TestGAMSSymbol, testGetFirstRecordSlice_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol j = db.getSet("j");
    EXPECT_THROW( j.firstRecord("chicago", "seattle"), GAMSException );

    // when, then
    GAMSSymbol a( db.getParameter("a") );
    EXPECT_THROW( a.firstRecord("seattle", "chicago"), GAMSException );

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_THROW( d.firstRecord("chicago", "seattle", "new-york"), GAMSException );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_THROW( supply.firstRecord("chicago"), GAMSException );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_THROW( x.firstRecord("seattle"), GAMSException );
}

TEST_F(TestGAMSSymbol, testGetLastRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    EXPECT_EQ( i.lastRecord().key(0).c_str(), "san-diego" );

    // when, then
    GAMSSymbol b( db.getParameter("b") );
    EXPECT_EQ( b.lastRecord().key(0).c_str(), "topeka");

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_EQ( d.lastRecord().key(1).c_str(), "topeka");
    EXPECT_EQ( d.lastRecord().key(0).c_str(), "san-diego" );

    // when, then
    GAMSSymbol f( db.getParameter("f") );
    ASSERT_TRUE( f.lastRecord().isValid() );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_EQ( supply.lastRecord().key(0).c_str(), "san-diego" );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_EQ( x.lastRecord().key(1).c_str(), "topeka");
    EXPECT_EQ( x.lastRecord().key(0).c_str(), "san-diego" );
}

TEST_F(TestGAMSSymbol, testGetLastRecordSlice) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol j = db.getSet("j");
    EXPECT_EQ( j.lastRecord("chicago").key(0).c_str(), "chicago" );

    // when, then
    GAMSSymbol a = db.getParameter("b");
    EXPECT_EQ( a.lastRecord("new-york").key(0).c_str(), "new-york");

    // when, then
    GAMSSymbol d = db.getParameter("d");
    EXPECT_EQ( d.lastRecord("san-diego", "chicago").key(1).c_str(), "chicago" );
    EXPECT_EQ( d.lastRecord("san-diego", "chicago").key(0).c_str(), "san-diego" );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    EXPECT_EQ( supply.lastRecord("seattle").key(0).c_str(), "seattle" );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    EXPECT_EQ( x.lastRecord("seattle", "topeka").key(1).c_str(), "topeka");
    EXPECT_EQ( x.lastRecord("seattle", "topeka").key(0).c_str(), "seattle" );
}

TEST_F(TestGAMSSymbol, testGetLastRecordSlice_InValidKeys) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    EXPECT_THROW( i.lastRecord("Albuquerque"), GAMSException );

    // when, then
    GAMSSymbol b = db.getParameter("b");
    EXPECT_THROW( b.lastRecord("Boston"), GAMSException );

    // when, then
    GAMSSymbol d = db.getParameter("d");
    EXPECT_THROW( d.lastRecord("seattle", "Omaha"), GAMSException );

    // when, then
    GAMSSymbol demand = db.getEquation("demand");
    EXPECT_THROW( demand.lastRecord("Atlanta"), GAMSException );

    // when, then
    GAMSSymbol x =  db.getVariable("x");
    EXPECT_THROW( x.lastRecord("seattle", "Dallas"), GAMSException );
}

TEST_F(TestGAMSSymbol, testGetLastRecordSlice_IncorrectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    EXPECT_THROW( i.lastRecord("chicago", "seattle"), GAMSException );

    // when, then
    GAMSSymbol b( db.getParameter("b") );
    EXPECT_THROW( b.firstRecord("seattle", "chicago"), GAMSException );

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_THROW( d.lastRecord("chicago"), GAMSException );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_THROW( supply.lastRecord("chicago"), GAMSException );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_THROW( x.lastRecord("seattle"), GAMSException );
}

TEST_F(TestGAMSSymbol, testFindRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    ASSERT_TRUE( i.findRecord("san-diego").isValid() );
    EXPECT_EQ( i.findRecord("san-diego").key(0).c_str(), "san-diego");

    // when, then
    GAMSSymbol b = db.getParameter("b");
    ASSERT_TRUE( b.findRecord("chicago").isValid() );
    EXPECT_EQ( b.findRecord("chicago").key(0).c_str(), "chicago");

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    ASSERT_TRUE( d.findRecord("seattle", "chicago").isValid() );
    EXPECT_EQ( d.findRecord("seattle", "chicago").key(0).c_str(), "seattle");
    EXPECT_EQ( d.findRecord("seattle", "chicago").key(1).c_str(), "chicago");

    // when, then
    GAMSSymbol f = db.getParameter("f");
    ASSERT_TRUE( f.findRecord().isValid() );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    ASSERT_TRUE( supply.findRecord("seattle").isValid() );
    EXPECT_EQ( supply.findRecord("seattle").key(0).c_str(), "seattle");

    // when, then
    GAMSSymbol x  = db.getVariable("x");
    ASSERT_TRUE( x.findRecord("seattle", "topeka").isValid() );
    EXPECT_EQ( x.findRecord("seattle", "topeka").key(0).c_str(), "seattle");
    EXPECT_EQ( x.findRecord("seattle", "topeka").key(1).c_str(), "topeka");
}

TEST_F(TestGAMSSymbol, testFindNonExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol j = db.getSet("j");
    EXPECT_THROW( j.findRecord("Albuquerque"), GAMSException );

    // when, then
    GAMSSymbol a = db.getParameter("a");
    EXPECT_THROW( a.findRecord("Albuquerque", "chicago"), GAMSException );

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    EXPECT_THROW( d.findRecord("Albuquerque", "chicago"), GAMSException );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    EXPECT_THROW( supply.findRecord("Albuquerque"), GAMSException );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    EXPECT_THROW( x.findRecord("Albuquerque"), GAMSException );
}

TEST_F(TestGAMSSymbol, testFindRecord_IncrorectDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    EXPECT_THROW( i.findRecord("seattle", "chicago"), GAMSException );

    // when, then
    GAMSSymbol b = db.getParameter("b");
    EXPECT_THROW( b.findRecord("seattle", "chicago"), GAMSException );

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    EXPECT_THROW( d.findRecord("chicago"), GAMSException );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    EXPECT_THROW( supply.findRecord(), GAMSException );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    EXPECT_THROW( x.findRecord("topeka"), GAMSException );
}

TEST_F(TestGAMSSymbol, testMergeExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSet j = db.getSet("j");
    int numberOfRecords = j.numberRecords();
    GAMSSetRecord recj = j.mergeRecord("chicago");
    EXPECT_EQ( j.numberRecords(), numberOfRecords );
    EXPECT_EQ( recj.key(0).c_str(), "chicago" );

    // when, then
    GAMSParameter b = db.getParameter("b");
    numberOfRecords = b.numberRecords();
    GAMSParameterRecord recb = b.mergeRecord("topeka");
    EXPECT_EQ( b.numberRecords(), numberOfRecords );
    EXPECT_EQ( recb.key(0).c_str(), "topeka" );

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    numberOfRecords = d.numberRecords();
    GAMSSymbolRecord recd = d.mergeRecord("seattle", "chicago");
    EXPECT_EQ( d.numberRecords(), numberOfRecords );
    EXPECT_EQ( recd.key(0).c_str(), "seattle" );
    EXPECT_EQ( recd.key(1).c_str(), "chicago" );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    numberOfRecords = supply.numberRecords();
    GAMSSymbolRecord recs = supply.mergeRecord("seattle");
    EXPECT_EQ( supply.numberRecords(), numberOfRecords );
    EXPECT_EQ( recs.key(0).c_str(), "seattle" );

    // when, then
    GAMSSymbol x  = db.getVariable("x");
    numberOfRecords = x.numberRecords();
    GAMSSymbolRecord recx = x.mergeRecord("seattle", "topeka");
    EXPECT_EQ( x.numberRecords(), numberOfRecords );
    EXPECT_EQ( recx.key(0).c_str(), "seattle" );
    EXPECT_EQ( recx.key(1).c_str(), "topeka" );
}

TEST_F(TestGAMSSymbol, testMergeNonExistingRecord) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSet j = db.getSet("j");
    int numberOfRecords = j.numberRecords();
    GAMSSetRecord recj = j.mergeRecord("Albuquerque");
    EXPECT_EQ( j.numberRecords(), numberOfRecords+1 );
    EXPECT_EQ( recj.key(0).c_str(), "Albuquerque" );

    // when, then
    GAMSParameter b = db.getParameter("b");
    numberOfRecords = b.numberRecords();
    GAMSParameterRecord recb = b.mergeRecord("Albuquerque");
    EXPECT_EQ( b.numberRecords(), numberOfRecords+1 );
    EXPECT_EQ( recb.key(0).c_str(), "Albuquerque" );

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    numberOfRecords = d.numberRecords();
    GAMSSymbolRecord recd = d.mergeRecord("seattle", "Albuquerque");
    EXPECT_EQ( d.numberRecords(), numberOfRecords+1 );
    EXPECT_EQ( recd.key(0).c_str(), "seattle" );
    EXPECT_EQ( recd.key(1).c_str(), "Albuquerque" );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    numberOfRecords = supply.numberRecords();
    GAMSSymbolRecord recs = supply.mergeRecord("Boston");
    EXPECT_EQ( supply.numberRecords(), numberOfRecords+1 );
    EXPECT_EQ( recs.key(0).c_str(), "Boston" );

    // when, then
    GAMSSymbol x  = db.getVariable("x");
    numberOfRecords = x.numberRecords();
    GAMSSymbolRecord recx = x.mergeRecord("san-diego", "Albuquerque");
    EXPECT_EQ( x.numberRecords(), numberOfRecords+1 );
    EXPECT_EQ( recx.key(0).c_str(), "san-diego" );
    EXPECT_EQ( recx.key(1).c_str(), "Albuquerque" );
}

TEST_F(TestGAMSSymbol, testCopySymbol) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSymbol i( db.getSet("i") );
    GAMSSymbol j( db.getSet("j") );
    // when
    i.copySymbol( j );
    // then
    EXPECT_EQ( i.numberRecords(), j.numberRecords() );
}

TEST_F(TestGAMSSymbol, testGetDatabase) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSSymbol i( db.getSet("i") );
    // when, then
    ASSERT_TRUE( i.database() == db );
    EXPECT_EQ( i.database().getNrSymbols(), db.getNrSymbols());
}

TEST_F(TestGAMSSymbol, testGetText) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i( db.getSet("i") );
    EXPECT_EQ( i.text().c_str(), "canning plants" );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    EXPECT_EQ( j.text().c_str(), "markets" );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    EXPECT_EQ( a.text().c_str(), "capacity of plant i in cases" );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    EXPECT_EQ( b.text().c_str(), "demand at market j in cases" );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_EQ( d.text().c_str(), "distance in thousands of miles" );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    EXPECT_EQ( f.text().c_str(), "freight in dollars per case per thousand miles" );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    EXPECT_EQ( c.text().c_str(), "transport cost in thousands of dollars per case" );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    EXPECT_EQ( cost.text().c_str(), "define objective function" );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_EQ( supply.text().c_str(), "observe supply limit at plant i" );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    EXPECT_EQ( demand.text().c_str(), "satisfy demand at market j" );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_EQ( x.text().c_str(), "shipment quantities in cases" );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    EXPECT_EQ( z.text().c_str(), "total transportation costs in thousands of dollars" );
}

TEST_F(TestGAMSSymbol, testGetName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i( db.getSet("i") );
    EXPECT_EQ( i.name().c_str(), "i" );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    EXPECT_EQ( j.name().c_str(), "j" );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    EXPECT_EQ( a.name().c_str(), "a" );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    EXPECT_EQ( b.name().c_str(), "b" );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_EQ( d.name().c_str(), "d" );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    EXPECT_EQ( f.name().c_str(), "f" );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    EXPECT_EQ( c.name().c_str(), "c" );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    EXPECT_EQ( cost.name().c_str(), "cost" );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_EQ( supply.name().c_str(), "supply" );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    EXPECT_EQ( demand.name().c_str(), "demand" );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_EQ( x.name().c_str(), "x" );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    EXPECT_EQ( z.name().c_str(), "z" );
}

TEST_F(TestGAMSSymbol, testGetNumberOfRecords) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i( db.getSet("i") );
    EXPECT_EQ( i.numberRecords(), 2 );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    EXPECT_EQ( j.numberRecords(), 3 );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    EXPECT_EQ( a.numberRecords(), 2 );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    EXPECT_EQ( b.numberRecords(), 3 );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_EQ( d.numberRecords(), 6 );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    EXPECT_EQ( f.numberRecords(), 1 );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    EXPECT_EQ( c.numberRecords(), 6 );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    EXPECT_EQ( cost.numberRecords(), 1 );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_EQ( supply.numberRecords(), 2 );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    EXPECT_EQ( demand.numberRecords(), 3 );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_EQ( x.numberRecords(), 6 );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    EXPECT_EQ( z.numberRecords(), 1 );
}

TEST_F(TestGAMSSymbol, testGetDim) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i( db.getSet("i") );
    EXPECT_EQ( i.dim(), 1 );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    EXPECT_EQ( j.dim(), 1 );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    EXPECT_EQ( a.dim(), 1 );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    EXPECT_EQ( b.dim(), 1 );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_EQ( d.dim(), 2 );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    EXPECT_EQ( f.dim(), 0 );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    EXPECT_EQ( c.dim(), 2 );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    EXPECT_EQ( cost.dim(), 0 );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_EQ( supply.dim(), 1 );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    EXPECT_EQ( demand.dim(), 1 );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_EQ( x.dim(), 2 );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    EXPECT_EQ( z.dim(), 0 );
}

TEST_F(TestGAMSSymbol, testGetType) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i( db.getSet("i") );
    EXPECT_EQ( i.type(), GAMSEnum::SymbolType::SymTypeSet );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    EXPECT_EQ( j.type(), GAMSEnum::SymbolType::SymTypeSet );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    EXPECT_EQ( a.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    EXPECT_EQ( b.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    EXPECT_EQ( d.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    EXPECT_EQ( f.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    EXPECT_EQ( c.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    EXPECT_EQ( cost.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    EXPECT_EQ( supply.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    EXPECT_EQ( demand.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    EXPECT_EQ( x.type(), GAMSEnum::SymbolType::SymTypeVar );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    EXPECT_EQ( z.type(), GAMSEnum::SymbolType::SymTypeVar );
}


