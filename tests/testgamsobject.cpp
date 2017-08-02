/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017 Jarungjit Parnjai <jeed@gams.com>
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

#include "gamsenum.h"
#include "gamsequation.h"
#include "gamsexception.h"
#include "gamsparameter.h"
#include "gamsparameterrecord.h"
#include "gamsset.h"
#include "gamssymbol.h"
#include "gamssymboliter.h"
#include "gamsvariable.h"
#include "testgamsobject.h"
#include "gamsoptions.h"
#include "gamsplatform.h"
#include "gamsversion.h"

#include <vector>
#include <map>
#include <QtTest>

using namespace gams;
using namespace std;

void TestGAMSObject::initTestCase() {
    testSystemDir = GAMSPlatform::findGams(0).c_str();
    testDebugLevel = qgetenv("GAMSOOAPIDEBUG");
    testGAMSVersion = GAMSOptions::gamsVersion();
    testAPIVersion = GAMSVersion::api();

    tests_Executed = 0;
    tests_Failed = 0;
//    qDebug() <<  testGAMSVersion << ", " << testDebugLevel.isEmpty();
}

void TestGAMSObject::cleanupTestCase() {
//    if (tests_Failed == 0) { }
}

void TestGAMSObject::init() {
    // @todo to remove irreleavant files and directories

}

void TestGAMSObject::cleanup() {
    tests_Executed++;
    if (QTest::currentTestFailed())
        tests_Failed++;
}

void TestGAMSObject::getTestData_DebugLevel() {
    QTest::addColumn<QString>("debugLevel");

    QTest::newRow("Off") << QString::number(GAMSEnum::DebugLevel::Off);
    QTest::newRow("KeepFiles") << QString::number(GAMSEnum::DebugLevel::KeepFiles);
    QTest::newRow("ShowLog") << QString::number(GAMSEnum::DebugLevel::ShowLog);
    QTest::newRow("Verbose") << QString::number(GAMSEnum::DebugLevel::Verbose);

}

void TestGAMSObject::getTestData_ModelLibraries() {
    QTest::addColumn<int>("index");
    QTest::addColumn<QString>("library");
    QTest::addColumn<QString>("modname");
    QTest::addColumn<QString>("extrafile");

    QTest::newRow("gamslib_trnsport") << 0 << "gamslib" << "trnsport" << "";
    QTest::newRow("gamslib_alan")     << 0 << "gamslib" << "alan"  << "";
    QTest::newRow("testlib_call1")    << 1 << "testlib" << "call1"    << "";
    QTest::newRow("testlib_onmult1")      << 1 << "testlib" << "onmulti1"     << "";
    QTest::newRow("datalib_CheckListbox") << 2 << "datalib" << "CheckListbox" << "CheckListbox.gms";
    QTest::newRow("datalib_Wiring")       << 2 << "datalib" << "Wiring"       << "Sample.mdb"      ;
    // QTest::newRow("emplib_goempgo")    << 3 << "emplib"  << "goempgo"       << "empmod.inc"      ;
    QTest::newRow("emplib_hark-monop")    << 3 << "emplib"  << "hark-monop"   << "hark-data.inc"      ;
    QTest::newRow("emplib_farmnbd")       << 3 << "emplib"  << "farmnbd"      << "" ;
    QTest::newRow("apilib_JDomainCheck")       << 4 << "apilib"  << "JDomainCheck"     << "" ;
    QTest::newRow("apilib_JSpecialValues")     << 4 << "apilib"  << "JSpecialValues"   << "" ;
    QTest::newRow("finlib_DedicationNoBorrow") << 5 << "finlib" << "DedicationNoBorrow"<< "BondData.inc"  ;
    QTest::newRow("finlib_StructuralModel")    << 5 << "finlib" << "StructuralModel"   << "InputData.gdx" ;
    QTest::newRow("noalib_reservoir")          << 6 << "noalib" << "reservoir"         << "" ;
    QTest::newRow("noalib_macro")              << 6 << "noalib" << "macro"             << "" ;
}


void TestGAMSObject::getTestData_InvalidModelLibraries() {
    QTest::addColumn<int>("index");
    QTest::addColumn<QString>("library");
    QTest::addColumn<QString>("modname");

    QTest::newRow("invalid_gamslib") << 0 << "gamslib" << "ThisIsAnUnusualModelName";
    QTest::newRow("invalid_testlib") << 1 << "testlib" << "ThisIsAnUnusualModelName";
    QTest::newRow("invalid_datalib") << 2 << "datalib" << "ThisIsAnUnusualModelName";
    QTest::newRow("invalid_emplib")  << 3 << "emplib"  << "ThisIsAnUnusualModelName";
    QTest::newRow("invalid_apilib")  << 4 << "apilib"  << "ThisIsAnUnusualModelName";
    QTest::newRow("invalid_finlib")  << 5 << "finlib"  << "ThisIsAnUnusualModelName";
    QTest::newRow("invalid_noalib")  << 6 << "noalib"  << "ThisIsAnUnusualModelName";
}


