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

#include "testgamsobject.h"
#include "gamscheckpoint.h"
#include "gamsexception.h"
#include "gamsset.h"
#include "gamsparameter.h"
#include "gamsoptions.h"
#include "gamsworkspace.h"
#include "gamsworkspaceinfo.h"
#include "gamsexception.h"
#include "gamspath.h"
#include "gamsversion.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits.h>

#ifdef _WIN32
#include <wchar.h>
#define NOMINMAX
#include <windows.h>
#endif

using namespace gams;

class TestGAMSWorkspace: public TestGAMSObject
{
};

// TODO(RG): is this test expected to fail?!
TEST_F(TestGAMSWorkspace, testDefaultConstructor) {
    try {
        GAMSWorkspace ws;

        // then
        TestGAMSObject::testDir( ws.systemDirectory() );
        std::string sdir = ws.systemDirectory();
        EXPECT_EQ( testSystemDir, sdir );
        TestGAMSObject::testDir( sdir );

        // TODO(RG): what is supposed to have happened here in between? someone should take a look at this
        TestGAMSObject::testDir( ws.workingDirectory() );
        EXPECT_EQ(ws.debug(), GAMSEnum::DebugLevel::Off);

    } catch (GAMSException &e) {
        FAIL() << e.what();
    }
}


TEST_F(TestGAMSWorkspace, testCopyConstructor) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws1(wsInfo);
    // when
    GAMSWorkspace ws2(ws1);
    // then
    EXPECT_EQ(ws1.systemDirectory(), ws2.systemDirectory());
    EXPECT_EQ(ws1.workingDirectory(), ws2.workingDirectory());
    EXPECT_EQ(ws1.debug(), ws2.debug());
    EXPECT_EQ(ws1, ws2);
}

TEST_F(TestGAMSWorkspace, testConstructor_EmptyWorkspaceInfo) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    // when
    GAMSWorkspace ws(wsInfo);
    // then
    TestGAMSObject::testDir( ws.systemDirectory() );
    TestGAMSObject::testDir( ws.workingDirectory() );
    EXPECT_EQ( ws.debug(), GAMSEnum::DebugLevel::Off );

    GAMSPath(ws.workingDirectory()).remove();
}

TEST_F(TestGAMSWorkspace, testConstructor_WorkspaceInfo) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    // when
    GAMSWorkspace ws(wsInfo);
    // then
    TestGAMSObject::testDir( ws.systemDirectory() );
    TestGAMSObject::testDir( ws.workingDirectory() );

    EXPECT_EQ( GAMSPath(ws.workingDirectory()), GAMSPath(ws.workingDirectory()) );
    EXPECT_EQ( ws.debug(), GAMSEnum::DebugLevel::Off );
}

TEST_F(TestGAMSWorkspace, testConstructor_WorkingDirSystemDirAndDebug) {
    // given
    std::string wdir = std::filesystem::current_path().string();
    std::string sdir = testSystemDir;
    // when
    GAMSWorkspace ws( wdir, sdir, GAMSEnum::DebugLevel::KeepFiles );
    // then
    TestGAMSObject::testDir( ws.workingDirectory() );
    EXPECT_EQ(GAMSPath(ws.workingDirectory()), GAMSPath(wdir));

    TestGAMSObject::testDir( ws.systemDirectory() );
    EXPECT_EQ(GAMSPath(ws.systemDirectory()), GAMSPath(sdir));

    EXPECT_EQ(ws.debug(), GAMSEnum::DebugLevel::KeepFiles);
}

TEST_F(TestGAMSWorkspace, testConstructor_WorkingDir) {
    // given
    std::string wdir = "";
    // when
    GAMSWorkspace ws(wdir, testSystemDir);
    // then
    TestGAMSObject::testDir( ws.workingDirectory() );
    EXPECT_EQ(ws.debug(), GAMSEnum::DebugLevel::Off);
}

TEST_F(TestGAMSWorkspace, testConstructor_WorkingDirSystemDir) {
    // given
    std::string wdir = std::filesystem::current_path().string();
    std::string sdir = testSystemDir;
    // when
    GAMSWorkspace ws(wdir, sdir);
    // then
    TestGAMSObject::testDir(ws.workingDirectory());
    EXPECT_EQ(ws.workingDirectory(), wdir);

    TestGAMSObject::testDir(ws.systemDirectory());
    EXPECT_EQ(ws.systemDirectory(), sdir);

    EXPECT_EQ(ws.debug(), GAMSEnum::DebugLevel::Off);
}

class ParameterizedTestConstructor_DebugLevel
        : public ::testing::WithParamInterface<std::tuple<const char*, GAMSEnum::DebugLevel>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testConstructor_DebugLevel,
                        ParameterizedTestConstructor_DebugLevel,
                        ::testing::Values (
                            std::make_tuple("Off",       GAMSEnum::DebugLevel::Off),
                            std::make_tuple("KeepFiles", GAMSEnum::DebugLevel::KeepFiles),
                            std::make_tuple("ShowLog",   GAMSEnum::DebugLevel::ShowLog),
                            std::make_tuple("Verbose",   GAMSEnum::DebugLevel::Verbose)
                        ));


