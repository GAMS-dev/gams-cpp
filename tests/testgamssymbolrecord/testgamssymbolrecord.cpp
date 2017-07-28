/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
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

#include "gamssymbolrecord.h"
#include "gamsequation.h"
#include "gamsequationrecord.h"
#include "gamsset.h"
#include "gamssetrecord.h"
#include "gamsparameter.h"
#include "gamsparameterrecord.h"
#include "gamsvariable.h"
#include "gamsvariablerecord.h"
#include "testgamssymbolrecord.h"

#include <sstream>
#include <string>

using namespace gams;

QString TestGAMSSymbolRecord::classname()  { return "TestGAMSSymbolRecord"; }

void TestGAMSSymbolRecord::testDefaultConstructor() {
    // when
    GAMSSymbolRecord rec;
    // then
    QVERIFY( ! rec.isValid() );
    QVERIFY_EXCEPTION_THROWN( rec.moveNext(), GAMSException);
}

void TestGAMSSymbolRecord::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db = ws.addDatabase();
    TestGAMSObject::getTestData_Set_plants_i(db);
    GAMSSymbolRecord rec = db.getSet("i").firstRecord();
    // when
    GAMSSymbolRecord newRecord = GAMSSymbolRecord( rec );
    // then
    QCOMPARE( newRecord, rec );
}


void TestGAMSSymbolRecord::testAssignmentOperator_data() {
    QTest::addColumn<int>("symbolType");
    QTest::addColumn<QString>("symbolID");

    QTest::newRow("plants_j")   << 0 << "j"       ;
    QTest::newRow("distance_a") << 1 << "a"       ;
    QTest::newRow("obj_x")      << 2 << "x"       ;
    QTest::newRow("supply")     << 3 << "supply"  ;
}

void TestGAMSSymbolRecord::testAssignmentOperator() {
    QFETCH(int, symbolType);
    QFETCH(QString, symbolID);

    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    switch(symbolType) {
      case GAMSEnum::SymbolType::SymTypeSet :
        {
          GAMSSetRecord rec = db.getSet(symbolID.toStdString()).firstRecord();
          GAMSSetRecord newRecord = rec;
          QCOMPARE( newRecord.keys().size(), rec.keys().size() );
          for(auto i=0UL; i<newRecord.keys().size(); ++i)
              QCOMPARE( newRecord.key(0), rec.key(0) );
          QCOMPARE( newRecord, rec );
        }
        break;
      case GAMSEnum::SymbolType::SymTypePar :
        {
          GAMSParameterRecord rec = db.getParameter(symbolID.toStdString()).firstRecord();
          GAMSParameterRecord newRecord = rec;
          QCOMPARE( newRecord.keys().size(), rec.keys().size() );
          for(auto i=0UL; i<newRecord.keys().size(); ++i) {
              QCOMPARE( newRecord.key(0), rec.key(0) );
          }
          QCOMPARE( newRecord, rec );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeVar :
        {
          GAMSVariableRecord rec = db.getVariable(symbolID.toStdString()).firstRecord();
          GAMSVariableRecord newRecord = rec;
          QCOMPARE( newRecord.keys().size(), rec.keys().size() );
          for(auto i=0UL; i<newRecord.keys().size(); ++i) {
              QCOMPARE( newRecord.key(0), rec.key(0) );
          }
          QCOMPARE( newRecord, rec );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeEqu :
        {
          GAMSEquationRecord rec = db.getEquation( symbolID.toStdString() ).firstRecord();;
          GAMSEquationRecord newRecord = rec;
          QCOMPARE( newRecord.keys().size(), rec.keys().size() );
          for(auto i=0UL; i<newRecord.keys().size(); ++i) {
              QCOMPARE( newRecord.key(0), rec.key(0) );
          }
          QCOMPARE( newRecord, rec );
          break;
       }
      default: break;
    }
}

void TestGAMSSymbolRecord::testAssignmentOperator_IncorrectType_data() {
    QTest::addColumn<int>("symbolType");
    QTest::addColumn<QString>("symbolID");

    QTest::newRow("markets_i")  << 0 << "i"       ;
    QTest::newRow("distance_d") << 1 << "d"       ;
    QTest::newRow("obj_z")      << 2 << "z"       ;
    QTest::newRow("supply")     << 3 << "supply"  ;
}

void TestGAMSSymbolRecord::testAssignmentOperator_IncorrectType() {
    QFETCH(int, symbolType);
    QFETCH(QString, symbolID);

    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    switch(symbolType) {
      case GAMSEnum::SymbolType::SymTypeSet :
        {
          GAMSSet symbol = db.getSet( symbolID.toStdString() );
          GAMSSymbolRecord rec = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSParameterRecord r = rec, GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSEquationRecord r = rec, GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord r = rec, GAMSException );
        }
        break;
      case GAMSEnum::SymbolType::SymTypePar :
        {
          GAMSParameter symbol = db.getParameter( symbolID.toStdString() );
          GAMSSymbolRecord rec = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSetRecord r = rec, GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSEquationRecord r = rec, GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord r = rec, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeVar :
        {
          GAMSVariable symbol = db.getVariable( symbolID.toStdString() );
          GAMSSymbolRecord rec = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSetRecord r = rec, GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSEquationRecord r = rec, GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSParameterRecord r = rec, GAMSException );
          break;
        }
      case GAMSEnum::SymbolType::SymTypeEqu :
        {
          GAMSEquation symbol = db.getEquation( symbolID.toStdString() );
          GAMSSymbolRecord rec = symbol.firstRecord();
          QVERIFY_EXCEPTION_THROWN( GAMSSetRecord r = rec, GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSVariableRecord r = rec, GAMSException );
          QVERIFY_EXCEPTION_THROWN( GAMSParameterRecord r = rec, GAMSException );
          break;
        }
      default: break;
    }
}

void TestGAMSSymbolRecord::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    { GAMSSymbol symbol = db.getSet( "i" );
      GAMSSymbolRecord rec1 = symbol.firstRecord();
      GAMSSymbolRecord rec2 = symbol.firstRecord();
      GAMSSymbolRecord rec3 = symbol.findRecord("seattle");
      // then
      QVERIFY( rec1 == rec2 );
      QVERIFY( rec1 == rec3 );
    }
    // when
    { GAMSSymbol symbol = db.getParameter( "f" );
      GAMSSymbolRecord rec1 = symbol.firstRecord();
      GAMSSymbolRecord rec2 = symbol.lastRecord();
      // then
      QVERIFY( rec1 == rec2 );
    }
}

