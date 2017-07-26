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

#define NOMINMAX // we need this in ordre for numerical_limits<double>::min() to work

#include "gamslog.h"
#include "gamsworkspaceimpl.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <QSettings>
#include <QProcess>
#include <QString>
#include <QDir>

#include <limits>
#include <stdio.h>
#include "gamsplatform.h"
#include "gamsoptions.h"
#include "gamsworkspacepool.h"

using namespace std;

namespace gams {

const static double CSpecValues[] = { GMS_SV_UNDEF, numeric_limits<double>::quiet_NaN(), numeric_limits<double>::infinity()
                                      , -numeric_limits<double>::infinity(), numeric_limits<double>::min() };

GAMSWorkspaceImpl::GAMSWorkspaceImpl(const GAMSWorkspaceImpl& ws)
    : mWorkingDir(ws.mWorkingDir), mSystemDir(ws.mSystemDir), mDebug(ws.mDebug)
{
    LoggerPool::instance().registerLogger(static_cast<LogId>(this), mDebug, stdout);
    GAMSWorkspacePool::registerWorkspacePath(mWorkingDir.toStdString());
}

GAMSWorkspaceImpl::GAMSWorkspaceImpl(const string& workingDir, const string& systemDir, GAMSEnum::DebugLevel debug)
    : mDebug(debug)
{
    LoggerPool::instance().registerLogger(static_cast<LogId>(this), mDebug, stdout);
    DEB << "---- Entering GAMSWorkspaceImpl constructor ----";

    // TODO: do we want to use these special values? What about quiet_NaN vs signaling_NaN ? Is this the right (and only)
    //       place where we need to initialize specValues?

    std::copy(std::begin(CSpecValues), std::end(CSpecValues), std::begin(specValues));

    char const* tmpEnvDebug = getenv("GAMSOOAPIDEBUG");
    if (tmpEnvDebug != NULL) {
        string envDebug(QString(tmpEnvDebug).toLower().toStdString());
        DEB << envDebug;
        if ("off" == envDebug)
            mDebug = GAMSEnum::Off;
        else if ("keepfiles" == envDebug)
            mDebug = GAMSEnum::KeepFiles;
        else if ("showlog" == envDebug)
            mDebug = GAMSEnum::ShowLog;
        else if ("verbose" == envDebug)
            mDebug = GAMSEnum::Verbose;
    }

    // handle working directory
    mUsingTmpWorkingDir = workingDir.empty();
    if (mUsingTmpWorkingDir) {
        GAMSPath tempDir = mWorkingDir.tempDir(QDir::tempPath() + "/gams");
        if (!tempDir.isDir()) {
            throw GAMSException("Cannot create workspace directory: " + tempDir.toStdString());
        }
        mWorkingDir = tempDir;
    } else {
        mWorkingDir = workingDir;
    }
    if (!mWorkingDir.mkDir()) {
        throw GAMSException("Cannot create workspace directory: " + mWorkingDir.toStdString());
    }
    mWorkingDir.pack();
    GAMSWorkspacePool::registerWorkspacePath(mWorkingDir.toStdString());

    // handle system directory
    mSystemDir = systemDir.empty() ? findGAMS() : systemDir;
    if (!mSystemDir.exists()) {
        throw GAMSException("Invalid GAMS system directory: " + systemDir);
    }
    mSystemDir.pack();

    GAMSPath joat64File = mSystemDir / (QString(cLibPrefix) + "joatdclib64" + cLibSuffix);
    int bitness = sizeof(int*);

    DEB << joat64File.absoluteFilePath();
    const char* bitsuf = (bitness == 8) ? "64" : "";

    // if 32 bit
    if (bitness == 4 && joat64File.exists())
        throw GAMSException("Expected GAMS system to be 32 bit but found 64 bit instead. System directory: " + mSystemDir.toStdString());
    // if 64 bit
    if (bitness == 8 && !joat64File.exists())
        throw GAMSException("Expected GAMS system to be 64 bit but found 32 bit instead. System directory: " + mSystemDir.toStdString());

    if (mSystemDir != mWorkingDir) {
        QStringList libstems = QStringList() << "gamsxdc" << "gdxdc" << "gmdcc" << "joatdc" << "optdc";
        QString libTmpl = QString(cLibPrefix) + "%1lib" + bitsuf + cLibSuffix;
        for (QString lib: libstems) {
            GAMSPath tmpLib = mWorkingDir / libTmpl.arg(lib);
            if (tmpLib.exists()) {
                MSG   << "--- Warning: Found library " << tmpLib.fileName() << " in the Working Directory (" << mWorkingDir.toStdString() << ").\n"
                      << "---          This could cause a problem when it is a different version than\n"
                      << "---          the one in the System Directory (" + mSystemDir.toStdString() + ").";
            }
        }
    }

    GAMSPlatform::ensureEnvPathContains(mSystemDir.c_str());

    // TODO: all the ProcessStartInfo stuff.

}

GAMSWorkspaceImpl::~GAMSWorkspaceImpl()
{
    DEB << "---- Entering GAMSWorkspaceImpl destructor ----";
    GAMSWorkspacePool::unregisterWorkspacePath(mWorkingDir.toStdString());
    if ((mDebug != GAMSEnum::DebugLevel::KeepFiles) && mUsingTmpWorkingDir) {
        if (!mWorkingDir.rmDirRecurse()) {
            DEB << "Error on cleaning workspace.";
        }
    }
    LoggerPool::instance().registerLogger(static_cast<LogId>(this), mDebug, stdout);
}

bool GAMSWorkspaceImpl::operator==(const GAMSWorkspaceImpl& other) const
{
    return !(operator!=(other));
}

bool GAMSWorkspaceImpl::operator!=(const GAMSWorkspaceImpl& other) const
{
    return mWorkingDir != other.mWorkingDir;
}


string GAMSWorkspaceImpl::registerCheckpoint(std::string checkpointName)
{
    std::lock_guard<std::mutex> lck(mCheckpointLock);
    string name = "";
    if (checkpointName.empty()) {
        name = mScratchFilePrefix + mDefCheckpointNameStem + to_string(mDefCheckpointNameCnt);
        while (mGamsCheckpoints.find(name) != mGamsCheckpoints.end())
            name = mScratchFilePrefix + mDefCheckpointNameStem + to_string(++mDefCheckpointNameCnt);

        mDefCheckpointNameCnt++;
    } else {
        name = checkpointName;
    }
    auto res = mGamsCheckpoints.insert(name);
    if (!res.second)
        throw GAMSException("GAMSWorkspaceImpl: checkpoint '" + name + "' already exists");
    return name;
}


string GAMSWorkspaceImpl::registerModelInstance(const string miName)
{
    std::lock_guard<std::mutex> lck(mModelInstanceLock);
    string name;
    if (miName == "") {
        name = mScratchFilePrefix + mDefModeliNameStem + to_string(mDefModeliNameCnt);
        while (mGamsModelInstances.find(name) != mGamsModelInstances.end())
            name = mScratchFilePrefix + mDefModeliNameStem + to_string(++mDefModeliNameCnt);

        mDefModeliNameCnt++;
    } else {
        name = miName;
    }
    auto res = mGamsModelInstances.insert(name);
    if (!res.second)
        throw GAMSException("GAMSWorkspaceImpl: model-instance '" + name + "' already exists");
    return name;
}


string GAMSWorkspaceImpl::registerJob(const string jobName)
{
    std::lock_guard<std::mutex> lck(mJobLock);
    string name;
    if (jobName == "") {
        name = mScratchFilePrefix + mDefJobNameStem + to_string(mDefJobNameCnt);
        while (mGamsJobs.find(name) != mGamsJobs.end())
            name = mScratchFilePrefix + mDefJobNameStem + to_string(++mDefJobNameCnt);
        mDefJobNameCnt++;
    } else {
        name = jobName;
    }
    auto res = mGamsJobs.insert(name);
    if (!res.second)
        throw GAMSException("GAMSWorkspaceImpl: job '" + name + "' already exists");
    return name;
}

GAMSOptions GAMSWorkspaceImpl::addOptions(GAMSWorkspace& ws, const GAMSOptions& optFrom)
{
    return GAMSOptions(ws, &optFrom);
}

GAMSOptions GAMSWorkspaceImpl::addOptions(GAMSWorkspace& ws, const string& optFile)
{
    return GAMSOptions(ws, optFile);
}

std::string GAMSWorkspaceImpl::writeSource(const string& gamsSource, const string& jobName)
{
    std::string fName = mWorkingDir / (jobName + ".gms");
    std::ofstream sourceWriter(fName);
    sourceWriter << gamsSource;
    sourceWriter.close();
    return fName;
}

std::string GAMSWorkspaceImpl::findSourceFile(const string& fileName)
{
    std::string fName;
    if (GAMSPath(fileName).isAbsolute())
       fName = fileName;
    else
       fName = mWorkingDir / fileName;
    if (!GAMSPath::exists(fName))
       throw GAMSException("Could not create GAMSJob instance from non-existing file [" + fName + "]");
    return fName;
}

GAMSCheckpoint GAMSWorkspaceImpl::addCheckpoint(GAMSWorkspace& ws, const string &checkpointName)
{
    DEB << "---- Entering GAMSCheckpointImpl constructor ----";
    string cpName;
    if(checkpointName == "") {
       cpName = registerCheckpoint();
    } else {
      if (registerCheckpoint(checkpointName).empty())
         throw GAMSException("Checkpoint with name " + checkpointName + " already exists");
      cpName = checkpointName;
    }

    return GAMSCheckpoint(ws, cpName);
}

GAMSJob GAMSWorkspaceImpl::addJobFromFile(GAMSWorkspace& ws, const string& fileName, const string& jobName)
{
    std::string jName = registerJob(jobName);
    if (jName.empty())
       throw GAMSException("Job with name " + jobName + " already exists");
    std::string fName = findSourceFile(fileName);
    return GAMSJob(ws, jName, fName, nullptr);

}

GAMSJob GAMSWorkspaceImpl::addJobFromFile(GAMSWorkspace& ws, const string& fileName, const GAMSCheckpoint& checkpoint, const string& jobName)
{
    std::string jName = registerJob(jobName);
    if (jName.empty())
       throw GAMSException("Job with name " + jobName + " already exists");
    std::string fName = findSourceFile(fileName);
    return GAMSJob(ws, jName, fName, &checkpoint);
}

GAMSJob GAMSWorkspaceImpl::addJobFromString(GAMSWorkspace& ws, const string& gamsSource, const string& jobName)
{
    std::string jName = registerJob(jobName);
    if (jName.empty())
       throw GAMSException("Job with name " + jobName + " already exists");
    std::string fName = writeSource(gamsSource, jName);
    return GAMSJob(ws, jName, fName, nullptr);
}

GAMSJob GAMSWorkspaceImpl::addJobFromString(GAMSWorkspace& ws, const string& gamsSource, const GAMSCheckpoint& checkpoint, const string& jobName)
{
    std::string jName = registerJob(jobName);
    if (jName.empty())
       throw GAMSException("Job with name " + jobName + " already exists");
    std::string fName = writeSource(gamsSource, jName);
    return GAMSJob(ws, jName, fName, &checkpoint);
}

GAMSDatabase GAMSWorkspaceImpl::addDatabase(GAMSWorkspace& ws, const string& databaseName, const string& inModelName)
{
     return GAMSDatabase(ws, specValues, databaseName, inModelName);
}

GAMSDatabase GAMSWorkspaceImpl::addDatabase(GAMSWorkspace& ws, const GAMSDatabase& sourceDatabase, const string& databaseName, const string& inModelName)
{
    return GAMSDatabase(ws, specValues, sourceDatabase.mImpl, databaseName, inModelName);
}

GAMSDatabase GAMSWorkspaceImpl::addDatabaseFromGDXForcedName(GAMSWorkspace& ws, const string& gdxFileName, const string& databaseName, const string& inModelName)
{
    return GAMSDatabase(gdxFileName, ws, specValues, databaseName, inModelName, true);
}

GAMSDatabase GAMSWorkspaceImpl::addDatabaseFromGDX(GAMSWorkspace& ws, const string& gdxFileName, const string& databaseName, const string& inModelName)
{
    return GAMSDatabase(gdxFileName, ws, specValues, databaseName, inModelName);
}

GAMSDatabase GAMSWorkspaceImpl::addDatabaseFromGMD(GAMSWorkspace& ws, void* gmdPtr)
{
    return GAMSDatabase(gmdPtr, ws);
}

string GAMSWorkspaceImpl::findGAMS()
{
    string GAMSDir = GAMSPlatform::findGams(logID());
    return GAMSDir;
}

void GAMSWorkspaceImpl::xxxLib(string libname, string model)
{
    QProcess proc;
    proc.setProgram(mSystemDir / (libname + "lib" + cExeSuffix));
    proc.setArguments(QStringList() << QString::fromStdString(model));
    proc.setWorkingDirectory(mWorkingDir);
    proc.start();
    if (!proc.waitForStarted())
        throw GAMSException(libname + "lib process could not be started");
    proc.waitForFinished(-1);

    MSG << proc.readAll();

    if (proc.exitCode() != 0)
        throw GAMSException(libname + "lib return code not 0 (" + std::to_string(proc.exitCode()) + ")");
}

void GAMSWorkspaceImpl::setScratchFilePrefix(const string& scratchFilePrefix)
{
    if (scratchFilePrefix.find_first_of("\t\n ")!=string::npos)
        throw GAMSException("ScratchFilePrefix must not contain whitespaces");
    mScratchFilePrefix = scratchFilePrefix;
}


string GAMSWorkspaceImpl::registerDatabase(const string databaseName)
{
    std::lock_guard<std::mutex> lck(mRegisterDatabaseLock);
    string name;
    if (databaseName == "")
        name = nextDatabaseName();
    else
        name = databaseName;
    auto res = mGamsDatabases.insert(name);
    if (!res.second)
        throw GAMSException("GAMSWorkspaceImpl: database '" + name + "' already exists");
    return name;
}

string GAMSWorkspaceImpl::nextDatabaseName()
{
    std::lock_guard<std::mutex> lck(mNextDatabaseNameLock);
    string name = mScratchFilePrefix + mDefDBNameStem + to_string(mDefDBNameCnt);
    while (mGamsDatabases.find(name) != mGamsDatabases.end())
        name = mScratchFilePrefix + mDefDBNameStem + to_string(++mDefDBNameCnt);
    mDefDBNameCnt++;
    return name;
}


string GAMSWorkspaceImpl::optFileExtension(int optfile)
{
    if (optfile < 2)
        return "opt";
    else if (optfile < 10)
        return "op" + std::to_string(optfile);
    else if (optfile < 100)
        return "o" + std::to_string(optfile);
    else
        return "" + std::to_string(optfile);
}

} // namespace gams
