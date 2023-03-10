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
#include <nlohmann/json.hpp>

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

// TODO(RG): move zip and unzip to a more fitting class?
void GAMSJobImpl::zip(string zipName, set<string> files)
{
    string gmsZip = "gmszip";
    gmsZip.append(cExeSuffix);

    cout << "zipping: " << zipName << endl; // TODO(rogo): delete this
    filesystem::path zipPath(mWs.systemDirectory());
    string zipCmd = zipPath.append(gmsZip + " " + zipName);
    for (const GAMSPath &f : files) {
        if (!f.exists())
            throw GAMSException("File " + f.string() + " is missing.");

        zipCmd.append(" -j " + f.string()); // -j: dont record directory names
    }
    system(zipCmd.c_str());
}

void GAMSJobImpl::unzip(string zipName, string destination)
{
    string gmsUnzip = "gmsunzip";
    gmsUnzip.append(cExeSuffix);

    filesystem::path zipPath(mWs.systemDirectory());
    string unzipCmd = zipPath.append(gmsUnzip + " " + zipName + " -o"); // -o: overwrite existing without asking
    if (!destination.empty())
        unzipCmd.append(" -d " + destination);

    system(unzipCmd.c_str());
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
                              .append("/_gams_data")
                              .append(to_string(dataZipCount)))) {
        dataZipCount++;
    }
    dataZipName = mWs.workingDirectory()
                        .append("/_gams_data")
                        .append(to_string(dataZipCount))
                        .append(".zip");

    zip(dataZipName, modelFiles);

    unordered_map<string, string> requestParams = unordered_map<string, string>();
    if (!engineOptions.empty())
        requestParams.insert(engineOptions.begin(), engineOptions.end());

    unordered_map<string, string> queryParams = unordered_map<string, string>(requestParams);

    cpr::Multipart fileParams{};

    queryParams["namespace"] = engineConfiguration.space();

    if (requestParams.count("engineOptions") || requestParams.count("model_data"))
        throw GAMSException("'engineOptions' must not include keys 'data' or 'model_data'. "
                            "Please use 'extraModelFiles' to provide additional files to send to GAMS Engine.");

