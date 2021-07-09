/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2021 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2021 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMSWORKSPACEIMPL_H
#define GAMSWORKSPACEIMPL_H

#include <string>
#include <set>
#include "gamscheckpoint.h"
#include "gamsdatabase.h"
#include "gamsjob.h"
#include "gamsenum.h"
#include "gclgms.h"
#include "gamspath.h"
#include "gamsversion.h"

#include <mutex>

namespace gams
{

class GAMSWorkspaceImpl
{

public:
    /// Initialize
    /// \param workingDir GAMS working directory, anchor for all file-based operations (determined automatically if omitted, in user's temporary folder)
    /// \param systemDir  GAMS system directory (determined automatically if omitted)
    /// \param debug Debug Flag
    GAMSWorkspaceImpl(const std::string& workingDir, const std::string& systemDir, GAMSEnum::DebugLevel debug);

    GAMSWorkspaceImpl(const GAMSWorkspaceImpl& ws);

    ~GAMSWorkspaceImpl();

    bool operator==(const GAMSWorkspaceImpl& other) const;

    bool operator!=(const GAMSWorkspaceImpl& other) const;

    double specValues[5];

    std::string writeSource(const std::string &gamsSource, const std::string &jobName);
    std::string findSourceFile(const std::string &fileName);

    GAMSCheckpoint addCheckpoint(GAMSWorkspace& ws, const std::string& checkpointName);

    GAMSJob addJobFromFile(GAMSWorkspace& ws, const std::string& fileName, const std::string& jobName = "");

    GAMSJob addJobFromFile(GAMSWorkspace& ws, const std::string& fileName, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    GAMSJob addJobFromString(GAMSWorkspace& ws, const std::string& gamsSource, const std::string& jobName = "");

    GAMSJob addJobFromString(GAMSWorkspace& ws, const std::string& gamsSource, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    GAMSDatabase addDatabase(GAMSWorkspace& ws, const std::string& databaseName = "", const std::string& inModelName = "");

    GAMSDatabase addDatabase(GAMSWorkspace& ws, const GAMSDatabase& sourceDatabase, const std::string& databaseName = ""
            , const std::string& inModelName = "");

    GAMSDatabase addDatabaseFromGDXForcedName(GAMSWorkspace &ws, const std::string& gdxFileName
                                              , const std::string& databaseName, const std::string& inModelName);

    GAMSDatabase addDatabaseFromGDX(GAMSWorkspace &ws, const std::string& gdxFileName, const std::string& databaseName = ""
            , const std::string& inModelName = "");

    GAMSDatabase addDatabaseFromGMD(GAMSWorkspace &ws, void* gmdPtr);

    /// Registers a GAMSDatabase at the workspace
    /// \param databaseName Database name to add (empty: generate default name)
    /// \returns Database name if all worked well, else an empty string
    std::string registerDatabase(const std::string databaseName = "");
    std::string nextDatabaseName();

    /// Registers a GAMSCheckpoint at the workspace
    /// \param checkpointName Checkpoint name to add (empty: generate default name)
    /// \returns Checkpoint name if all worked well, else an empty string
    std::string registerCheckpoint(std::string checkpointName = "");

    /// Registers a GAMSModelInstance at the workspace
    /// \param modelInstanceName Model instance name to add (empty: generate default name)
    /// \returns ModelInstance name if all worked well, else an empty string
    std::string registerModelInstance(const std::string miName = "");

    /// Registers a GAMSJob at the workspace
    /// \param jobName  Job name to add (empty: generate default name)
    /// \returns        Job name if all worked well, else an empty string
    std::string registerJob(const std::string jobName = "");

    GAMSOptions addOptions(GAMSWorkspace& ws, const GAMSOptions& optFrom);
    GAMSOptions addOptions(GAMSWorkspace& ws, const std::string& optFile = "");

    LogId logID() { return static_cast<LogId>(this); }

    void xxxLib(std::string libname, std::string model);

    GAMSEnum::DebugLevel debug() { return mDebug; }

    GAMSPath workingDir() const {return mWorkingDir;}
    GAMSPath systemDir() const {return mSystemDir;}
    bool usingTmpWorkingDir() {return mUsingTmpWorkingDir;}

    std::string gamsVersion() const {return GAMSVersion::gamsVersion();}
    int gamsRelease() const {return GAMSVersion::gamsMajor();}
    int gamsSubRel() const {return GAMSVersion::gamsMinor();}
    int gamsGoldRel() const {return GAMSVersion::gamsBuild();}

    static std::string apiVersionDetail() {return GAMSVersion::apiDetail();}
    static std::string apiVersion() {return GAMSVersion::api();}
    static int apiRelease() {return GAMSVersion::apiMajor();}
    static int apiSubRel() {return GAMSVersion::apiMinor();}
    static int apiGoldRel() {return GAMSVersion::apiBuild();}

    std::string scratchFilePrefix() const {return mScratchFilePrefix;}
    void setScratchFilePrefix(const std::string &scratchFilePrefix);

    std::string optFileExtension(int index);

private:

    GAMSPath mWorkingDir;
    GAMSPath mSystemDir;

    std::string mScratchFilePrefix = "_gams_cpp_";
    bool mUsingTmpWorkingDir = false;

    GAMSEnum::DebugLevel mDebug = GAMSEnum::DebugLevel::Off;

    std::set<std::string> mGamsCheckpoints;
    std::set<std::string> mGamsDatabases;
    std::set<std::string> mGamsJobs;
    std::set<std::string> mGamsModelInstances;

    std::mutex mCheckpointLock;
    std::mutex mRegisterDatabaseLock;
    std::mutex mNextDatabaseNameLock;
    std::mutex mJobLock;
    std::mutex mModelInstanceLock;
    std::mutex mDebugLock;

    int mDefDBNameCnt = 0;
    int mDefJobNameCnt = 0;
    int mDefCheckpointNameCnt = 0;
    int mDefModeliNameCnt = 0;

    std::string mDefDBNameStem = "gdb";
    std::string mDefJobNameStem = "gjo";
    std::string mDefCheckpointNameStem = "gcp";
    std::string mDefModeliNameStem = "gmi";
};

} // namespace gams

#endif // GAMSWORKSPACEIMPL_H
