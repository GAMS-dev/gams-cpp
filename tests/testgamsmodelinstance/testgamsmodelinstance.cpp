/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>
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

#include "testgamsobject.h"
#include "gamscheckpoint.h"
#include "gamsmodelinstance.h"
#include "gamsoptions.h"
#include "gamsparameter.h"
#include "gamsvariable.h"

using namespace gams;

class TestGAMSModelInstance: public TestGAMSObject
{
public:
    static std::string getModelText();
};

TEST_F(TestGAMSModelInstance, testDefaultConstructor) {
     GAMSModelInstance mi;
     ASSERT_TRUE( ! mi.isValid() );
     GAMSModifier mod;

     EXPECT_THROW( mi.name(), GAMSException );
     EXPECT_THROW( mi.copyModelInstance(), GAMSException );
     EXPECT_THROW( mi.instantiate("solve", mod), GAMSException );
     EXPECT_THROW( mi.interrupt(), GAMSException );
     EXPECT_THROW( mi.syncDb(), GAMSException );
     EXPECT_THROW( mi.solve(), GAMSException );
     EXPECT_THROW( mi.checkpoint(), GAMSException );
     EXPECT_THROW( mi.modelStatus(), GAMSException );
     EXPECT_THROW( mi.solveStatus(), GAMSException );
     EXPECT_THROW( mi.solveStatusAsString(), GAMSException );
     EXPECT_THROW( mi.logID(), GAMSException );

     GAMSModelInstance anothermi;
     ASSERT_TRUE( ! anothermi.isValid() );
     ASSERT_TRUE( anothermi == mi );

     anothermi = mi;
     ASSERT_TRUE( ! anothermi.isValid() );
     ASSERT_TRUE( mi == anothermi );
}

TEST_F(TestGAMSModelInstance, testAssignmentOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSModelInstance mi1 = ws.addCheckpoint().addModelInstance();

    GAMSModelInstance mi2 = mi1;
    ASSERT_TRUE( mi2 == mi1 );

    GAMSModelInstance mi3;
    mi1 = mi3;
    ASSERT_TRUE( mi1 == mi3 );
    ASSERT_TRUE( mi1 != mi2 );
}

TEST_F(TestGAMSModelInstance, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSModelInstance mi1 = ws.addCheckpoint().addModelInstance();

    GAMSModelInstance mi2 = mi1;
    ASSERT_TRUE( mi2 == mi1 );

    GAMSModelInstance mi3;
    mi1 = mi3;
    ASSERT_TRUE( mi1 == mi3 );
}

TEST_F(TestGAMSModelInstance, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint("mycp");
    GAMSModelInstance mi1 = cp.addModelInstance();

    GAMSModelInstance mi2 = cp.addModelInstance();
    ASSERT_TRUE( mi2 != mi1 );

    GAMSModelInstance mi3 = ws.addCheckpoint().addModelInstance();
    ASSERT_TRUE( mi1 != mi3 );
}


TEST_F(TestGAMSModelInstance, testIsValid) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    // when, then
    ASSERT_TRUE( cp.isValid() );
}

TEST_F(TestGAMSModelInstance, testGetSyncDb) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();

    GAMSModelInstance mi = cp.addModelInstance();
    // when, then
    EXPECT_EQ( mi.syncDb().getNrSymbols(), 0);

    GAMSParameter bmult = mi.syncDb().addParameter("bmult", 0, "demand multiplier");
    // when, then
    EXPECT_EQ( mi.syncDb().getNrSymbols(), 1);
    ASSERT_TRUE( mi.syncDb().getParameter("bmult") == bmult );
}

TEST_F(TestGAMSModelInstance, testInstantiateBeforeInitializingCP) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString(getModelText());

    GAMSModelInstance mi = cp.addModelInstance();
    GAMSParameter bmult = mi.syncDb().addParameter("bmult", 0, "demand multiplier");
    GAMSOptions opt = ws.addOptions();
    opt.setAllModelTypes("cplex");
    // when, then
    EXPECT_THROW( mi.instantiate("transport use lp min z",  opt, GAMSModifier(bmult)) ,
                              GAMSException );
}

TEST_F(TestGAMSModelInstance, testSolveBeforeInstantiate) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    GAMSModelInstance mi = cp.addModelInstance();
    // when, then
    EXPECT_THROW( mi.solve(), GAMSException);

    ws.addJobFromString(getModelText()).run(cp);
    mi = cp.addModelInstance();
    // when, then
    EXPECT_THROW( mi.solve(), GAMSException);
}