TEST_P(ParameterizedTestConstructor_DebugLevel, testConstructor_DebugLevel) {
    // TODO add case when "GAMSOOAPIDEBUG" has been set
    // given
    std::string debugLevel = std::get<0>(GetParam());
    GAMSEnum::DebugLevel debugLevelEnum = std::get<1>(GetParam());

    std::string dir;
    {
        GAMSWorkspace ws("", testSystemDir, debugLevelEnum);
        dir = ws.workingDirectory();
    }

    // then
    switch(debugLevelEnum) {
       case GAMSEnum::DebugLevel::Off:
           ASSERT_FALSE( GAMSPath(dir).exists() );
           break;
       case GAMSEnum::DebugLevel::KeepFiles:
           ASSERT_TRUE( GAMSPath(dir).exists() );
           /* TODO QEXPECT_FAIL("", "More tests to be implemented ", Abort); ASSERT_TRUE(false); */
           break;
      case GAMSEnum::DebugLevel::ShowLog:
           ASSERT_TRUE( GAMSPath(dir).exists() );
           /* TODO QEXPECT_FAIL("", "More tests to be implemented ", Abort); ASSERT_TRUE(false); */
           break;
      case GAMSEnum::DebugLevel::Verbose:
           ASSERT_TRUE( GAMSPath(dir).exists() );
           /* TODO QEXPECT_FAIL("", "More tests to be implemented ", Abort); ASSERT_TRUE(false); */
           break;
      default:
           break;
    }
    GAMSPath qdir(dir);
    if (qdir.exists())
        qdir.remove();
}

TEST_F(TestGAMSWorkspace, testConstructorFromOccupiedWorkingDirectory) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    wsInfo.setWorkingDirectory(std::filesystem::current_path().string());
    GAMSWorkspace ws1(wsInfo);
    // when creating workspace from working directory that is already in use, then
    EXPECT_THROW( GAMSWorkspace ws2(wsInfo), GAMSException);
}

TEST_F(TestGAMSWorkspace, testGetGAMSVersion) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    EXPECT_EQ(testGAMSVersion, ws.version());

    std::vector<std::string> testGAMSVersionList;
    std::stringstream sstream(testGAMSVersion);
    std::string item;
    while (std::getline(sstream, item, '.'))
        testGAMSVersionList.push_back(item);

    if (testGAMSVersionList.size() != 3)
        FAIL() << "The GAMSVersion shall consist of three numbers (MAJOR.MINOR.PATCH)";

    EXPECT_EQ(ws.majorRelNumber(), std::stoi(testGAMSVersionList[0]));
    EXPECT_EQ(ws.minorRelNumber(), std::stoi(testGAMSVersionList[1]));
    EXPECT_EQ(ws.goldRelNumber(),  std::stoi(testGAMSVersionList[2]));
}

TEST_F(TestGAMSWorkspace, testGetAPIVersion) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    EXPECT_EQ(testAPIVersion, ws.apiVersion());
    std::vector<std::string> testAPIVersionList;
    std::stringstream sstream(testAPIVersion);
    std::string item;
    while (std::getline(sstream, item, '.'))
        testAPIVersionList.push_back(item);

    EXPECT_EQ(ws.apiMajorRelNumber(), std::stoi(testAPIVersionList[0]));
    EXPECT_EQ(ws.apiMinorRelNumber(), std::stoi(testAPIVersionList[1]));
    EXPECT_EQ(ws.apiGoldRelNumber(),  std::stoi(testAPIVersionList[2]));
}

TEST_F(TestGAMSWorkspace, testGetScratchFilePrefix) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    ASSERT_TRUE( ! ws.scratchFilePrefix().empty() );
    EXPECT_EQ( ws.scratchFilePrefix(), defaultScratchFilePrefix );
}

class ParameterizedTestSetScratchFilePrefix
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testSetScratchFilePrefix,
                        ParameterizedTestSetScratchFilePrefix,
                        ::testing::Values (
                             //              description,           prefix name, valid
                            std::make_tuple("NormalPrefix"        , "myPrefix" , true ),
                            std::make_tuple("PrefixWithUnderScore", "_myPrefix", true ),
                            std::make_tuple("PrefixWithWhiteSpace", "my Prefix", false),
                            std::make_tuple("EmptyStringPrefix"   , ""         , true )
                        ));

TEST_P(ParameterizedTestSetScratchFilePrefix, testSetScratchFilePrefix) {
    // given
    std::string prefixname = std::get<1>(GetParam());
    bool valid = std::get<2>(GetParam());


    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    if (valid) {
       // when
       ws.setScratchFilePrefix(prefixname);
       // then
       EXPECT_EQ( ws.scratchFilePrefix(), prefixname);
    } else {
        // then
       EXPECT_THROW( ws.setScratchFilePrefix(prefixname), GAMSException );
    }
}

TEST_F(TestGAMSWorkspace, testGetMyEPS) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    EXPECT_EQ( ws.myEPS(),  defaultEPS );
}

TEST_F(TestGAMSWorkspace, testSetMyEPS) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    double myEPS = 5e-300;
    // when
    ws.setMyEPS (myEPS );
    EXPECT_EQ(  ws.myEPS(), myEPS );
}

class ParameterizedTestAddDatabaseFromGDX1
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddDatabaseFromGDX1,
                        ParameterizedTestAddDatabaseFromGDX1,
                        ::testing::Values (
                             //              description,           gdxfilename,                      fromDataLib,   valid
                            std::make_tuple("DemandData.gdx"         , "DemandData.gdx"             , "GDXINExample1", true),
                            std::make_tuple("NonExistingFileName.gdx", "ThisGDXFileDoesNotExist.gdx", "", false),
                            std::make_tuple("EmptyStringFileName"    , ""                           , "", false)
                        ));

TEST_P(ParameterizedTestAddDatabaseFromGDX1, testAddDatabaseFromGDX1) {
    // given
    std::string gdxfilename = std::get<1>(GetParam());
    std::string fromDataLib = std::get<2>(GetParam());
    bool valid = std::get<3>(GetParam());


    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    if (valid) {
        std::string s = fromDataLib;
        ws.dataLib(s);
        // when
        try {
            GAMSDatabase db = ws.addDatabaseFromGDX(gdxfilename );

            EXPECT_EQ( db.getNrSymbols(), 2 );
            EXPECT_EQ( db.getParameter("demand").numberRecords(), 3);
            EXPECT_EQ( db.getSet("markets").numberRecords(), 3);
        } catch(GAMSException& e) {
            FAIL() << "Unexpected GAMSException raised by: " << e.what();
        }
    } else {
        // when, then
        EXPECT_THROW(ws.addDatabaseFromGDX(gdxfilename) , GAMSException);
    }
}

