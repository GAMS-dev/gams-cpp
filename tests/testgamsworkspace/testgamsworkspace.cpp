/*
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

#include "gamscheckpoint.h"
#include "gamsexception.h"
#include "gamsset.h"
#include "gamsparameter.h"
#include "gamsoptions.h"
#include "gamsworkspaceinfo.h"
#include "gamsexception.h"
#include "testgamsworkspace.h"
#include "gamspath.h"

#include <QString>
#include <iostream>

using namespace gams;

QString TestGAMSWorkspace::classname()  { return "TestGAMSWorkspace"; }

void TestGAMSWorkspace::testDefaultConstructor() {
    // when
    GAMSWorkspace ws;

    // then
    TestGAMSObject::testDir( QString::fromStdString(ws.systemDirectory()) );
    QString sdir = QString::fromStdString(ws.systemDirectory());
    QCOMPARE( testSystemDir.canonicalPath(), QDir(sdir).canonicalPath()+"/" );
    TestGAMSObject::testDir( sdir );

    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
    QCOMPARE(ws.debug(), GAMSEnum::DebugLevel::Off);
}


void TestGAMSWorkspace::testCopyConstructor() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws1(wsInfo);
    // when
    GAMSWorkspace ws2(ws1);
    // then
    QCOMPARE(ws1.systemDirectory(), ws2.systemDirectory());
    QCOMPARE(ws1.workingDirectory(), ws2.workingDirectory());
    QCOMPARE(ws1.debug(), ws2.debug());
    QCOMPARE(ws1, ws2);
}

void TestGAMSWorkspace::testConstructor_EmptyWorkspaceInfo() {
    // given
    GAMSWorkspaceInfo wsInfo;
    // when
    GAMSWorkspace ws(wsInfo);
    // then
    TestGAMSObject::testDir( QString::fromStdString(ws.systemDirectory()) );
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
    QCOMPARE( ws.debug(), GAMSEnum::DebugLevel::Off );

    QDir(QString::fromStdString(ws.workingDirectory())).removeRecursively();
}

void TestGAMSWorkspace::testConstructor_WorkspaceInfo() {
    // given
    GAMSWorkspaceInfo wsInfo;
    // when
    GAMSWorkspace ws(wsInfo);
    // then
    TestGAMSObject::testDir( QString::fromStdString(ws.systemDirectory()) );
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );

    QCOMPARE( GAMSPath(ws.workingDirectory()), GAMSPath(ws.workingDirectory()) );
    QCOMPARE( ws.debug(), GAMSEnum::DebugLevel::Off );
}

void TestGAMSWorkspace::testConstructor_WorkingDirSystemDirAndDebug() {
    // given
    std::string wdir = QDir::currentPath().toStdString();
    std::string sdir = testSystemDir.absolutePath().toStdString();
    // when
    GAMSWorkspace ws( wdir, sdir, GAMSEnum::DebugLevel::KeepFiles );
    // then
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
    QCOMPARE(GAMSPath(ws.workingDirectory()), GAMSPath(wdir));

    TestGAMSObject::testDir( QString::fromStdString(ws.systemDirectory()) );
    QCOMPARE(GAMSPath(ws.systemDirectory()), GAMSPath(sdir));

    QCOMPARE(ws.debug(), GAMSEnum::DebugLevel::KeepFiles);
}

void TestGAMSWorkspace::testConstructor_WorkingDir() {
    // given
    std::string wdir = "";
    // when
    GAMSWorkspace ws( wdir );
    // then
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
    QCOMPARE(ws.debug(), GAMSEnum::DebugLevel::Off);
}

void TestGAMSWorkspace::testConstructor_WorkingDirSystemDir() {
    // given
    std::string wdir = QDir::currentPath().toStdString();
    std::string sdir = testSystemDir.absolutePath().toStdString();
    // when
    GAMSWorkspace ws( wdir, sdir );
    // then
    TestGAMSObject::testDir(QDir(QString::fromStdString(ws.workingDirectory())).absolutePath());
    QCOMPARE(GAMSPath(ws.workingDirectory()), GAMSPath(wdir));

    TestGAMSObject::testDir(QDir(QString::fromStdString(ws.systemDirectory())).absolutePath());
    QCOMPARE(GAMSPath(ws.systemDirectory()), GAMSPath(sdir));

    QCOMPARE(ws.debug(), GAMSEnum::DebugLevel::Off);
}

void TestGAMSWorkspace::testConstructor_DebugLevel_data() {
    TestGAMSObject::getTestData_DebugLevel();
}

void TestGAMSWorkspace::testConstructor_DebugLevel() {
    // TODO add case when "GAMSOOAPIDEBUG" has been set
    // given
    QFETCH(QString, debugLevel);
    GAMSEnum::DebugLevel debugLevelEnum = static_cast<GAMSEnum::DebugLevel>(debugLevel.toInt());

    QString dir;
    // when
    {
        if (debugLevelEnum != GAMSEnum::Off) {
            QEXPECT_FAIL("", "Avoiding access violation for debugLevelEnum != GAMSEnum::Off ", Abort); QVERIFY(false);
        }
        GAMSWorkspace ws( debugLevelEnum );
        dir = QString::fromStdString(ws.workingDirectory());
    }
    // then
    switch(debugLevel.toInt()) {
       case 0: // Off
           QVERIFY( ! QDir(dir).exists() );
           break;
       case 1: // KeepFiles
           QVERIFY( QDir(dir).exists() );
           /* TODO QEXPECT_FAIL("", "More tests to be implemented ", Abort); QVERIFY(false); */
           break;
      case 2: // ShowLog
           QVERIFY( ! QDir(dir).exists() );
           /* TODO QEXPECT_FAIL("", "More tests to be implemented ", Abort); QVERIFY(false); */
           break;
      case 3: // Verbose
           QVERIFY( ! QDir(dir).exists() );
           /* TODO QEXPECT_FAIL("", "More tests to be implemented ", Abort); QVERIFY(false); */
           break;
      default:
           break;
     }
}

