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

#include "gamscheckpoint.h"
#include "gamsexception.h"
#include "gamsset.h"
#include "gamsparameter.h"
#include "gamsoptions.h"
#include "gamsworkspaceinfo.h"
#include "gamsexception.h"
#include "testgamsworkspace.h"
#include "gamspath.h"

#include <iostream>

using namespace gams;

class TestGAMSWorkspace: public TestGAMSObject
{
};

TEST_F(TestGAMSWorkspace, testDefaultConstructor) {
    try {
        GAMSWorkspace ws;

        // then
        TestGAMSObject::testDir( QString::fromStdString(ws.systemDirectory()) );
        QString sdir = QString::fromStdString(ws.systemDirectory());
        EXPECT_EQ( testSystemDir, QDir(sdir) );
        TestGAMSObject::testDir( sdir );

        TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
        EXPECT_EQ(ws.debug(), GAMSEnum::DebugLevel::Off);

    } catch (GAMSException &e) {
        QEXPECT_FAIL("", e.what(), Abort);
        ASSERT_TRUE(false);
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
    TestGAMSObject::testDir( QString::fromStdString(ws.systemDirectory()) );
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
    EXPECT_EQ( ws.debug(), GAMSEnum::DebugLevel::Off );

    QDir(QString::fromStdString(ws.workingDirectory())).removeRecursively();
}

TEST_F(TestGAMSWorkspace, testConstructor_WorkspaceInfo) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    // when
    GAMSWorkspace ws(wsInfo);
    // then
    TestGAMSObject::testDir( QString::fromStdString(ws.systemDirectory()) );
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );

    EXPECT_EQ( GAMSPath(ws.workingDirectory()), GAMSPath(ws.workingDirectory()) );
    EXPECT_EQ( ws.debug(), GAMSEnum::DebugLevel::Off );
}

TEST_F(TestGAMSWorkspace, testConstructor_WorkingDirSystemDirAndDebug) {
    // given
    std::string wdir = QDir::currentPath().toStdString();
    std::string sdir = testSystemDir.absolutePath().toStdString();
    // when
    GAMSWorkspace ws( wdir, sdir, GAMSEnum::DebugLevel::KeepFiles );
    // then
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
    EXPECT_EQ(GAMSPath(ws.workingDirectory()), GAMSPath(wdir));

    TestGAMSObject::testDir( QString::fromStdString(ws.systemDirectory()) );
    EXPECT_EQ(GAMSPath(ws.systemDirectory()), GAMSPath(sdir));

    EXPECT_EQ(ws.debug(), GAMSEnum::DebugLevel::KeepFiles);
}

TEST_F(TestGAMSWorkspace, testConstructor_WorkingDir) {
    // given
    std::string wdir = "";
    // when
    GAMSWorkspace ws(wdir, testSystemDir);
    // then
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
    EXPECT_EQ(ws.debug(), GAMSEnum::DebugLevel::Off);
}

TEST_F(TestGAMSWorkspace, testConstructor_WorkingDirSystemDir) {
    // given
    std::string wdir = QDir::currentPath().toStdString();
    std::string sdir = testSystemDir.absolutePath().toStdString();
    // when
    GAMSWorkspace ws(wdir, sdir);
    // then
    TestGAMSObject::testDir(QDir(QString::fromStdString(ws.workingDirectory())).absolutePath());
    EXPECT_EQ(GAMSPath(ws.workingDirectory()), GAMSPath(wdir));

    TestGAMSObject::testDir(QDir(QString::fromStdString(ws.systemDirectory())).absolutePath());
    EXPECT_EQ(GAMSPath(ws.systemDirectory()), GAMSPath(sdir));

    EXPECT_EQ(ws.debug(), GAMSEnum::DebugLevel::Off);
}

TEST_F(TestGAMSWorkspace, testConstructor_DebugLevel_data) {
    TestGAMSObject::getTestData_DebugLevel();
}

