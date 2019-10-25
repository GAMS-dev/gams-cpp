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

#include "gamsequation.h"
#include "gamsparameter.h"
#include "gamssymbol.h"
#include "gamsvariable.h"
#include "gamsworkspace.h"
#include "gamsworkspaceinfo.h"
#include "testgamssymbol.h"

#include <sstream>

using namespace gams;

QString TestGAMSSymbol::classname()  { return "TestGAMSSymbol"; }

void TestGAMSSymbol::testDefaultConstructor() {
    // when
    GAMSSymbol symbol;
    // then
    QVERIFY( ! symbol.isValid() );
    QVERIFY_EXCEPTION_THROWN( symbol.clear(), GAMSException);
}

void TestGAMSSymbol::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    try {
        GAMSWorkspace ws(wsInfo);
        GAMSDatabase db = ws.addDatabase();
        TestGAMSObject::getTestData_Set_plants_i( db );
        // when
        GAMSSymbol symbol( db.getSet("i") );
        // then
        QVERIFY( symbol.isValid() );
        QCOMPARE( symbol.name(), db.getSet("i").name() );
        QCOMPARE( symbol.numberRecords(), db.getSet("i").numberRecords() );
    } catch (GAMSException &e) {
        QCOMPARE(e.what(), "");
    }
}

void TestGAMSSymbol::testAssignmentOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();

    // when, then
    GAMSSet i = db.getSet("i");
    GAMSSymbol symbol_i = i;
    QCOMPARE( symbol_i.name(), i.name() );
    QCOMPARE( symbol_i.dim(), i.dim() );
    QCOMPARE( symbol_i.text(), i.text() );
    QCOMPARE( symbol_i.numberRecords(), i.numberRecords() );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSParameter b = db.getParameter("b");
    GAMSSymbol symbol_b = b;
    QCOMPARE( symbol_b.name(), b.name() );
    QCOMPARE( symbol_b.dim(), b.dim() );
    QCOMPARE( symbol_b.text(), b.text() );
    QCOMPARE( symbol_b.numberRecords(), b.numberRecords() );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSParameter f = db.getParameter("f");
    GAMSSymbol symbol_f = f;
    QCOMPARE( symbol_f.name(), f.name() );
    QCOMPARE( symbol_f.dim(), f.dim() );
    QCOMPARE( symbol_f.text(), f.text() );
    QCOMPARE( symbol_f.numberRecords(), f.numberRecords() );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSVariable x = db.getVariable("x");
    GAMSSymbol symbol_x = x;
    QCOMPARE( symbol_x.name(), x.name() );
    QCOMPARE( symbol_x.dim(), x.dim() );
    QCOMPARE( symbol_x.text(), x.text() );
    QCOMPARE( symbol_x.numberRecords(), x.numberRecords() );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSEquation cost = db.getEquation("cost");
    GAMSSymbol symbol_cost = cost;
    QCOMPARE( symbol_cost.name(), cost.name() );
    QCOMPARE( symbol_cost.dim(), cost.dim() );
    QCOMPARE( symbol_cost.text(), cost.text() );
    QCOMPARE( symbol_cost.numberRecords(), cost.numberRecords() );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols);
}

void TestGAMSSymbol::testAssignmentOperator_IncorrectSymbolType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();

    // when, then
    GAMSSymbol i = db.getSet("i");
    QVERIFY_EXCEPTION_THROWN( GAMSParameter p = i, GAMSException) ;
    QVERIFY_EXCEPTION_THROWN( GAMSVariable v = i, GAMSException) ;
    QVERIFY_EXCEPTION_THROWN( GAMSEquation eq = i, GAMSException) ;
    QCOMPARE( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSSymbol a = db.getParameter("a");
    QVERIFY_EXCEPTION_THROWN( GAMSSet s = a, GAMSException) ;
    QVERIFY_EXCEPTION_THROWN( GAMSVariable v = a, GAMSException) ;
    QVERIFY_EXCEPTION_THROWN( GAMSEquation eq = a, GAMSException) ;
    QCOMPARE( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSSymbol x = db.getVariable("x");
    QVERIFY_EXCEPTION_THROWN( GAMSSet s = x, GAMSException) ;
    QVERIFY_EXCEPTION_THROWN( GAMSParameter p = x, GAMSException) ;
    QVERIFY_EXCEPTION_THROWN( GAMSEquation eq = x, GAMSException) ;
    QCOMPARE( db.getNrSymbols(), numberOfSymbols);

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    QVERIFY_EXCEPTION_THROWN( GAMSSet s = supply, GAMSException) ;
    QVERIFY_EXCEPTION_THROWN( GAMSParameter p = supply, GAMSException) ;
    QVERIFY_EXCEPTION_THROWN( GAMSVariable v = supply, GAMSException) ;
    QCOMPARE( db.getNrSymbols(), numberOfSymbols);
}

void TestGAMSSymbol::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();

    // given
    GAMSSet j = db.getSet("j");
    GAMSSymbol symbol_j = j;
    QVERIFY( ! (symbol_j != j) );
    // when, then
    QVERIFY(  symbol_j != (GAMSSymbol)db.getSet("i") );
    QVERIFY(  symbol_j != (GAMSSymbol)db.getParameter("a") );
    QVERIFY(  symbol_j != (GAMSSymbol)db.getVariable("x") );
    QVERIFY(  symbol_j != (GAMSSymbol)db.getEquation("cost") );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSParameter a = db.getParameter("a");
    GAMSSymbol symbol_a = a;
    // when, then
    QVERIFY( !( symbol_a != a ) );
    QVERIFY( symbol_a != (GAMSSymbol)db.getSet("j") );
    QVERIFY( symbol_a != (GAMSSymbol)db.getParameter("b") );
    QVERIFY( symbol_a != (GAMSSymbol)db.getVariable("x") );
    QVERIFY( symbol_a != (GAMSSymbol)db.getEquation("cost") );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSVariable z = db.getVariable("z");
    GAMSSymbol symbol_z = z;
    // when, then
    QVERIFY( !( symbol_z != z) );
    QVERIFY( symbol_z != (GAMSSymbol)db.getSet("i") );
    QVERIFY( symbol_z != (GAMSSymbol)db.getParameter("b") );
    QVERIFY( symbol_z != (GAMSSymbol)db.getVariable("x") );
    QVERIFY( symbol_z != (GAMSSymbol)db.getEquation("cost") );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSEquation cost = db.getEquation("cost");
    GAMSSymbol symbol_cost = cost;
    // when, then
    QVERIFY( !( symbol_cost != cost) );
    QVERIFY( symbol_cost != (GAMSSymbol)db.getSet("i") );
    QVERIFY( symbol_cost != (GAMSSymbol)db.getParameter("b") );
    QVERIFY( symbol_cost != (GAMSSymbol)db.getVariable("x") );
    QVERIFY( symbol_cost != (GAMSSymbol)db.getEquation("demand") );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );
}

