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

#ifndef TESTGMD_H
#define TESTGMD_H

#include <QTest>
#include <QtTest>
#include <QObject>
#include <QString>

#include "gamsset.h"
#include "gclgms.h"
#include "gmdcc.h"

class TestGMD: public QObject
{
  Q_OBJECT
  private Q_SLOTS:
    void initTestCase();

    void testAddingSymbols();
    void testAddingDatabases();
    void testIteratingRecordsPointer();

  protected:
    QString classname();

  private:
    void* hugeTestSet;
    gmdHandle_t gmd;
    gdxStrIndex_t strIndex;
    gdxStrIndexPtrs_t strIndexPtrs;
    char msg[GMS_SSSIZE];

    // Number of items for tests that need a huge amount of elements
    const int NR_ITEMS_HUGE_TEST = 1500000;

    // Number of items for tests that need a big amount of elements
    const int NR_ITEMS_BIG_TEST = 1500;
};

#endif // TESTGMD_H
