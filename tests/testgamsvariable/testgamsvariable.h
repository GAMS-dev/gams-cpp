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

#ifndef TESTGAMSVARIABLE_H
#define TESTGAMSVARIABLE_H

#include "testgamsobject.h"

class TestGAMSVariable: public TestGAMSObject
{
  Q_OBJECT
  private slots:
    void testDefaultConstructor();
    void testCopyConstructor();
    void testCopyConstructor_IncorrectType();
    void testIterator();
    void testAssignmentOperator();
    void testAssignmentOperator_IncorrectType();
    void testGetFirstRecord();
    void testGetFirstRecordSlice();
    void testGetFirstRecordSlice_InvalidKeys_IncorrectDimension();
    void testGetLastRecord();
    void testGetLastRecordSlice();
    void testGetLastRecordSlice_InvalidKeys_IncorrectDimension();
    void testFindRecord();
    void testFindRecord_InsensitiveCaseKeys();
    void testAddRecord();
    void testAddRecord_DuplicatedKeys_IncorrectDimension();
    void testMergeExistingRecord();
    void testMergeNonExistingRecord();
    void testGetVarType();
  protected:
    QString classname();
};
#endif // TESTGAMSVARIABLE_H