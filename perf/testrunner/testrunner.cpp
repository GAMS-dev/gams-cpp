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

#include "testrunner.h"
#include "testperformance.h"

#include <iostream>
#include <QObject>
#include <QDebug>
#include <QTest>
#include <QMetaObject>

using namespace std;
using namespace gams;

vector<string> testList = {};

int main(int argc, char *argv[]) {

    qInfo() << "\n- GAMS Performance Test Suite -";
    qInfo() << "Type 'testrunner help' to see a list of available tests.";

    // collect list of tests
    TestPerformance* testClassObject = new TestPerformance();
    const QMetaObject* qmo = testClassObject->metaObject();
    QStringList methods;

    for (int i = qmo->methodOffset(); i < qmo->methodCount(); ++i) {
        string meth = qmo->method(i).name().toStdString();

        if (meth.compare(0, 4, "test") == 0) { // TODO: exclude $NAME_data() functions
            testList.push_back( meth );
        }
    }

    // no arguments given. run all tests, neatly executed by QTest.
    if (argc == 1) {
        qInfo() << "No arguments specified, running all performance tests.";
        return QTest::qExec(testClassObject, argc, argv);

    // tests to run specified via cl argument(s), try to run each test manually. no QTest support.
    } else {

        // has args, decide which tests to run
        for (int i = 1; i < argc; i++) {

            if ( strcmp(argv[i], "help") == 0) {
                qInfo() << "Specify one or more test names, seperated by spaces. Available tests are:";
                for(string s : testList) {
                    qInfo() << s.c_str();
                }

            } else {

                // on first test
                if (i == 1) {
                    // check if there's an init function
                    int init = qmo->indexOfMethod("initTestCase()");
                    if(init != -1) {
                        qInfo() << "Found initTestCase function. Executing...";
                        qmo->invokeMethod(testClassObject, "initTestCase", Qt::DirectConnection);
                        qInfo() << "Test class initialized.";
                    }
                }

                if ( std::find( testList.begin(), testList.end(), argv[i] ) != testList.end() ) {
                    // TODO: run $NAME_data() function for test setup, if existing

                    qInfo() << "Running " << argv[i];
                    qmo->invokeMethod(testClassObject, argv[i], Qt::DirectConnection);
                } else {
                    qInfo() << "Could not find'" << argv[i] << "'";
                }
            }
        }

    }
}