TEST_F(TestGAMSModelInstance, testCopyModelInstance) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    testCleanupDirs.insert(ws.workingDirectory());
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString(getModelText()).run(cp);

    GAMSModelInstance mi = cp.addModelInstance();
    GAMSParameter bmult = mi.syncDb().addParameter("bmult", 0, "demand multiplier");
    mi.instantiate("transport use lp min z", GAMSModifier(bmult));

    // when
    GAMSModelInstance newmi = mi.copyModelInstance("new transport mi");
    // then
    ASSERT_TRUE( newmi.isValid() );
    EXPECT_EQ( mi.syncDb().getNrSymbols(), newmi.syncDb().getNrSymbols() );
    EXPECT_EQ( newmi.syncDb().getParameter("bmult").numberRecords(), mi.syncDb().getParameter("bmult").numberRecords() );
}

TEST_F(TestGAMSModelInstance, testGetCheckpoint) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    GAMSModelInstance mi = cp.addModelInstance();
    // when, then
    EXPECT_EQ( mi.checkpoint(), cp);
}

TEST_F(TestGAMSModelInstance, testGetName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();

    // when, then
    std::string cpname = "mycp";
    GAMSModelInstance mi1 = cp.addModelInstance(cpname);
    EXPECT_EQ( mi1.name(), cpname );

    // when, then
    GAMSModelInstance mi2 = cp.addModelInstance("");
    ASSERT_TRUE( mi2.name().find(defaultScratchFilePrefix) == 0 );

    // when, then
    GAMSModelInstance mi3 = cp.addModelInstance();
    ASSERT_TRUE( mi3.name().find(defaultScratchFilePrefix) == 0 );

    // when, then
    GAMSModelInstance mi4 = cp.addModelInstance();
    ASSERT_TRUE( mi4.name().find(defaultScratchFilePrefix) == 0 );
    ASSERT_TRUE( mi4.name() != mi3.name() );
}

TEST_F(TestGAMSModelInstance, testGetLogID) {
     // given
     GAMSWorkspaceInfo wsInfo("", testSystemDir);
     GAMSWorkspace ws(wsInfo);
     GAMSCheckpoint cp = ws.addCheckpoint();
     std::string cpname = "mycp";
     GAMSModelInstance mi1 = cp.addModelInstance(cpname);
     GAMSModelInstance mi2 = mi1;
     // when, then
     ASSERT_TRUE( mi1.logID() == mi2.logID() );

     GAMSCheckpoint cp3 = ws.addCheckpoint(cpname);
     GAMSModelInstance mi3 = cp3.addModelInstance();
     ASSERT_TRUE( mi3.logID() == mi1.logID());

     GAMSWorkspaceInfo anotherWsInfo("", testSystemDir);
     GAMSWorkspace anotherWs(anotherWsInfo);
     GAMSCheckpoint cp4 = anotherWs.addCheckpoint(cpname);
     GAMSModelInstance mi4 = cp4.addModelInstance();
     ASSERT_TRUE(mi4.logID() == anotherWs.logID());
     ASSERT_TRUE(mi4.logID() != mi3.logID());
}

class ParameterizedTestGAMSModelInstance
        : public ::testing::WithParamInterface<std::tuple<std::string, double, int,
                                                          std::string, int, std::string, double>>,
          public TestGAMSModelInstance {
};

INSTANTIATE_TEST_SUITE_P(testSolve,
                        ParameterizedTestGAMSModelInstance,
                        ::testing::Values (
                             //      description, modifier, SolveStatus, SolveStatusStr, ModelStatus, ModelStatusStr, obj
                             std::make_tuple("bmult=0.6", 0.6, 1, "1 Normal Completion", 1, "1 Optimal"   , 92.20499999999998),
                             std::make_tuple("bmult=0.7", 0.7, 1, "1 Normal Completion", 1, "1 Optimal"   , 107.57249999999999),
                             std::make_tuple("bmult=0.8", 0.8, 1, "1 Normal Completion", 1, "1 Optimal"   , 122.94),
                             std::make_tuple("bmult=0.9", 0.9, 1, "1 Normal Completion", 1, "1 Optimal"   , 138.3075),
                             std::make_tuple("bmult=1.0", 1.0, 1, "1 Normal Completion", 1, "1 Optimal"   , 153.675),
                             std::make_tuple("bmult=1.1", 1.1, 1, "1 Normal Completion", 4, "4 Infeasible", 169.04250000000002),
                             std::make_tuple("bmult=1.2", 1.2, 1, "1 Normal Completion", 4, "4 Infeasible", 184.40999999999997),
                             std::make_tuple("bmult=1.3", 1.3, 1, "1 Normal Completion", 4, "4 Infeasible", 199.77750000000003)
                        ));

