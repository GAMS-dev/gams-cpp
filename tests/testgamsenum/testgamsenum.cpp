/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2021 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2021 GAMS Development Corp. <support@gams.com>
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

using namespace gams;

class TestGAMSEnum: public TestGAMSObject
{
};

TEST_F(TestGAMSEnum, testGamsReturnCodeToText) {
    // given
    const int valid[] = {0,1,2,3,4,5,6,7,8,9,10,11
                        ,109,110, 112,113,114,115
                        ,400,401,402, 404,405,406,407,408,409,410,411,412
                        ,909
                        ,1000,2000,3000,4000,5000};
    std::string invalid("InvalidReturnCode");
    for (int i = 0; i < 6001; ++i) {
        // when
        std::string code = GAMSEnum::text(static_cast<GAMSEnum::GAMSExitCode>(i));
        // then
        if (std::find(std::begin(valid), std::end(valid), i) != std::end(valid)) {
            ASSERT_TRUE(invalid.compare(code) != 0);
        } else {
            EXPECT_EQ(code, invalid);
        }
    }
}
