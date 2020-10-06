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

#include "testgamsplatform.h"
#include "gamsplatform.h"
#include "gamspath.h"

#include <thread>
#include <QtTest>

using namespace gams;

class TestGAMSPlatform: public TestGAMSObject
{
};

void TestGAMSPlatform::testFindGams()
{
    std::string gams = GAMSPlatform::findGams(0);
    QVERIFY2(!gams.empty(), "GAMS not found");
}

void TestGAMSPlatform::testRunProcessParallel()
{
    std::string output;
    std::string gams = GAMSPlatform::findGams(0);
    GAMSPath g = GAMSPlatform::findGams(0) + "/gams.exe";

//    qDebug() << "g2" << g2.string().c_str(); // rogo: delete
    std::thread t = GAMSPlatform::runProcessParallel(g.string(), "clad.exe");
    qDebug() << t.native_handle(); // rogo: delete
//    t.detach();

    qDebug() << "joinable" << t.joinable(); // rogo: delete
    QTest::qWait(1000);
    qDebug() << "joinabl2" << t.joinable(); // rogo: delete

    t.~thread();
}


