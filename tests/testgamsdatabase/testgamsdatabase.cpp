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

#include "gclgms.h"
#include "gamsenum.h"
#include "gamsdatabase.h"
#include "gamsdatabaseiter.h"
#include "gamsdatabasedomainviolation.h"
#include "gamsequation.h"
#include "gamsparameter.h"
#include "gamsset.h"
#include "gamssymboldomainviolation.h"
#include "gamsvariable.h"
#include "testgamsobject.h"
#include "gamspath.h"
#include <sstream>
#include <string>

using namespace gams;

class TestGAMSDatabase: public TestGAMSObject
{
};

TEST_F(TestGAMSDatabase, testDefaultConstructor) {
    // when
    GAMSDatabase db;
    // then
    ASSERT_TRUE(! db.isValid() );
    EXPECT_THROW( db.name(), GAMSException );
    EXPECT_THROW( db.getNrSymbols(), GAMSException );
    EXPECT_THROW( db.clear(), GAMSException );
    EXPECT_THROW( db.begin(), GAMSException );
    EXPECT_THROW( db.end(), GAMSException );
    EXPECT_THROW( db.getSet("x"), GAMSException );
    EXPECT_THROW( db.getParameter("x"), GAMSException );
    EXPECT_THROW( db.getEquation("x"), GAMSException );
    EXPECT_THROW( db.getVariable("x"), GAMSException );
    EXPECT_THROW( db.doExport(), GAMSException );
    EXPECT_THROW( db.logID(), GAMSException );
    EXPECT_THROW( db.workspace(), GAMSException );
}

TEST_F(TestGAMSDatabase, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSDatabase db1 = ws.addDatabase();
    TestGAMSDatabase::getTestData_Set_plants_i(db1);
    TestGAMSDatabase::getTestData_Parameter_distance_d(db1);
    // when
    GAMSDatabase db2 = db1;
    EXPECT_EQ( db2.getNrSymbols(), db1.getNrSymbols());
    EXPECT_EQ( db2.getSet("i").numberRecords(), 2 );
    EXPECT_EQ( db2.getParameter("d").numberRecords(), 6 );
    EXPECT_EQ( db2.logID(), db1.logID() );
}

TEST_F(TestGAMSDatabase, testOutOfScopeDatabaseAssignement) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1;
    // when assignedDatabaseFrom is out of scope
    {
         GAMSDatabase db2 = ws.addDatabase();
         TestGAMSObject::getTestData_Set_plants_i( db2 );
         TestGAMSObject::getTestData_Parameter_capacity_a( db2 );
         EXPECT_EQ( db2.getNrSymbols(), 2 );
         db1 = db2;
    }
    // then
    ASSERT_TRUE( db1.isValid() );
    EXPECT_EQ( db1.getNrSymbols(), 2 );
    EXPECT_EQ( db1.getSet("i").numberRecords(), 2 );
    EXPECT_EQ( db1.getParameter("a").numberRecords(), 2 );
    EXPECT_THROW( db1.getSet( "x" ), GAMSException );
}

TEST_F(TestGAMSDatabase, testIsValid) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    GAMSDatabase db;
    ASSERT_TRUE( ! db.isValid() );
    // when, then
    db = ws.addDatabase();
    ASSERT_TRUE( db.isValid() );
}

TEST_F(TestGAMSDatabase, testBegin) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    ASSERT_TRUE( db.isValid() );
    GAMSDatabaseIter it = db.begin();
    EXPECT_STREQ((*it).name().c_str(), "i");
}

TEST_F(TestGAMSDatabase, testEnd) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    GAMSDatabaseIter it = db.begin();
    GAMSDatabaseIter it_end = db.end();
    for(int i=0; i<db.getNrSymbols(); i++) {
        ++it;
    }
    ASSERT_TRUE( it == it_end );
}

TEST_F(TestGAMSDatabase, testLogID) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    GAMSDatabase db2 = ws.addDatabase();

    // when, then
    EXPECT_EQ( db1.logID(), db2.logID() );

    GAMSWorkspaceInfo anotherWsInfo("", testSystemDir);
    GAMSWorkspace anotherws(anotherWsInfo);
    GAMSDatabase db3 = anotherws.addDatabase();
    ASSERT_TRUE( db3.logID() != db2.logID());
}

