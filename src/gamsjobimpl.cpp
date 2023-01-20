/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2023 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2023 GAMS Development Corp. <support@gams.com>
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

#include "gamsjobimpl.h"
#include "gmomcc.h"
#include "gamscheckpoint.h"
#include "gamslog.h"
#include "gamsoptions.h"
#include "gamsplatform.h"
#include "gamspath.h"
#include "gamsexceptionexecution.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <array>

using namespace std;

namespace gams {

GAMSJobImpl::GAMSJobImpl(GAMSWorkspace& workspace,
                         const std::string& jobName,
                         const std::string& fileName,
                         const GAMSCheckpoint* checkpoint)
    : mWs(workspace), mJobName(jobName), mFileName(fileName)
{
    DEB << "---- Entering GAMSJob constructor ----";
    if (checkpoint != nullptr) {
        if (!GAMSPath::exists(checkpoint->fileName()) )
            throw GAMSException("Checkpoint file " + checkpoint->fileName() + " does not exist");
        mCheckpointStart = new GAMSCheckpoint(*checkpoint);
    }
}

bool GAMSJobImpl::operator!=(const GAMSJobImpl& other) const
{
    return (mWs != other.mWs) || (mJobName != other.mJobName);
}

bool GAMSJobImpl::operator==(const GAMSJobImpl& other) const
{
    return !(operator!=(other));
}

GAMSDatabase GAMSJobImpl::outDB()
{
    return mOutDb;
}

GAMSJobImpl::~GAMSJobImpl() {
    delete mCheckpointStart; // this is intended to only free the wrapper, not the *Impl if used anywhere
}

void GAMSJobImpl::run(GAMSOptions* gamsOptions, GAMSCheckpoint* checkpoint, ostream* output, bool createOutDb,
                      vector<GAMSDatabase> databases)
{
    // TODO(JM) backward replacement of pointer logic with instance of gamsOptions
    GAMSOptions tmpOpt = GAMSOptions(mWs, gamsOptions);
    GAMSCheckpoint* tmpCP = nullptr;

    if (mCheckpointStart)
        tmpOpt.setRestart(mCheckpointStart->fileName());

    if (checkpoint) {
        if (mCheckpointStart != checkpoint) {
            tmpCP = new GAMSCheckpoint(mWs, "");
            tmpOpt.setSave(tmpCP->fileName());
        } else {
            tmpOpt.setSave(checkpoint->fileName());
        }
    }

    if (mWs.debug() >= GAMSEnum::DebugLevel::ShowLog) {
        tmpOpt.setLogOption(3);
    } else {
        // can only happen if we are called from GAMSModelInstance
        if (tmpOpt.logOption() != 2) {
            if (output == nullptr)
                tmpOpt.setLogOption(0);
            else
                tmpOpt.setLogOption(3);
        }
    }

    if (!databases.empty()) {
        for (GAMSDatabase db: databases) {
            db.doExport("");
            if (db.inModelName() != "")
                tmpOpt.setDefine(db.inModelName(), db.name());
        }
    }
    GAMSPath jobFileInfo(GAMSPath(mWs.workingDirectory()) / mJobName);

    if (createOutDb && tmpOpt.gdx() == "")
        tmpOpt.setGdx(mWs.nextDatabaseName());

    if (tmpOpt.logFile() == "")
        tmpOpt.setLogFile(jobFileInfo.suffix(".log").toStdString());

    if (!tmpOpt.output().empty())
        tmpOpt.setOutput(mJobName + ".lst");
    tmpOpt.setCurDir(mWs.workingDirectory());
    tmpOpt.setInput(mFileName);
    GAMSPath pfFileName = jobFileInfo.suffix(".pf");
    try {
        tmpOpt.writeOptionFile(pfFileName);
    } catch (GAMSException& e) {
        throw GAMSException(e.what() + (" for GAMSJob " + mJobName));
    }

    auto gamsExe = filesystem::path(mWs.systemDirectory());
    gamsExe.append(string("gams") + cExeSuffix);

    string args = "dummy pf=";
    GAMSPath pf(mWs.workingDirectory(), mJobName + ".pf");
    args.append(pf.string());

    string result;
    int exitCode = runProcess(gamsExe.string(), args, result);

    if (createOutDb) {
        GAMSPath gdxPath(tmpOpt.gdx());
        if (!gdxPath.is_absolute())
            gdxPath = GAMSPath(mWs.workingDirectory()) / gdxPath;

        gdxPath.setSuffix(".gdx");
        if (gdxPath.exists())
            mOutDb = mWs.addDatabaseFromGDXForcedName(gdxPath.toStdString(), gdxPath.suffix("").filename().string(), "");
    }

    if (output && mWs.debug() >= GAMSEnum::DebugLevel::ShowLog)
        MSG << result;
    else if (output)
        *output << result;
    if (exitCode != 0) {
        cerr << "GAMS Error code: " << exitCode << std::endl;
        cerr << "  with args: " << args << std::endl;
        cerr << "  in " << mWs.workingDirectory() << std::endl;
        if ((mWs.debug() < GAMSEnum::DebugLevel::KeepFiles) && mWs.usingTmpWorkingDir())
            throw GAMSExceptionExecution("GAMS return code not 0 (" + to_string(exitCode) +
                                         "), set GAMSWorkspace.Debug to KeepFiles or higher or define the \
                                         GAMSWorkspace.WorkingDirectory to receive a listing file with more details",
                                         exitCode);
        else
            throw GAMSExceptionExecution("GAMS return code not 0 (" + to_string(exitCode) + "), check " +
                                         (GAMSPath(mWs.workingDirectory()) / tmpOpt.output()).toStdString() +
                                         " for more details", exitCode);
    }
    if (tmpCP) {
        GAMSPath implFile(checkpoint->fileName());
        if (implFile.exists())
            implFile.remove();

        implFile = tmpCP->fileName();
        implFile.rename(checkpoint->fileName());
        delete tmpCP; tmpCP=nullptr;
    }
    if (mWs.debug() < GAMSEnum::DebugLevel::KeepFiles) {
        // TODO(RG): this is not good style, but apparently needed
        try { pfFileName.remove(); } catch (...) { }
    }
}

bool GAMSJobImpl::interrupt()
{
    /*qint64*/ int pid = 0 /*mProc.processId()*/; // TODO(RG): we need std process handling here
    if(pid == 0)
        return false;
    return GAMSPlatform::interrupt(pid);
}

int GAMSJobImpl::runProcess(const string what, const string args, string& output)
{
    ostringstream ssp;
    string result;
    FILE* out;

#ifdef _WIN32
    filesystem::path p = filesystem::current_path();

    ssp << "\"" << what << "\" " << args ;
    out = _popen(ssp.str().c_str(), "rt");
#else
    ssp << "\"" << what << "\" " << args;
    out = popen(ssp.str().c_str(), "r");
#endif
    if (!out) {
        std::cerr << "Couldn't start command: " << ssp.str() << std::endl;
        return -1;
    }
    std::array<char, 128> buffer;
    while (fgets(buffer.data(), 128, out))
        result += buffer.data();

    output = result;

    int exitCode;
#ifdef _WIN32
    exitCode = _pclose(out);
#else
    exitCode = pclose(out);
#endif
    return exitCode;
}

}