void TestGAMSSymbol::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    int numberOfSymbols = db.getNrSymbols();

    // given
    GAMSSet j = db.getSet("j");
    GAMSSymbol symbol_j = j;
    QVERIFY( symbol_j == j );
    // when, then
    QVERIFY( symbol_j == db.getSet("j") );
    QVERIFY( !( symbol_j == (GAMSSymbol)db.getSet("i")) );
    QVERIFY( !( symbol_j == (GAMSSymbol)db.getParameter("a")) );
    QVERIFY( !( symbol_j == (GAMSSymbol)db.getVariable("x")) );
    QVERIFY( !( symbol_j == (GAMSSymbol)db.getEquation("cost")) );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSParameter a = db.getParameter("a");
    GAMSSymbol symbol_a = a;
    // when, then
    QVERIFY( symbol_a == a );
    QVERIFY( symbol_a == db.getParameter("a") );
    QVERIFY( !( symbol_a == (GAMSSymbol)db.getSet("j")) );
    QVERIFY( !( symbol_a == (GAMSSymbol)db.getParameter("b")) );
    QVERIFY( !( symbol_a == (GAMSSymbol)db.getVariable("x")) );
    QVERIFY( !( symbol_a == (GAMSSymbol)db.getEquation("cost")) );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSVariable z = db.getVariable("z");
    GAMSSymbol symbol_z = z;
    // when, then
    QVERIFY( symbol_z == z );
    QVERIFY( symbol_z == db.getVariable("z") );
    QVERIFY( !( symbol_z == (GAMSSymbol)db.getSet("i")) );
    QVERIFY( !( symbol_z == (GAMSSymbol)db.getParameter("b")) );
    QVERIFY( !( symbol_z == (GAMSSymbol)db.getVariable("x")) );
    QVERIFY( !( symbol_z == (GAMSSymbol)db.getEquation("cost")) );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );

    // given
    GAMSEquation cost = db.getEquation("cost");
    GAMSSymbol symbol_cost = cost;
    // when, then
    QVERIFY( symbol_cost == cost );
    QVERIFY( symbol_cost == db.getEquation("cost") );
    QVERIFY( !( symbol_cost == (GAMSSymbol)db.getSet("i")) );
    QVERIFY( !( symbol_cost == (GAMSSymbol)db.getParameter("b")) );
    QVERIFY( !( symbol_cost == (GAMSSymbol)db.getVariable("x")) );
    QVERIFY( !( symbol_cost == (GAMSSymbol)db.getEquation("demand")) );
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );
}

void TestGAMSSymbol::testIsValid() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    int numberOfSymbols = 0;
    for(GAMSSymbol symbol : db) {
        QVERIFY( symbol.isValid() );
        ++numberOfSymbols;
    }
    QCOMPARE( db.getNrSymbols(), numberOfSymbols );
}

void TestGAMSSymbol::testBegin() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    GAMSSet i = db.getSet("i");
    // when
    GAMSSymbolIter<GAMSSet> it_i = i.begin();
    // then
    QVERIFY( (*it_i) == i.firstRecord() );

    GAMSParameter d = db.getParameter("d");
    // when
    GAMSSymbolIter<GAMSParameter> it_d = d.begin();
    // then
    QVERIFY( (*it_d) == d.firstRecord() );

    GAMSEquation supply = db.getEquation("supply");
    // when
    GAMSSymbolIter<GAMSEquation> it_supply = supply.begin();
    // then
    QVERIFY( (*it_supply) == supply.firstRecord() );

    GAMSVariable z = db.getVariable("z");
    // when
    GAMSSymbolIter<GAMSVariable> it_z = z.begin();
    // then
    QVERIFY( (*it_z) == z.firstRecord() );
}

void TestGAMSSymbol::testEnd() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    GAMSSet i = db.getSet("i");
    // when
    GAMSSymbolIter<GAMSSet> it_i = i.end();
    // then
    QVERIFY(it_i == i.end());

    GAMSParameter d = db.getParameter("d");
    // when
    GAMSSymbolIter<GAMSParameter> it_d = d.end();
    // then
    QVERIFY( it_d == d.end() );

    GAMSEquation supply = db.getEquation("supply");
    // when
    GAMSSymbolIter<GAMSEquation> it_supply = supply.end();
    // then
    QVERIFY( it_supply == supply.end() );

    GAMSVariable z = db.getVariable("z");
    // when
    GAMSSymbolIter<GAMSVariable> it_z = z.end();
    // then
    QVERIFY( it_z == z.end() );
}

void TestGAMSSymbol::testAddRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    int numberOfRecords = i.numberRecords();
    i.addRecord("Albuquerque");
    QCOMPARE( i.numberRecords(), numberOfRecords+1);
    QVERIFY( i.findRecord("Albuquerque").isValid() );

    // when, then
    GAMSSymbol a = db.getParameter("a");
    numberOfRecords = a.numberRecords();
    a.addRecord("Florida");
    QCOMPARE( a.numberRecords(), numberOfRecords+1);
    QVERIFY( a.findRecord("Florida").isValid() );

    // when, then
    GAMSSymbol d = db.getParameter("d");
    numberOfRecords = d.numberRecords();
    d.addRecord("Albuquerque", "Florida");
    QCOMPARE( d.numberRecords(), numberOfRecords+1);
    QVERIFY( d.findRecord("Albuquerque", "Florida").isValid() );

    // when, then
    GAMSSymbol f = db.getParameter("f");
    numberOfRecords = f.numberRecords();
    QVERIFY_EXCEPTION_THROWN( f.addRecord(), GAMSException );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    numberOfRecords = supply.numberRecords();
    supply.addRecord("Albuquerque");
    QCOMPARE( supply.numberRecords(), numberOfRecords+1);
    QVERIFY( supply.findRecord("Albuquerque").isValid() );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    numberOfRecords = x.numberRecords();
    x.addRecord("Albuquerque", "Florida");
    QCOMPARE( x.numberRecords(), numberOfRecords+1);
    QVERIFY( x.findRecord("Albuquerque", "Florida").isValid() );
}