TEST_F(TestGAMSDatabase, testGetNrSymbols) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    GAMSDatabase db = ws.addDatabase();
    EXPECT_EQ( db.getNrSymbols(), 0 );
    // when, then
    TestGAMSObject::getTestData_Set_markets_j(db);
    EXPECT_EQ( db.getNrSymbols(), 1 );
    // when, then
    TestGAMSObject::getTestData_Set_plants_i(db);
    EXPECT_EQ( db.getNrSymbols(), 2 );
    // when, then
    TestGAMSObject::getTestData_Parameter_freightcost_f(db);
    EXPECT_EQ( db.getNrSymbols(), 3 );
}

TEST_F(TestGAMSDatabase, testGetSuppressAutoDomainChecking) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    db.setSuppressAutoDomainChecking( true );
    // then
    EXPECT_EQ( db.suppressAutoDomainChecking() , true );

    // when
    db.setSuppressAutoDomainChecking( false );
    // then
    EXPECT_EQ( db.suppressAutoDomainChecking() , false );
}

TEST_F(TestGAMSDatabase, testGetName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    // when, then
    ASSERT_FALSE( db1.name().empty() );

    // when, then
    std::string dbname = "myDatabase";
    GAMSDatabase db2 = ws.addDatabase(dbname);
    EXPECT_EQ( db2.name(), dbname );
}

TEST_F(TestGAMSDatabase, testGetWorkspace) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_EQ( db.workspace(), ws );
}

TEST_F(TestGAMSDatabase, testDoExport) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i( db );
    db.doExport();
    // then
    GAMSPath gdxfile(ws.workingDirectory(), db.name() + ".gdx");
    ASSERT_TRUE( gdxfile.exists() );
    // TODO: more check on gdxfile
}

TEST_F(TestGAMSDatabase, testDoExportWithName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSDatabase db = ws.addDatabase("capacity");
    TestGAMSObject::getTestData_Parameter_capacity_a( db );
    db.doExport("capacity");
    // then
    GAMSPath gdxfile(ws.workingDirectory(), db.name() + ".gdx");
    ASSERT_TRUE( gdxfile.exists() );
    // TODO: more check on gdxfile
}

TEST_F(TestGAMSDatabase, testDoExportWithDomainViolation) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase("domain");
    GAMSSet i = db.addSet("i", "");
    i.addRecord("i1");
    GAMSParameter a = db.addParameter("x", "", i);
    a.addRecord("i1").setValue( 0.5 );

    // when
    a.addRecord("i2").setValue( 0.5 );
    // then
    EXPECT_THROW( db.doExport("domain.gdx"), GAMSException );
}

TEST_F(TestGAMSDatabase, testCheckDomains) {
    // when
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    // when, then
    GAMSSet i = db.addSet("i", "");
    ASSERT_TRUE( db.checkDomains() );

    // when, then
    i.addRecord("i1");
    GAMSParameter a = db.addParameter("x", "", i);
    a.addRecord("i1").setValue( 0.5 ); ;
    ASSERT_TRUE( db.checkDomains() );

    // when, then
    a.addRecord("i2").setValue( 0.5 ); ;
    ASSERT_TRUE( ! db.checkDomains() );
}

TEST_F(TestGAMSDatabase, testAddSet) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    db.addSet("j", 1, "markets");
    // then
    EXPECT_STREQ( db.getSet("j").name().c_str(), "j" );
    EXPECT_EQ( db.getSet("j").dim(), 1 );
    EXPECT_STREQ( db.getSet("j").text().c_str(), "markets" );
    EXPECT_THROW( db.addSet("j", 2), GAMSException);
}

TEST_F(TestGAMSDatabase, testAddSet_NonPositiveDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addSet("i", 0, "set i"), GAMSException);
    EXPECT_THROW( db.addSet("i", -1, "set i"), GAMSException);
}

TEST_F(TestGAMSDatabase, testAddSet_ExceedingMaxDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addSet("i", GLOBAL_MAX_INDEX_DIM + 1, "set i"), GAMSException);

    // when
    int max_dim = GLOBAL_MAX_INDEX_DIM;
    GAMSSet max = db.addSet("max", max_dim, "set with maximum dimension");
    // then
    EXPECT_EQ( max.dim(), max_dim ) ;
}

