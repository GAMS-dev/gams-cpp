/**
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

#include "gamsenum.h"
#include "gamsworkspaceinfo.h"
#include "testgamsworkspaceinfo.h"

#include <QString>

using namespace gams;

QString TestGAMSWorkspaceInfo::classname() { return "TestGAMSWorkspaceInfo"; }

void TestGAMSWorkspaceInfo::testConstructorDefaultValue()
{
    // when
    GAMSWorkspaceInfo wsInfo;
    // then
    QVERIFY(QString::fromStdString(wsInfo.systemDirectory()).isEmpty());
    QVERIFY(QString::fromStdString(wsInfo.workingDirectory()).isEmpty());
    QCOMPARE(wsInfo.debug(), GAMSEnum::DebugLevel::Off);

}

void TestGAMSWorkspaceInfo::testSetSystemDirectory()
{
    // given
    GAMSWorkspaceInfo wsInfo;
    QTemporaryDir dir;
    // when
    wsInfo.setSystemDirectory( dir.path().toStdString() );
    // then
    QCOMPARE( QString::fromStdString(wsInfo.systemDirectory()), dir.path() );
}

void TestGAMSWorkspaceInfo::testSetWorkingDirectory()
{
    // given
    GAMSWorkspaceInfo wsInfo;
    QTemporaryDir dir;
    // when
    wsInfo.setWorkingDirectory( dir.path().toStdString() );
    // then
    QCOMPARE( QString::fromStdString(wsInfo.workingDirectory()), dir.path() );
}

void TestGAMSWorkspaceInfo::testSetDebug_data()
{
    TestGAMSObject::getTestData_DebugLevel();
}

void TestGAMSWorkspaceInfo::testSetDebug()
{
    // given
    QFETCH(QString, debugLevel);

    GAMSWorkspaceInfo wsInfo;
    GAMSEnum::DebugLevel debug = static_cast<GAMSEnum::DebugLevel>(debugLevel.toInt());
    // when
    wsInfo.setDebug( debug );
    // then
    QCOMPARE( wsInfo.debug(), debug ) ;
}

QTEST_MAIN(TestGAMSWorkspaceInfo)