void TestGAMSWorkspace::testConstructorFromOccupiedWorkingDirectory() {
    // given
    GAMSWorkspaceInfo wsInfo;
    wsInfo.setWorkingDirectory(QDir::currentPath().toStdString());
    GAMSWorkspace ws1(wsInfo);
    // when creating workspace from working directory that is already in use, then
    QVERIFY_EXCEPTION_THROWN( GAMSWorkspace ws2(wsInfo), GAMSException);
}

void TestGAMSWorkspace::testGetVersion() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when, then
    QCOMPARE(testGAMSVersion, QString::fromStdString(ws.version()));
    QStringList testGAMSVersionList = testGAMSVersion.split( "." );
    if (testGAMSVersionList.size() != 3)
        QFAIL("The GAMSVersion shall consist of three numbers (MAJOR.MINOR.PATCH)");
    QCOMPARE(ws.majorRelNumber(), testGAMSVersionList[0].toInt());
    QCOMPARE(ws.minorRelNumber(), testGAMSVersionList[1].toInt());
    QCOMPARE(ws.goldRelNumber(), testGAMSVersionList[2].toInt());
}

void TestGAMSWorkspace::testGetAPIVersion() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when, then
    QCOMPARE(testAPIVersion, QString::fromStdString(ws.apiVersion()));
    QStringList testAPIVersionList = testAPIVersion.split( "." );
    QCOMPARE(ws.apiMajorRelNumber(), testAPIVersionList[0].toInt());
    QCOMPARE(ws.apiMinorRelNumber(), testAPIVersionList[1].toInt());
    QCOMPARE(ws.apiGoldRelNumber(), testAPIVersionList[2].toInt());
}

void TestGAMSWorkspace::testGetScratchFilePrefix() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when, then
    QVERIFY( ! ws.scratchFilePrefix().empty() );
    QCOMPARE( ws.scratchFilePrefix(), defaultScratchFilePrefix );
}

void TestGAMSWorkspace::testSetScratchFilePrefix_data() {
    QTest::addColumn<QString>("prefixname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("NormalPrefix")          << "myPrefix" << true ;
    QTest::newRow("PrefixWithUnderScore") << "_myPrefix" << true ;
    QTest::newRow("PrefixWithWhiteSpace") << "my Prefix" << false;
    QTest::newRow("EmptyStringPrefix")    << ""          << true ;
}

void TestGAMSWorkspace::testSetScratchFilePrefix() {
    // given
    QFETCH(QString, prefixname);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    if (valid) {
       // when
       ws.setScratchFilePrefix(prefixname.toStdString());
       // then
       QCOMPARE( ws.scratchFilePrefix(), prefixname.toStdString());
    } else {
        // then
       QVERIFY_EXCEPTION_THROWN( ws.setScratchFilePrefix(prefixname.toStdString()), GAMSException );
    }
}

void TestGAMSWorkspace::testGetMyEPS() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when, then
    QCOMPARE( ws.myEPS(),  defaultEPS );
}

