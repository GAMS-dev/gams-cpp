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

#include "gamscheckpoint.h"
#include "gamsmodelinstance.h"
#include "gamsoptions.h"
#include "gamsparameter.h"
#include "gamsvariable.h"
#include "testgamsmodelinstance.h"

using namespace gams;

QString TestGAMSModelInstance::classname()  { return "TestGAMSModelInstance"; }

void TestGAMSModelInstance::testDefaultConstructor()  {
     GAMSModelInstance mi;
     QVERIFY( ! mi.isValid() );
     GAMSModifier mod;

     QVERIFY_EXCEPTION_THROWN( mi.name(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mi.copyModelInstance(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mi.instantiate("solve", mod), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mi.interrupt(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mi.syncDb(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mi.solve(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mi.checkpoint(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mi.modelStatus(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mi.solveStatus(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mi.solveStatusAsString(), GAMSException );
     QVERIFY_EXCEPTION_THROWN( mi.logID(), GAMSException );

     GAMSModelInstance anothermi;
     QVERIFY( ! anothermi.isValid() );
     QVERIFY( anothermi == mi );

     anothermi = mi;
     QVERIFY( ! anothermi.isValid() );
     QVERIFY( mi == anothermi );
}

void TestGAMSModelInstance::testAssignmentOperator()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSModelInstance mi1 = ws.addCheckpoint().addModelInstance();

    GAMSModelInstance mi2 = mi1;
    QVERIFY( mi2 == mi1 );

    GAMSModelInstance mi3;
    mi1 = mi3;
    QVERIFY( mi1 == mi3 );
    QVERIFY( mi1 != mi2 );
}

void TestGAMSModelInstance::testEqualToOperator()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSModelInstance mi1 = ws.addCheckpoint().addModelInstance();

    GAMSModelInstance mi2 = mi1;
    QVERIFY( mi2 == mi1 );

    GAMSModelInstance mi3;
    mi1 = mi3;
    QVERIFY( mi1 == mi3 );
}

void TestGAMSModelInstance::testNotEqualToOperator()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint("mycp");
    GAMSModelInstance mi1 = cp.addModelInstance();

    GAMSModelInstance mi2 = cp.addModelInstance();
    QVERIFY( mi2 != mi1 );

    GAMSModelInstance mi3 = ws.addCheckpoint().addModelInstance();
    QVERIFY( mi1 != mi3 );
}


void TestGAMSModelInstance::testIsValid()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    // when, then
    QVERIFY( cp.isValid() );
}

void TestGAMSModelInstance::testGetSyncDb()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();

    GAMSModelInstance mi = cp.addModelInstance();
    // when, then
    QCOMPARE( mi.syncDb().getNrSymbols(), 0);

    GAMSParameter bmult = mi.syncDb().addParameter("bmult", 0, "demand multiplier");
    // when, then
    QCOMPARE( mi.syncDb().getNrSymbols(), 1);
    QVERIFY( mi.syncDb().getParameter("bmult") == bmult );
}

void TestGAMSModelInstance::testInstantiate_data()  { getTestData(); }
void TestGAMSModelInstance::testInstantiate()  {
        QFETCH(double, modifier);

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString(getModelText()).run(cp);

    GAMSModelInstance mi = cp.addModelInstance();
    GAMSParameter bmult = mi.syncDb().addParameter("bmult", 0, "demand multiplier");
    GAMSOptions opt = ws.addOptions();
    opt.setAllModelTypes("cplexd");
    // when, then
    try {
       mi.instantiate("transport use lp min z", opt, GAMSModifier(bmult));
       bmult.addRecord().setValue( modifier );
       mi.solve();
    } catch(GAMSException&) { QVERIFY(false); }
}

void TestGAMSModelInstance::testInstantiateBeforeInitializingCP()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString(getModelText());

    GAMSModelInstance mi = cp.addModelInstance();
    GAMSParameter bmult = mi.syncDb().addParameter("bmult", 0, "demand multiplier");
    GAMSOptions opt = ws.addOptions();
    opt.setAllModelTypes("cplexd");
    // when, then
    QVERIFY_EXCEPTION_THROWN( mi.instantiate("transport use lp min z",  opt, GAMSModifier(bmult)) ,
                              GAMSException );
}

void TestGAMSModelInstance::testSolve_data() { getTestData(); }

void TestGAMSModelInstance::testSolve()  {
    QFETCH(double, modifier);
    QFETCH(int, SolveStatus);
    QFETCH(int, ModelStatus);
    QFETCH(double, obj);

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString(getModelText()).run(cp);

    GAMSModelInstance mi = cp.addModelInstance();

    GAMSParameter bmult = mi.syncDb().addParameter("bmult", 0, "demand multiplier");
    GAMSOptions opt = ws.addOptions();
    opt.setAllModelTypes("cplexd");

    mi.instantiate("transport use lp min z", opt, GAMSModifier(bmult));
    bmult.addRecord().setValue( modifier );

    // when
    mi.solve();
    // then
    QVERIFY( mi.solveStatus() == SolveStatus );
    QVERIFY( mi.modelStatus() == ModelStatus );
    QVERIFY( equals( mi.syncDb().getVariable("z").findRecord().level(), obj) );
}

void TestGAMSModelInstance::testSolveBeforeInstantiate()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    GAMSModelInstance mi = cp.addModelInstance();
    // when, then
    QVERIFY_EXCEPTION_THROWN( mi.solve(), GAMSException);

    ws.addJobFromString(getModelText()).run(cp);
    mi = cp.addModelInstance();
    // when, then
    QVERIFY_EXCEPTION_THROWN( mi.solve(), GAMSException);
}

void TestGAMSModelInstance::testGetModelSolveStatus_data() { getTestData(); }

void TestGAMSModelInstance::testGetModelSolveStatus()  {
    QFETCH(double, modifier);
    QFETCH(int, SolveStatus);
    QFETCH(QString, SolveStatusStr);
    QFETCH(int, ModelStatus);
    QFETCH(QString, ModelStatusStr);

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString(getModelText()).run(cp);

    GAMSModelInstance mi = cp.addModelInstance();
    GAMSParameter bmult = mi.syncDb().addParameter("bmult", 0, "demand multiplier");
    GAMSOptions opt = ws.addOptions();
    opt.setAllModelTypes("cplexd");

    mi.instantiate("transport use lp min z", opt, GAMSModifier(bmult));
    bmult.addRecord().setValue( modifier );

    // when
    mi.solve();
    // then
    QVERIFY( mi.solveStatus() == SolveStatus );
    QCOMPARE( QString::fromStdString(mi.solveStatusAsString()).trimmed(), SolveStatusStr );
    QVERIFY( mi.modelStatus() == ModelStatus );
    QCOMPARE( QString::fromStdString(mi.modelStatusAsString()).trimmed(), ModelStatusStr );
}

void TestGAMSModelInstance::testCopyModelInstance() {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    testCleanupDirs << QString::fromStdString(ws.workingDirectory());
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString(getModelText()).run(cp);

    GAMSModelInstance mi = cp.addModelInstance();
    GAMSParameter bmult = mi.syncDb().addParameter("bmult", 0, "demand multiplier");
    mi.instantiate("transport use lp min z", GAMSModifier(bmult));

    // when
    GAMSModelInstance newmi = mi.copyModelInstance("new transport mi");
    // then
    QVERIFY( newmi.isValid() );
    QCOMPARE( mi.syncDb().getNrSymbols(), newmi.syncDb().getNrSymbols() );
    QCOMPARE( newmi.syncDb().getParameter("bmult").numberRecords(), mi.syncDb().getParameter("bmult").numberRecords() );
}

void TestGAMSModelInstance::testGetCheckpoint()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    GAMSModelInstance mi = cp.addModelInstance();
    // when, then
    QCOMPARE( mi.checkpoint(), cp);
}

