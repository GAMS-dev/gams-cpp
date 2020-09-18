/*
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

#ifndef TESTGAMSCPP_H
#define TESTGAMSCPP_H

#include "gamsdatabase.h"
#include "gamsjob.h"
#include "gamsworkspace.h"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <cmath>
#include <limits>
#include <QObject>
#include <QDir>
#include <QTest>
#include <QSet>

class TestGAMSObject: public QObject
{
  Q_OBJECT
  private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
  protected:
     virtual QString classname() = 0;

     static std::string getShortModelText();
     static std::string getLongModelText();
     static std::string getDataText();

     std::string defaultScratchFilePrefix = "_gams_cpp_";
     double defaultUNDEF = 1.0E300;
     double defaultNA    = std::numeric_limits<double>::quiet_NaN();
     double defaultPINF  = std::numeric_limits<double>::infinity();
     double defaultMINF  = - std::numeric_limits<double>::infinity();
     double defaultEPS   = std::numeric_limits<double>::min();

     double tolerance = 2e-6;

     bool equals(double actual, double expect) {
         return std::abs(actual - expect) <= tolerance ;
     }
     void testDir(QString dir);
     void testJobBeforeRun(gams::GAMSJob job, gams::GAMSWorkspace ws);
     void testEmptyDatabase(gams::GAMSDatabase db, gams::GAMSWorkspace ws);

     void getTestData_DebugLevel();
     void getTestData_ModelLibraries();
     void getTestData_InvalidModelLibraries();
     void getTestData_SpecialValues();
     void getTestData_TransportModel(gams::GAMSDatabase db);
     void getTestData_Set_plants_i(gams::GAMSDatabase db);
     void getTestData_Set_markets_j(gams::GAMSDatabase db);
     void getTestData_Parameter_capacity_a(gams::GAMSDatabase db);
     void getTestData_Parameter_demand_b(gams::GAMSDatabase db);
     void getTestData_Parameter_distance_d(gams::GAMSDatabase db);
     void getTestData_Parameter_freightcost_f(gams::GAMSDatabase db);
     void getTestData_Database_DomainViolations(gams::GAMSDatabase db);

     QDir testSystemDir;
     QString testGAMSVersion;
     QString testAPIVersion;
     QString testDebugLevel;
     QSet<QString> testCleanupDirs;

     int tests_Executed;
     int tests_Failed;

};

#endif // TESTGAMSCPP_H
