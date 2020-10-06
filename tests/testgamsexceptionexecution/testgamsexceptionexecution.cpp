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

#include "testgamsexceptionexecution.h"
#include "gamsexceptionexecution.h"

using namespace gams;

class TestGAMSExceptionExecution: public TestGAMSObject
{
};

TEST_F(TestGAMSExceptionExecution, testConstructor_string) {
    std::string what = "testConstructor_string()";
    try {
        throw GAMSExceptionExecution(what, 13);
    } catch(GAMSExceptionExecution & e) {
        EXPECT_EQ( e.rc(), 13 );
        EXPECT_EQ( e.what(), what.c_str());
    }

    try{
       try {
            throw GAMSException(what);
       } catch(GAMSExceptionExecution & ) {
           QFAIL("do not expect GAMSExceptionExecution to be thrown");
       }
    } catch(GAMSException & e) {
        EXPECT_EQ( e.what(), what.c_str());
    }
}

TEST_F(TestGAMSExceptionExecution, testConstructor_charptr) {
    try {
        throw GAMSExceptionExecution("testConstructor_charptr()", 17);
    } catch(GAMSExceptionExecution & e) {
        EXPECT_EQ( e.rc(), 17 );
        EXPECT_EQ( e.what(), "testConstructor_charptr()");
    }

    try{
       try {
            throw GAMSException("testConstructor_charptr()");
       } catch(GAMSExceptionExecution & ) {
           QFAIL("do not expect GAMSExceptionExecution to be thrown");
       }
    } catch(GAMSException & e) {
        EXPECT_EQ( e.what(), "testConstructor_charptr()");
    }
}

TEST_F(TestGAMSExceptionExecution, testGetRc) {
    try {
        throw GAMSExceptionExecution("testGetRc()", 13);
    } catch(GAMSExceptionExecution & e) {
        EXPECT_EQ( e.rc(), 13 );
    }
    try {
        throw GAMSExceptionExecution("testGetRc()", 0);
    } catch(GAMSExceptionExecution & e) {
        EXPECT_EQ( e.rc(), 0 );
    }
    try {
        throw GAMSExceptionExecution("testGetRc()", -1);
    } catch(GAMSExceptionExecution & e) {
        EXPECT_EQ( e.rc(), -1 );
    }
}


