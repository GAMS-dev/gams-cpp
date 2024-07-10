/**
 * GAMS C++ API
 *
 * Copyright (c) 2017-2024 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2024 GAMS Development Corp. <support@gams.com>
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

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <array>

#include "gamslog.h"
#include "gamsworkspaceimpl.h"
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

    copy(begin(CSpecValues), end(CSpecValues), begin(specValues));

    char* envDebug = getenv("GAMSOOAPIDEBUG");
    if (envDebug && envDebug[0] == '\0') {
        for (unsigned int i = 0; i < (unsigned)strlen(envDebug); i++)
            envDebug[i] = tolower(envDebug[i]);

        DEB << envDebug;
        if (strcmp("off", envDebug))
            mDebug = GAMSEnum::Off;
        else if (strcmp("keepfilesonerror", envDebug))
            mDebug = GAMSEnum::KeepFilesOnError;
        else if (strcmp("keepfiles", envDebug))
            mDebug = GAMSEnum::KeepFiles;
        else if (strcmp("showlog", envDebug))
            mDebug = GAMSEnum::ShowLog;
        else if (strcmp("verbose", envDebug))
            mDebug = GAMSEnum::Verbose;
    }

    // handle working directory
    mUsingTmpWorkingDir = workingDir.empty();
    if (mUsingTmpWorkingDir) {
        GAMSPath tempDir = mWorkingDir.tempDir(filesystem::temp_directory_path().string());
        if (!tempDir.exists()) {
            throw GAMSException("Cannot create temp-workspace directory: " + tempDir.toStdString());
        }
        mWorkingDir = tempDir;
    } else {
        mWorkingDir = workingDir;
    }

    if (!mWorkingDir.exists() && !mWorkingDir.mkDir()) {
        throw GAMSException("Cannot create workspace directory: " + mWorkingDir.toStdString());
    }
    mWorkingDir.pack();
    GAMSWorkspacePool::registerWorkspacePath(mWorkingDir.toStdString());

    // handle system directory
    mSystemDir = systemDir.empty() ? GAMSPlatform::findGams(logID()) : systemDir;
    if (!mSystemDir.exists()) {
        throw GAMSException("Invalid GAMS system directory: " + systemDir);
    }
    mSystemDir.pack();

    ostringstream sstream;
    sstream << cLibPrefix << "joatdclib64" << cLibSuffix;
    string lib = sstream.str();

    GAMSPath joat64File = mSystemDir / lib;

    DEB << joat64File.string();

    if (mSystemDir != mWorkingDir) {
        vector<string> libstems = {"gdxdc", "gmdcc", "joatdc", "optdc"};
        for (const string &lib: libstems) {
            ostringstream libstream;
            libstream << cLibPrefix << lib << "64" << cLibSuffix;
            string libTmpl = libstream.str();

            GAMSPath tmpLib = mWorkingDir / libTmpl;
            if (tmpLib.exists()) {
                MSG   << "\n--- Warning: Found library " << tmpLib.filename() << " in the Working Directory (" << mWorkingDir.toStdString() << ")."
                      << "\n---          This could cause a problem when it is a different version than"
                      << "\n---          the one in the System Directory (" + mSystemDir.toStdString() + ").";
            }
        }
    }

    GAMSPlatform::ensureEnvPathContains(mSystemDir.c_str());
    // Check GAMS version of the system directory
    string compiledVersion = GAMSVersion::gamsVersion();
    string systemVersion = GAMSVersion::systemVersion(mSystemDir.toStdString());
    if (systemVersion.empty()) {
        MSG << "Warning: The GAMS version at '" << mSystemDir.c_str() << "' could not be determined.";
    } else if (compiledVersion != systemVersion) {
        MSG << "\n--- Warning: This library is developed for GAMS version " << compiledVersion << "."
            << "\n---          The selected system path '" << mSystemDir.c_str() << "' contains GAMS version " << systemVersion << "."
            << "\n---          This may result in unpredictable behavior.";
    }

    // TODO: all the ProcessStartInfo stuff.
}

GAMSWorkspaceImpl::~GAMSWorkspaceImpl()
{
    DEB << "---- Entering GAMSWorkspaceImpl destructor ----";
    GAMSWorkspacePool::unregisterWorkspacePath(mWorkingDir.toStdString());
    if (mUsingTmpWorkingDir &&
        (mDebug == GAMSEnum::DebugLevel::Off || (mDebug == GAMSEnum::DebugLevel::KeepFilesOnError && !mHasError))) {
        if ( (mWorkingDir.rmDirRecurse())) {
            MSG << "Error on cleaning workspace.";
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


string GAMSWorkspaceImpl::registerCheckpoint(string checkpointName)
{
    lock_guard<std::mutex> lck(mCheckpointLock);
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
    lock_guard<std::mutex> lck(mModelInstanceLock);
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
    lock_guard<std::mutex> lck(mJobLock);
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

string GAMSWorkspaceImpl::writeSource(const string& gamsSource, const string& jobName)
{
    string fName = mWorkingDir / (jobName + ".gms");
    ofstream sourceWriter(fName);
    sourceWriter << gamsSource;
    sourceWriter.close();
    return fName;
}

string GAMSWorkspaceImpl::findSourceFile(const string& fileName)
{
    string fName;
    if (GAMSPath(fileName).is_absolute())
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
    string jName = registerJob(jobName);
    if (jName.empty())
       throw GAMSException("Job with name " + jobName + " already exists");
    string fName = findSourceFile(fileName);
    return GAMSJob(ws, jName, fName, nullptr);

}

GAMSJob GAMSWorkspaceImpl::addJobFromFile(GAMSWorkspace& ws, const string& fileName, const GAMSCheckpoint& checkpoint, const string& jobName)
{
    string jName = registerJob(jobName);
    if (jName.empty())
       throw GAMSException("Job with name " + jobName + " already exists");
    string fName = findSourceFile(fileName);
    return GAMSJob(ws, jName, fName, &checkpoint);
}

GAMSJob GAMSWorkspaceImpl::addJobFromString(GAMSWorkspace& ws, const string& gamsSource, const string& jobName)
{
    string jName = registerJob(jobName);
    if (jName.empty())
       throw GAMSException("Job with name " + jobName + " already exists");
    string fName = writeSource(gamsSource, jName);
    return GAMSJob(ws, jName, fName, nullptr);
}

GAMSJob GAMSWorkspaceImpl::addJobFromString(GAMSWorkspace& ws, const string& gamsSource, const GAMSCheckpoint& checkpoint, const string& jobName)
{
    string jName = registerJob(jobName);
    if (jName.empty())
       throw GAMSException("Job with name " + jobName + " already exists");
    string fName = writeSource(gamsSource, jName);
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

void GAMSWorkspaceImpl::xxxLib(string libname, string model)
{
    string lib = libname + "lib" + cExeSuffix;
    GAMSPath libPath(mSystemDir / lib);

    string output;
    int exitCode = GAMSPlatform::runProcess(mWorkingDir.string(), libPath.string(), model, output);
    if (exitCode != 0)
        throw GAMSException(libname + "lib return code not 0 (" + to_string(exitCode) + ")");
}

void GAMSWorkspaceImpl::setScratchFilePrefix(const string& scratchFilePrefix)
{
    if (scratchFilePrefix.find_first_of("\t\n ")!=string::npos)
        throw GAMSException("ScratchFilePrefix must not contain whitespaces");
    mScratchFilePrefix = scratchFilePrefix;
}


string GAMSWorkspaceImpl::registerDatabase(const string databaseName)
{
    lock_guard<std::mutex> lck(mRegisterDatabaseLock);
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
    lock_guard<std::mutex> lck(mNextDatabaseNameLock);
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
        return "op" + to_string(optfile);
    else if (optfile < 100)
        return "o" + to_string(optfile);
    else
        return "" + to_string(optfile);
}

bool GAMSWorkspaceImpl::hasError() const
{
    return mHasError;
}

void GAMSWorkspaceImpl::setHasError(bool newHasError)
{
    mHasError = newHasError;
}

} // namespace gams
