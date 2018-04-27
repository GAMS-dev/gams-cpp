/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
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

#ifndef TESTGAMSWORKSPACE_H
#define TESTGAMSWORKSPACE_H

#include "testgamsobject.h"

class TestGAMSWorkspace: public TestGAMSObject
{
  Q_OBJECT
  private slots:
    void testDefaultConstructor();
    void testCopyConstructor();
    void testConstructor_WorkspaceInfo();
    void testConstructor_EmptyWorkspaceInfo();
    void testConstructor_WorkingDirSystemDirAndDebug();
    void testConstructor_WorkingDir();
    void testConstructor_WorkingDirSystemDir();
    void testConstructor_DebugLevel_data();
    void testConstructor_DebugLevel();
    void testConstructorFromOccupiedWorkingDirectory();
    void testGetVersion();
    void testGetAPIVersion();
    void testGetScratchFilePrefix();
    void testSetScratchFilePrefix_data();
    void testSetScratchFilePrefix();
    void testGetMyEPS();
    void testSetMyEPS();
    void testAddDatabaseFromGDX1_data();
    void testAddDatabaseFromGDX1();
    void testAddDatabaseFromGDX2_data();
    void testAddDatabaseFromGDX2();
    void testAddDatabaseFromGDX3_data();
    void testAddDatabaseFromGDX3();
    void testAddDatabase();
    void testAddDatabase_Name();
    void testAddDatabase_NameAndModelName();
    void testAddDatabase_Source();
    void testAddDatabase_SourceAndName();
    void testAddDatabase_SourceNameAndModelName();
    void testAddCheckpoint();
    void testAddCheckpoint_Name_data();
    void testAddCheckpoint_Name();

    void testAddJobFromFile();
    void testAddJobFromFile_JobName();
    void testAddJobFromFile_SameJobName();
    void testAddJobFromFile_Checkpoint();
    void testAddJobFromFile_CheckpointAndJobName();
    void testAddJobFromEmptyString();
    void testAddJobFromString();
    void testAddJobFromString_Checkpoint();
    void testAddJobFromString_JobName();
    void testAddJobFromString_SameJobName();
    void testAddValidJobFromString_CheckpointAndJobName();
    void testAddInvalidJobFromString_CheckpointAndJobName();
    void testAddJobFromApiLibrary_data();
    void testAddJobFromApiLibrary();
    void testAddJobFromGamsLibrary_data();
    void testAddJobFromGamsLibrary();
    void testAddJobFromDataLibrary_data();
    void testAddJobFromDataLibrary();
    void testAddJobFromEmpLibrary_data();
    void testAddJobFromEmpLibrary();
    void testAddJobFromFinLibrary_data();
    void testAddJobFromFinLibrary();
    void testAddJobFromNoaLibrary_data();
    void testAddJobFromNoaLibrary();
//    void testAddJobFromPsoptLibrary_data();
//    void testAddJobFromPsoptLibrary();
    void testAddJobFromTestLibrary_data();
    void testAddJobFromTestLibrary();
    void testAddJobFromLibrary_Checkpoint_data();
    void testAddJobFromLibrary_Checkpoint();
    void testAddJobFromLibrary_CheckpointAndJobName_data();
    void testAddJobFromLibrary_CheckpointAndJobName();

    void testAddOptions();
    void testAddOptions_OptFrom();
    void testAddOptions_OptFile_data();
    void testAddOptions_OptFile();
    void testRetrievingModelFromLibrary_data();
    void testRetrievingModelFromLibrary();
    void testRetrievingInvalidModelFromLibrary_data();
    void testRetrievingInvalidModelFromLibrary();
    void testGetWorkingDirectory();
    void testGetSystemDirectory();
    void testEqualToOperator();
    void testNotEqualToOperator();
    void testGetDebug();
    void testGetLogID();
    void testGetSpecValues_data();
    void testGetSpecValues();

protected:
    QString classname();
};
#endif // TESTGAMSWORKSPACE_H
