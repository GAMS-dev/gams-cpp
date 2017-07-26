/*
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

#ifndef TESTGAMSDATABASE_H
#define TESTGAMSDATABASE_H

#include "testgamsobject.h"

class TestGAMSDatabase: public TestGAMSObject
{
  Q_OBJECT
  private slots:
    void testDefaultConstructor();
    void testAssignmentOperator();
    void testOutOfScopeDatabaseAssignement();
    void testIsValid();
    void testBegin();
    void testEnd();
    void testGetNrSymbols();
    void testSetSuppressAutoDomainChecking_data();
    void testSetSuppressAutoDomainChecking();
    void testGetSuppressAutoDomainChecking();
    void testGetName();
    void testGetWorkspace();
    void testDoExport();
    void testDoExportWithName();
    void testDoExportWithDomainViolation();
    void testCheckDomains();

    void testGetSymbol_data();
    void testGetSymbol();

    void testAddSet();
    void testAddSet_NonPositiveDimension();
    void testAddSet_ExceedingMaxDimension();
    void testAddSet_DuplicateIdentifier();
    void testAddSet_InvalidStringIdentifier();

    void testAddParameter();
    void testAddParameter_NegativeDimension();
    void testAddParameter_ExceedingMaxDimension();
    void testAddParameter_DuplicateIdentifier();
    void testAddParameter_InvalidStringIdentifier();

    void testAddVariable();
    void testAddVariable_NegativeDimension();
    void testAddVariable_ExceedingMaxDimension();
    void testAddVariable_DuplicateIdentifier();
    void testAddVariable_InvalidStringIdentifier();

    void testAddEquation();
    void testAddEquation_NegativeDimension();
    void testAddEquation_ExceedingMaxDimension();
    void testAddEquation_DuplicateIdentifier();
    void testAddEquation_InvalidStringIdentifier();

    void testGetEquation();

    void testGetDatabaseDVs();
    void testGetDatabaseDVs_MaxViolation();

    void testIterator();

    void testEqualToOperator();
    void testNotEqualToOperator();
    void testClear();
    void testClearOutOfScopedDatabase();
    void testLogID();

protected:
    QString classname();
};
#endif // TESTGAMSDATABASE_H