class ParameterizedTestAddDatabaseFromGDX2
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, std::string, int>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddDatabaseFromGDX2,
                        ParameterizedTestAddDatabaseFromGDX2,
                        ::testing::Values (
                             //              description,           gdxfilename,                      fromDataLib,   numberOfSymbols
                            std::make_tuple("db_ExecTimeReadTrnsportGDX", "Trnsport.gdx"  , "ExecTimeReadTrnsportGDX", 12),
                            std::make_tuple("db_DemandData"             , "DemandData.gdx", "GDXINExample2"          ,  2)
                        ));

TEST_P(ParameterizedTestAddDatabaseFromGDX2, testAddDatabaseFromGDX2) {
    // given
    std::string description = std::get<0>(GetParam());
    std::string gdxfilename = std::get<1>(GetParam());
    std::string fromDataLib = std::get<2>(GetParam());
    int numberOfSymbols     = std::get<3>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    ws.dataLib( fromDataLib );
    try {
       // when
       GAMSDatabase db = ws.addDatabaseFromGDX(gdxfilename, description);
       EXPECT_EQ(db.name().c_str(), description);
       EXPECT_EQ(db.getNrSymbols(), numberOfSymbols);
    } catch(GAMSException& e) {
       std::cerr << "error adding db from "+ gdxfilename + " at "+ ws.workingDirectory()+"." << std::endl;
       FAIL() << "Unexpected GAMSException thrown: " << e.what();
    }
}

class ParameterizedTestAddDatabaseFromGDX3
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, std::string, int>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddDatabaseFromGDX3,
                        ParameterizedTestAddDatabaseFromGDX3,
                        ::testing::Values (
                             //              description,           gdxfilename,       fromDataLib,       numberOfSymbols
                            std::make_tuple("db_GDXCOPYExample19", "UNStatistics.gdx", "GDXCOPYExample19", 8),
                            std::make_tuple("db_Trnsport"        , "Trnsport.gdx"    , "GDXINExample3"   ,12)
                        ));

TEST_P(ParameterizedTestAddDatabaseFromGDX3, testAddDatabaseFromGDX3) {
    // given
    std::string description = std::get<0>(GetParam());
    std::string gdxfilename = std::get<1>(GetParam());
    std::string fromDataLib = std::get<2>(GetParam());
    int numberOfSymbols     = std::get<3>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    ws.dataLib( fromDataLib );
    try {
       // when
       std::string inModelName = "myModel";
       GAMSDatabase db = ws.addDatabaseFromGDX(gdxfilename, description, inModelName);
       EXPECT_EQ( db.name().c_str(),  description);
       // @todo EXPECT_EQ( db.inModelName(), inModelName );
       EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
    } catch(GAMSException& e) {
        std::cerr << "error adding db from "+ gdxfilename + " at "+ ws.workingDirectory()+"." << std::endl;
       FAIL() << "Unexpected GAMSException thrown: " << e.what();
    }
}

TEST_F(TestGAMSWorkspace, testAddDatabase) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSDatabase db = ws.addDatabase();
    // then
    TestGAMSObject::testEmptyDatabase(db, ws);
}

TEST_F(TestGAMSWorkspace, testAddDatabase_Name) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    std::string dbname = "myDatabase";
    // when
    GAMSDatabase db = ws.addDatabase(dbname);
    // then
    EXPECT_EQ( db.name(), dbname );
    TestGAMSObject::testEmptyDatabase(db, ws);
}

TEST_F(TestGAMSWorkspace, testAddDatabase_NameAndModelName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    std::string dbname = "myDatabase";
    std::string modname = "myModel";
    // when
    GAMSDatabase db = ws.addDatabase(dbname, modname);
    // then
    EXPECT_EQ( db.name(), dbname );
    TestGAMSObject::testEmptyDatabase(db, ws);
    // @todo EXPECT_EQ( db.inModelName(), modname );
}

TEST_F(TestGAMSWorkspace, testAddDatabase_Source) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    db1.addSet("x", 2, "Set x");
    db1.addParameter("y", 2, "Parameter y");
    // when
    GAMSDatabase db2 = ws.addDatabase(db1);
    // then
    ASSERT_TRUE( db2.isValid() );
    EXPECT_EQ( db2.getNrSymbols(), 2 );
}

TEST_F(TestGAMSWorkspace, testAddDatabase_SourceAndName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    GAMSDatabase db2 = ws.addDatabase(db1);
    TestGAMSObject::testEmptyDatabase(db2, ws);

    db1.addSet("x", 2, "Set x");
    db1.addParameter("y", 2, "Parameter y");
    std::string dbname = "myDatabase";
    // when
    GAMSDatabase db3 = ws.addDatabase(db1, dbname);
    // then
    ASSERT_TRUE( db3.isValid() );
    ASSERT_TRUE( db3.getNrSymbols() == 2 );
    EXPECT_EQ( db3.name(), dbname );
}

TEST_F(TestGAMSWorkspace, testAddDatabase_SourceNameAndModelName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    db1.addSet("x", 2, "Set x");
    db1.addParameter("y", 2, "Parameter y");

    std::string dbname = "myDatabase";
    std::string modname = "myModel";
    // when
    GAMSDatabase db2 = ws.addDatabase(db1, dbname, modname );
    // then
    ASSERT_TRUE( db2.isValid() );
    EXPECT_EQ( db2.getNrSymbols(), 2 );
    EXPECT_EQ( db2.name(), dbname );
}