void TestGAMSSymbol::testAddRecord_DuplicatedKeys() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    int numberOfRecords = i.numberRecords();
    QVERIFY_EXCEPTION_THROWN( i.addRecord("seattle"), GAMSException );
    QCOMPARE( i.numberRecords(), numberOfRecords );

    // when
    GAMSSymbol d = db.getParameter("d");
    numberOfRecords = d.numberRecords();
    QVERIFY_EXCEPTION_THROWN( d.addRecord("san-diego", "new-york"), GAMSException );
    QVERIFY_EXCEPTION_THROWN( d.addRecord("seattle", "topeka"), GAMSException );
    QCOMPARE( d.numberRecords(), numberOfRecords );

    // when, then
    GAMSSymbol demand =  db.getEquation("demand");
    numberOfRecords = demand.numberRecords();
    QVERIFY_EXCEPTION_THROWN( demand.addRecord("chicago"), GAMSException );
    QCOMPARE( demand.numberRecords(), numberOfRecords );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    numberOfRecords = x.numberRecords();
    QVERIFY_EXCEPTION_THROWN( x.addRecord("seattle", "new-york"), GAMSException );
    QCOMPARE( x.numberRecords(), numberOfRecords );
}

void TestGAMSSymbol::testAddRecord_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    int numberOfRecords = i.numberRecords();
    QVERIFY_EXCEPTION_THROWN( i.addRecord(), GAMSException );
    QVERIFY_EXCEPTION_THROWN( i.addRecord("i1", "j1"), GAMSException );
    QCOMPARE( i.numberRecords(), numberOfRecords );

    // when
    GAMSSymbol f = db.getParameter("f");
    numberOfRecords = f.numberRecords();
    QVERIFY_EXCEPTION_THROWN( f.addRecord("f1"), GAMSException );
    QVERIFY_EXCEPTION_THROWN( f.addRecord("f1", "f2"), GAMSException );
    QCOMPARE( f.numberRecords(), numberOfRecords );

    // when, then
    GAMSSymbol demand =  db.getEquation("demand");
    numberOfRecords = demand.numberRecords();
    QVERIFY_EXCEPTION_THROWN( demand.addRecord("seattle", "Albuquerque"), GAMSException );
    QCOMPARE( demand.numberRecords(), numberOfRecords );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    numberOfRecords = x.numberRecords();
    QVERIFY_EXCEPTION_THROWN( x.addRecord("Albuquerque"), GAMSException );
    QCOMPARE( x.numberRecords(), numberOfRecords );
}

void TestGAMSSymbol::testDeleteRecord_Set_InvalidKeys() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );

    // when, then
    GAMSSymbol i = db.getSet("i");
    std::vector<std::string> plant = { "Albuquerque"  };
    QVERIFY_EXCEPTION_THROWN( i.deleteRecord(plant), GAMSException);
    QCOMPARE( i.numberRecords(), 2 );
}

void TestGAMSSymbol::testDeleteRecord_Parameter_InvalidKeys() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Parameter_demand_b( db );
    TestGAMSObject::getTestData_Parameter_distance_d( db );

    //when, then
    GAMSSymbol b = db.getParameter("b");
    std::vector<std::string> market = { "Albuquerque"  };
    QVERIFY_EXCEPTION_THROWN( b.deleteRecord( market ), GAMSException);
    QCOMPARE( b.numberRecords(), 3 );

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    std::vector<std::string> distance = { "san-diego", "Albuquerque"  };
    QVERIFY_EXCEPTION_THROWN( d.deleteRecord( distance ), GAMSException);
    QCOMPARE( d.numberRecords(), 6 );

}

void TestGAMSSymbol::testDeleteRecord_InvalidKeys() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    std::vector<std::string> supply_plants = { "seattle", "Albuquerque" };
    QVERIFY_EXCEPTION_THROWN( supply.deleteRecord( supply_plants ), GAMSException);
    QCOMPARE( supply.numberRecords(), 2 );

    // when, then
    GAMSSymbol x  = db.getVariable("x");
    std::vector<std::string> shipment = { "Albuquerque"  };
    QVERIFY_EXCEPTION_THROWN( x.deleteRecord( shipment ), GAMSException);
    QCOMPARE( x.numberRecords(), 6 );
}

void TestGAMSSymbol::testDeleteRecord_IncorrectDimension() {
        // given
        GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
        GAMSWorkspace ws(wsInfo);
        GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
        job.run();
        GAMSDatabase db = job.outDB();

        // when, then
        GAMSSymbol i = db.getSet("i");
        std::vector<std::string> plant = { };
        QVERIFY_EXCEPTION_THROWN( i.deleteRecord(plant), GAMSException);
        QCOMPARE( i.numberRecords(), 2 );

        // when, then
        GAMSSymbol b = db.getParameter("b");
        std::vector<std::string> market = { "san-diego", "chicago" };
        QVERIFY_EXCEPTION_THROWN( b.deleteRecord( market ), GAMSException);
        QCOMPARE( b.numberRecords(), 3 );

        // when, then
        GAMSSymbol d =  db.getParameter("d");
        std::vector<std::string> distance = { "san-diego", "chicago", "Albuquerque" };
        QVERIFY_EXCEPTION_THROWN( d.deleteRecord( distance ), GAMSException);
        QCOMPARE( d.numberRecords(), 6 );

        // when, then
        GAMSSymbol f = db.getParameter("f");
        std::vector<std::string> freight_cost = { "chicago" };
        QVERIFY_EXCEPTION_THROWN( f.deleteRecord( freight_cost ), GAMSException);
        QCOMPARE( f.numberRecords(), 1 );

        // when, then
        GAMSSymbol supply = db.getEquation("supply");
        std::vector<std::string> supply_plants = { "seattle", "topeka" };
        QVERIFY_EXCEPTION_THROWN( supply.deleteRecord( supply_plants ), GAMSException);
        QCOMPARE( supply.numberRecords(), 2 );

        // when, then
        GAMSSymbol x  = db.getVariable("x");
        std::vector<std::string> shipment = { "san-diego" };
        QVERIFY_EXCEPTION_THROWN( x.deleteRecord( shipment ), GAMSException);
        QCOMPARE( x.numberRecords(), 6 );
}