void TestGAMSSymbolRecord::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();
    // when
    { GAMSSymbol symbol = db.getSet( "i" );
      GAMSSymbolRecord rec1 = symbol.firstRecord();
      GAMSSymbolRecord rec2 = symbol.lastRecord();
      GAMSSymbolRecord rec3 = symbol.findRecord("seattle");
      // then
      QVERIFY( rec1 != rec2 );
      QVERIFY( rec1 == rec3 );
      QVERIFY( rec2 != rec3 );
    }
}

void TestGAMSSymbolRecord::testIsValid() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    { GAMSSymbolRecord rec;
      QVERIFY( ! rec.isValid() );
    }
    // when, then
    for(GAMSSymbolRecord rec : db.getSet("j")) {
        QVERIFY( rec.isValid() );
    }

    // when, then
    for(GAMSSymbolRecord rec : db.getParameter("b")) {
       QVERIFY( rec.isValid() );
    }

    // when, then
    for(GAMSSymbolRecord rec : db.getParameter("f")) {
       QVERIFY( rec.isValid() );
    }

    // when, then
    for(GAMSSymbolRecord rec : db.getVariable("x")) {
       QVERIFY( rec.isValid() );
    }

    // when, then
    for(GAMSSymbolRecord rec : db.getEquation("demand")) {
       QVERIFY( rec.isValid() );
    }
}