void TestGAMSModelInstance::testGetName()  {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();

    // when, then
    std::string cpname = "mycp";
    GAMSModelInstance mi1 = cp.addModelInstance(cpname);
    QCOMPARE( mi1.name(), cpname );

    // when, then
    GAMSModelInstance mi2 = cp.addModelInstance("");
    QVERIFY( mi2.name().find(defaultScratchFilePrefix) == 0 );

    // when, then
    GAMSModelInstance mi3 = cp.addModelInstance();
    QVERIFY( mi3.name().find(defaultScratchFilePrefix) == 0 );

    // when, then
    GAMSModelInstance mi4 = cp.addModelInstance();
    QVERIFY( mi4.name().find(defaultScratchFilePrefix) == 0 );
    QVERIFY( mi4.name() != mi3.name() );
}

void TestGAMSModelInstance::testGetLogID()  {
     // given
     GAMSWorkspaceInfo wsInfo("", testSystemDir.path().toStdString());
     GAMSWorkspace ws(wsInfo);
     GAMSCheckpoint cp = ws.addCheckpoint();
     std::string cpname = "mycp";
     GAMSModelInstance mi1 = cp.addModelInstance(cpname);
     GAMSModelInstance mi2 = mi1;
     // when, then
     QVERIFY( mi1.logID() == mi2.logID() );

     GAMSCheckpoint cp3 = ws.addCheckpoint(cpname);
     GAMSModelInstance mi3 = cp3.addModelInstance();
     QVERIFY( mi3.logID() == mi1.logID());

     GAMSWorkspaceInfo anotherWsInfo("", testSystemDir.path().toStdString());
     GAMSWorkspace anotherWs(anotherWsInfo);
     GAMSCheckpoint cp4 = anotherWs.addCheckpoint(cpname);
     GAMSModelInstance mi4 = cp4.addModelInstance();
     QVERIFY(mi4.logID() == anotherWs.logID());
     QVERIFY(mi4.logID() != mi3.logID());
}