void TestGAMSObject::getTestData_SpecialValues() {
    QTest::addColumn<int>("index");
    QTest::addColumn<double>("value");

    QTest::newRow("UNDF") << 0 << defaultUNDEF;
    QTest::newRow("NA")   << 1 << defaultNA;
    QTest::newRow("PINF") << 2 << defaultPINF;
    QTest::newRow("MINF") << 3 << defaultMINF;
    QTest::newRow("EPS")  << 4 << defaultEPS;
}

void TestGAMSObject::getTestData_TransportModel(GAMSDatabase db) {
    getTestData_Set_plants_i(db);
    getTestData_Set_markets_j(db);
    getTestData_Parameter_capacity_a(db);
    getTestData_Parameter_demand_b(db);
    getTestData_Parameter_distance_d(db);
    getTestData_Parameter_freightcost_f(db);
}

void TestGAMSObject::getTestData_Set_plants_i(GAMSDatabase db) {
    vector<string> plants = { "Seattle", "San-Diego" };
    GAMSSet i = db.addSet("i", 1, "canning plants");
    for (string p: plants)
        i.addRecord(p);
}

void TestGAMSObject::getTestData_Set_markets_j(GAMSDatabase db) {
    vector<string> markets = { "New-York", "Chicago", "Topeka" };
    GAMSSet j = db.addSet("j", 1, "markets");
    for (string m: markets)
        j.addRecord(m);
}

void TestGAMSObject::getTestData_Parameter_capacity_a(GAMSDatabase db) {
    vector<string> plants = { "Seattle", "San-Diego"  };
    map<string, double> capacity = {
        { "Seattle", 350.0 }, { "San-Diego", 600.0 }
    };
    GAMSParameter a = db.addParameter("a", 1, "capacity of plant i in cases");
    for (string p: plants)
        a.addRecord(p).setValue(capacity[p]);
}

void TestGAMSObject::getTestData_Parameter_demand_b(GAMSDatabase db) {
    vector<string> markets = { "New-York", "Chicago", "Topeka" };
    map<string, double> demand = {
        { "New-York", 325.0 }, { "Chicago", 300.0 }, { "Topeka", 275.0 }
    };
    GAMSParameter b = db.addParameter("b", 1, "demand at market j in cases");
    for (string m: markets)
        b.addRecord(m).setValue(demand[m]);
}

void TestGAMSObject::getTestData_Parameter_distance_d(GAMSDatabase db) {
    map<tuple<string, string>, double> distance = {
        { make_tuple("Seattle", "New-York"), 2.5 },
        { make_tuple("Seattle", "Chicago"), 1.7 },
        { make_tuple("Seattle", "Topeka"), 1.8 },
        { make_tuple("San-Diego", "New-York"), 2.5 },
        { make_tuple("San-Diego", "Chicago"), 1.8 },
        { make_tuple("San-Diego", "Topeka"), 1.4 }
    };
    GAMSParameter d = db.addParameter("d", 2, "distance in thousands of miles");
    for (map<tuple<string, string>, double>::iterator it = distance.begin(); it != distance.end(); it++)
        d.addRecord(get<0>(it->first), get<1>(it->first)).setValue(it->second);
}

void TestGAMSObject::getTestData_Parameter_freightcost_f(GAMSDatabase db) {
    GAMSParameter f = db.addParameter("f", 0, "freight in dollars per case per thousand miles");
    ((GAMSParameterRecord)f.addRecord()).setValue(90);
}

void TestGAMSObject::getTestData_Database_DomainViolations(GAMSDatabase db) {
    TestGAMSObject::getTestData_Set_plants_i( db );
    TestGAMSObject::getTestData_Set_markets_j( db );

    GAMSParameter a = db.addParameter("a", "capacity of plant i in cases", db.getSet("i"));
    a.addRecord("Seattle").setValue(350.0);
    a.addRecord("Alburquerque").setValue(500.0);
    a.addRecord("Sanfrancisco").setValue(120.0);

    GAMSParameter b = db.addParameter("b", "market", db.getSet("j"));
    b.addRecord("Braunschweig").setValue(420.0);

    TestGAMSObject::getTestData_Parameter_distance_d( db );
    GAMSParameter d = db.getParameter("d");
    d.addRecord("Seattle", "Alburquerque").setValue( 1.5 );
}