void TestGAMSWorkspace::testSetMyEPS()  {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    double myEPS = 5e-300;
    // when
    ws.setMyEPS (myEPS );
    QCOMPARE(  ws.myEPS(), myEPS );
}

void TestGAMSWorkspace::testAddDatabaseFromGDX1_data() {
    QTest::addColumn<QString>("gdxfilename");
    QTest::addColumn<QString>("fromDataLib");
    QTest::addColumn<bool>("valid");

    QTest::newRow("DemandData.gdx")          << "DemandData.gdx"               << "GDXINExample1"  << true ;
    QTest::newRow("NonExistingFileName.gdx") << "ThisGDXFileDoesNotExist.gdx"  << ""  << false  ;
    QTest::newRow("EmptyStringFileName")     << ""                             << ""  << false  ;
}

void TestGAMSWorkspace::testAddDatabaseFromGDX1() {
    // given
    QFETCH(QString, gdxfilename);
    QFETCH(QString, fromDataLib);
    QFETCH(bool, valid);

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    if (valid) {
       ws.dataLib( fromDataLib.toStdString() );
       // when
       try {
          GAMSDatabase db = ws.addDatabaseFromGDX(gdxfilename.toStdString() );
          QCOMPARE( db.getNrSymbols(), 2 );
          QCOMPARE( db.getParameter("demand").numberRecords(), 3) ;
          QCOMPARE( db.getSet("markets").numberRecords(), 3);
       } catch(GAMSException e) {
          qDebug() << QString::fromStdString( ws.workingDirectory() );
          QFAIL(qPrintable( "Unexpected GAMSException raised by: "+ QString::fromStdString(e.what()) ));
       }
    } else {
        // when, then
        QVERIFY_EXCEPTION_THROWN(ws.addDatabaseFromGDX(gdxfilename.toStdString()) , GAMSException);
    }
}

void TestGAMSWorkspace::testAddDatabaseFromGDX2_data() {
    QTest::addColumn<QString>("gdxfilename");
    QTest::addColumn<QString>("fromDataLib");
    QTest::addColumn<int>("numberOfSymbols");

    QTest::newRow("db_ExecTimeReadTrnsportGDX") << "Trnsport.gdx"   << "ExecTimeReadTrnsportGDX" << 12 ;
    QTest::newRow("db_DemandData")              << "DemandData.gdx" << "GDXINExample2"           << 2 ;
}

void TestGAMSWorkspace::testAddDatabaseFromGDX2() {
    // given
    QFETCH(QString, gdxfilename);
    QFETCH(QString, fromDataLib);
    QFETCH(int, numberOfSymbols);

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    ws.dataLib( fromDataLib.toStdString() );
    try {
       // when
       GAMSDatabase db = ws.addDatabaseFromGDX(gdxfilename.toStdString(), QTest::currentDataTag() );
       QCOMPARE( db.name().c_str(),  QTest::currentDataTag() );
       QCOMPARE( db.getNrSymbols(), numberOfSymbols );
    } catch(GAMSException e) {
       qDebug() << qPrintable("error adding db from ["+ gdxfilename + "] at ["+  QString::fromStdString( ws.workingDirectory() )+"].");
       QFAIL(qPrintable( "Unexpected GAMSException thrown: "+ QString::fromStdString(e.what()) ));
    }
}

void TestGAMSWorkspace::testAddDatabaseFromGDX3_data() {
    QTest::addColumn<QString>("gdxfilename");
    QTest::addColumn<QString>("fromDataLib");
    QTest::addColumn<int>("numberOfSymbols");

    QTest::newRow("db_GDXCOPYExample19") << "UNStatistics.gdx" << "GDXCOPYExample19"  << 8 ;
    QTest::newRow("db_Trnsport")         << "Trnsport.gdx"     << "GDXINExample3"     << 12 ;
}

