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

#ifndef TESTGAMSMODELINSTANCEOPT_H
#define TESTGAMSMODELINSTANCEOPT_H

#include "testgamsobject.h"

class TestGAMSModelInstanceOpt: public TestGAMSObject
{
  Q_OBJECT
  private slots:
    void testDefaultConstructor();
    void testConstructor();
    void testAssignmentOperator();
    void testEqualToOperator();
    void testGetSetSolver_data();
    void testGetSetSolver();
    void testGetSetOptFile_data();
    void testGetSetOptFile();
    void testGetSetNoMatchLimit_data();
    void testGetSetNoMatchLimit();
    void testGetSetDebug_data();
    void testGetSetDebug();
protected:
  QString classname();

};
#endif // TESTGAMSMODELINSTANCEOPT_H