void TestGAMSObject::testDir(QString dir) {
    QVERIFY( ! dir.isNull() );
    QVERIFY( ! dir.isEmpty() );
    QVERIFY( QDir(dir).exists() );
}

void TestGAMSObject::testJobBeforeRun(GAMSJob job, GAMSWorkspace ws) {
    QVERIFY( ! job.name().empty() );
    QVERIFY( ! job.outDB().isValid() );
    QVERIFY( job.workspace() == ws );
}

void TestGAMSObject::testEmptyDatabase(gams::GAMSDatabase db, gams::GAMSWorkspace ws) {
    QVERIFY( db.isValid() );
    QCOMPARE( db.getNrSymbols(), 0 );
    QVERIFY( db.workspace() == ws );

    for (gams::GAMSDatabaseIter it = db.begin(); it != db.end(); ++it)
        QFAIL("does not expect a symbol in a newly created database.");

    QVERIFY_EXCEPTION_THROWN( db.getSet("x"), gams::GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.getParameter("x"), gams::GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.getVariable("x"), gams::GAMSException);
    QVERIFY_EXCEPTION_THROWN( db.getEquation("x"), gams::GAMSException);
}

std::string TestGAMSObject::getShortModelText()
{
    const std::string model =
            "Sets                     \n"
            "      i   canning plants \n"
            "      j   markets        \n"
            "Positive Variable x ;    \n"
            "Display x.l, x.m ;       \n";

    return model;
}


std::string TestGAMSObject::getLongModelText()
{
    const std::string model =
            " Sets                                                               \n"
            "     i   canning plants   / seattle, san-diego /                    \n"
            "     j   markets          / new-york, chicago, topeka / ;           \n"
            "                                                                    \n"
            "Parameters                                                          \n"
            "     a(i)  capacity of plant i in cases                             \n"
            "       /    seattle     350                                         \n"
            "            san-diego   600  /                                      \n"
            "                                                                    \n"
            "     b(j)  demand at market j in cases                              \n"
            "       /    new-york    325                                         \n"
            "            chicago     300                                         \n"
            "            topeka      275  / ;                                    \n"
            "                                                                    \n"
            "Table d(i,j)  distance in thousands of miles                        \n"
            "                  new-york       chicago      topeka                \n"
            "    seattle          2.5           1.7          1.8                 \n"
            "    san-diego        2.5           1.8          1.4  ;              \n"
            "                                                                    \n"
            "Scalar f      freight in dollars per case per thousand miles  /90/ ;\n"
            "Scalar bmult  demand multiplier /1/;                                \n"
            "                                                                    \n"
            "Parameter c(i,j)  transport cost in thousands of dollars per case ; \n"
            "                                                                    \n"
            "          c(i,j) = f * d(i,j) / 1000 ;                              \n"
            "                                                                    \n"
            "Variables                                                           \n"
            "     x(i,j)  shipment quantities in cases                           \n"
            "     z       total transportation costs in thousands of dollars ;   \n"
            "                                                                    \n"
            "Positive Variable x ;                                               \n"
            "                                                                    \n"
            "Equations                                                           \n"
            "     cost        define objective function                          \n"
            "     supply(i)   observe supply limit at plant i                    \n"
            "     demand(j)   satisfy demand at market j ;                       \n"
            "                                                                    \n"
            "cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;                  \n"
            "                                                                    \n"
            "supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;                          \n"
            "                                                                    \n"
            "demand(j) ..   sum(i, x(i,j))  =g=  bmult*b(j) ;                    \n"
            "                                                                    \n"
            "Model transport /all/ ;                                             \n"
            "Scalar ms 'model status', ss 'solve status';                        \n";

    return model;
}

std::string TestGAMSObject::getDataText()
{
    const std::string data =
            "Sets                                                           \n"
            "  i   canning plants   / seattle, san-diego /                  \n"
            "  j   markets          / new-york, chicago, topeka / ;         \n"
            "Parameters                                                     \n"
            "                                                               \n"
            "  a(i)  capacity of plant i in cases                           \n"
            "                     /    seattle     350                      \n"
            "                          san-diego   600  /                   \n"
            "                                                               \n"
            "  b(j)  demand at market j in cases                            \n"
            "                     /    new-york    325                      \n"
            "                          chicago     300                      \n"
            "                          topeka      275  / ;                 \n"
            "                                                               \n"
            "Table d(i,j)  distance in thousands of miles                   \n"
            "               new-york       chicago      topeka              \n"
            "  seattle          2.5           1.7          1.8              \n"
            "  san-diego        2.5           1.8          1.4  ;           \n"
            "                                                               \n"
            "Scalar f  freight in dollars per case per thousand miles  /90/;\n";

    return data;
}