void TestGAMSWorkspace::testAddDatabaseFromGDX3() {
    // given
    QFETCH(QString, gdxfilename);
    QFETCH(QString, fromDataLib);
    QFETCH(int, numberOfSymbols);

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    ws.dataLib( fromDataLib.toStdString() );
    try {
       // when
       std::string inModelName = "myModel";
       GAMSDatabase db = ws.addDatabaseFromGDX(gdxfilename.toStdString(), QTest::currentDataTag(), inModelName);
       QCOMPARE( db.name().c_str(),  QTest::currentDataTag() );
       // @todo QCOMPARE( db.inModelName(), inModelName );
       QCOMPARE( db.getNrSymbols(), numberOfSymbols );
    } catch(GAMSException e) {
       qDebug() << qPrintable("error adding db from ["+ gdxfilename + "] at ["+  QString::fromStdString( ws.workingDirectory() )+"].");
       QFAIL(qPrintable( "Unexpected GAMSException thrown: "+ QString::fromStdString(e.what()) ));
    }
}

void TestGAMSWorkspace::testAddDatabase() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSDatabase db = ws.addDatabase();
    // then
    TestGAMSObject::testEmptyDatabase(db, ws);
}

void TestGAMSWorkspace::testAddDatabase_Name() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    std::string dbname = "myDatabase";
    // when
    GAMSDatabase db = ws.addDatabase(dbname);
    // then
    QCOMPARE( db.name(), dbname );
    TestGAMSObject::testEmptyDatabase(db, ws);
}

void TestGAMSWorkspace::testAddDatabase_NameAndModelName() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    std::string dbname = "myDatabase";
    std::string modname = "myModel";
    // when
    GAMSDatabase db = ws.addDatabase(dbname, modname);
    // then
    QCOMPARE( db.name(), dbname );
    TestGAMSObject::testEmptyDatabase(db, ws);
    // @todo QCOMPARE( db.inModelName(), modname );
}

void TestGAMSWorkspace::testAddDatabase_Source() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    db1.addSet("x", 2, "Set x");
    db1.addParameter("y", 2, "Parameter y");
    // when
    GAMSDatabase db2 = ws.addDatabase(db1);
    // then
    QVERIFY( db2.isValid() );
    QCOMPARE( db2.getNrSymbols(), 2 );
}

void TestGAMSWorkspace::testAddDatabase_SourceAndName() {
    // given
    GAMSWorkspaceInfo wsInfo;
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
    QVERIFY( db3.isValid() );
    QVERIFY( db3.getNrSymbols() == 2 );
    QCOMPARE( db3.name(), dbname );
}

void TestGAMSWorkspace::testAddDatabase_SourceNameAndModelName() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSDatabase db1 = ws.addDatabase();
    db1.addSet("x", 2, "Set x");
    db1.addParameter("y", 2, "Parameter y");

    std::string dbname = "myDatabase";
    std::string modname = "myModel";
    // when
    GAMSDatabase db2 = ws.addDatabase(db1, dbname, modname );
    // then
    QVERIFY( db2.isValid() );
    QCOMPARE( db2.getNrSymbols(), 2 );
    QCOMPARE( db2.name(), dbname );
}

void TestGAMSWorkspace::testAddCheckpoint() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSCheckpoint cp = ws.addCheckpoint();
    // then
    QVERIFY( ! cp.name().empty() );
    QVERIFY( QString::fromStdString(cp.name()).startsWith(defaultScratchFilePrefix.c_str()) );
}

void TestGAMSWorkspace::testAddCheckpoint_Name_data() {
    QTest::addColumn<QString>("cpname");
    QTest::addColumn<bool>("name_assigned");

    QTest::newRow("validname")          << "myCheckpoint"  << true ;
    QTest::newRow("nameWithUnderScore") << "my_checkpoint" << true ;
    QTest::newRow("nameWithWhiteSpace") << "my Checkpoint" << true ;
    QTest::newRow("EmptyStringName")    << ""              << false ;
}