void TestGAMSSymbol::testDeleteRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    std::vector<std::string> plant = { "San-Diego" };
    i.deleteRecord(plant);
    QCOMPARE( i.numberRecords(), 1 );
    QVERIFY_EXCEPTION_THROWN( i.findRecord( plant ), GAMSException);

    // when, then
    GAMSSymbol b = db.getParameter("b");
    std::vector<std::string> market = { "chicago" };
    b.deleteRecord( market );
    QCOMPARE( b.numberRecords(), 2 );
    QVERIFY_EXCEPTION_THROWN( b.findRecord( market ), GAMSException);

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    std::vector<std::string> distance = { "san-diego", "chicago" };
    d.deleteRecord( distance );
    QCOMPARE( d.numberRecords(), 5 );
    QVERIFY_EXCEPTION_THROWN( d.findRecord( distance ), GAMSException);

    // when, then
    GAMSSymbol f = db.getParameter("f");
    std::vector<std::string> freight_cost = { };
    f.deleteRecord( freight_cost );
    QCOMPARE( f.numberRecords(), 0 );
    QVERIFY_EXCEPTION_THROWN( f.findRecord( freight_cost ), GAMSException);

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    std::vector<std::string> supply_plants = { "seattle" };
    supply.deleteRecord( supply_plants );
    QCOMPARE( supply.numberRecords(), 1 );
    QVERIFY_EXCEPTION_THROWN( supply.findRecord( supply_plants ), GAMSException);

    // when, then
    GAMSSymbol x  = db.getVariable("x");
    std::vector<std::string> shipment = { "san-diego", "chicago" };
    x.deleteRecord( shipment );
    QCOMPARE( x.numberRecords(), 5 );
    QVERIFY_EXCEPTION_THROWN( x.findRecord( shipment ), GAMSException);
}

void TestGAMSSymbol::testClear() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i ( db.getSet("i") );
    i.clear();
    QCOMPARE( i.numberRecords(), 0 );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    j.clear();
    QCOMPARE( j.numberRecords(), 0 );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    a.clear();
    QCOMPARE( a.numberRecords(), 0 );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    b.clear();
    QCOMPARE( b.numberRecords(), 0 );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    d.clear();
    QCOMPARE( d.numberRecords(), 0 );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    f.clear();
    QCOMPARE( f.numberRecords(), 0 );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    c.clear();
    QCOMPARE( c.numberRecords(), 0 );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    cost.clear();
    QCOMPARE( cost.numberRecords(), 0 );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    supply.clear();
    QCOMPARE( supply.numberRecords(), 0 );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    demand.clear();
    QCOMPARE( demand.numberRecords(), 0 );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    x.clear();
    QCOMPARE( x.numberRecords(), 0 );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    z.clear();
    QCOMPARE( z.numberRecords(), 0 );
}

void TestGAMSSymbol::testGetDomains_set() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);

    GAMSDatabase db = ws.addDatabase();

    GAMSSet i = db.addSet("i", 1, "canning plants");
    QCOMPARE(i.domains().size(), size_t(1)); // there is always the universe ("*") domain.

    for(GAMSDomain dom : i.domains()) {
        QVERIFY( dom.isValid() );
        QVERIFY_EXCEPTION_THROWN( dom.getSet().name(), GAMSException);
    }

    GAMSSet ii = db.addSet("ii", "local plants", i);
    std::map<std::string, bool> domainMap;
    for(GAMSDomain dom : ii.domains()) {
        domainMap[dom.getSet().name()] = dom.isRelaxed();
        QVERIFY( dom.isValid() );
    }
    // then
    QCOMPARE( domainMap.size(), size_t(1) );
    QCOMPARE( domainMap["i"], false );
    domainMap.clear();
}

void TestGAMSSymbol::testGetDomains() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();

    GAMSDatabase db = job.outDB();

    std::map<std::string, bool> domainMap;

    GAMSSet i = db.getSet("i");
    GAMSSet ii = db.addSet("ii",  "canning plants", i);
    for(GAMSDomain dom : ii.domains()) {
        domainMap[dom.getSet().name()] = dom.isRelaxed();
        QVERIFY( dom.isValid() );
    }
    // then
    QCOMPARE( domainMap.size(), size_t(1) );
    QCOMPARE( domainMap["i"], false );
    domainMap.clear();

    GAMSParameter d = db.getParameter("d");
    for(GAMSDomain dom : d.domains()) {
        domainMap[dom.getSet().name()] = dom.isRelaxed();
        QVERIFY( dom.isValid() );
    }
    // then
    QCOMPARE( domainMap.size(), size_t(2) );
    QCOMPARE( domainMap["i"], false );
    QCOMPARE( domainMap["j"], false );
    domainMap.clear();

    GAMSParameter f = db.getParameter("f");
    for(GAMSDomain dom : f.domains()) {
       QVERIFY(false);
    }

    GAMSVariable x = db.getVariable("x");
    for(GAMSDomain dom : x.domains()) {
        domainMap[dom.getSet().name()] = dom.isRelaxed();
        QVERIFY( dom.isValid() );
    }
    // then
    QCOMPARE( domainMap.size(), size_t(2) );
    QCOMPARE( domainMap["i"], false );
    QCOMPARE( domainMap["j"], false );
    domainMap.clear();

    GAMSEquation supply = db.getEquation("supply");
    for(GAMSDomain dom : supply.domains()) {
        domainMap[dom.getSet().name()] = dom.isRelaxed();
        QVERIFY( dom.isValid() );
    }
    // then
    QCOMPARE( domainMap.size(), size_t(1) );
    QCOMPARE( domainMap["i"], false );
    domainMap.clear();

    GAMSVariable z = db.getVariable("z");
    for(GAMSDomain dom : z.domains()) {
       QVERIFY(false);
    }
}

