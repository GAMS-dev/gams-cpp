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

#include "gamsmodelinstanceopt.h"
#include "testgamsmodelinstanceopt.h"

using namespace gams;

QString TestGAMSModelInstanceOpt::classname()  { return "TestGAMSModelInstanceOpt"; }

void TestGAMSModelInstanceOpt::testDefaultConstructor()  {
    GAMSModelInstanceOpt miopt;
    ASSERT_TRUE( miopt.solver().empty() );
    QCOMPARE( miopt.optFile(), -1 );
    QCOMPARE( miopt.noMatchLimit(), 0);
    ASSERT_TRUE( ! miopt.debug() );
}

void TestGAMSModelInstanceOpt::testConstructor()  {
    GAMSModelInstanceOpt miopt("x", 1, 1, true);
    QCOMPARE( miopt.solver().c_str(), "x" );
    QCOMPARE( miopt.noMatchLimit(), 1 );
    QCOMPARE( miopt.optFile(), 1 );
    ASSERT_TRUE( miopt.debug() );
}

void TestGAMSModelInstanceOpt::testAssignmentOperator()  {
    GAMSModelInstanceOpt miopt1;

    GAMSModelInstanceOpt miopt = miopt1;
    ASSERT_TRUE( miopt == miopt1 );

    GAMSModelInstanceOpt miopt2("x", 1, 0, true);
    miopt = miopt2;
    ASSERT_TRUE( miopt == miopt2 );
}

void TestGAMSModelInstanceOpt::testEqualToOperator()  {
    GAMSModelInstanceOpt miopt1;

    GAMSModelInstanceOpt miopt = miopt1;
    QCOMPARE( miopt.solver(), miopt1.solver() );
    QCOMPARE( miopt.noMatchLimit(), miopt1.noMatchLimit() );
    QCOMPARE( miopt.optFile(), miopt1.optFile() );
    QCOMPARE( miopt.debug(), miopt1.debug() );
    ASSERT_TRUE( miopt == miopt1 );

    GAMSModelInstanceOpt miopt2("x", 1, 0, true);
    miopt = miopt2;
    QCOMPARE( miopt.solver(), miopt2.solver() );
    QCOMPARE( miopt.noMatchLimit(), miopt2.noMatchLimit() );
    QCOMPARE( miopt.optFile(), miopt2.optFile() );
    QCOMPARE( miopt.debug(), miopt2.debug() );
    ASSERT_TRUE( miopt == miopt2 );
}

void TestGAMSModelInstanceOpt::testGetSetSolver_data()  {
    QTest::addColumn<QString>("solver");

    QTest::newRow("empty")        << "";
    QTest::newRow("x")            << "x";
    QTest::newRow("conopt")       << "conopt";
}

void TestGAMSModelInstanceOpt::testGetSetSolver()  {
    QFETCH(QString, solver);

    GAMSModelInstanceOpt miopt1(solver.toStdString(), -1, 0, false);
    QCOMPARE(  miopt1.solver(), solver.toStdString() );

    GAMSModelInstanceOpt miopt2;
    miopt2.setSolver( solver.toStdString() );
    QCOMPARE(  miopt2.solver(), solver.toStdString() );
}

void TestGAMSModelInstanceOpt::testGetSetOptFile_data()  {
    QTest::addColumn<int>("optfile");
    QTest::newRow("minus")        << -1;
    QTest::newRow("zero")         << 0;
    QTest::newRow("one")          << 1;
    QTest::newRow("onehundread")  << 100;
}

void TestGAMSModelInstanceOpt::testGetSetOptFile()  {
    QFETCH(int, optfile);

    GAMSModelInstanceOpt miopt1("x", optfile, 0, false);
    QCOMPARE(  miopt1.optFile(), optfile );

    GAMSModelInstanceOpt miopt2;
    miopt2.setOptFile( optfile );
    QCOMPARE(  miopt2.optFile(), optfile );
}

void TestGAMSModelInstanceOpt::testGetSetNoMatchLimit_data()  {
    QTest::addColumn<int>("limit");
    QTest::newRow("minus")        << -1;
    QTest::newRow("zero")         << 0;
    QTest::newRow("one")          << 1;
    QTest::newRow("onehundread")  << 100;
}

void TestGAMSModelInstanceOpt::testGetSetNoMatchLimit()  {
    QFETCH(int, limit);

    GAMSModelInstanceOpt miopt1("x", 1, limit, false);
    QCOMPARE(  miopt1.noMatchLimit(), limit );

    GAMSModelInstanceOpt miopt2;
    miopt2.setNoMatchLimit( limit );
    QCOMPARE(  miopt2.noMatchLimit(), limit );
}

void TestGAMSModelInstanceOpt::testGetSetDebug_data()  {
    QTest::addColumn<bool>("debug");
    QTest::newRow("true")   << true;
    QTest::newRow("false")   << true;
}

void TestGAMSModelInstanceOpt::testGetSetDebug()  {
    QFETCH(bool, debug);

    GAMSModelInstanceOpt miopt1("x", 1, 0, debug);
    ASSERT_TRUE( debug ? miopt1.debug() : ! miopt1.debug() );

    miopt1.setDebug( debug );
    ASSERT_TRUE( debug ? miopt1.debug() : ! miopt1.debug() );

    GAMSModelInstanceOpt miopt2;
    miopt2.setDebug( debug );
    ASSERT_TRUE( debug ? miopt2.debug() : ! miopt2.debug() );
}

QTEST_MAIN(TestGAMSModelInstanceOpt)