TEST_F(TestGAMSWorkspace, testAddCheckpoint) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSCheckpoint cp = ws.addCheckpoint();
    // then
    ASSERT_TRUE( ! cp.name().empty() );
    ASSERT_TRUE( cp.name().find(defaultScratchFilePrefix) == 0 );
}
class ParameterizedTestAddCheckpoint_Name
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P( testAddCheckpoint_Name,
                        ParameterizedTestAddCheckpoint_Name,
                        ::testing::Values (
                            std::make_tuple("validname"         , "myCheckpoint" , true ),
                            std::make_tuple("nameWithUnderScore", "my_checkpoint", true ),
                            std::make_tuple("nameWithWhiteSpace", "my Checkpoint", true ),
                            std::make_tuple("EmptyStringName"   , ""             , false)
                        ));

TEST_P(ParameterizedTestAddCheckpoint_Name,  testAddCheckpoint_Name) {
    std::string description = std::get<0>(GetParam());
    std::string cpname = std::get<1>(GetParam());
    bool name_assigned = std::get<2>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSCheckpoint cp = ws.addCheckpoint(cpname);
    // then
    ASSERT_TRUE( ! cp.name().empty() );
    if (name_assigned)
       EXPECT_EQ( cp.name(), cpname );
    else
       ASSERT_TRUE( cp.name().find(defaultScratchFilePrefix) == 0 );
}

TEST_F(TestGAMSWorkspace, testAddJobFromFile) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    ws.gamsLib("chenery");
    // when
    GAMSJob job = ws.addJobFromFile("chenery.gms");
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
}

TEST_F(TestGAMSWorkspace, testAddJobFromFile_JobName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    ws.gamsLib("alan");
    std::string jobname = "myJobName";
    // when
    GAMSJob job = ws.addJobFromFile("alan.gms", jobname);
    // then
    EXPECT_EQ(job.name(), jobname);
    TestGAMSObject::testJobBeforeRun(job, ws);
}

TEST_F(TestGAMSWorkspace, testAddJobFromFile_SameJobName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    ws.gamsLib("alan");
    ws.gamsLib("chenery");

    std::string jobname = "myJobName";
    ws.addJobFromFile("alan.gms", jobname);

    // when creating another job of the same name, then GAMSException raised
    EXPECT_THROW( ws.addJobFromFile("chenery.gms", jobname), GAMSException);
}

TEST_F(TestGAMSWorkspace, testAddJobFromFile_Checkpoint) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.gamsLib("whouse");

    // when not initialized cp, then GAMSException raised
    EXPECT_THROW( ws.addJobFromFile("whouse.gms", cp), GAMSException);

    // given
    ws.addJobFromFile("whouse.gms").run(cp);
    // when
    GAMSJob job = ws.addJobFromFile("whouse.gms", cp);
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
}

TEST_F(TestGAMSWorkspace, testAddJobFromFile_CheckpointAndJobName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.gamsLib("trnsport");
    ws.addJobFromFile("trnsport.gms").run(cp);
    std::string jobname = "myJobName";
    // when
    GAMSJob job = ws.addJobFromFile("trnsport.gms", cp, jobname);
    // then
    EXPECT_EQ(job.name(), jobname);
    TestGAMSObject::testJobBeforeRun(job, ws);
}

TEST_F(TestGAMSWorkspace, testAddJobFromEmptyString) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSJob job = ws.addJobFromString( "" );
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
    EXPECT_THROW( job.run(), GAMSException );
}

TEST_F(TestGAMSWorkspace, testAddJobFromString) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSJob job = ws.addJobFromString( getLongModelText() );
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
}

TEST_F(TestGAMSWorkspace, testAddJobFromString_Checkpoint) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSCheckpoint cp = ws.addCheckpoint();
    // when not initialized cp, then GAMSException raised
    EXPECT_THROW( ws.addJobFromString(getLongModelText(), cp), GAMSException);

    // given
    ws.addJobFromString( getLongModelText() ).run(cp);
    // when
    GAMSJob job = ws.addJobFromString("bmult=0.9; solve transport min z use lp; ms=transport.modelstat; ss=transport.solvestat;", cp);
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
}

TEST_F(TestGAMSWorkspace, testAddJobFromString_JobName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSJob job = ws.addJobFromString( getShortModelText() );
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
}

TEST_F(TestGAMSWorkspace, testAddJobFromString_SameJobName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    std::string jobname = "myJobName";
    ws.addJobFromString( getShortModelText(), jobname );

    // when creating another job of the same name, then GAMSException raised
    EXPECT_THROW( ws.addJobFromString(getShortModelText(), jobname), GAMSException);
}

TEST_F(TestGAMSWorkspace, testAddValidJobFromString_CheckpointAndJobName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString( getLongModelText() ).run(cp);

    std::string jobname = "myJobName";
    // when
    GAMSJob job = ws.addJobFromString("bmult=0.9; solve transport min z use lp; ms=transport.modelstat; ss=transport.solvestat;", cp, jobname);
    // then
    EXPECT_EQ( job.name() , jobname );
    TestGAMSObject::testJobBeforeRun(job, ws);
}

TEST_F(TestGAMSWorkspace, testAddInvalidJobFromString_CheckpointAndJobName) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    // when, then
    EXPECT_THROW( ws.addJobFromString( getLongModelText(), cp, "myJobName" ), GAMSException );
}

class ParameterizedTestAddJobFromApiLibrary
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddJobFromApiLibrary,
                        ParameterizedTestAddJobFromApiLibrary,
                        ::testing::Values (
                            std::make_tuple("apiutil"       , "apiutil"   , true ),
                            std::make_tuple("PInterrupt"    , "PInterrupt", true ),
                            std::make_tuple("Invalid_apilib", "ThisIsAnUnusualModelName", false)
                        ));

TEST_P(ParameterizedTestAddJobFromApiLibrary, testAddJobFromApiLibrary) {
    std::string modname = std::get<1>(GetParam());
    bool valid = std::get<2>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromApiLib(modname);
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        EXPECT_THROW(ws.addJobFromApiLib(modname), GAMSException);
    }
}