void TestGAMSSymbol::testCheckDomains() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    TestGAMSObject::getTestData_Set_markets_j( db );

    { // strong domain
      GAMSParameter a = db.addParameter("a", "capacity of plant i in cases", GAMSDomain(db.getSet("i")));
      a.addRecord("Seattle").setValue(350.0);
      a.addRecord("San-Diego").setValue(600.0);
      // when, then
      QVERIFY( a.checkDomains() );
      QCOMPARE( a.getSymbolDVs(0).size(), size_t(0));

      a.addRecord("Alburqurque").setValue(123.45);
      // when, then
      QVERIFY( ! a.checkDomains() );
      QCOMPARE( a.getSymbolDVs(0).size(), size_t(1));
    }
    { // relaxed domain
      GAMSParameter b = db.addParameter("b", "demand at market j in cases", GAMSDomain("j"));
      b.addRecord("New-York").setValue(325.0);
      // when, then
      QVERIFY( b.checkDomains() );
      QCOMPARE( b.getSymbolDVs(0).size(), size_t(0));

      b.addRecord("Alburqurque").setValue(123.45);
      // when, then
      QVERIFY(  b.checkDomains() );
      QCOMPARE( b.getSymbolDVs(0).size(), size_t(0));
    }
    { // no domain info
      GAMSParameter d = db.addParameter("c", 2, "demand at market j in cases");
      d.addRecord("Alburqurque","New-York").setValue(123.45);

      // when, then
      QVERIFY( d.checkDomains() );
      QCOMPARE( d.getSymbolDVs(0).size(), size_t(0));
    }
}



void TestGAMSSymbol::testGetSymbolDVs() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

   {  GAMSParameter a = db.getParameter("a");
      a.addRecord("Alburquerque").setValue(123.45);
      a.addRecord("SantaFe").setValue(123.45);
      QVERIFY( ! a.checkDomains() );
      // when, then
      QCOMPARE( a.getSymbolDVs(0).size(), size_t(2) );
      std::map<std::string, size_t> recordCounter;
      for(GAMSSymbolDomainViolation dom : a.getSymbolDVs(0)) {
          QVERIFY( dom.isValid() );
          recordCounter[dom.violRec().key(0)]++;
          QCOMPARE( dom.violInd().size(), size_t(1) );
          QVERIFY( dom.violInd().at(0) );
      }
      QCOMPARE( recordCounter.size(), size_t(2) );
      QCOMPARE( recordCounter["Alburquerque"], size_t(1) );
      QCOMPARE( recordCounter["SantaFe"], size_t(1) );
      recordCounter.clear();

      a.addRecord("SanFrancisco").setValue(123.45);

      // when, then
      QCOMPARE( a.getSymbolDVs(0).size(), size_t(3) );
      for(GAMSSymbolDomainViolation dom : a.getSymbolDVs(2)) {
          QVERIFY( dom.isValid() );
          QCOMPARE( dom.violRec().type(), GAMSEnum::SymbolType::SymTypePar );
          recordCounter[dom.violRec().key(0)]++;
          QCOMPARE( dom.violInd().size(), size_t(1) );
          QVERIFY( dom.violInd().at(0) );
      }
      QCOMPARE( recordCounter.size(), size_t(2) );
      QCOMPARE( recordCounter["Alburquerque"], size_t(1) );
      QCOMPARE( recordCounter["SantaFe"], size_t(1) );
    }
    {  GAMSEquation supply = db.getEquation("supply");
       supply.addRecord("Alburquerque").setMarginal(123.45);
       supply.addRecord("Florida").setScale(1.2345);

       // when, then
       QCOMPARE( supply.getSymbolDVs(0).size(), size_t(2) );
       std::map<std::string, size_t> recordCounter;
       for(GAMSSymbolDomainViolation dom : supply.getSymbolDVs(5)) {
           QVERIFY( dom.isValid() );
           QCOMPARE( dom.violRec().type(), GAMSEnum::SymbolType::SymTypeEqu );
           recordCounter[dom.violRec().key(0)]++;
           QCOMPARE( dom.violInd().size(), size_t(1) );
           QVERIFY( dom.violInd().at(0) );
       }
       QCOMPARE( recordCounter.size(), size_t(2) );
       QCOMPARE( recordCounter["Alburquerque"], size_t(1) );
       QCOMPARE( recordCounter["Florida"], size_t(1) );
       recordCounter.clear();
    }
    {  GAMSVariable x = db.getVariable("x");
       x.addRecord("seattle", "Alburquerque").setMarginal(123.45);
       x.addRecord("Alburquerque", "topeka").setMarginal(54.321);

       // when, then
       QCOMPARE( x.getSymbolDVs(0).size(), size_t(2) );
       std::map<std::string, int> recordMap;
       for(GAMSSymbolDomainViolation dom : x.getSymbolDVs(5)) {
           QVERIFY( dom.isValid() );
           QCOMPARE( dom.violRec().type(), GAMSEnum::SymbolType::SymTypeVar );

           if (dom.violRec().key(0).compare("seattle")==0 ) {
               QVERIFY( dom.violRec().key(1).compare("Alburquerque")==0 );
               std::vector<bool> vi = dom.violInd();
               QCOMPARE( vi.size(), size_t(2) );
               QVERIFY( ! vi[0] );
               QVERIFY( vi[1] );
           } else if (dom.violRec().key(0).compare("Alburquerque")==0 ) {
               QVERIFY(dom.violRec().key(1).compare("topeka")==0);
               std::vector<bool> vi = dom.violInd();
               QCOMPARE( vi.size(), size_t(2) );
               QVERIFY( vi[0] );
               QVERIFY( ! vi[1] );
           }
           std::stringstream ss;
           ss << dom.violRec().key(0) << "_" << dom.violRec().key(1) ;
           recordMap[ss.str()]++;

       }
       QCOMPARE( recordMap.size(), size_t(2) );
       QVERIFY( equals( recordMap["seattle_Alburquerque"], size_t(1)) );
       QVERIFY( equals( recordMap["Alburquerque_topeka"], size_t(1)) );
    }
}