TEST_F(TestGAMSWorkspace, testConstructor_DebugLevel) {
    // TODO add case when "GAMSOOAPIDEBUG" has been set
    // given
    QFETCH(QString, debugLevel);
    GAMSEnum::DebugLevel debugLevelEnum = static_cast<GAMSEnum::DebugLevel>(debugLevel.toInt());

    QString dir;
    // when
    {
        GAMSWorkspace ws("", testSystemDir, debugLevelEnum);
        dir = QString::fromStdString(ws.workingDirectory());
    }
    // then
    switch(debugLevel.toInt()) {
       case 0: // Off
           ASSERT_TRUE( ! QDir(dir).exists() );
           break;
       case 1: // KeepFiles
           ASSERT_TRUE( QDir(dir).exists() );
           /* TODO QEXPECT_FAIL("", "More tests to be implemented ", Abort); ASSERT_TRUE(false); */
           break;
      case 2: // ShowLog
           ASSERT_TRUE( ! QDir(dir).exists() );
           /* TODO QEXPECT_FAIL("", "More tests to be implemented ", Abort); ASSERT_TRUE(false); */
           break;
      case 3: // Verbose
           ASSERT_TRUE( ! QDir(dir).exists() );
           /* TODO QEXPECT_FAIL("", "More tests to be implemented ", Abort); ASSERT_TRUE(false); */
           break;
      default:
           break;
    }
    QDir qdir(dir);
    if (qdir.exists()) {
        qdir.removeRecursively();
    }
}

TEST_F(TestGAMSWorkspace, testConstructorFromOccupiedWorkingDirectory) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    wsInfo.setWorkingDirectory(QDir::currentPath().toStdString());
    GAMSWorkspace ws1(wsInfo);
    // when creating workspace from working directory that is already in use, then
    EXPECT_THROW( GAMSWorkspace ws2(wsInfo), GAMSException);
}

TEST_F(TestGAMSWorkspace, testGetVersion) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    EXPECT_EQ(testGAMSVersion, QString::fromStdString(ws.version()));
    QStringList testGAMSVersionList = testGAMSVersion.split( "." );
    if (testGAMSVersionList.size() != 3)
        FAIL() << "The GAMSVersion shall consist of three numbers (MAJOR.MINOR.PATCH)";
    EXPECT_EQ(ws.majorRelNumber(), testGAMSVersionList[0].toInt());
    EXPECT_EQ(ws.minorRelNumber(), testGAMSVersionList[1].toInt());
    EXPECT_EQ(ws.goldRelNumber(), testGAMSVersionList[2].toInt());
}

TEST_F(TestGAMSWorkspace, testGetAPIVersion) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    EXPECT_EQ(testAPIVersion, QString::fromStdString(ws.apiVersion()));
    QStringList testAPIVersionList = testAPIVersion.split( "." );
    EXPECT_EQ(ws.apiMajorRelNumber(), testAPIVersionList[0].toInt());
    EXPECT_EQ(ws.apiMinorRelNumber(), testAPIVersionList[1].toInt());
    EXPECT_EQ(ws.apiGoldRelNumber(), testAPIVersionList[2].toInt());
}

TEST_F(TestGAMSWorkspace, testGetScratchFilePrefix) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    ASSERT_TRUE( ! ws.scratchFilePrefix().empty() );
    EXPECT_EQ( ws.scratchFilePrefix(), defaultScratchFilePrefix );
}