class ParameterizedTestAddJobFromGamsLibrary
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddJobFromGamsLibrary,
                        ParameterizedTestAddJobFromGamsLibrary,
                        ::testing::Values (
                            std::make_tuple("whouse" , "whouse" , true),
                            std::make_tuple("chenery", "chenery", true),
                            std::make_tuple("Invalid_gamslib1", "ThisIsAnUnusualModelName" , false),
                            std::make_tuple("Invalid_gamslib2", "ThisIsAnUnusualModelName2", false)
                        ));

TEST_P(ParameterizedTestAddJobFromGamsLibrary, testAddJobFromGamsLibrary) {
    // given
    std::string modname = std::get<1>(GetParam());
    bool valid = std::get<2>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromGamsLib(modname);
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        EXPECT_THROW(ws.addJobFromGamsLib(modname), GAMSException);
    }
}

class ParameterizedTestAddJobFromTestLibrary
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddJobFromTestLibrary,
                        ParameterizedTestAddJobFromTestLibrary,
                        ::testing::Values (
                            std::make_tuple("testutil"       , "testutil"                , true),
                            std::make_tuple("fnsqr"          , "fnsqr"                   , true),
                            std::make_tuple("Invalid_testlib", "ThisIsAnUnusualModelName", false)
                        ));

TEST_P(ParameterizedTestAddJobFromTestLibrary, testAddJobFromTestLibrary) {
    // given
    std::string modname = std::get<1>(GetParam());
    bool valid = std::get<2>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromTestLib(modname);
        ASSERT_TRUE( ! job.name().empty() );
        ASSERT_TRUE( ! job.outDB().isValid() );
    } else {
        EXPECT_THROW(ws.addJobFromTestLib(modname), GAMSException);
    }
}

class ParameterizedTestAddJobFromDataLibrary
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddJobFromDataLibrary,
                        ParameterizedTestAddJobFromDataLibrary,
                        ::testing::Values (
                            std::make_tuple("Excel"          , "Excel"                   , true),
                            std::make_tuple("Portfolio"      , "Portfolio"               , true),
                            std::make_tuple("Invalid_datalib", "ThisIsAnUnusualModelName", false)
                        ));

TEST_P(ParameterizedTestAddJobFromDataLibrary, testAddJobFromDataLibrary) {
    // given
    std::string modname = std::get<1>(GetParam());
    bool valid = std::get<2>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromDataLib(modname);
        ASSERT_TRUE( ! job.name().empty() );
    } else {
        EXPECT_THROW(ws.addJobFromDataLib(modname), GAMSException);
    }
}

class ParameterizedTestAddJobFromEmpLibrary
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddJobFromEmpLibrary,
                        ParameterizedTestAddJobFromEmpLibrary,
                        ::testing::Values (
                            std::make_tuple("emputil"       , "emputil"                   , true),
                            std::make_tuple("traffic"       , "traffic"               , true),
                            std::make_tuple("Invalid_emplib", "ThisIsAnUnusualModelName", false)
                        ));

TEST_P(ParameterizedTestAddJobFromEmpLibrary, testAddJobFromEmpLibrary) {
    // given
    std::string modname = std::get<1>(GetParam());
    bool valid = std::get<2>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromEmpLib(modname);
        ASSERT_TRUE( ! job.name().empty() );
    } else {
        EXPECT_THROW(ws.addJobFromEmpLib(modname), GAMSException);
    }
}

class ParameterizedTestAddJobFromFinLibrary
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddJobFromFinLibrary,
                        ParameterizedTestAddJobFromFinLibrary,
                        ::testing::Values (
                            std::make_tuple("DedicationNoBorrow", "DedicationNoBorrow"      , true),
                            std::make_tuple("Horizon"           , "Horizon"                 , true),
                            std::make_tuple("Invalid_finlib"    , "ThisIsAnUnusualModelName", false)
                        ));

TEST_P(ParameterizedTestAddJobFromFinLibrary, testAddJobFromFinLibrary) {
    // given
    std::string modname = std::get<1>(GetParam());
    bool valid = std::get<2>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromFinLib(modname);
        ASSERT_TRUE( ! job.name().empty() );
    } else {
        EXPECT_THROW(ws.addJobFromFinLib(modname), GAMSException);
    }
}

class ParameterizedTestAddJobFromNoaLibrary
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddJobFromNoaLibrary,
                        ParameterizedTestAddJobFromNoaLibrary,
                        ::testing::Values (
                            std::make_tuple("reservoir"     , "reservoir"               , true),
                            std::make_tuple("ethanol"       , "ethanol"                 , true),
                            std::make_tuple("Invalid_noalib", "ThisIsAnUnusualModelName", false)
                        ));

TEST_P(ParameterizedTestAddJobFromNoaLibrary, testAddJobFromNoaLibrary) {
    // given
    std::string modname = std::get<1>(GetParam());
    bool valid = std::get<2>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromNoaLib(modname);
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        EXPECT_THROW(ws.addJobFromNoaLib(modname), GAMSException);
    }
}

class ParameterizedTestAddJobFromPsoptLibrary
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddJobFromPsoptLibrary,
                        ParameterizedTestAddJobFromPsoptLibrary,
                        ::testing::Values (
                            std::make_tuple("SimpleLP"        , "SimpleLP"                , true),
                            std::make_tuple("Invalid_psoptlib", "ThisIsAnUnusualModelName", false)
                        ));

TEST_P(ParameterizedTestAddJobFromPsoptLibrary, testAddJobFromPsoptLibrary) {
    // given
    std::string modname = std::get<1>(GetParam());
    bool valid = std::get<2>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromPsoptLib(modname);
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        EXPECT_THROW(ws.addJobFromPsoptLib(modname), GAMSException);
    }
}