void TestGAMSSymbol::testGetFirstRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    QCOMPARE( i.firstRecord().key(0).c_str(), "seattle" );

    // when, then
    GAMSSymbol b( db.getParameter("b") );
    QCOMPARE( b.firstRecord().key(0).c_str(), "new-york");

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QCOMPARE( d.firstRecord().key(1).c_str(), "new-york");
    QCOMPARE( d.firstRecord().key(0).c_str(), "seattle" );

    // when, then
    GAMSSymbol f( db.getParameter("f") );
    QVERIFY( f.firstRecord().isValid() );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QCOMPARE( supply.firstRecord().key(0).c_str(), "seattle" );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QCOMPARE( x.firstRecord().key(1).c_str(), "new-york");
    QCOMPARE( x.firstRecord().key(0).c_str(), "seattle" );
}

void TestGAMSSymbol::testGetFirstRecordSlice() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol j = db.getSet("j");
    QCOMPARE( j.firstRecord("chicago").key(0).c_str(), "chicago" );

    // when, then
    GAMSSymbol b( db.getParameter("b") );
    QCOMPARE( b.firstRecord("topeka").key(0).c_str(), "topeka");

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QCOMPARE( d.firstRecord("seattle", "topeka").key(0).c_str(), "seattle" );
    QCOMPARE( d.firstRecord("seattle", "topeka").key(1).c_str(), "topeka");

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QCOMPARE( supply.firstRecord("san-diego").key(0).c_str(), "san-diego" );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QCOMPARE( x.firstRecord("san-diego", "new-york").key(0).c_str(), "san-diego" );
    QCOMPARE( x.firstRecord("san-diego", "new-york").key(1).c_str(), "new-york");
}

void TestGAMSSymbol::testGetFirstRecordSlice_InvalidKeys() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol j = db.getSet("j");
    QVERIFY_EXCEPTION_THROWN( j.firstRecord("Albuquerque"), GAMSException );

    // when, then
    GAMSSymbol a( db.getParameter("a") );
    QVERIFY_EXCEPTION_THROWN( a.firstRecord("Boston"), GAMSException );

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QVERIFY_EXCEPTION_THROWN( d.firstRecord("seattle", "Omaha"), GAMSException );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QVERIFY_EXCEPTION_THROWN( supply.firstRecord("Atlanta"), GAMSException );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QVERIFY_EXCEPTION_THROWN( x.firstRecord("seattle", "Dallas"), GAMSException );
}

void TestGAMSSymbol::testGetFirstRecordSlice_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol j = db.getSet("j");
    QVERIFY_EXCEPTION_THROWN( j.firstRecord("chicago", "seattle"), GAMSException );

    // when, then
    GAMSSymbol a( db.getParameter("a") );
    QVERIFY_EXCEPTION_THROWN( a.firstRecord("seattle", "chicago"), GAMSException );

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QVERIFY_EXCEPTION_THROWN( d.firstRecord("chicago", "seattle", "new-york"), GAMSException );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QVERIFY_EXCEPTION_THROWN( supply.firstRecord("chicago"), GAMSException );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QVERIFY_EXCEPTION_THROWN( x.firstRecord("seattle"), GAMSException );
}

void TestGAMSSymbol::testGetLastRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    QCOMPARE( i.lastRecord().key(0).c_str(), "san-diego" );

    // when, then
    GAMSSymbol b( db.getParameter("b") );
    QCOMPARE( b.lastRecord().key(0).c_str(), "topeka");

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QCOMPARE( d.lastRecord().key(1).c_str(), "topeka");
    QCOMPARE( d.lastRecord().key(0).c_str(), "san-diego" );

    // when, then
    GAMSSymbol f( db.getParameter("f") );
    QVERIFY( f.lastRecord().isValid() );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QCOMPARE( supply.lastRecord().key(0).c_str(), "san-diego" );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QCOMPARE( x.lastRecord().key(1).c_str(), "topeka");
    QCOMPARE( x.lastRecord().key(0).c_str(), "san-diego" );
}

void TestGAMSSymbol::testGetLastRecordSlice() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol j = db.getSet("j");
    QCOMPARE( j.lastRecord("chicago").key(0).c_str(), "chicago" );

    // when, then
    GAMSSymbol a = db.getParameter("b");
    QCOMPARE( a.lastRecord("new-york").key(0).c_str(), "new-york");

    // when, then
    GAMSSymbol d = db.getParameter("d");
    QCOMPARE( d.lastRecord("san-diego", "chicago").key(1).c_str(), "chicago" );
    QCOMPARE( d.lastRecord("san-diego", "chicago").key(0).c_str(), "san-diego" );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    QCOMPARE( supply.lastRecord("seattle").key(0).c_str(), "seattle" );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    QCOMPARE( x.lastRecord("seattle", "topeka").key(1).c_str(), "topeka");
    QCOMPARE( x.lastRecord("seattle", "topeka").key(0).c_str(), "seattle" );
}

void TestGAMSSymbol::testGetLastRecordSlice_InValidKeys() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    QVERIFY_EXCEPTION_THROWN( i.lastRecord("Albuquerque"), GAMSException );

    // when, then
    GAMSSymbol b = db.getParameter("b");
    QVERIFY_EXCEPTION_THROWN( b.lastRecord("Boston"), GAMSException );

    // when, then
    GAMSSymbol d = db.getParameter("d");
    QVERIFY_EXCEPTION_THROWN( d.lastRecord("seattle", "Omaha"), GAMSException );

    // when, then
    GAMSSymbol demand = db.getEquation("demand");
    QVERIFY_EXCEPTION_THROWN( demand.lastRecord("Atlanta"), GAMSException );

    // when, then
    GAMSSymbol x =  db.getVariable("x");
    QVERIFY_EXCEPTION_THROWN( x.lastRecord("seattle", "Dallas"), GAMSException );
}