TEST_F(TestGAMSWorkspace, testSetScratchFilePrefix_data) {
    QTest::addColumn<QString>("prefixname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("NormalPrefix")          << "myPrefix" << true ;
    QTest::newRow("PrefixWithUnderScore") << "_myPrefix" << true ;
    QTest::newRow("PrefixWithWhiteSpace") << "my Prefix" << false;
    QTest::newRow("EmptyStringPrefix")    << ""          << true ;
}

TEST_F(TestGAMSWorkspace, testSetScratchFilePrefix) {
    // given
    QFETCH(QString, prefixname);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    if (valid) {
       // when
       ws.setScratchFilePrefix(prefixname.toStdString());
       // then
       EXPECT_EQ( ws.scratchFilePrefix(), prefixname.toStdString());
    } else {
        // then
       EXPECT_THROW( ws.setScratchFilePrefix(prefixname.toStdString()), GAMSException );
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

TEST_F(TestGAMSWorkspace, testAddDatabaseFromGDX1_data) {
    QTest::addColumn<QString>("gdxfilename");
    QTest::addColumn<QString>("fromDataLib");
    QTest::addColumn<bool>("valid");

    QTest::newRow("DemandData.gdx")          << "DemandData.gdx"               << "GDXINExample1"  << true ;
    QTest::newRow("NonExistingFileName.gdx") << "ThisGDXFileDoesNotExist.gdx"  << ""  << false  ;
    QTest::newRow("EmptyStringFileName")     << ""                             << ""  << false  ;
}

TEST_F(TestGAMSWorkspace, testAddDatabaseFromGDX1) {
    // given
    QFETCH(QString, gdxfilename);
    QFETCH(QString, fromDataLib);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    if (valid) {
        std::string s = fromDataLib.toStdString();
        ws.dataLib(s);
        // when
        try {
            GAMSDatabase db = ws.addDatabaseFromGDX(gdxfilename.toStdString() );

            EXPECT_EQ( db.getNrSymbols(), 2 );
            EXPECT_EQ( db.getParameter("demand").numberRecords(), 3);
            EXPECT_EQ( db.getSet("markets").numberRecords(), 3);
        } catch(GAMSException& e) {
            qDebug() << QString::fromStdString( ws.workingDirectory() );
            FAIL() << qPrintable( "Unexpected GAMSException raised by: "+ QString::fromStdString(e.what()) );
        }
    } else {
        // when, then
        EXPECT_THROW(ws.addDatabaseFromGDX(gdxfilename.toStdString()) , GAMSException);
    }
}

TEST_F(TestGAMSWorkspace, testAddDatabaseFromGDX2_data) {
    QTest::addColumn<QString>("gdxfilename");
    QTest::addColumn<QString>("fromDataLib");
    QTest::addColumn<int>("numberOfSymbols");

    QTest::newRow("db_ExecTimeReadTrnsportGDX") << "Trnsport.gdx"   << "ExecTimeReadTrnsportGDX" << 12 ;
    QTest::newRow("db_DemandData")              << "DemandData.gdx" << "GDXINExample2"           << 2 ;
}

TEST_F(TestGAMSWorkspace, testAddDatabaseFromGDX2) {
    // given
    QFETCH(QString, gdxfilename);
    QFETCH(QString, fromDataLib);
    QFETCH(int, numberOfSymbols);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    ws.dataLib( fromDataLib.toStdString() );
    try {
       // when
       GAMSDatabase db = ws.addDatabaseFromGDX(gdxfilename.toStdString(), QTest::currentDataTag() );
       EXPECT_EQ( db.name().c_str(),  QTest::currentDataTag() );
       EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
    } catch(GAMSException& e) {
       qDebug() << qPrintable("error adding db from ["+ gdxfilename + "] at ["+  QString::fromStdString( ws.workingDirectory() )+"].");
       FAIL() << qPrintable( "Unexpected GAMSException thrown: "+ QString::fromStdString(e.what()) );
    }
}

TEST_F(TestGAMSWorkspace, testAddDatabaseFromGDX3_data) {
    QTest::addColumn<QString>("gdxfilename");
    QTest::addColumn<QString>("fromDataLib");
    QTest::addColumn<int>("numberOfSymbols");

    QTest::newRow("db_GDXCOPYExample19") << "UNStatistics.gdx" << "GDXCOPYExample19"  << 8 ;
    QTest::newRow("db_Trnsport")         << "Trnsport.gdx"     << "GDXINExample3"     << 12 ;
}

TEST_F(TestGAMSWorkspace, testAddDatabaseFromGDX3) {
    // given
    QFETCH(QString, gdxfilename);
    QFETCH(QString, fromDataLib);
    QFETCH(int, numberOfSymbols);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    ws.dataLib( fromDataLib.toStdString() );
    try {
       // when
       std::string inModelName = "myModel";
       GAMSDatabase db = ws.addDatabaseFromGDX(gdxfilename.toStdString(), QTest::currentDataTag(), inModelName);
       EXPECT_EQ( db.name().c_str(),  QTest::currentDataTag() );
       // @todo EXPECT_EQ( db.inModelName(), inModelName );
       EXPECT_EQ( db.getNrSymbols(), numberOfSymbols );
    } catch(GAMSException& e) {
       qDebug() << qPrintable("error adding db from ["+ gdxfilename + "] at ["+  QString::fromStdString( ws.workingDirectory() )+"].");
       FAIL() << qPrintable( "Unexpected GAMSException thrown: "+ QString::fromStdString(e.what()) );
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
    ASSERT_TRUE( QString::fromStdString(cp.name()).startsWith(defaultScratchFilePrefix.c_str()) );
}

TEST_F(TestGAMSWorkspace, testAddCheckpoint_Name_data) {
    QTest::addColumn<QString>("cpname");
    QTest::addColumn<bool>("name_assigned");

    QTest::newRow("validname")          << "myCheckpoint"  << true ;
    QTest::newRow("nameWithUnderScore") << "my_checkpoint" << true ;
    QTest::newRow("nameWithWhiteSpace") << "my Checkpoint" << true ;
    QTest::newRow("EmptyStringName")    << ""              << false ;
}

TEST_F(TestGAMSWorkspace, testAddCheckpoint_Name) {
    // given
    QFETCH(QString, cpname);
    QFETCH(bool, name_assigned);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSCheckpoint cp = ws.addCheckpoint(cpname.toStdString());
    // then
    ASSERT_TRUE( ! cp.name().empty() );
    if (name_assigned)
       EXPECT_EQ( cp.name(), cpname.toStdString() );
    else
       ASSERT_TRUE( QString::fromStdString(cp.name()).startsWith(defaultScratchFilePrefix.c_str()) );
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

TEST_F(TestGAMSWorkspace, testAddJobFromApiLibrary_data) {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("apiutil")        << "apiutil"    << true ;
    QTest::newRow("PInterrupt")     << "PInterrupt" << true ;
    QTest::newRow("Invalid_apilib") << "ThisIsAnUnusualModelName" << false ;
}

TEST_F(TestGAMSWorkspace, testAddJobFromApiLibrary) {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromApiLib(modname.toStdString());
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        EXPECT_THROW(ws.addJobFromApiLib(modname.toStdString()), GAMSException);
    }
}

TEST_F(TestGAMSWorkspace, testAddJobFromGamsLibrary_data) {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("whouse")   << "whouse"  << true ;
    QTest::newRow("chenery")  << "chenery" << true ;
    QTest::newRow("Invalid_gamslib1")   << "ThisIsAnUnusualModelName" << false ;
    QTest::newRow("Invalid_gamslib2")   << "ThisIsAnUnusualModelName2" << false ;
}

TEST_F(TestGAMSWorkspace, testAddJobFromGamsLibrary) {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromGamsLib(modname.toStdString());
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        EXPECT_THROW(ws.addJobFromGamsLib(modname.toStdString()), GAMSException);
    }
}

TEST_F(TestGAMSWorkspace, testAddJobFromTestLibrary_data) {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("testutil")        << "testutil"     << true ;
    QTest::newRow("fnsqr")           << "fnsqr"        << true ;
    QTest::newRow("Invalid_testlib") << "ThisIsAnUnusualModelName" << false ;
}

TEST_F(TestGAMSWorkspace, testAddJobFromTestLibrary) {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromTestLib(modname.toStdString());
        ASSERT_TRUE( ! job.name().empty() );
        ASSERT_TRUE( ! job.outDB().isValid() );
    } else {
        EXPECT_THROW(ws.addJobFromTestLib(modname.toStdString()), GAMSException);
    }
}

TEST_F(TestGAMSWorkspace, testAddJobFromDataLibrary_data) {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("Excel")           << "Excel"     << true ;
    QTest::newRow("Portfolio")       << "Portfolio" << true ;
    QTest::newRow("Invalid_datalib") << "ThisIsAnUnusualModelName" << false ;
}

TEST_F(TestGAMSWorkspace, testAddJobFromDataLibrary) {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromDataLib(modname.toStdString());
        ASSERT_TRUE( ! job.name().empty() );
    } else {
        EXPECT_THROW(ws.addJobFromDataLib(modname.toStdString()), GAMSException);
    }
}

TEST_F(TestGAMSWorkspace, testAddJobFromEmpLibrary_data) {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("emputil")        << "emputil"     << true ;
    QTest::newRow("traffic")        << "traffic"     << true ;
    QTest::newRow("Invalid_emplib") << "ThisIsAnUnusualModelName" << false;
}

TEST_F(TestGAMSWorkspace, testAddJobFromEmpLibrary) {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromEmpLib(modname.toStdString());
        ASSERT_TRUE( ! job.name().empty() );
    } else {
        EXPECT_THROW(ws.addJobFromEmpLib(modname.toStdString()), GAMSException);
    }
}

