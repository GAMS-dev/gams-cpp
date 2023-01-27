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
#include "gamscheckpoint.h"
#include "gamslog.h"
#include "gamsoptions.h"
#include "gamsplatform.h"
#include "gamspath.h"
#include "gamsoptions.h"
#include "gamsexceptionexecution.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <array>
#include <cpr/cpr.h>

using namespace std;

namespace gams {

GAMSJobImpl::GAMSJobImpl(GAMSWorkspace& workspace,
                         const string& jobName,
                         const string& fileName,
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

string GAMSJobImpl::prepareRun(GAMSOptions* tmpOptions, const GAMSCheckpoint* checkpoint,
                               GAMSCheckpoint* tmpCP, ostream* output, bool createOutDb, bool relativePaths,
                               set<string> dbPaths, vector<GAMSDatabase> databases)
{
    // TODO (RG): check if tmpCP needs to be deleted

    if (mCheckpointStart) {
        if (relativePaths)
            tmpOptions->setRestart(filesystem::relative(mCheckpointStart->fileName(), mWs.workingDirectory()).string());
        else
            tmpOptions->setRestart(mCheckpointStart->fileName());
    }

    if (checkpoint) {
        if (mCheckpointStart != checkpoint) {
            tmpCP = new GAMSCheckpoint(mWs, "");
            if (relativePaths)
                tmpOptions->setSave(filesystem::relative(tmpCP->fileName(), mWs.workingDirectory()).string());
            else tmpOptions->setSave(tmpCP->fileName());
        } else {
            if (relativePaths)
                tmpOptions->setSave(filesystem::relative(checkpoint->fileName(), mWs.workingDirectory()).string());
            else tmpOptions->setSave(checkpoint->fileName());
        }
    }

    if (mWs.debug() >= GAMSEnum::DebugLevel::ShowLog) {
        tmpOptions->setLogOption(3);
    } else {
        // can only happen if we are called from GAMSModelInstance
        if (tmpOptions->logOption() != 2) {
            if (output == nullptr)
                tmpOptions->setLogOption(0);
            else tmpOptions->setLogOption(3);
        }
    }

    if (!databases.empty()) {
        for (GAMSDatabase db: databases) {
            filesystem::path p = mWs.workingDirectory();
            p /= db.name() + ".gdx";
            dbPaths.insert(p);

            db.doExport("");
            if (db.inModelName() != "")
                tmpOptions->setDefine(db.inModelName(), db.name());
        }
    }
    GAMSPath jobFileInfo(GAMSPath(mWs.workingDirectory()) / mJobName);

    if (createOutDb && tmpOptions->gdx() == "")
        tmpOptions->setGdx(mWs.nextDatabaseName());

    if (tmpOptions->logFile() == "")
        tmpOptions->setLogFile(jobFileInfo.suffix(".log").toStdString());

    if (!tmpOptions->output().empty())
        tmpOptions->setOutput(mJobName + ".lst");

    if (relativePaths) {
        tmpOptions->setInput(filesystem::relative(mFileName, mWs.workingDirectory()).string());
    } else {
        tmpOptions->setCurDir(mWs.workingDirectory());
        tmpOptions->setInput(mFileName);
    }

    filesystem::path pfFile = mWs.workingDirectory();
    pfFile /= mJobName + ".pf";
    tmpOptions->writeOptionFile(pfFile.string());

    return pfFile.string();
}

void GAMSJobImpl::run(GAMSOptions* gamsOpt, const GAMSCheckpoint* checkpoint,
                      ostream* output, bool createOutDb, vector<GAMSDatabase> databases)
{
    GAMSOptions tmpOpt(mWs, gamsOpt);
    GAMSCheckpoint* tmpCP = nullptr;
    string pfFileName = prepareRun(&tmpOpt, checkpoint, tmpCP, output, false);

    filesystem::path gamsExe = filesystem::path(mWs.systemDirectory());
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
        filesystem::remove(pfFileName);
    }
}

void GAMSJobImpl::runEngine(GAMSEngineConfiguration engineConfiguration, GAMSOptions &gamsOptions,
                            GAMSCheckpoint* checkpoint, ostream *output, set<string> extraModelFiles,
                            unordered_map<string, string> engineOptions, bool createOutDB,
                            bool removeResults, vector<GAMSDatabase> databases)
{
    GAMSOptions tmpOpt(mWs, &gamsOptions);
    GAMSCheckpoint* tmpCp = nullptr;
    set<string> dbPaths = set<string>();

    string pfFileName = prepareRun(&tmpOpt, checkpoint, tmpCp, output,
                                   createOutDB, true, dbPaths, databases);

    // TODO(rogo): move down to where its needed
    bool captureOutput = mWs.debug() >= GAMSEnum::ShowLog || output;

    string mainFileName;
    if (filesystem::exists(mFileName))
        mainFileName = mFileName;
    else
        mainFileName = mFileName.append(".gms");

    set<string> modelFiles = { mainFileName, pfFileName };
    modelFiles.insert(dbPaths.begin(), dbPaths.end());

    if (mCheckpointStart)
        modelFiles.insert(mCheckpointStart->fileName());

    if (!extraModelFiles.empty()) {
        set<string> extraModelFilesCleaned = set<string>();
        for (const string& f : extraModelFiles) {
            filesystem::path p(f);
            if (p.is_absolute())
                extraModelFilesCleaned.insert(f);
            else extraModelFilesCleaned.insert(filesystem::absolute(mWs.workingDirectory()) / f);
        }
        modelFiles.insert(extraModelFilesCleaned.begin(), extraModelFilesCleaned.end());
    }

    string dataZipName;
    int dataZipCount = 1;

    while (filesystem::exists(mWs.workingDirectory()
                              .append(to_string(filesystem::path::preferred_separator))
                              .append("_gams_data")
                              .append(to_string(dataZipCount)))) {
        dataZipCount++;
    }
    dataZipName = mWs.workingDirectory()
                        .append(to_string(filesystem::path::preferred_separator))
                        .append("_gams_data")
                        .append(to_string(dataZipCount));

    // TODO(rogo): add platform switch
    string gmsZip = "gmszip"; // +.exe on windows i assume, what about macos?

    GAMSPath zipPath(mWs.systemDirectory(), gmsZip);
    string zipCmd = zipPath.append(" " + dataZipName);
    for (const GAMSPath &f : modelFiles) {
        if (!f.exists())
            throw GAMSException("File " + f.string() + " is missing.");

        if (f.is_absolute())
            zipCmd.append(" " + filesystem::relative(mWs.workingDirectory(), f).string());
        else zipCmd.append(" " + f.string());
    }
    system(zipCmd.c_str());

    unordered_map<string, string> requestParams = unordered_map<string,string>();
    if (!engineOptions.empty())
        requestParams.insert(engineOptions.begin(), engineOptions.end());

    unordered_map<string, string> queryParams = unordered_map<string, string>(requestParams);
    queryParams = engineOptions;

    queryParams["namespace"] = engineConfiguration.space();

    if (requestParams.count("engineOptions") ||
            requestParams.count("model_data"))
        throw GAMSException("'engineOptions' must not include keys 'data' or 'model_data'. "
                            "Please use 'extraModelFiles' to provide additional files to send to GAMS Engine.");

    if (requestParams.count("inex_file")) {
        if (!filesystem::exists(queryParams["inex_file"])) {
            throw GAMSException("The 'inex_file' '" + queryParams["inex_file"] + "' does not exist.");
        }
    }


    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
                               cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
                               cpr::Parameters{{"anon", "true"}, {"key", "value"}});
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