void TestGAMSSymbol::testGetLastRecordSlice_IncorrectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    QVERIFY_EXCEPTION_THROWN( i.lastRecord("chicago", "seattle"), GAMSException );

    // when, then
    GAMSSymbol b( db.getParameter("b") );
    QVERIFY_EXCEPTION_THROWN( b.firstRecord("seattle", "chicago"), GAMSException );

    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QVERIFY_EXCEPTION_THROWN( d.lastRecord("chicago"), GAMSException );

    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QVERIFY_EXCEPTION_THROWN( supply.lastRecord("chicago"), GAMSException );

    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QVERIFY_EXCEPTION_THROWN( x.lastRecord("seattle"), GAMSException );
}

void TestGAMSSymbol::testFindRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    QVERIFY( i.findRecord("san-diego").isValid() );
    QCOMPARE( i.findRecord("san-diego").key(0).c_str(), "san-diego");

    // when, then
    GAMSSymbol b = db.getParameter("b");
    QVERIFY( b.findRecord("chicago").isValid() );
    QCOMPARE( b.findRecord("chicago").key(0).c_str(), "chicago");

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    QVERIFY( d.findRecord("seattle", "chicago").isValid() );
    QCOMPARE( d.findRecord("seattle", "chicago").key(0).c_str(), "seattle");
    QCOMPARE( d.findRecord("seattle", "chicago").key(1).c_str(), "chicago");

    // when, then
    GAMSSymbol f = db.getParameter("f");
    QVERIFY( f.findRecord().isValid() );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    QVERIFY( supply.findRecord("seattle").isValid() );
    QCOMPARE( supply.findRecord("seattle").key(0).c_str(), "seattle");

    // when, then
    GAMSSymbol x  = db.getVariable("x");
    QVERIFY( x.findRecord("seattle", "topeka").isValid() );
    QCOMPARE( x.findRecord("seattle", "topeka").key(0).c_str(), "seattle");
    QCOMPARE( x.findRecord("seattle", "topeka").key(1).c_str(), "topeka");
}

void TestGAMSSymbol::testFindNonExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol j = db.getSet("j");
    QVERIFY_EXCEPTION_THROWN( j.findRecord("Albuquerque"), GAMSException );

    // when, then
    GAMSSymbol a = db.getParameter("a");
    QVERIFY_EXCEPTION_THROWN( a.findRecord("Albuquerque", "chicago"), GAMSException );

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    QVERIFY_EXCEPTION_THROWN( d.findRecord("Albuquerque", "chicago"), GAMSException );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    QVERIFY_EXCEPTION_THROWN( supply.findRecord("Albuquerque"), GAMSException );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    QVERIFY_EXCEPTION_THROWN( x.findRecord("Albuquerque"), GAMSException );
}

void TestGAMSSymbol::testFindRecord_IncrorectDimension() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i = db.getSet("i");
    QVERIFY_EXCEPTION_THROWN( i.findRecord("seattle", "chicago"), GAMSException );

    // when, then
    GAMSSymbol b = db.getParameter("b");
    QVERIFY_EXCEPTION_THROWN( b.findRecord("seattle", "chicago"), GAMSException );

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    QVERIFY_EXCEPTION_THROWN( d.findRecord("chicago"), GAMSException );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    QVERIFY_EXCEPTION_THROWN( supply.findRecord(), GAMSException );

    // when, then
    GAMSSymbol x = db.getVariable("x");
    QVERIFY_EXCEPTION_THROWN( x.findRecord("topeka"), GAMSException );
}

void TestGAMSSymbol::testMergeExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSet j = db.getSet("j");
    int numberOfRecords = j.numberRecords();
    GAMSSetRecord recj = j.mergeRecord("chicago");
    QCOMPARE( j.numberRecords(), numberOfRecords );
    QCOMPARE( recj.key(0).c_str(), "chicago" );

    // when, then
    GAMSParameter b = db.getParameter("b");
    numberOfRecords = b.numberRecords();
    GAMSParameterRecord recb = b.mergeRecord("topeka");
    QCOMPARE( b.numberRecords(), numberOfRecords );
    QCOMPARE( recb.key(0).c_str(), "topeka" );

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    numberOfRecords = d.numberRecords();
    GAMSSymbolRecord recd = d.mergeRecord("seattle", "chicago");
    QCOMPARE( d.numberRecords(), numberOfRecords );
    QCOMPARE( recd.key(0).c_str(), "seattle" );
    QCOMPARE( recd.key(1).c_str(), "chicago" );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    numberOfRecords = supply.numberRecords();
    GAMSSymbolRecord recs = supply.mergeRecord("seattle");
    QCOMPARE( supply.numberRecords(), numberOfRecords );
    QCOMPARE( recs.key(0).c_str(), "seattle" );

    // when, then
    GAMSSymbol x  = db.getVariable("x");
    numberOfRecords = x.numberRecords();
    GAMSSymbolRecord recx = x.mergeRecord("seattle", "topeka");
    QCOMPARE( x.numberRecords(), numberOfRecords );
    QCOMPARE( recx.key(0).c_str(), "seattle" );
    QCOMPARE( recx.key(1).c_str(), "topeka" );
}