TEST_F(TestGAMSWorkspace, testAddJobFromFinLibrary_data) {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("DedicationNoBorrow")  << "DedicationNoBorrow"  << true ;
    QTest::newRow("Horizon")             << "Horizon"             << true ;
    QTest::newRow("Invalid_finlib")      << "ThisIsAnUnusualModelName" << false ;
}

TEST_F(TestGAMSWorkspace, testAddJobFromFinLibrary) {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromFinLib(modname.toStdString());
        ASSERT_TRUE( ! job.name().empty() );
    } else {
        EXPECT_THROW(ws.addJobFromFinLib(modname.toStdString()), GAMSException);
    }
}

TEST_F(TestGAMSWorkspace, testAddJobFromNoaLibrary_data) {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("reservoir")      << "reservoir" << true ;
    QTest::newRow("ethanol")        << "ethanol"   << true ;
    QTest::newRow("Invalid_noalib") << "ThisIsAnUnusualModelName" << false ;
}

TEST_F(TestGAMSWorkspace, testAddJobFromNoaLibrary) {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromNoaLib(modname.toStdString());
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        EXPECT_THROW(ws.addJobFromNoaLib(modname.toStdString()), GAMSException);
    }
}

TEST_F(TestGAMSWorkspace, testAddJobFromPsoptLibrary_data) {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("SimpleLP")        << "SimpleLP"   << true;
    QTest::newRow("Invalid_psoptlib") << "ThisIsAnUnusualModelName" << false;
}