TEST_F(TestGAMSDatabase, testAddSet_DuplicateIdentifier) {
     GAMSWorkspaceInfo wsInfo("", testSystemDir);
     GAMSWorkspace ws(wsInfo);

     GAMSDatabase db = ws.addDatabase();
     GAMSSet i = db.addSet("i", 1, "canning plants");

     EXPECT_THROW( db.addSet(i.name(), i.dim(),  i.text()), GAMSException);
     EXPECT_THROW( db.addSet(i.name(), 2, i.text()), GAMSException);
     EXPECT_THROW( db.addSet(i.name(), i.dim(),  "new canning plants"), GAMSException);
 }

TEST_F(TestGAMSDatabase, testAddSet_InvalidStringIdentifier) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addSet("", 1, "empty set"), GAMSException);
    EXPECT_THROW( db.addSet("*", 1, "star/univerise"), GAMSException);
    EXPECT_THROW( db.addSet("i j", 1, "white space"), GAMSException);
    EXPECT_THROW( db.addSet("i*j", 1, "star/universe"), GAMSException);
    EXPECT_THROW( db.addSet("n!", 1, "exclamation marks"), GAMSException);
    EXPECT_THROW( db.addSet("n/", 1, "slash"), GAMSException);
}

TEST_F(TestGAMSDatabase, testAddParameter) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    db.addParameter("b", 1, "demand at market j in cases");
    // then
    EXPECT_STREQ( db.getParameter("b").name().c_str(), "b" );
    EXPECT_EQ( db.getParameter("b").dim(), 1 );
    EXPECT_STREQ( db.getParameter("b").text().c_str(),"demand at market j in cases" );
}

TEST_F(TestGAMSDatabase, testAddParameter_NegativeDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addParameter("p", -1, "parameter p"), GAMSException);

    // when
    GAMSParameter p = db.addParameter("p", 0, "parameter p");
    // then
    EXPECT_EQ( p.dim(), 0 ) ;
}

TEST_F(TestGAMSDatabase, testAddParameter_ExceedingMaxDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addParameter("p2", GLOBAL_MAX_INDEX_DIM + 1, "parameter p2"), GAMSException);

    // when
    int max_dim = GLOBAL_MAX_INDEX_DIM;
    GAMSParameter max = db.addParameter("max", max_dim, "parameter with maximum dimension");
    EXPECT_EQ( max.dim(), max_dim );
}

TEST_F(TestGAMSDatabase, testAddParameter_DuplicateIdentifier) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    db.addParameter("p", 1, "parameter p");
    // when, then
    EXPECT_THROW( db.addParameter("p", 2, "parameter p1"), GAMSException);
}

TEST_F(TestGAMSDatabase, testAddParameter_InvalidStringIdentifier) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();

    // when, then
    EXPECT_THROW( db.addParameter("", 1, "empty string"), GAMSException);
    EXPECT_THROW( db.addParameter("*", 1, "star/univerise"), GAMSException);
    EXPECT_THROW( db.addParameter("i*j", 1, "star/univerise"), GAMSException);
    EXPECT_THROW( db.addParameter("n!", 1, "exclamation mark"), GAMSException);
    EXPECT_THROW( db.addParameter("n/", 1, "slash"), GAMSException);
}

TEST_F(TestGAMSDatabase, testAddVariable) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    db.addVariable("x", 2, GAMSEnum::VarType::Positive, "shipment quantities in cases");
    // then
    EXPECT_STREQ( db.getVariable("x").name().c_str(), "x" );
    EXPECT_EQ( db.getVariable("x").dim(), 2 );
    EXPECT_EQ( db.getVariable("x").varType(),  GAMSEnum::VarType::Positive);
    EXPECT_STREQ( db.getVariable("x").text().c_str(),"shipment quantities in cases" );

    // when
    int max_dimension = GLOBAL_MAX_INDEX_DIM;
    db.addVariable("y", max_dimension, GAMSEnum::VarType::Positive, "variable y");
    EXPECT_EQ( db.getVariable("y").dim(), max_dimension );
}

TEST_F(TestGAMSDatabase, testAddVariable_NegativeDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addVariable("x",  -1, GAMSEnum::VarType::Positive, "variable x"), GAMSException);

    // when
    GAMSVariable x = db.addVariable("x",  0, GAMSEnum::VarType::Positive, "variable x");
    // then
    EXPECT_EQ( x.dim(), 0 ) ;
}