void TestGAMSWorkspace::testAddCheckpoint_Name() {
    // given
    QFETCH(QString, cpname);
    QFETCH(bool, name_assigned);

    GAMSWorkspace ws;
    // when
    GAMSCheckpoint cp = ws.addCheckpoint(cpname.toStdString());
    // then
    QVERIFY( ! cp.name().empty() );
    if (name_assigned)
       QCOMPARE( cp.name(), cpname.toStdString() );
    else
       QVERIFY( QString::fromStdString(cp.name()).startsWith(defaultScratchFilePrefix.c_str()) );
}

void TestGAMSWorkspace::testAddJobFromFile() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    ws.gamsLib("chenery");
    // when
    GAMSJob job = ws.addJobFromFile("chenery.gms");
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
}

void TestGAMSWorkspace::testAddJobFromFile_JobName() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    ws.gamsLib("alan");
    std::string jobname = "myJobName";
    // when
    GAMSJob job = ws.addJobFromFile("alan.gms", jobname);
    // then
    QCOMPARE(job.name(), jobname);
    TestGAMSObject::testJobBeforeRun(job, ws);
}

void TestGAMSWorkspace::testAddJobFromFile_SameJobName() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    ws.gamsLib("alan");
    ws.gamsLib("chenery");

    std::string jobname = "myJobName";
    ws.addJobFromFile("alan.gms", jobname);

    // when creating another job of the same name, then GAMSException raised
    QVERIFY_EXCEPTION_THROWN( ws.addJobFromFile("chenery.gms", jobname), GAMSException);
}

void TestGAMSWorkspace::testAddJobFromFile_Checkpoint() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.gamsLib("whouse");

    // when not initialized cp, then GAMSException raised
    QVERIFY_EXCEPTION_THROWN( ws.addJobFromFile("whouse.gms", cp), GAMSException);

    // given
    ws.addJobFromFile("whouse.gms").run(cp);
    // when
    GAMSJob job = ws.addJobFromFile("whouse.gms", cp);
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
}

void TestGAMSWorkspace::testAddJobFromFile_CheckpointAndJobName() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.gamsLib("trnsport");
    ws.addJobFromFile("trnsport.gms").run(cp);
    std::string jobname = "myJobName";
    // when
    GAMSJob job = ws.addJobFromFile("trnsport.gms", cp, jobname);
    // then
    QCOMPARE(job.name(), jobname);
    TestGAMSObject::testJobBeforeRun(job, ws);
}

void TestGAMSWorkspace::testAddJobFromEmptyString() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSJob job = ws.addJobFromString( "" );
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
    QVERIFY_EXCEPTION_THROWN( job.run(), GAMSException );
}

void TestGAMSWorkspace::testAddJobFromString() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSJob job = ws.addJobFromString( getLongModelText() );
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
}

void TestGAMSWorkspace::testAddJobFromString_Checkpoint() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    GAMSCheckpoint cp = ws.addCheckpoint();
    // when not initialized cp, then GAMSException raised
    QVERIFY_EXCEPTION_THROWN( ws.addJobFromString(getLongModelText(), cp), GAMSException);

    // given
    ws.addJobFromString( getLongModelText() ).run(cp);
    // when
    GAMSJob job = ws.addJobFromString("bmult=0.9; solve transport min z use lp; ms=transport.modelstat; ss=transport.solvestat;", cp);
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
}

void TestGAMSWorkspace::testAddJobFromString_JobName() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when
    GAMSJob job = ws.addJobFromString( getShortModelText() );
    // then
    TestGAMSObject::testJobBeforeRun(job, ws);
}

void TestGAMSWorkspace::testAddJobFromString_SameJobName() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    std::string jobname = "myJobName";
    ws.addJobFromString( getShortModelText(), jobname );

    // when creating another job of the same name, then GAMSException raised
    QVERIFY_EXCEPTION_THROWN( ws.addJobFromString(getShortModelText(), jobname), GAMSException);
}

void TestGAMSWorkspace::testAddValidJobFromString_CheckpointAndJobName() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    GAMSCheckpoint cp = ws.addCheckpoint();
    ws.addJobFromString( getLongModelText() ).run(cp);

    std::string jobname = "myJobName";
    // when
    GAMSJob job = ws.addJobFromString("bmult=0.9; solve transport min z use lp; ms=transport.modelstat; ss=transport.solvestat;", cp, jobname);
    // then
    QCOMPARE( job.name() , jobname );
    TestGAMSObject::testJobBeforeRun(job, ws);
}