class ParameterizedTestAddJobFromLibrary_Checkpoint
        : public ::testing::WithParamInterface<std::tuple<std::string, int, std::string, std::string, std::string>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddJobFromLibrary_Checkpoint,
                        ParameterizedTestAddJobFromLibrary_Checkpoint,
                        ::testing::Values (
                             //             description,        index, library,      modname, extrafile
                             std::make_tuple("gamslib_trnsport", 0, "gamslib", "trnsport", ""),
                             std::make_tuple("gamslib_alan", 0, "gamslib", "alan", ""),
                             std::make_tuple("testlib_call1", 1, "testlib", "call1", ""),
                             std::make_tuple("testlib_onmult1", 1, "testlib", "onmulti1", ""),
                             std::make_tuple("datalib_CheckListbox", 2, "datalib", "CheckListbox", "CheckListbox.gms"),
                             std::make_tuple("datalib_Wiring", 2, "datalib", "Wiring", "Sample.mdb"),
                             // std::make_tuple("emplib_goempgo", 3, "emplib", "goempgo", "empmod.inc"),
                             std::make_tuple("emplib_hark-monop", 3, "emplib", "hark-monop", "hark-data.inc"),
                             std::make_tuple("emplib_farmnbd", 3, "emplib", "farmnbd", ""),
                             std::make_tuple("apilib_JDomainCheck", 4, "apilib", "JDomainCheck", ""),
                             std::make_tuple("apilib_JSpecialValues", 4, "apilib", "JSpecialValues", ""),
                             std::make_tuple("finlib_DedicationNoBorrow", 5, "finlib", "DedicationNoBorrow", "BondData.inc"),
                             std::make_tuple("finlib_StructuralModel", 5, "finlib", "StructuralModel", "InputData.gdx"),
                             std::make_tuple("noalib_reservoir", 6, "noalib", "reservoir", ""),
                             std::make_tuple("noalib_macro", 6, "noalib", "macro", "")
                        ));

TEST_P(ParameterizedTestAddJobFromLibrary_Checkpoint, testAddJobFromLibrary_Checkpoint) {
    // given
    int index = std::get<1>(GetParam());
    std::string library = std::get<2>(GetParam());
    std::string modname = std::get<3>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString("Scalar x;").run(cp);
    // when
    switch(index) {
       case 0: // gamslib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromGamsLib( modname, cp), ws);
          break;
       case 1: // testlib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromTestLib( modname, cp), ws);
          break;
       case 2: // datalib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromDataLib( modname, cp), ws);
          break;
       case 3: // emplib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromEmpLib( modname, cp), ws);
          break;
       case 4: // apilib
           TestGAMSObject::testJobBeforeRun(ws.addJobFromApiLib( modname, cp), ws);
          break;
       case 5: // finlib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromFinLib( modname, cp), ws);
          break;
       case 6: // noalib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromNoaLib( modname, cp), ws);
          break;
       default:  break;
    }
}

class ParameterizedTestAddJobFromLibrary_CheckpointAndJobName
        : public ::testing::WithParamInterface<std::tuple<std::string, int, std::string, std::string, std::string>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddJobFromLibrary_CheckpointAndJobName,
                        ParameterizedTestAddJobFromLibrary_CheckpointAndJobName,
                        ::testing::Values (
                             //             description,        index, library,      modname, extrafile
                             std::make_tuple("gamslib_trnsport", 0, "gamslib", "trnsport", ""),
                             std::make_tuple("gamslib_alan", 0, "gamslib", "alan", ""),
                             std::make_tuple("testlib_call1", 1, "testlib", "call1", ""),
                             std::make_tuple("testlib_onmult1", 1, "testlib", "onmulti1", ""),
                             std::make_tuple("datalib_CheckListbox", 2, "datalib", "CheckListbox", "CheckListbox.gms"),
                             std::make_tuple("datalib_Wiring", 2, "datalib", "Wiring", "Sample.mdb"),
                             // std::make_tuple("emplib_goempgo", 3, "emplib", "goempgo", "empmod.inc"),
                             std::make_tuple("emplib_hark-monop", 3, "emplib", "hark-monop", "hark-data.inc"),
                             std::make_tuple("emplib_farmnbd", 3, "emplib", "farmnbd", ""),
                             std::make_tuple("apilib_JDomainCheck", 4, "apilib", "JDomainCheck", ""),
                             std::make_tuple("apilib_JSpecialValues", 4, "apilib", "JSpecialValues", ""),
                             std::make_tuple("finlib_DedicationNoBorrow", 5, "finlib", "DedicationNoBorrow", "BondData.inc"),
                             std::make_tuple("finlib_StructuralModel", 5, "finlib", "StructuralModel", "InputData.gdx"),
                             std::make_tuple("noalib_reservoir", 6, "noalib", "reservoir", ""),
                             std::make_tuple("noalib_macro", 6, "noalib", "macro", "")
                        ));

TEST_P(ParameterizedTestAddJobFromLibrary_CheckpointAndJobName, testAddJobFromLibrary_CheckpointAndJobName) {
    // given
    int index = std::get<1>(GetParam());
    std::string library = std::get<2>(GetParam());
    std::string modname = std::get<3>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString("Scalar x;").run(cp);
    // when
    switch(index) {
       case 0: // gamslib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromGamsLib( modname, cp, "myJobName"), ws);
          break;
       case 1: // testlib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromTestLib( modname, cp, "myJobName"), ws);
          break;
       case 2: // datalib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromDataLib( modname, cp, "myJobName"), ws);
          break;
       case 3: // emplib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromEmpLib( modname, cp, "myJobName"), ws);
          break;
       case 4: // apilib
           TestGAMSObject::testJobBeforeRun(ws.addJobFromApiLib( modname, cp, "myJobName"), ws);
          break;
       case 5: // finlib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromFinLib( modname, cp, "myJobName"), ws);
          break;
       case 6: // noalib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromNoaLib( modname, cp, "myJobName"), ws);
          break;
       default:  break;
    }
}

TEST_F(TestGAMSWorkspace, testAddOptions) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    try {
        // when
       GAMSOptions opt = ws.addOptions();
    } catch(GAMSException& )  {
       FAIL() << "Do not expect a GAMSException to be thrown";
    }
}