TEST_F(TestGAMSDatabase, testAddVariable_ExceedingMaxDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addVariable("x",  GLOBAL_MAX_INDEX_DIM + 1, GAMSEnum::VarType::Positive, "variable x")
                              , GAMSException);

    // when
    int max_dim = GLOBAL_MAX_INDEX_DIM;
    GAMSVariable max = db.addVariable("max", max_dim, GAMSEnum::VarType::Positive,"variable with maximum dimension");
    EXPECT_EQ( max.dim(), max_dim ) ;
}

TEST_F(TestGAMSDatabase, testAddVariable_DuplicateIdentifier) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    db.addVariable("x", 1,  GAMSEnum::VarType::Positive, "variable x");
    // when, then
    EXPECT_THROW( db.addVariable("x", 2, GAMSEnum::VarType::Free, "new variable x"), GAMSException);
}

TEST_F(TestGAMSDatabase, testAddVariable_InvalidStringIdentifier) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addVariable("", 1, GAMSEnum::VarType::Free, "empty string"), GAMSException);
    EXPECT_THROW( db.addVariable("*", 1, GAMSEnum::VarType::Free, "whitespace"), GAMSException);
    EXPECT_THROW( db.addVariable("i j", 1, GAMSEnum::VarType::Free, "variable x"), GAMSException);
    EXPECT_THROW( db.addVariable("n!", 1, GAMSEnum::VarType::Free, "exclamation mark"), GAMSException);
    EXPECT_THROW( db.addVariable("n/", 1, GAMSEnum::VarType::Free, "slash"), GAMSException);
}

TEST_F(TestGAMSDatabase, testAddEquation) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    auto equation = db.addEquation("s", 1, GAMSEnum::EquType::L, "observe supply limit at plant i");
    // then
    EXPECT_EQ( db.getNrSymbols(), 1);
    try {
        EXPECT_STREQ(equation.name().c_str(), "s");
        EXPECT_EQ(equation.dim(), 1);
        EXPECT_EQ(equation.equType(), GAMSEnum::EquType::L);
        EXPECT_STREQ(equation.text().c_str(), "observe supply limit at plant i");
    } catch(GAMSException& e) {
        FAIL() << "Unexpected GAMSException raised by: " << e.what();
    }
}

TEST_F(TestGAMSDatabase, testGetEquation) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when
    auto initEquation = db.addEquation("s", 1, GAMSEnum::EquType::L, "observe supply limit at plant i");
    // then
    EXPECT_EQ( db.getNrSymbols(), 1);
    try {
        auto getEquation = db.getEquation("s");
        EXPECT_EQ(initEquation, getEquation);
    } catch(GAMSException& e) {
        FAIL() << "Unexpected GAMSException raised by: " << e.what();
    }
}

TEST_F(TestGAMSDatabase, testAddEquation_NegativeDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addEquation("eq",  -1, GAMSEnum::EquType::E, "equation eq")
                              , GAMSException);

    // when
    GAMSEquation eq = db.addEquation("eq", 0, GAMSEnum::EquType::E, "equation eq");
    // then
    EXPECT_EQ( eq.dim(), 0 ) ;
}

TEST_F(TestGAMSDatabase, testAddEquation_ExceedingMaxDimension) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addEquation("eq",  GLOBAL_MAX_INDEX_DIM+1, GAMSEnum::EquType::E, "equation eq")
                              , GAMSException);
    // when
    int max_dim = GLOBAL_MAX_INDEX_DIM;
    GAMSEquation max = db.addEquation("max", max_dim, GAMSEnum::EquType::E, "equation with maximum dimension");
    EXPECT_EQ( max.dim(), max_dim ) ;
}

TEST_F(TestGAMSDatabase, testAddEquation_DuplicateIdentifier) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    db.addEquation("s", 1, GAMSEnum::EquType::L, "observe supply limit at plant i");
    // when, then
    EXPECT_THROW( db.addEquation("s", 2, GAMSEnum::EquType::E, "new equation s"), GAMSException);
}