void TestGAMSSymbolRecord::testIterator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    { std::map<std::string, int> symbolCounter;
      for(GAMSSymbolRecord rec : db.getSet("i"))
          symbolCounter[rec.key(0)]++;
      QCOMPARE( symbolCounter.size(), size_t(2) );
      QCOMPARE( symbolCounter["seattle"], 1);
      QCOMPARE( symbolCounter["san-diego"], 1);
    }

    { std::map<std::string, int> symbolCounter;
      for(GAMSSymbolRecord rec : db.getParameter("b"))
          symbolCounter[rec.key(0)]++;
      QCOMPARE( symbolCounter.size(), size_t(3) );
      QCOMPARE( symbolCounter["new-york"], 1);
      QCOMPARE( symbolCounter["chicago"], 1);
      QCOMPARE( symbolCounter["topeka"], 1);
    }

    {  int i = 0;
       for(GAMSSymbolRecord rec : db.getParameter("f"))
           ++i;
       QCOMPARE( i, 1 );
    }

    { std::map<std::string, int> symbolCounter;
      for(GAMSSymbolRecord rec : db.getParameter("d")) {
          std::stringstream ss;
          ss << rec.key(0) << "_" <<rec.key(1) ;
          symbolCounter[ss.str()]++;
      }
      QCOMPARE( symbolCounter.size(), size_t(6) );
      QCOMPARE( symbolCounter["seattle_new-york"], 1);
      QCOMPARE( symbolCounter["seattle_chicago"], 1);
      QCOMPARE( symbolCounter["seattle_topeka"], 1);
      QCOMPARE( symbolCounter["san-diego_new-york"], 1);
      QCOMPARE( symbolCounter["san-diego_chicago"], 1);
      QCOMPARE( symbolCounter["san-diego_topeka"], 1);
    }

    { std::map<std::string, int> symbolCounter;
      for(GAMSSymbolRecord rec : db.getVariable("x")) {
          std::stringstream ss;
          ss << rec.key(0) << "_" <<rec.key(1) ;
          symbolCounter[ss.str()]++;
      }
      QCOMPARE( symbolCounter.size(), size_t(6) );
      QCOMPARE( symbolCounter["seattle_new-york"], 1);
      QCOMPARE( symbolCounter["seattle_chicago"], 1);
      QCOMPARE( symbolCounter["seattle_topeka"], 1);
      QCOMPARE( symbolCounter["san-diego_new-york"], 1);
      QCOMPARE( symbolCounter["san-diego_chicago"], 1);
      QCOMPARE( symbolCounter["san-diego_topeka"], 1);
    }

    { std::map<std::string, int> symbolCounter;
      for(GAMSSymbolRecord rec : db.getEquation("supply"))
         symbolCounter[rec.key(0)]++;
      QCOMPARE( symbolCounter.size(), size_t(2) );
      QCOMPARE( symbolCounter["seattle"], 1);
      QCOMPARE( symbolCounter["san-diego"], 1);
    }
}

void TestGAMSSymbolRecord::testGetKeys() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbolRecord rec_j = db.getSet("j").lastRecord();
    QCOMPARE( rec_j.keys().size(), size_t(1));

    GAMSSymbolRecord rec_d = db.getParameter("d").lastRecord();
    QCOMPARE( rec_d.keys().size(), size_t(2));

    GAMSSymbolRecord rec_f = db.getParameter("f").lastRecord();
    QCOMPARE( rec_f.keys().size(), size_t(0));

    GAMSSymbolRecord rec_x = db.getVariable("x").lastRecord();
    QCOMPARE( rec_x.keys().size(), size_t(2));

    GAMSSymbolRecord rec_cost = db.getEquation("cost").lastRecord();
    QCOMPARE( rec_cost.keys().size(), size_t(0));

    GAMSSymbolRecord rec_demand = db.getEquation("demand").lastRecord();
    QCOMPARE( rec_demand.keys().size(), size_t(1));
}

void TestGAMSSymbolRecord::testGetKey_index() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbolRecord rec_i = db.getSet("i").lastRecord();
    QCOMPARE( rec_i.key(0).c_str(), "san-diego");
    QVERIFY_EXCEPTION_THROWN( rec_i.key(1).c_str(), GAMSException );

    GAMSSymbolRecord rec_d = db.getParameter("d").lastRecord();
    QCOMPARE( rec_d.key(0).c_str(), "san-diego");
    QCOMPARE( rec_d.key(1).c_str(), "topeka");
    QVERIFY_EXCEPTION_THROWN( rec_d.key(2), GAMSException );

    GAMSSymbolRecord rec_f = db.getParameter("f").lastRecord();
    QVERIFY_EXCEPTION_THROWN( rec_f.key(0), GAMSException );

    GAMSSymbolRecord rec_x = db.getVariable("x").lastRecord();
    QCOMPARE( rec_x.key(0).c_str(), "san-diego");
    QCOMPARE( rec_x.key(1).c_str(), "topeka");
    QVERIFY_EXCEPTION_THROWN( rec_x.key(2).c_str(), GAMSException );

    GAMSSymbolRecord rec_cost = db.getEquation("cost").lastRecord();
    QVERIFY_EXCEPTION_THROWN( rec_cost.key(0), GAMSException );

    GAMSSymbolRecord rec_supply = db.getEquation("supply").lastRecord();
    QCOMPARE( rec_supply.key(0).c_str(), "san-diego");
    QVERIFY_EXCEPTION_THROWN( rec_supply.key(1).c_str(), GAMSException );
}