void TestGAMSWorkspace::testAddInvalidJobFromString_CheckpointAndJobName() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    GAMSCheckpoint cp = ws.addCheckpoint();
    // when, then
    QVERIFY_EXCEPTION_THROWN( ws.addJobFromString( getLongModelText(), cp, "myJobName" ), GAMSException );
}

void TestGAMSWorkspace::testAddJobFromApiLibrary_data() {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("apiutil")        << "apiutil"    << true ;
    QTest::newRow("PInterrupt")     << "PInterrupt" << true ;
    QTest::newRow("Invalid_apilib") << "ThisIsAnUnusualModelName" << false ;
}

void TestGAMSWorkspace::testAddJobFromApiLibrary() {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspace ws;
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromApiLib(modname.toStdString());
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        QVERIFY_EXCEPTION_THROWN(ws.addJobFromApiLib(modname.toStdString()), GAMSException);
    }
}

void TestGAMSWorkspace::testAddJobFromGamsLibrary_data() {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("whouse")   << "whouse"  << true ;
    QTest::newRow("chenery")  << "chenery" << true ;
    QTest::newRow("Invalid_gamslib1")   << "ThisIsAnUnusualModelName" << false ;
    QTest::newRow("Invalid_gamslib2")   << "ThisIsAnUnusualModelName2" << false ;
}

void TestGAMSWorkspace::testAddJobFromGamsLibrary() {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspace ws;
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromGamsLib(modname.toStdString());
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        QVERIFY_EXCEPTION_THROWN(ws.addJobFromGamsLib(modname.toStdString()), GAMSException);
    }
}

void TestGAMSWorkspace::testAddJobFromTestLibrary_data() {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("testutil")        << "testutil"     << true ;
    QTest::newRow("fnsqr")           << "fnsqr"        << true ;
    QTest::newRow("Invalid_testlib") << "ThisIsAnUnusualModelName" << false ;
}

void TestGAMSWorkspace::testAddJobFromTestLibrary() {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspace ws;
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromTestLib(modname.toStdString());
        QVERIFY( ! job.name().empty() );
        QVERIFY( ! job.outDB().isValid() );
    } else {
        QVERIFY_EXCEPTION_THROWN(ws.addJobFromTestLib(modname.toStdString()), GAMSException);
    }
}

void TestGAMSWorkspace::testAddJobFromDataLibrary_data() {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("Excel")           << "Excel"     << true ;
    QTest::newRow("Portfolio")       << "Portfolio" << true ;
    QTest::newRow("Invalid_datalib") << "ThisIsAnUnusualModelName" << false ;
}

void TestGAMSWorkspace::testAddJobFromDataLibrary() {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspace ws;
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromDataLib(modname.toStdString());
        QVERIFY( ! job.name().empty() );
    } else {
        QVERIFY_EXCEPTION_THROWN(ws.addJobFromDataLib(modname.toStdString()), GAMSException);
    }
}

void TestGAMSWorkspace::testAddJobFromEmpLibrary_data() {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("emputil")        << "emputil"     << true ;
    QTest::newRow("traffic")        << "traffic"     << true ;
    QTest::newRow("Invalid_emplib") << "ThisIsAnUnusualModelName" << false;
}

void TestGAMSWorkspace::testAddJobFromEmpLibrary() {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspace ws;
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromEmpLib(modname.toStdString());
        QVERIFY( ! job.name().empty() );
    } else {
        QVERIFY_EXCEPTION_THROWN(ws.addJobFromEmpLib(modname.toStdString()), GAMSException);
    }
}

void TestGAMSWorkspace::testAddJobFromFinLibrary_data() {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("DedicationNoBorrow")  << "DedicationNoBorrow"  << true ;
    QTest::newRow("Horizon")             << "Horizon"             << true ;
    QTest::newRow("Invalid_finlib")      << "ThisIsAnUnusualModelName" << false ;
}

void TestGAMSWorkspace::testAddJobFromFinLibrary() {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspace ws;
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromFinLib(modname.toStdString());
        QVERIFY( ! job.name().empty() );
    } else {
        QVERIFY_EXCEPTION_THROWN(ws.addJobFromFinLib(modname.toStdString()), GAMSException);
    }
}