TEST_F(TestGAMSDatabase, testAddEquation_InvalidStringIdentifier) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    EXPECT_THROW( db.addEquation("", 1, GAMSEnum::EquType::E, "new equation"), GAMSException);
    EXPECT_THROW( db.addEquation("*", 1, GAMSEnum::EquType::L, "whitespace"), GAMSException);
    EXPECT_THROW( db.addEquation("i j", 1, GAMSEnum::EquType::G, "variable x"), GAMSException);
    EXPECT_THROW( db.addEquation("n!", 1, GAMSEnum::EquType::E, "exclamation mark"), GAMSException);
    EXPECT_THROW( db.addEquation("n/", 1, GAMSEnum::EquType::L, "slash"), GAMSException);
}

TEST_F(TestGAMSDatabase, testIterator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    // when, then
    for (GAMSSymbol symbol : db) {
        FAIL() << "unexpected symbol from empty database";
        symbol.name();
    }

    // given
    TestGAMSObject::getTestData_TransportModel(db);
    // when
    std::map<std::string, int> symbolCounter;
    for (GAMSSymbol sym : db) {
        switch(sym.type()) {
           case GAMSEnum::SymbolType::SymTypeSet :
               symbolCounter["GAMSSet"]++;
               break;
           case GAMSEnum::SymbolType::SymTypePar :
               symbolCounter["GAMSParameter"]++;
               break;
           default:
               FAIL() << "unexpected symbol type of a symbol from transport database";
               break;
       }
    }
    // then
    EXPECT_EQ( symbolCounter["GAMSSet"], 2 );
    EXPECT_EQ( symbolCounter["GAMSParameter"], 4 );
    // cleanup
    try {
        db.clear();
    } catch(GAMSException&) {
        FAIL() << "Do not expect exception when clearing database";
    }
}

TEST_F(TestGAMSDatabase, testGetDatabaseDVs) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    EXPECT_EQ( db.getDatabaseDVs(0, 5).size(), size_t(2) );

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
    EXPECT_EQ( recordCounter.size(), size_t(3) );
    EXPECT_EQ( recordCounter["a_Alburquerque"], size_t(1) );
    EXPECT_EQ( recordCounter["a_Sanfrancisco"], size_t(1) );
    EXPECT_EQ( recordCounter["b_Braunschweig"], size_t(1) );
}

TEST_F(TestGAMSDatabase, testGetDatabaseDVs_MaxViolation) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Database_DomainViolations( db );

    // when, then
    { EXPECT_EQ( db.getDatabaseDVs(1, 1).size(), size_t(1) );
      std::map<std::string, size_t> symbolCounter;
      std::map<std::string, size_t> recordCounter;
      for(GAMSDatabaseDomainViolation domViol : db.getDatabaseDVs(1, 1)){
          for(GAMSSymbolDomainViolation rec : domViol.violRecs()) {
              symbolCounter[domViol.violSym().name()]++;
              recordCounter[rec.violRec().key(0)]++;
          }
      }
      EXPECT_EQ( symbolCounter.size(), size_t(1) );
      EXPECT_EQ( symbolCounter["a"] + symbolCounter["b"], size_t(1) );
      EXPECT_EQ( recordCounter.size(), size_t(1) );
    }

    // when, then
    for(auto &item : db.getDatabaseDVs(3, 1)){
       EXPECT_EQ(item.violRecs().size(), size_t(1));
    }
    EXPECT_EQ(db.getDatabaseDVs(3, 1).size(), size_t(2));


    for(auto &item : db.getDatabaseDVs(2, 1)){
       EXPECT_EQ(item.violRecs().size(), size_t(1));
    }
    EXPECT_EQ(db.getDatabaseDVs(2, 1).size(), size_t(2));


    int counter = 0;
    for(auto &item : db.getDatabaseDVs(0, 5)){
        switch (counter) {
            case 0:
                EXPECT_EQ(item.violRecs().size(), size_t(2));
            break;
            case 1:
                EXPECT_EQ(item.violRecs().size(), size_t(1));
            break;
        }
        counter++;
    }
    EXPECT_EQ(db.getDatabaseDVs(0, 5).size(), size_t(2));


    for(auto &item : db.getDatabaseDVs(2, 0)){
       EXPECT_EQ(item.violRecs().size(), size_t(2));
    }
    EXPECT_EQ(db.getDatabaseDVs(2, 0).size(), size_t(1));
}