void TestGAMSSymbolRecord::testIndexOperator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbolRecord rec_i = db.getSet("i").lastRecord();
    QCOMPARE( rec_i[0].c_str(), "san-diego");
    QVERIFY_EXCEPTION_THROWN( rec_i[1], GAMSException );

    GAMSSymbolRecord rec_d = db.getParameter("d").lastRecord();
    QCOMPARE( rec_d[0].c_str(), "san-diego");
    QCOMPARE( rec_d[1].c_str(), "topeka");
    QVERIFY_EXCEPTION_THROWN( rec_d[2], GAMSException );

    GAMSSymbolRecord rec_f = db.getParameter("f").lastRecord();
    QVERIFY_EXCEPTION_THROWN( rec_f[0], GAMSException );

    GAMSSymbolRecord rec_x = db.getVariable("x").lastRecord();
    QCOMPARE( rec_x[0].c_str(), "san-diego");
    QCOMPARE( rec_x[1].c_str(), "topeka");
    QVERIFY_EXCEPTION_THROWN( rec_x[2].c_str(), GAMSException );

    GAMSSymbolRecord rec_cost = db.getEquation("cost").lastRecord();
    QVERIFY_EXCEPTION_THROWN( rec_cost[0], GAMSException );

    GAMSSymbolRecord rec_supply = db.getEquation("supply").lastRecord();
    QCOMPARE( rec_supply[0].c_str(), "san-diego");
    QVERIFY_EXCEPTION_THROWN( rec_supply[1], GAMSException );
}

void TestGAMSSymbolRecord::testGetType() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSJob job = ws.addJobFromGamsLib( "trnsport" );
    job.run();
    GAMSDatabase db = job.outDB();

    // when, then
    GAMSSymbolRecord rec_i( db.getSet("i").firstRecord() );
    QCOMPARE( rec_i.type(), GAMSEnum::SymbolType::SymTypeSet );
    // when, then
    GAMSSymbolRecord rec_j( db.getSet("j").lastRecord() );
    QCOMPARE( rec_j.type(), GAMSEnum::SymbolType::SymTypeSet );
    // when, then
    GAMSSymbolRecord rec_a( db.getParameter("a").firstRecord() );
    QCOMPARE( rec_a.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbolRecord rec_b( db.getParameter("b").lastRecord() );
    QCOMPARE( rec_b.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbolRecord rec_d( db.getParameter("d").firstRecord() );
    QCOMPARE( rec_d.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbolRecord rec_f( db.getParameter("f").lastRecord() );
    QCOMPARE( rec_f.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbolRecord rec_c( db.getParameter("c").firstRecord() );
    QCOMPARE( rec_c.type(), GAMSEnum::SymbolType::SymTypePar );
    // when, then
    GAMSSymbolRecord rec_cost( db.getEquation("cost").lastRecord() );
    QCOMPARE( rec_cost.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbolRecord rec_supply( db.getEquation("supply").firstRecord() );
    QCOMPARE( rec_supply.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbolRecord rec_demand( db.getEquation("demand").lastRecord() );
    QCOMPARE( rec_demand.type(), GAMSEnum::SymbolType::SymTypeEqu );
    // when, then
    GAMSSymbolRecord rec_x( db.getVariable("x").firstRecord() );
    QCOMPARE( rec_x.type(), GAMSEnum::SymbolType::SymTypeVar );
    // when, then
    GAMSSymbolRecord rec_z( db.getVariable("z").lastRecord() );
    QCOMPARE( rec_z.type(), GAMSEnum::SymbolType::SymTypeVar );
}

QTEST_MAIN(TestGAMSSymbolRecord)