TEST_F(TestGAMSWorkspace, testAddOptions_OptFrom) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    try {
       GAMSOptions opt1 = ws.addOptions();
       opt1.setDefine("gdxincname", "tdata");
       opt1.setDefine("useBig", "1");
       GAMSOptions opt2 = ws.addOptions(opt1);
    } catch(GAMSException&)  {
       FAIL() << "Do not expect a GAMSException to be thrown";
    }
}

class ParameterizedTestAddOptions_OptFile
        : public ::testing::WithParamInterface<std::tuple<std::string, std::string, bool>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testAddOptions_OptFile,
                        ParameterizedTestAddOptions_OptFile,
                        ::testing::Values (
                            std::make_tuple("valid_optfile"  , "validoptionfile.pf", true),
                            std::make_tuple("invalid_optfile", "ThereWillExistThisFile.pf", false)
                        ));

TEST_P(ParameterizedTestAddOptions_OptFile, testAddOptions_OptFile) {
    // setup
    std::string workdir = std::filesystem::current_path().string();

    GAMSPath finalPath = GAMSPath(workdir, "TestGAMSWorkspace");
    if (finalPath.mkDir())
        workdir = finalPath;

    std::string filename="validoptionfile.pf";
    std::ofstream file;
    file.open(GAMSPath(workdir, filename), std::ios::out);

    if ( file.is_open() )  {
      file << "IterLim=1" << std::endl;
      file << "LimCol=4" << std::endl;
      file << "LimRow=5" << std::endl;
      file.close();
   }

    // when
    std::string optfilename = std::get<1>(GetParam());
    bool exists             = std::get<2>(GetParam());

    GAMSPath dir(std::filesystem::current_path());
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    wsInfo.setWorkingDirectory( dir );
    wsInfo.setDebug(GAMSEnum::DebugLevel::KeepFiles);
    GAMSWorkspace ws(wsInfo);
    GAMSOptions o = ws.addOptions();
    if (exists) {
        try {
           std::string filename = GAMSPath(dir, optfilename);
           GAMSOptions opt = ws.addOptions(filename);
           EXPECT_EQ( opt.iterLim(), 1 );
           EXPECT_EQ( opt.limCol(), 4 );
           EXPECT_EQ( opt.limRow(), 5 );
        } catch (GAMSException& e) {
            FAIL() <<  "AddOptions from existing optfile should not fail: " << e.what();
        }
    } else {
       EXPECT_THROW( ws.addOptions(optfilename), GAMSException);
    }

    std::string appDir;
#ifdef _WIN32
    char result[MAX_PATH];
    appDir =  GetModuleFileName(NULL, result, MAX_PATH);
#else
    char result[ PATH_MAX ];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    appDir = std::string (result, (count > 0) ? count : 0 );
#endif
// TODO(RG): this doesnt work as intended
//    if (appDir != dir.string())
//       dir.remove();
}

class ParameterizedTestRetrievingModelFromLibrary
        : public ::testing::WithParamInterface<std::tuple<std::string, int, std::string, std::string, std::string>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testRetrievingModelFromLibrary,
                        ParameterizedTestRetrievingModelFromLibrary,
                        ::testing::Values (
                             //             description,        index, library,      modname, extrafile
                             std::make_tuple("gamslib_trnsport", 0, "gamslib", "trnsport", ""),
                             std::make_tuple("gamslib_alan", 0, "gamslib", "alan", ""),
                             std::make_tuple("testlib_call1", 1, "testlib", "call1", ""),
                             std::make_tuple("testlib_onmult1", 1, "testlib", "onmulti1", ""),
                             std::make_tuple("datalib_CheckListbox", 2, "datalib", "CheckListbox", "CheckListbox.gms"),
                             std::make_tuple("datalib_Wiring", 2, "datalib", "Wiring", "Sample.mdb"),
                             // std::make_tuple("emplib_goempgo", 3, "emplib", "goempgo", "empmod.inc"),
                             std::make_tuple("emplib_hark-monop", 3, "emplib", "hark-monop", "hark-data.inc"),
                             std::make_tuple("emplib_farmnbd", 3, "emplib", "farmnbd", ""),
                             std::make_tuple("apilib_JDomainCheck", 4, "apilib", "JDomainCheck", ""),
                             std::make_tuple("apilib_JSpecialValues", 4, "apilib", "JSpecialValues", ""),
                             std::make_tuple("finlib_DedicationNoBorrow", 5, "finlib", "DedicationNoBorrow", "BondData.inc"),
                             std::make_tuple("finlib_StructuralModel", 5, "finlib", "StructuralModel", "InputData.gdx"),
                             std::make_tuple("noalib_reservoir", 6, "noalib", "reservoir", ""),
                             std::make_tuple("noalib_macro", 6, "noalib", "macro", "")
                        ));

TEST_P(ParameterizedTestRetrievingModelFromLibrary, testRetrievingModelFromLibrary) {
    // given
    int index = std::get<1>(GetParam());
    std::string library = std::get<2>(GetParam());
    std::string modname = std::get<3>(GetParam());
    std::string extrafile = std::get<4>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    switch(index) {
       case 0: // gamslib
          ws.gamsLib(modname);
          break;
       case 1: // testlib
          ws.testLib(modname);
          break;
       case 2: // datalib
          ws.dataLib(modname);
          break;
       case 3: // emplib
          ws.empLib(modname);
          break;
       case 4: // apilib
          ws.apiLib(modname);
          break;
       case 5: // finlib
          ws.finLib(modname);
          break;
       case 6: // noalib
          ws.noaLib(modname);
          break;
       default:  break;
    }
    // then
    std::string filename = modname+".gms";
    GAMSPath modfile( ws.workingDirectory(), filename);

    ASSERT_TRUE(modfile.exists()) << "Expect ["+filename+"] file from ["+library+"]";

    if (!extrafile.empty()) {
        GAMSPath morefile(ws.workingDirectory(), extrafile);
        ASSERT_TRUE(morefile.exists()) << "Expect ["+extrafile+"] file with ["+modname+"] from ["+library+"]";
    }
}