void TestGAMSWorkspace::testAddJobFromNoaLibrary_data() {
    QTest::addColumn<QString>("modname");
    QTest::addColumn<bool>("valid");

    QTest::newRow("reservoir")      << "reservoir" << true ;
    QTest::newRow("ethanol")        << "ethanol"   << true ;
    QTest::newRow("Invalid_noalib") << "ThisIsAnUnusualModelName" << false ;
}

void TestGAMSWorkspace::testAddJobFromNoaLibrary() {
    // given
    QFETCH(QString, modname);
    QFETCH(bool, valid);

    GAMSWorkspace ws;
    // when, then
    if (valid) {
        GAMSJob job = ws.addJobFromNoaLib(modname.toStdString());
        TestGAMSObject::testJobBeforeRun(job, ws);
    } else {
        QVERIFY_EXCEPTION_THROWN(ws.addJobFromNoaLib(modname.toStdString()), GAMSException);
    }
}

void TestGAMSWorkspace::testAddJobFromLibrary_Checkpoint_data() {
    TestGAMSObject::getTestData_ModelLibraries();
}

void TestGAMSWorkspace::testAddJobFromLibrary_Checkpoint() {
    // given
    QFETCH(int, index);
    QFETCH(QString, library);
    QFETCH(QString, modname);

    GAMSWorkspaceInfo wsInfo;
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

void TestGAMSWorkspace::testAddJobFromLibrary_CheckpointAndJobName_data() {
    TestGAMSObject::getTestData_ModelLibraries();
}

void TestGAMSWorkspace::testAddJobFromLibrary_CheckpointAndJobName() {
    // given
    QFETCH(int, index);
    QFETCH(QString, library);
    QFETCH(QString, modname);

    GAMSWorkspaceInfo wsInfo;
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

void TestGAMSWorkspace::testAddOptions() {
    // given
    GAMSWorkspace ws;
    try {
        // when
       GAMSOptions opt = ws.addOptions();
    } catch(GAMSException )  {
       QFAIL("Do not expect a GAMSException to be thrown");
    }
}

void TestGAMSWorkspace::testAddOptions_OptFrom() {
    // given
    GAMSWorkspace ws;
    try {
       GAMSOptions opt1 = ws.addOptions();
       opt1.setDefine("gdxincname", "tdata");
       opt1.setDefine("useBig", "1");
       GAMSOptions opt2 = ws.addOptions(opt1);
    } catch(GAMSException e)  {
       QFAIL("Do not expect a GAMSException to be thrown");
    }
}

void TestGAMSWorkspace::testAddOptions_OptFile_data() {
    QString currentWorkingPath = QCoreApplication::applicationDirPath();
    QDir dir(currentWorkingPath);
    QString workdir1 = dir.absolutePath();
    QString workdir2 = dir.absolutePath();

    QString finalPath = dir.filePath(TestGAMSWorkspace::classname());
    if (dir.mkpath(finalPath)) {
        workdir1 = finalPath;
        workdir2 = finalPath;
    }
    QTest::addColumn<QString>("optfilename");
    QTest::addColumn<bool>("exist");
    QTest::addColumn<QString>("workdir");

    QTest::newRow("valid_optfile")     << "test.pf"                   << true  << workdir1 ;
    QTest::newRow("invalid_optfile")   << "ThereWillExistThisFile.pf" << false << workdir2 ;

    QString filename="test.pf";
    QFile file( QDir(workdir1).filePath( filename ) );
    if ( file.open(QIODevice::WriteOnly) )  {
      QTextStream stream( &file );
      stream << "IterLim=1" << endl;
      stream << "LimCol=4" << endl;
      stream << "LimRow=5" << endl;
      file.close();
   }
}

void TestGAMSWorkspace::testAddOptions_OptFile() {

    // when
    QFETCH(QString, optfilename);
    QFETCH(bool, exist);
    QFETCH(QString, workdir);

    QDir dir(workdir);
    GAMSWorkspaceInfo wsInfo;
    wsInfo.setWorkingDirectory( dir.absolutePath().toStdString() );
    wsInfo.setDebug(GAMSEnum::DebugLevel::KeepFiles);
    GAMSWorkspace ws(wsInfo);
    GAMSOptions o = ws.addOptions();
    if (exist) {
        try {
           QString filename = dir.absoluteFilePath(optfilename);
           GAMSOptions opt = ws.addOptions(filename.toStdString());
           QCOMPARE( opt.iterLim(), 1 );
           QCOMPARE( opt.limCol(), 4 );
           QCOMPARE( opt.limRow(), 5 );
        } catch(GAMSException e) {
            QFAIL( qPrintable("AddOptions from existing optfile should not fail: "+QString::fromStdString(e.what())) );
        }
    } else {
       QVERIFY_EXCEPTION_THROWN( ws.addOptions(optfilename.toStdString()), GAMSException);
    }
    // clean up
    dir.removeRecursively();
}

void TestGAMSWorkspace::testRetrievingModelFromLibrary_data() {
    TestGAMSObject::getTestData_ModelLibraries();
}

void TestGAMSWorkspace::testRetrievingModelFromLibrary() {
    // given
    QFETCH(int, index);
    QFETCH(QString, library);
    QFETCH(QString, modname);
    QFETCH(QString, extrafile);

    GAMSWorkspaceInfo wsInfo;
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

void TestGAMSWorkspace::testRetrievingInvalidModelFromLibrary_data() {
    TestGAMSObject::getTestData_InvalidModelLibraries();
}

void TestGAMSWorkspace::testRetrievingInvalidModelFromLibrary() {
    // given
    QFETCH(int, index);
    QFETCH(QString, library);
    QFETCH(QString, modname);

    GAMSWorkspaceInfo wsInfo;
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
       QFAIL(qPrintable("Expect GAMSException due to invalid model name ["+modname+"] on calling ["+library+"]"));
    } catch(GAMSException &) {
        // then
        QString filename = modname+".gms";
        QFileInfo modfile(QDir(ws.workingDirectory().c_str()), filename);
        QVERIFY2(!modfile.exists(), qPrintable("Do not expect a file named ["+filename+"]  from ["+library+"]"));
    }
}

void TestGAMSWorkspace::testGetWorkingDirectory() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when, then
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
}

void TestGAMSWorkspace::testGetSystemDirectory() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when, then
    TestGAMSObject::testDir( QString::fromStdString(ws.workingDirectory()) );
}