TEST_F(TestGAMSDatabase, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    TestGAMSObject::getTestData_TransportModel(db1);

    // when, then
    GAMSDatabase db2 = db1;
    ASSERT_TRUE( db2 == db1 );

    // when, then
    GAMSDatabase db3 = ws.addDatabase();
    ASSERT_TRUE( ! (db3 == db1) );

    // when, then
    GAMSDatabase db4 = ws.addDatabase(db1);
    ASSERT_TRUE( ! (db4 == db1) );

}

TEST_F(TestGAMSDatabase, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    getTestData_TransportModel(db1);

    // when, then
    GAMSDatabase db2 = db1;
    ASSERT_TRUE( !( db2 != db1) );

    // when, then
    GAMSDatabase db3 = ws.addDatabase();
    ASSERT_TRUE( db3 != db1 );

    // when, then
    GAMSDatabase db4 = ws.addDatabase(db1);
    ASSERT_TRUE( db4 != db1 );
}

TEST_F(TestGAMSDatabase, testClear) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_TransportModel(db);
    int numberOfSymbols = db.getNrSymbols();
    // when
    db.clear();
    // then
    EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
    EXPECT_EQ( db.getSet("i").numberRecords(), 0 );
    EXPECT_EQ( db.getSet("j").numberRecords(), 0 );
    EXPECT_EQ( db.getParameter("a").numberRecords(), 0 );
    EXPECT_EQ( db.getParameter("b").numberRecords(), 0 );
    EXPECT_EQ( db.getParameter("d").numberRecords(), 0 );
    EXPECT_EQ( db.getParameter("f").numberRecords(), 0 );
}

TEST_F(TestGAMSDatabase, testClearOutOfScopedDatabase) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    ASSERT_TRUE(GAMSPath::exists(ws.systemDirectory()));
    GAMSDatabase db1 = ws.addDatabase();
    TestGAMSObject::getTestData_TransportModel( db1 );
    int numberOfSymbols = db1.getNrSymbols();
    // when
    {
       GAMSDatabase db2 = db1;
       db2.clear();
    }
    // then
    EXPECT_EQ( db1.getNrSymbols(), numberOfSymbols );
    EXPECT_EQ( db1.getSet("i").numberRecords(), 0 );
    EXPECT_EQ( db1.getSet("j").numberRecords(), 0 );
    EXPECT_EQ( db1.getParameter("a").numberRecords(), 0 );
    EXPECT_EQ( db1.getParameter("b").numberRecords(), 0 );
    EXPECT_EQ( db1.getParameter("d").numberRecords(), 0 );
    EXPECT_EQ( db1.getParameter("f").numberRecords(), 0 );
}

// data driven tests:
class ParameterizedTestGAMSDatabaseAutoDomain
        : public ::testing::WithParamInterface<std::tuple<const std::string, const bool, const std::string>>,
          public TestGAMSObject {
};

INSTANTIATE_TEST_SUITE_P(testSetSuppressAutoDomainChecking,
                        ParameterizedTestGAMSDatabaseAutoDomain,
                        ::testing::Values (
                             // description, suppressed, gdxfilename
                            std::make_tuple("SuppressedAutoDomainChecking", true, "suppressed.gdx"),
                            std::make_tuple("UnsuppressedAutoDomainChecking", false, "unsuppressed.gdx")
                        ));

TEST_P(ParameterizedTestGAMSDatabaseAutoDomain, testSetSuppressAutoDomainChecking) {
    std::string description = std::get<0>(GetParam());
    bool suppressed         = std::get<1>(GetParam());
    std::string gdxfilename = std::get<2>(GetParam());

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSDatabase db = ws.addDatabase();
    GAMSSet i = db.addSet("i", "");
    i.addRecord("i1");
    GAMSParameter a = db.addParameter("x", "", i);
    a.addRecord("i1").setValue( 0.5 );
    a.addRecord("i2").setValue( 1.2 );
    // when
    db.setSuppressAutoDomainChecking(suppressed);
    // then
    if (suppressed) {
        db.doExport();
        GAMSPath gdxfile(ws.workingDirectory(), db.name() + ".gdx");
        ASSERT_TRUE(gdxfile.exists());
    }  else {
        EXPECT_THROW( db.doExport(gdxfilename), GAMSException);
    }
}

