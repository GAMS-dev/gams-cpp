/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2023 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2023 GAMS Development Corp. <support@gams.com>
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
#include "gamsenum.h"
#include "gamspath.h"
#include "gamsworkspaceinfo.h"

using namespace gams;

class TestGAMSWorkspaceInfo: public TestGAMSObject
{
};

TEST_F(TestGAMSWorkspaceInfo, testConstructorDefaultValue) {
    // when
    GAMSWorkspaceInfo wsInfo;
    // then
    ASSERT_TRUE(wsInfo.systemDirectory().empty());
    ASSERT_TRUE(wsInfo.workingDirectory().empty());
    EXPECT_EQ(wsInfo.debug(), GAMSEnum::DebugLevel::Off);

}

TEST_F(TestGAMSWorkspaceInfo, testSetSystemDirectory) {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSPath dir;
    // when
    wsInfo.setSystemDirectory(dir.path().toStdString());
    // then
    EXPECT_EQ(wsInfo.systemDirectory(), dir.path());
}

TEST_F(TestGAMSWorkspaceInfo, testSetWorkingDirectory) {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSPath dir;
    // when
    wsInfo.setWorkingDirectory(dir.path().toStdString());
    // then
    EXPECT_EQ(wsInfo.workingDirectory(), dir.path());
}

class ParameterizedTestSetDebug
        : public ::testing::WithParamInterface<std::tuple<std::string, GAMSEnum::DebugLevel>>,
          public TestGAMSWorkspaceInfo {
};

INSTANTIATE_TEST_SUITE_P(testSetDebug,
                        ParameterizedTestSetDebug,
                        ::testing::Values (
                             std::make_tuple("Off", GAMSEnum::DebugLevel::Off),
                             std::make_tuple("KeepFiles", GAMSEnum::DebugLevel::KeepFiles),
                             std::make_tuple("ShowLog", GAMSEnum::DebugLevel::ShowLog),
                             std::make_tuple("Verbose", GAMSEnum::DebugLevel::Verbose)
                        ));

TEST_P(ParameterizedTestSetDebug, testSetDebug) {
    GAMSWorkspaceInfo wsInfo;
    GAMSEnum::DebugLevel debug = std::get<1>(GetParam());
    // when
    wsInfo.setDebug( debug );
    // then
    EXPECT_EQ( wsInfo.debug(), debug ) ;
}