class ParameterizedTestRetrievingInvalidModelFromLibrary
        : public ::testing::WithParamInterface<std::tuple<std::string, int, std::string, std::string>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testRetrievingInvalidModelFromLibrary,
                        ParameterizedTestRetrievingInvalidModelFromLibrary,
                        ::testing::Values (
                             //             description,        index, library,      modname
                                 std::make_tuple("invalid_gamslib", 0, "gamslib", "ThisIsAnUnusualModelName"),
                                 std::make_tuple("invalid_testlib", 1, "testlib", "ThisIsAnUnusualModelName"),
                                 std::make_tuple("invalid_datalib", 2, "datalib", "ThisIsAnUnusualModelName"),
                                 std::make_tuple("invalid_emplib",  3, "emplib",  "ThisIsAnUnusualModelName"),
                                 std::make_tuple("invalid_apilib",  4, "apilib",  "ThisIsAnUnusualModelName"),
                                 std::make_tuple("invalid_finlib",  5, "finlib",  "ThisIsAnUnusualModelName"),
                                 std::make_tuple("invalid_noalib",  6, "noalib",  "ThisIsAnUnusualModelName")
                        ));

TEST_P(ParameterizedTestRetrievingInvalidModelFromLibrary, testRetrievingInvalidModelFromLibrary) {
    // given
    int index = std::get<1>(GetParam());
    std::string library = std::get<2>(GetParam());
    std::string modname = std::get<3>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    try {
        switch(index) {
          case 0: // gamslib
             ws.gamsLib(modname);
             break;
          case 1: // testlib
             ws.testLib(modname);
             break;
          case 2: // datalib
             ws.dataLib(modname);
             break;
          case 3: // emplib
             ws.empLib(modname);
             break;
          case 4: // apilib
             ws.apiLib(modname);
             break;
          case 5: // finlib
             ws.finLib(modname);
             break;
          case 6: // noalib
             ws.noaLib(modname);
             break;
          default:  break;
       }
       FAIL() << "Expect GAMSException due to invalid model name ["+modname+"] on calling ["+library+"]";
    } catch(GAMSException &) {
        // then
        std::string filename = modname+".gms";
        GAMSPath modfile(ws.workingDirectory(), filename);
        ASSERT_TRUE(!modfile.exists()) << "Do not expect a file named ["+filename+"]  from ["+library+"]";
    }
}

TEST_F(TestGAMSWorkspace, testGetWorkingDirectory) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    TestGAMSObject::testDir( ws.workingDirectory() );
}

TEST_F(TestGAMSWorkspace, testGetSystemDirectory) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    TestGAMSObject::testDir( ws.workingDirectory() );
}

TEST_F(TestGAMSWorkspace, testEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws1(wsInfo);
    GAMSWorkspace ws2(ws1);
    // when, then
    ASSERT_TRUE( ws1 == ws2 );

    GAMSWorkspace ws3(wsInfo);
    // when, then
    ASSERT_TRUE(!(ws3 == ws2));
    ASSERT_TRUE(!(ws3 == ws1) );
}

TEST_F(TestGAMSWorkspace, testNotEqualToOperator) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws1(wsInfo);
    GAMSWorkspace ws2(ws1);
    // when, then
    ASSERT_TRUE( !(ws1 != ws2) );

    GAMSWorkspace ws3(wsInfo);
    // when, then
    ASSERT_TRUE( ws3 != ws2);
    ASSERT_TRUE( ws3 != ws1 );
}

TEST_F(TestGAMSWorkspace, testGetDebug) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    ASSERT_TRUE( ws.debug() >= GAMSEnum::DebugLevel::Off );
    ASSERT_TRUE( ws.debug() <= GAMSEnum::DebugLevel::Verbose );
}

TEST_F(TestGAMSWorkspace, testGetLogID) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws1(wsInfo);
    GAMSWorkspace ws2(ws1);
    // when, then
    EXPECT_EQ( ws1.logID(), ws2.logID() );
    ASSERT_TRUE( ws1.logID()== ws2.logID() );

    GAMSWorkspace ws3(wsInfo);
    // when, then
    ASSERT_TRUE( ws1.logID() != ws3.logID());
}

class ParameterizedTestGetSpecValues
        : public ::testing::WithParamInterface<std::tuple<std::string, int, double>>,
          public TestGAMSWorkspace {
};

INSTANTIATE_TEST_SUITE_P(testGetSpecValues,
                        ParameterizedTestGetSpecValues,
                        ::testing::Values (
                             // TODO(RG): these values need either be made static or we need some other way of accessing them here
                             //         description, int,  double
                             std::make_tuple("UNDF", 0, 1.0E300),
                             std::make_tuple("NA",   1, std::numeric_limits<double>::quiet_NaN()),
                             std::make_tuple("PINF", 2, std::numeric_limits<double>::infinity()),
                             std::make_tuple("MINF", 3, -std::numeric_limits<double>::infinity()),
                             std::make_tuple("EPS",  4, std::numeric_limits<double>::min() )
                        ));

TEST_P(ParameterizedTestGetSpecValues, testGetSpecValues) {
    // given
    int index    = std::get<1>(GetParam());
    double value = std::get<2>(GetParam());

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    switch(index) {
        case 1: // NA
            ASSERT_TRUE( std::isnan( ws.specValues()[index] ) );
            break;
        case 2: // PINF
            ASSERT_TRUE( std::isinf( ws.specValues()[index] ) );
            break;
        case 3: // MINF
            ASSERT_TRUE( std::isinf( - ws.specValues()[index] ) );
            break;
        default:
            ASSERT_TRUE( (ws.specValues()[index] - value) < std::numeric_limits<double>::min() );
            break;
    }
}