void TestGAMSModelInstance::getTestData() {
    QTest::addColumn<double>("modifier");
    QTest::addColumn<int>("SolveStatus");
    QTest::addColumn<QString>("SolveStatusStr");
    QTest::addColumn<int>("ModelStatus");
    QTest::addColumn<QString>("ModelStatusStr");
    QTest::addColumn<double>("obj");

    QTest::newRow("bmult=0.6")   << 0.6 <<  1 << "1 Normal Completion" << 1 << "1 Optimal"    << 92.20499999999998;
    QTest::newRow("bmult=0.7")   << 0.7 <<  1 << "1 Normal Completion" << 1 << "1 Optimal"    << 107.57249999999999;
    QTest::newRow("bmult=0.8")   << 0.8 <<  1 << "1 Normal Completion" << 1 << "1 Optimal"    << 122.94;
    QTest::newRow("bmult=0.9")   << 0.9 <<  1 << "1 Normal Completion" << 1 << "1 Optimal"    << 138.3075;
    QTest::newRow("bmult=1.0")   << 1.0 <<  1 << "1 Normal Completion" << 1 << "1 Optimal"    << 153.675;
    QTest::newRow("bmult=1.1")   << 1.1 <<  1 << "1 Normal Completion" << 4 << "4 Infeasible" << 169.04250000000002;
    QTest::newRow("bmult=1.2")   << 1.2 <<  1 << "1 Normal Completion" << 4 << "4 Infeasible" << 184.40999999999997;
    QTest::newRow("bmult=1.3")   << 1.3 <<  1 << "1 Normal Completion" << 4 << "4 Infeasible" << 199.77750000000003;
}

std::string TestGAMSModelInstance::getModelText() {
    return " Sets                                                               \n"
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
           "Model transport /all/ ;                                             \n";
}

QTEST_MAIN(TestGAMSModelInstance)