TEST_P(ParameterizedTestGAMSModelInstance, testSolve) {
    double modifier = std::get<1>(GetParam());
    int SolveStatus = std::get<2>(GetParam());
    int ModelStatus = std::get<4>(GetParam());
    double obj      = std::get<6>(GetParam());

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
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
    ASSERT_TRUE( mi.solveStatus() == SolveStatus );
    ASSERT_TRUE( mi.modelStatus() == ModelStatus );
    ASSERT_TRUE( equals( mi.syncDb().getVariable("z").findRecord().level(), obj) );
}

INSTANTIATE_TEST_SUITE_P(testGetModelSolveStatus,
                        ParameterizedTestGAMSModelInstance,
                        ::testing::Values (
                             //      description, modifier, SolveStatus, SolveStatusStr, ModelStatus, ModelStatusStr, obj
                             std::make_tuple("bmult=0.6", 0.6, 1, "1 Normal Completion", 1, "1 Optimal"   , 92.20499999999998),
                             std::make_tuple("bmult=0.7", 0.7, 1, "1 Normal Completion", 1, "1 Optimal"   , 107.57249999999999),
                             std::make_tuple("bmult=0.8", 0.8, 1, "1 Normal Completion", 1, "1 Optimal"   , 122.94),
                             std::make_tuple("bmult=0.9", 0.9, 1, "1 Normal Completion", 1, "1 Optimal"   , 138.3075),
                             std::make_tuple("bmult=1.0", 1.0, 1, "1 Normal Completion", 1, "1 Optimal"   , 153.675),
                             std::make_tuple("bmult=1.1", 1.1, 1, "1 Normal Completion", 4, "4 Infeasible", 169.04250000000002),
                             std::make_tuple("bmult=1.2", 1.2, 1, "1 Normal Completion", 4, "4 Infeasible", 184.40999999999997),
                             std::make_tuple("bmult=1.3", 1.3, 1, "1 Normal Completion", 4, "4 Infeasible", 199.77750000000003)
                        ));

TEST_P(ParameterizedTestGAMSModelInstance, testGetModelSolveStatus) {
    double modifier            = std::get<1>(GetParam());
    int SolveStatus            = std::get<2>(GetParam());
    std::string SolveStatusStr = std::get<3>(GetParam());
    int ModelStatus            = std::get<4>(GetParam());
    std::string ModelStatusStr = std::get<5>(GetParam());

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
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
    ASSERT_TRUE( mi.solveStatus() == SolveStatus );
    // we use find here as we do not care about trailing spaces
    EXPECT_TRUE( mi.solveStatusAsString().find(SolveStatusStr) == 0 );
    ASSERT_TRUE( mi.modelStatus() == ModelStatus );
    EXPECT_TRUE( mi.modelStatusAsString().find(ModelStatusStr) == 0 );
}

INSTANTIATE_TEST_SUITE_P(testInstantiate,
                        ParameterizedTestGAMSModelInstance,
                        ::testing::Values (
                             //      description, modifier, SolveStatus, SolveStatusStr, ModelStatus, ModelStatusStr, obj
                             std::make_tuple("bmult=0.6", 0.6, 1, "1 Normal Completion", 1, "1 Optimal"   , 92.20499999999998),
                             std::make_tuple("bmult=0.7", 0.7, 1, "1 Normal Completion", 1, "1 Optimal"   , 107.57249999999999),
                             std::make_tuple("bmult=0.8", 0.8, 1, "1 Normal Completion", 1, "1 Optimal"   , 122.94),
                             std::make_tuple("bmult=0.9", 0.9, 1, "1 Normal Completion", 1, "1 Optimal"   , 138.3075),
                             std::make_tuple("bmult=1.0", 1.0, 1, "1 Normal Completion", 1, "1 Optimal"   , 153.675),
                             std::make_tuple("bmult=1.1", 1.1, 1, "1 Normal Completion", 4, "4 Infeasible", 169.04250000000002),
                             std::make_tuple("bmult=1.2", 1.2, 1, "1 Normal Completion", 4, "4 Infeasible", 184.40999999999997),
                             std::make_tuple("bmult=1.3", 1.3, 1, "1 Normal Completion", 4, "4 Infeasible", 199.77750000000003)
                        ));

TEST_P(ParameterizedTestGAMSModelInstance, testInstantiate) {
    double modifier = std::get<1>(GetParam());

    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
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
    } catch(GAMSException&) { ASSERT_TRUE(false); }
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