void TestGAMSWorkspace::testEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws1(wsInfo);
    GAMSWorkspace ws2(ws1);
    // when, then
    QVERIFY( ws1 == ws2 );

    GAMSWorkspace ws3;
    // when, then
    QVERIFY(!(ws3 == ws2));
    QVERIFY(!(ws3 == ws1) );
}

void TestGAMSWorkspace::testNotEqualToOperator() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws1(wsInfo);
    GAMSWorkspace ws2(ws1);
    // when, then
    QVERIFY( !(ws1 != ws2) );

    GAMSWorkspace ws3;
    // when, then
    QVERIFY( ws3 != ws2);
    QVERIFY( ws3 != ws1 );
}

void TestGAMSWorkspace::testGetDebug() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);
    // when, then
    QVERIFY( ws.debug() >= GAMSEnum::DebugLevel::Off );
    QVERIFY( ws.debug() <= GAMSEnum::DebugLevel::Verbose );
}

void TestGAMSWorkspace::testGetLogID() {
    // given
    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws1(wsInfo);
    GAMSWorkspace ws2(ws1);
    // when, then
    QCOMPARE( ws1.logID(), ws2.logID() );
    QVERIFY( ws1.logID()== ws2.logID() );

    GAMSWorkspace ws3;
    // when, then
    QVERIFY( ws1.logID() != ws3.logID());
}

void TestGAMSWorkspace::testGetSpecValues_data() {
    TestGAMSObject::getTestData_SpecialValues();
}

void TestGAMSWorkspace::testGetSpecValues() {
    QFETCH(int, index);
    QFETCH(double, value);

    GAMSWorkspaceInfo wsInfo;
    GAMSWorkspace ws(wsInfo);

    switch(index) {
        case 1: // NA
            QVERIFY( std::isnan( ws.specValues()[index] ) );
            break;
        case 2: // PINF
            QVERIFY( std::isinf( ws.specValues()[index] ) );
            break;
        case 3: // MINF
            QVERIFY( std::isinf( - ws.specValues()[index] ) );
            break;
        default:
            QVERIFY( (ws.specValues()[index] - value) < std::numeric_limits<double>::min() );
            break;
    }
}

QTEST_MAIN(TestGAMSWorkspace)