// TODO(RG): re-add the following code
//    if (requestParams.count("inex_file")) {
//        if (!filesystem::exists(queryParams["inex_file"])) {
//            throw GAMSException("The 'inex_file' '" + queryParams["inex_file"] + "' does not exist.");
//        }
//        fileParams.parts.emplace_back(cpr::Part("file", cpr::File(queryParams["inex_file"]), "application/json"));
//        queryParams.erase("inex_file");
//    } else if (!requestParams.count("inex_string")) {
//        inexFile excludeFiles("exclude");
//        for (const std::string &f : modelFiles) {
//            fileParams.parts.emplace_back(cpr::Part("inex_file", cpr::File(f), "application/json") );
//            excludeFiles.files.emplace_back(filesystem::relative(f, mWs.workingDirectory()));
//        }
//        // TODO(RG): fileParams.insert(JSON.serializedObject(excludeFiles));
//        //             see GAMSJob.cs line 845
//    }

    cout << "datazip: " << dataZipName << endl; // TODO(rogo): delete this
    if (requestParams.count("model")) {
// TODO(rogo): original line:
//        fileParams.parts.emplace_back(cpr::Part("data", cpr::File(dataZipName), filesystem::path(dataZipName).stem()));
        cout << "test this:" << endl; // TODO(rogo): delete this
        fileParams.parts.emplace_back(cpr::Part(dataZipName, cpr::File(dataZipName)));
    } else {
        queryParams["run"] = tmpOpt.input();
        queryParams["model"] = GAMSPath(filesystem::path(tmpOpt.input()).parent_path(),
                                        filesystem::path(tmpOpt.input()).stem());
//        fileParams.parts.emplace_back(cpr::Part("model-data", cpr::File(dataZipName)));
        fileParams.parts.emplace_back(cpr::Part("model_data", cpr::File(dataZipName), filesystem::path(dataZipName).stem()));
    }

    queryParams["arguments"] = "pf=" + mJobName + ".pf";

    cpr::Parameters encodedParams;
    cout << "encoded params: " << endl; // TODO(rogo): delete this
    for (const auto &p : queryParams){
        encodedParams.Add(cpr::Parameter(p.first, p.second));
        cout << p.first << "=" << p.second << endl; // TODO(rogo): delete this
    }

    cout << "posting job..." << endl;
    cpr::Response response = cpr::Post(cpr::Url{engineConfiguration.host() + "/jobs"},
                                       cpr::Authentication{engineConfiguration.username(),
                                                         engineConfiguration.password(),
                                                         cpr::AuthMode::BASIC},
                                        fileParams, encodedParams);

    cout << "url: " << response.url << endl; // TODO(rogo): delete this
    cout << "status: " << response.status_code << endl;
    if (!cpr::status::is_success(response.status_code)) {
        throw GAMSException("Creating job on GAMS Engine failed with status code: "
                            + to_string(response.status_code) + "." " Message: " + response.text);
    }

    auto json = nlohmann::json::parse(response.text);
    mEngineJob = new GAMSEngineJob(json.at("token").get<string>(), engineConfiguration);

    int exitCode = 0;
    while (true) {
        response = cpr::Delete(cpr::Url{ engineConfiguration.host() + "/jobs/" + mEngineJob->token() + "/unread-logs" },
                               cpr::Authentication{engineConfiguration.username(),
                                                 engineConfiguration.password(),
                                                 cpr::AuthMode::BASIC});

        if (response.status_code == 403) { // job still in queue
            cout << "job still in queue. waiting..." << endl;
            this_thread::sleep_for(1000ms);
            continue;
        }
        if (!cpr::status::is_success(response.status_code)) {
            throw GAMSException("Getting logs failed with status code: " + to_string(response.status_code) + "."
                                " Message: " + response.text);
        }
        if (mWs.debug() >= GAMSEnum::ShowLog || output) {
            string stdOutData = response.text;
            if (mWs.debug() >= GAMSEnum::DebugLevel::ShowLog) {
                if (!stdOutData.empty())
                    cout << stdOutData << endl;
            } else if (output) {
                if (!stdOutData.empty())
                    *output << stdOutData << endl; // TODO(RG): check if this does what it's supposed to
            }
        }

        json = nlohmann::json::parse(response.text);
        if (json.at("queue_finished").get<bool>()) {
            exitCode = json.at("gams_return_code").get<int>();
            break;
        }
        this_thread::sleep_for(1000ms);
    }

    string resultZipName;
    int resultZipCount = 1;
    while (filesystem::exists(mWs.workingDirectory()
                              .append("/_gams_result").append(to_string(resultZipCount))
                              .append(".zip"))) {
        resultZipCount++;
    }
    resultZipName = mWs.workingDirectory().append("/_gams_result").append(to_string(resultZipCount)).append(".zip");

    std::ofstream of(resultZipName, std::ios::binary);
    response = cpr::Download(of, cpr::Url{ engineConfiguration.host() + "/jobs/" + mEngineJob->token() + "/result" },
                             cpr::Authentication{engineConfiguration.username(),
                                           engineConfiguration.password(),
                                           cpr::AuthMode::BASIC});

    if (!cpr::status::is_success(response.status_code)) {
        throw GAMSException("Downloading job result failed with status code: " + to_string(response.status_code) + "."
                            " Message: " + response.text);
    }

    string destination = mWs.workingDirectory();
    unzip(resultZipName, mWs.workingDirectory());

    if (removeResults) {
        response = cpr::Delete(cpr::Url{engineConfiguration.host() + "/jobs/" + mEngineJob->token() + "/result" },
                               cpr::Authentication{engineConfiguration.username(),
                                                 engineConfiguration.password(),
                                                 cpr::AuthMode::BASIC});

        if (!cpr::status::is_success(response.status_code)) {
            throw GAMSException("Removing job result failed with status code: " + to_string(response.status_code) + "."
                                " Message: " + response.text);
        }
    }
    if (createOutDB) {
        filesystem::path gdxPath = tmpOpt.gdx();
        if (!gdxPath.is_absolute())
            gdxPath = filesystem::absolute(gdxPath);
        if (filesystem::exists(gdxPath)) {
            // TODO(RG): addDatabaseFromGDXForcedName in Csharp takes two arguments, set a default here?
            mOutDb = mWs.addDatabaseFromGDXForcedName(gdxPath, gdxPath.filename().stem(), "");
        }
    }

    if (exitCode != 0) {
        if (mWs.debug() < GAMSEnum::KeepFiles && mWs.usingTmpWorkingDir())
            throw GAMSExceptionExecution("GAMS return code not 0 (" + to_string(exitCode) +
                                         "), set GAMSWorkspace.Debug to KeepFiles or higher "
                                         "or define the GAMSWorkspace.workingDirectory to "
                                         "receive a listing file with more details", exitCode);
        else
            throw GAMSExceptionExecution("GAMS return code not 0 (" + to_string(exitCode) +
                                         "), check " +  + " for more details", exitCode);
    }
    mEngineJob = nullptr;

    if (tmpCp) {
        filesystem::remove(checkpoint->fileName());
        filesystem::rename(tmpCp->fileName(), checkpoint->fileName());
    }

// TODO(RG): dispose is not implemented in c++ api
//    tmpOpt.dispose();
    if (mWs.debug() < GAMSEnum::KeepFiles) {
        filesystem::remove(pfFileName);
        filesystem::remove(dataZipName);
        filesystem::remove(resultZipName);
    }

    // TODO(RG): we might want to use GAMSPath or std::fs for files instead of string
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