TEST_F(TestGAMSWorkspace, testAddJobFromPsoptLibrary) {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromPsoptLib(modname.toStdString());
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        EXPECT_THROW(ws.addJobFromPsoptLib(modname.toStdString()), GAMSException);
    }
}

TEST_F(TestGAMSWorkspace, testAddJobFromLibrary_Checkpoint_data) {
    TestGAMSObject::getTestData_ModelLibraries();
}

TEST_F(TestGAMSWorkspace, testAddJobFromLibrary_Checkpoint) {
    // given
    QFETCH(int, index);
    QFETCH(QString, library);
    QFETCH(QString, modname);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString("Scalar x;").run(cp);
    // when
    switch(index) {
       case 0: // gamslib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromGamsLib( modname.toStdString(), cp), ws);
          break;
       case 1: // testlib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromTestLib( modname.toStdString(), cp), ws);
          break;
       case 2: // datalib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromDataLib( modname.toStdString(), cp), ws);
          break;
       case 3: // emplib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromEmpLib( modname.toStdString(), cp), ws);
          break;
       case 4: // apilib
           TestGAMSObject::testJobBeforeRun(ws.addJobFromApiLib( modname.toStdString(), cp), ws);
          break;
       case 5: // finlib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromFinLib( modname.toStdString(), cp), ws);
          break;
       case 6: // noalib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromNoaLib( modname.toStdString(), cp), ws);
          break;
       default:  break;
    }
}

