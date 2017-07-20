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

#ifndef TESTGAMSDOMAIN_H
#define TESTGAMSDOMAIN_H

#include "testgamsobject.h"

class TestGAMSDomain: public TestGAMSObject
{
  Q_OBJECT
  private slots:
    void testConstructor_Set();
    void testConstructor_StringRelaxedName();
    void testConstructor_CharPtrRelaxedName();
    void testCopyConstructor();
    void testAssignmentOperator_Set();
    void testAssignmentOperator_StringReleaxedName();
    void testAssignmentOperator_CharPtrReleaxedName();
    void testAssignmentOperator_GAMSDomain();
    void testGetName();
    void testGetSet();
    void testIsRelaxed();
protected:
  QString classname();

};

#endif // TESTGAMSDOMAIN_H