class ParameterizedTestGAMSDatabaseSymbol
        : public ::testing::WithParamInterface<std::tuple<const std::string, const int, const std::string, const int, const std::string, int>>,
          public TestGAMSObject {
};

INSTANTIATE_TEST_SUITE_P(testGetSymbol,
                        ParameterizedTestGAMSDatabaseSymbol,
                        ::testing::Values (
                             // description, symbolType, symbolID, dimension, text, numberOfRecords
                            std::make_tuple("markets_i",     0, "i",      1, "canning plants", 2),
                            std::make_tuple("plants_j",      0, "j",      1, "markets", 3),
                            std::make_tuple("capacity_a",    1, "a",      1, "capacity of plant i in cases", 2),
                            std::make_tuple("demand_b",      1, "b",      1, "demand at market j in cases", 3),
                            std::make_tuple("distance_d",    1, "d",      2, "distance in thousands of miles", 6),
                            std::make_tuple("freightcost_f", 1, "f",      0, "freight in dollars per case per thousand miles", 1),
                            std::make_tuple("cost_c",        1, "c",      2, "transport cost in thousands of dollars per case", 6),
                            std::make_tuple("shipment_x",    2, "x",      2, "shipment quantities in cases", 6),
                            std::make_tuple("obj_z",         2, "z",      0, "total transportation costs in thousands of dollars", 1),
                            std::make_tuple("supply",        3, "supply", 1, "observe supply limit at plant i", 2),
                            std::make_tuple("demand",        3, "demand", 1, "satisfy demand at market j", 3)
                        ));

TEST_P(ParameterizedTestGAMSDatabaseSymbol, testGetSymbol) {
    std::string description = std::get<0>(GetParam());
    int symbolType          = std::get<1>(GetParam());
    std::string symbolID    = std::get<2>(GetParam());
    int dimension           = std::get<3>(GetParam());
    std::string text        = std::get<4>(GetParam());
    int numberOfRecords     = std::get<5>(GetParam());

    std::cout << description << std::endl;

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    switch(symbolType) {
      case GAMSEnum::SymbolType::SymTypeSet :
        {
          GAMSSet set = db.getSet( symbolID );
          EXPECT_EQ( set.dim(), dimension );
          EXPECT_EQ( set.text(), text );
          EXPECT_EQ( set.numberRecords(), numberOfRecords );
          EXPECT_THROW( db.getParameter( symbolID ), GAMSException);
          EXPECT_THROW( db.getEquation( symbolID ), GAMSException);
          EXPECT_THROW( db.getVariable( symbolID ), GAMSException);
        }
        break;
      case GAMSEnum::SymbolType::SymTypePar :
        {
          GAMSParameter param = db.getParameter( symbolID );
          EXPECT_EQ( param.dim(), dimension );
          EXPECT_EQ( param.text(), text );
          EXPECT_EQ( param.numberRecords(), numberOfRecords );
          EXPECT_THROW( db.getSet( symbolID ), GAMSException);
          EXPECT_THROW( db.getEquation( symbolID ), GAMSException);
          EXPECT_THROW( db.getVariable( symbolID ), GAMSException);
          break;
        }
      case GAMSEnum::SymbolType::SymTypeVar :
        {
          GAMSVariable var = db.getVariable( symbolID );
          EXPECT_EQ( var.dim(), dimension );
          EXPECT_EQ( var.text(), text );
          EXPECT_EQ( var.numberRecords(), numberOfRecords );
          EXPECT_THROW( db.getSet( symbolID ), GAMSException);
          EXPECT_THROW( db.getEquation( symbolID ), GAMSException);
          EXPECT_THROW( db.getParameter( symbolID ), GAMSException);
          break;
        }
      case GAMSEnum::SymbolType::SymTypeEqu :
        {
          GAMSEquation eq = db.getEquation( symbolID );
          EXPECT_EQ( eq.dim(),  dimension );
          EXPECT_EQ( eq.text(), text );
          EXPECT_EQ( eq.numberRecords(), numberOfRecords );
          EXPECT_THROW( db.getSet( symbolID ), GAMSException);
          EXPECT_THROW( db.getParameter( symbolID ), GAMSException);
          EXPECT_THROW( db.getVariable( symbolID ), GAMSException);
          break;
        }
      default: break;
    }
}