void TestGAMSSymbol::testMergeNonExistingRecord() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSet j = db.getSet("j");
    int numberOfRecords = j.numberRecords();
    GAMSSetRecord recj = j.mergeRecord("Albuquerque");
    QCOMPARE( j.numberRecords(), numberOfRecords+1 );
    QCOMPARE( recj.key(0).c_str(), "Albuquerque" );

    // when, then
    GAMSParameter b = db.getParameter("b");
    numberOfRecords = b.numberRecords();
    GAMSParameterRecord recb = b.mergeRecord("Albuquerque");
    QCOMPARE( b.numberRecords(), numberOfRecords+1 );
    QCOMPARE( recb.key(0).c_str(), "Albuquerque" );

    // when, then
    GAMSSymbol d =  db.getParameter("d");
    numberOfRecords = d.numberRecords();
    GAMSSymbolRecord recd = d.mergeRecord("seattle", "Albuquerque");
    QCOMPARE( d.numberRecords(), numberOfRecords+1 );
    QCOMPARE( recd.key(0).c_str(), "seattle" );
    QCOMPARE( recd.key(1).c_str(), "Albuquerque" );

    // when, then
    GAMSSymbol supply = db.getEquation("supply");
    numberOfRecords = supply.numberRecords();
    GAMSSymbolRecord recs = supply.mergeRecord("Boston");
    QCOMPARE( supply.numberRecords(), numberOfRecords+1 );
    QCOMPARE( recs.key(0).c_str(), "Boston" );

    // when, then
    GAMSSymbol x  = db.getVariable("x");
    numberOfRecords = x.numberRecords();
    GAMSSymbolRecord recx = x.mergeRecord("san-diego", "Albuquerque");
    QCOMPARE( x.numberRecords(), numberOfRecords+1 );
    QCOMPARE( recx.key(0).c_str(), "san-diego" );
    QCOMPARE( recx.key(1).c_str(), "Albuquerque" );
}

void TestGAMSSymbol::testCopySymbol() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    TestGAMSObject::getTestData_Set_markets_j( db );
    GAMSSymbol i( db.getSet("i") );
    GAMSSymbol j( db.getSet("j") );
    // when
    i.copySymbol( j );
    // then
    QCOMPARE( i.numberRecords(), j.numberRecords() );
}

void TestGAMSSymbol::testGetDatabase() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    GAMSSymbol i( db.getSet("i") );
    // when, then
    QVERIFY( i.database() == db );
    QCOMPARE( i.database().getNrSymbols(), db.getNrSymbols());
}

void TestGAMSSymbol::testGetText() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i( db.getSet("i") );
    QCOMPARE( i.text().c_str(), "canning plants" );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    QCOMPARE( j.text().c_str(), "markets" );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    QCOMPARE( a.text().c_str(), "capacity of plant i in cases" );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    QCOMPARE( b.text().c_str(), "demand at market j in cases" );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QCOMPARE( d.text().c_str(), "distance in thousands of miles" );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    QCOMPARE( f.text().c_str(), "freight in dollars per case per thousand miles" );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    QCOMPARE( c.text().c_str(), "transport cost in thousands of dollars per case" );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    QCOMPARE( cost.text().c_str(), "define objective function" );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QCOMPARE( supply.text().c_str(), "observe supply limit at plant i" );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    QCOMPARE( demand.text().c_str(), "satisfy demand at market j" );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QCOMPARE( x.text().c_str(), "shipment quantities in cases" );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    QCOMPARE( z.text().c_str(), "total transportation costs in thousands of dollars" );
}

void TestGAMSSymbol::testGetName() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i( db.getSet("i") );
    QCOMPARE( i.name().c_str(), "i" );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    QCOMPARE( j.name().c_str(), "j" );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    QCOMPARE( a.name().c_str(), "a" );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    QCOMPARE( b.name().c_str(), "b" );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QCOMPARE( d.name().c_str(), "d" );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    QCOMPARE( f.name().c_str(), "f" );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    QCOMPARE( c.name().c_str(), "c" );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    QCOMPARE( cost.name().c_str(), "cost" );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QCOMPARE( supply.name().c_str(), "supply" );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    QCOMPARE( demand.name().c_str(), "demand" );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QCOMPARE( x.name().c_str(), "x" );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    QCOMPARE( z.name().c_str(), "z" );
}

void TestGAMSSymbol::testGetNumberOfRecords() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i( db.getSet("i") );
    QCOMPARE( i.numberRecords(), 2 );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    QCOMPARE( j.numberRecords(), 3 );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    QCOMPARE( a.numberRecords(), 2 );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    QCOMPARE( b.numberRecords(), 3 );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QCOMPARE( d.numberRecords(), 6 );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    QCOMPARE( f.numberRecords(), 1 );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    QCOMPARE( c.numberRecords(), 6 );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    QCOMPARE( cost.numberRecords(), 1 );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QCOMPARE( supply.numberRecords(), 2 );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    QCOMPARE( demand.numberRecords(), 3 );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QCOMPARE( x.numberRecords(), 6 );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    QCOMPARE( z.numberRecords(), 1 );
}

void TestGAMSSymbol::testGetDim() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i( db.getSet("i") );
    QCOMPARE( i.dim(), 1 );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    QCOMPARE( j.dim(), 1 );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    QCOMPARE( a.dim(), 1 );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    QCOMPARE( b.dim(), 1 );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QCOMPARE( d.dim(), 2 );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    QCOMPARE( f.dim(), 0 );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    QCOMPARE( c.dim(), 2 );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    QCOMPARE( cost.dim(), 0 );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QCOMPARE( supply.dim(), 1 );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    QCOMPARE( demand.dim(), 1 );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QCOMPARE( x.dim(), 2 );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    QCOMPARE( z.dim(), 0 );
}

void TestGAMSSymbol::testGetType() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbol i( db.getSet("i") );
    QCOMPARE( i.type(), GAMSEnum::SymbolType::SymTypeSet );
    // when, then
    GAMSSymbol j( db.getSet("j") );
    QCOMPARE( j.type(), GAMSEnum::SymbolType::SymTypeSet );
    // when, then
    GAMSSymbol a( db.getParameter("a") );
    QCOMPARE( a.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbol b( db.getParameter("b") );
    QCOMPARE( b.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbol d( db.getParameter("d") );
    QCOMPARE( d.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbol f( db.getParameter("f") );
    QCOMPARE( f.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbol c( db.getParameter("c") );
    QCOMPARE( c.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbol cost( db.getEquation("cost") );
    QCOMPARE( cost.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbol supply( db.getEquation("supply") );
    QCOMPARE( supply.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbol demand( db.getEquation("demand") );
    QCOMPARE( demand.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbol x( db.getVariable("x") );
    QCOMPARE( x.type(), GAMSEnum::SymbolType::SymTypeVar );
    // when, then
    GAMSSymbol z( db.getVariable("z") );
    QCOMPARE( z.type(), GAMSEnum::SymbolType::SymTypeVar );
}

QTEST_MAIN(TestGAMSSymbol)