TEST_F(TestGAMSWorkspace, testAddJobFromLibrary_CheckpointAndJobName_data) {
    TestGAMSObject::getTestData_ModelLibraries();
}

TEST_F(TestGAMSWorkspace, testAddJobFromLibrary_CheckpointAndJobName) {
    // given
    QFETCH(int, index);
    QFETCH(QString, library);
    QFETCH(QString, modname);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);

    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString("Scalar x;").run(cp);
    // when
    switch(index) {
       case 0: // gamslib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromGamsLib( modname.toStdString(), cp, "myJobName"), ws);
          break;
       case 1: // testlib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromTestLib( modname.toStdString(), cp, "myJobName"), ws);
          break;
       case 2: // datalib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromDataLib( modname.toStdString(), cp, "myJobName"), ws);
          break;
       case 3: // emplib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromEmpLib( modname.toStdString(), cp, "myJobName"), ws);
          break;
       case 4: // apilib
           TestGAMSObject::testJobBeforeRun(ws.addJobFromApiLib( modname.toStdString(), cp, "myJobName"), ws);
          break;
       case 5: // finlib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromFinLib( modname.toStdString(), cp, "myJobName"), ws);
          break;
       case 6: // noalib
          TestGAMSObject::testJobBeforeRun(ws.addJobFromNoaLib( modname.toStdString(), cp, "myJobName"), ws);
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

TEST_F(TestGAMSWorkspace, testAddOptions_OptFile_data) {
    QString currentWorkingPath = QCoreApplication::applicationDirPath();
    QDir dir(currentWorkingPath);

    QString workdir = dir.absolutePath();

    QString finalPath = dir.filePath(TestGAMSWorkspace::classname());
    if (dir.mkpath(finalPath)) {
        workdir = finalPath;
    }
    QTest::addColumn<QString>("optfilename");
    QTest::addColumn<bool>("exist");
    QTest::addColumn<QString>("workdir");

    QTest::newRow("valid_optfile")     << "test.pf"                   << true  << workdir ;
    QTest::newRow("invalid_optfile")   << "ThereWillExistThisFile.pf" << false << workdir ;

    QString filename="test.pf";
    QFile file( QDir(workdir).filePath( filename ) );
    if ( file.open(QIODevice::WriteOnly) )  {
      QTextStream stream( &file );
      stream << "IterLim=1" << endl;
      stream << "LimCol=4" << endl;
      stream << "LimRow=5" << endl;
      file.close();
   }
}

TEST_F(TestGAMSWorkspace, testAddOptions_OptFile) {

    // when
    QFETCH(QString, optfilename);
    QFETCH(bool, exist);
    QFETCH(QString, workdir);

    QDir dir(workdir);
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    wsInfo.setWorkingDirectory( dir.absolutePath().toStdString() );
    wsInfo.setDebug(GAMSEnum::DebugLevel::KeepFiles);
    GAMSWorkspace ws(wsInfo);
    GAMSOptions o = ws.addOptions();
    if (exist) {
        try {
           QString filename = dir.absoluteFilePath(optfilename);
           GAMSOptions opt = ws.addOptions(filename.toStdString());
           EXPECT_EQ( opt.iterLim(), 1 );
           EXPECT_EQ( opt.limCol(), 4 );
           EXPECT_EQ( opt.limRow(), 5 );
        } catch(GAMSException& e) {
            FAIL() <<  qPrintable("AddOptions from existing optfile should not fail: "+QString::fromStdString(e.what())) ;
        }
    } else {
       EXPECT_THROW( ws.addOptions(optfilename.toStdString()), GAMSException);
    }
    // clean up only when working dir is not application dir
    if (QFileInfo(QCoreApplication::applicationDirPath()) != QFileInfo(dir.canonicalPath()))
       dir.removeRecursively();
}

TEST_F(TestGAMSWorkspace, testRetrievingModelFromLibrary_data) {
    TestGAMSObject::getTestData_ModelLibraries();
}

TEST_F(TestGAMSWorkspace, testRetrievingModelFromLibrary) {
    // given
    QFETCH(int, index);
    QFETCH(QString, library);
    QFETCH(QString, modname);
    QFETCH(QString, extrafile);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    switch(index) {
       case 0: // gamslib
          ws.gamsLib(modname.toStdString());
          break;
       case 1: // testlib
          ws.testLib(modname.toStdString());
          break;
       case 2: // datalib
          ws.dataLib(modname.toStdString());
          break;
       case 3: // emplib
          ws.empLib(modname.toStdString());
          break;
       case 4: // apilib
          ws.apiLib(modname.toStdString());
          break;
       case 5: // finlib
          ws.finLib(modname.toStdString());
          break;
       case 6: // noalib
          ws.noaLib(modname.toStdString());
          break;
       default:  break;
    }
    // then
    QString filename = modname+".gms";
    QFileInfo modfile( QString(ws.workingDirectory().c_str()), filename);
    QVERIFY2(modfile.exists(), qPrintable("Expect ["+filename+"] file from ["+library+"]"));
    if (!extrafile.isEmpty()) {
        QFileInfo morefile(QString(ws.workingDirectory().c_str()), extrafile);
        QVERIFY2(morefile.exists(), qPrintable("Expect ["+extrafile+"] file with ["+modname+"] from ["+library+"]"));
    }
}

TEST_F(TestGAMSWorkspace, testRetrievingInvalidModelFromLibrary_data) {
    TestGAMSObject::getTestData_InvalidModelLibraries();
}

TEST_F(TestGAMSWorkspace, testRetrievingInvalidModelFromLibrary) {
    // given
    QFETCH(int, index);
    QFETCH(QString, library);
    QFETCH(QString, modname);

    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when
    try {
        switch(index) {
          case 0: // gamslib
             ws.gamsLib(modname.toStdString());
             break;
          case 1: // testlib
             ws.testLib(modname.toStdString());
             break;
          case 2: // datalib
             ws.dataLib(modname.toStdString());
             break;
          case 3: // emplib
             ws.empLib(modname.toStdString());
             break;
          case 4: // apilib
             ws.apiLib(modname.toStdString());
             break;
          case 5: // finlib
             ws.finLib(modname.toStdString());
             break;
          case 6: // noalib
             ws.noaLib(modname.toStdString());
             break;
          default:  break;
       }
       FAIL() << qPrintable("Expect GAMSException due to invalid model name ["+modname+"] on calling ["+library+"]");
    } catch(GAMSException &) {
        // then
        QString filename = modname+".gms";
        QFileInfo modfile(QDir(ws.workingDirectory().c_str()), filename);
        QVERIFY2(!modfile.exists(), qPrintable("Do not expect a file named ["+filename+"]  from ["+library+"]"));
    }
}

TEST_F(TestGAMSWorkspace, testGetWorkingDirectory) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
}

TEST_F(TestGAMSWorkspace, testGetSystemDirectory) {
    // given
    GAMSWorkspaceInfo wsInfo("", testSystemDir);
    GAMSWorkspace ws(wsInfo);
    // when, then
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
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

TEST_F(TestGAMSWorkspace, testGetSpecValues_data) {
    TestGAMSObject::getTestData_SpecialValues();
}

TEST_F(TestGAMSWorkspace, testGetSpecValues) {
    QFETCH(int, index);
    QFETCH(double, value);

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


