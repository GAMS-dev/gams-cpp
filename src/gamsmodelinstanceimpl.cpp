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

#include "gamslog.h"
#include "gamsoptions.h"
#include "gamsjob.h"
#include "gamsmodelinstanceimpl.h"
#include "gamsmodelinstanceopt.h"
#include "gamsparameter.h"
#include "gamsvariable.h"
#include "gamsequation.h"
#include "gamsexception.h"
#include "gmdcc.h"
#include "gevmcc.h"
#include "gmomcc.h"
#include "gamsversion.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <fstream>

using namespace std;

namespace gams {

GAMSModelInstanceImpl::GAMSModelInstanceImpl(GAMSCheckpoint& checkpoint, const std::string& modelInstanceName)
    : mCheckpoint(checkpoint), mModelInstanceName(modelInstanceName)
{
    char msg[GMS_SSSIZE];
    GAMSWorkspace ws = mCheckpoint.workspace();
    mScrDir = GAMSPath(ws.workingDirectory()) / mModelInstanceName;
    mSyncDb = GAMSDatabase(ws, ws.specValues());
    dbLockRecords();

    if (!gevCreateD(&mGEV, ws.systemDirectory().c_str(), msg, sizeof(msg)))
        throw GAMSException(std::string(msg));
    if (!gmoCreateD(&mGMO, ws.systemDirectory().c_str(), msg, sizeof(msg)))
        throw GAMSException(std::string(msg));
    mInstantiated = false;
}


GAMSModelInstanceImpl::GAMSModelInstanceImpl(std::shared_ptr<GAMSModelInstanceImpl> sourceMI
                                             , const std::string& modelInstanceName)
    : mCheckpoint(sourceMI->mCheckpoint)
{
    char msg[GMS_SSSIZE];
    DEB << "---- Entering GAMSModelInstance constructor ----";

    if (!sourceMI->mInstantiated)
        throw GAMSException("Source GAMSModelInstance not instantiated, cannot copy from it");

    if (modelInstanceName == "") {
        mModelInstanceName = sourceMI->mCheckpoint.workspace().registerModelInstance();
    } else {
        if (sourceMI->mCheckpoint.workspace().registerModelInstance(modelInstanceName).empty())
            throw GAMSException("ModelInstance with name " + modelInstanceName + " already exists");
        this->mModelInstanceName = modelInstanceName;
    }

    mScrDir = GAMSPath(mCheckpoint.workspace().workingDirectory()) / mModelInstanceName;
    if (mScrDir.exists()) {
        mScrDir.rmDirRecurse();
    }
    mSyncDb = mCheckpoint.workspace().addDatabase(sourceMI->mSyncDb);

    if (!gevCreateD(&mGEV, mCheckpoint.workspace().systemDirectory().c_str(), msg, sizeof(msg)))
        throw GAMSException(std::string(msg));
    if (!gmoCreateD(&mGMO, mCheckpoint.workspace().systemDirectory().c_str(), msg, sizeof(msg)))
        throw GAMSException(std::string(msg));

    for (GAMSModifier& mod : sourceMI->mModifiers) {
        try {
            GAMSModifier *gmp = 0;
            if (mod.isParameter())  //Parameter
                gmp = new GAMSModifier(mSyncDb.getParameter(mod.gamsSymbol().name()), mod.updType());
            else if (mod.isVariable())
                gmp = new GAMSModifier(mSyncDb.getVariable(mod.gamsSymbol().name()), mod.updAction(), mSyncDb.getParameter(mod.dataSymbol().name()), mod.updType());
            else if (mod.isEquation())
                gmp = new GAMSModifier(mSyncDb.getEquation(mod.gamsSymbol().name()), mod.updAction(), mSyncDb.getParameter(mod.dataSymbol().name()), mod.updType());
            else
                throw GAMSException("Unexpected Symbol as Modifier");
            mModifiers.push_back(*gmp);
        } catch (std::runtime_error& e) {
            cout << "error: " << e.what();
        }
    }

    char ctrlFN[GMS_SSSIZE];
    if (gevDuplicateScratchDir(sourceMI->mGEV, mScrDir.c_str(), (mScrDir / "gamslog.dat").c_str(), ctrlFN) != 0)
        throw GAMSException("Problem duplicating scratch directory");

    if (gevInitEnvironmentLegacy(this->mGEV, ctrlFN) != 0)
        throw GAMSException("Could not initialize model instance");

    gmoRegisterEnvironment(this->mGMO, this->mGEV, msg);
    if (gmoLoadDataLegacy(this->mGMO, msg) != 0)
        throw GAMSException("Could not load model instance: " + string(msg));

    checkForGMDError(gmdRegisterGMO(gmd(), this->mGMO), __FILE__, __LINE__);

    this->mLogAvailable = sourceMI->mLogAvailable;
    this->mSelectedSolver = sourceMI->mSelectedSolver;

    char buffer[GMS_SSSIZE];
    string optFileName = gmoNameOptFile(sourceMI->mGMO, buffer);
    gmoNameOptFileSet(this->mGMO, (GAMSPath(optFileName).up() / this->mSelectedSolver + GAMSPath(optFileName).suffix()).c_str());

    mInstantiated = true;
    dbLockSymbols();
}


GAMSModelInstanceImpl::~GAMSModelInstanceImpl()
{
    if (mGMO != NULL)
        gmoFree(&mGMO);
    if (mGEV != NULL)
        gevFree(&mGEV);
}

bool GAMSModelInstanceImpl::operator!=(const GAMSModelInstanceImpl& other) const
{
    return (mGMO != other.mGMO) || (mGEV != other.mGEV);
}

bool GAMSModelInstanceImpl::operator==(const GAMSModelInstanceImpl& other) const
{
    return !(operator!=(other));
}

GAMSEnum::ModelStat GAMSModelInstanceImpl::modelStatus()
{
    return static_cast<GAMSEnum::ModelStat>(gmoModelStat(mGMO));
}

string GAMSModelInstanceImpl::modelStatusAsString()
{
    char buffer[GMS_SSSIZE];
    gmoGetModelStatusTxt(mGMO, gmoModelStat(mGMO), buffer);
    return buffer;
}

GAMSEnum::SolveStat GAMSModelInstanceImpl::solveStatus()
{
    return static_cast<GAMSEnum::SolveStat>(gmoSolveStat(mGMO));
}

string GAMSModelInstanceImpl::solveStatusAsString()
{
    char buffer[GMS_SSSIZE];
    gmoGetSolveStatusTxt(mGMO, gmoSolveStat(mGMO), buffer);
    return buffer;
}

void GAMSModelInstanceImpl::interrupt()
{
    gevTerminateRaise(mGEV);
}

void GAMSModelInstanceImpl::instantiate(const std::string& modelDefinition, const GAMSOptions* options, const std::vector<GAMSModifier>& modifiers)
{
    if (mInstantiated) {
        throw GAMSException("ModelInstance " + mModelInstanceName + " already instatiated");
    }
    GAMSOptions tmpOpt = GAMSOptions(mCheckpoint.workspace(), options);
    {
        int i = 0;
        double d = 0.0;
        char s[GMS_SSSIZE];
        checkForGMDError(gmdInfo(gmd(), GMD_NRUELS, &i, &d, s), __FILE__, __LINE__);
        if (i > 0) {
            throw GAMSException("SyncDB of " + mModelInstanceName + " with unique elements. No AddRecord allowed prior to instantiate");
        }
    }
    bool havePar = false;
    for (GAMSModifier mod: modifiers) {
        if (mod.gamsSymbol().database() != mSyncDb) {
            throw GAMSException("Symbol " + mod.gamsSymbol().name() + " not part of SyncDB");
        }
        if (mod.isParameter()) {
            havePar = true;
        }
        mModifiers.push_back(mod);
    }
    // TODO make sure that GAMSParameters in syncDB are modifiers

    std::string model = "option limrow = 0, limcol = 0; \n";
    if (GAMSVersion::gamsMajor() < 34)
        throw GAMSException("GAMS version >= 34 required.");

    if (havePar) {
        model += "Set s__(*) /'s0'/;\n";
        for (GAMSModifier mod: modifiers) {
            if (mod.isParameter()) {
                model += "Parameter s__" + mod.gamsSymbol().name() + "(s__";
                for (int i = 0; i < mod.gamsSymbol().dim(); i++) {
                    model += ",*";
                }
                model += "); s__" + mod.gamsSymbol().name() + "(s__";
                for (int i = 0; i < mod.gamsSymbol().dim(); i++) {
                    model += ",s__";
                }
                model += ") = Eps;\n";
            }
        }

        model += "Set dict(*,*,*) /\n's__'.'scenario'.''";
        for (GAMSModifier mod: modifiers) {
            if (mod.isParameter()) {
                model += ",\n'" + mod.gamsSymbol().name() + "'.'param'.'s__" + mod.gamsSymbol().name() + "'";
            }
        }
        model += "/;\n";
    }
    std::size_t pos = modelDefinition.find(" ");
    std::string modelName = modelDefinition.substr(0, pos);
    model += modelName + ".justScrDir=1; \n";

    model += "solve " + modelDefinition;
    if (havePar) {
        model += " scenario dict;";
    }

    GAMSJob myJob = mCheckpoint.workspace().addJobFromString(model, mCheckpoint, "");
    tmpOpt.setScrDir(mScrDir);
    if (mCheckpoint.workspace().debug() >= GAMSEnum::DebugLevel::ShowLog) {
        tmpOpt.setLogOption(3);
        mLogAvailable = false;
    } else {
        tmpOpt.setLogOption(2);
        mLogAvailable = true;
    }
    mScrDir.mkDir();

    tmpOpt.setLogFile(mScrDir / "gamslog.dat");
    tmpOpt.setSolverCntr("gamscntr.dat");

    myJob.run(tmpOpt);

    if (gevInitEnvironmentLegacy(mGEV, (mScrDir / tmpOpt.solverCntr()).c_str()) != 0)
        throw GAMSException("Could not initialize model instance");

    char buf[GMS_SSSIZE];
    gmoRegisterEnvironment(mGMO, mGEV, buf);

    if (gmoLoadDataLegacy(mGMO, buf) != 0)
        throw GAMSException("Could not load model instance: " + std::string(buf));

    gmoGetModelTypeTxt(mGMO, gmoModelType(mGMO), buf);
    mSelectedSolver = tmpOpt.getSolver(buf);
    GAMSPath optFile(gmoNameOptFile(mGMO, buf));
    // TODO(jm) parentPath-Methode
    gmoNameOptFileSet(mGMO, (optFile.up() / mSelectedSolver / optFile.suffix()).c_str());

    checkForGMDError(gmdInitFromDict(gmd(), mGMO), __FILE__, __LINE__);

    mInstantiated = true;
    dbLockSymbols();
}


void GAMSModelInstanceImpl::solve(GAMSEnum::SymbolUpdateType updateType, ostream* output, GAMSModelInstanceOpt miOpt)
{
    int noMatchLimit = miOpt.noMatchLimit();
    if (!mInstantiated)
        throw GAMSException("Model instance " + mModelInstanceName + " not instantiated");

    checkForGMDError(gmdInitUpdate(gmd(), mGMO), __FILE__, __LINE__);

    int accumulateNoMatchCnt = 0;
    int noMatchCnt = 0;
    for (GAMSModifier mod: mModifiers) {
        int locIntSUT = updateType;
        // If user sets the updateType to Inherit on the Solve call use BaseCase
        if (updateType == GAMSEnum::SymbolUpdateType::Inherit)
            locIntSUT = GAMSEnum::SymbolUpdateType::BaseCase;

        if (mod.updType() != GAMSEnum::SymbolUpdateType::Inherit)
            locIntSUT = mod.updType();

        int result = gmdUpdateModelSymbol(gmd(), mod.gamsSymbol().symPtr(), mod.updAction(), mod.dataSymbol().symPtr(), locIntSUT, &noMatchCnt);
        checkForGMDError(result, __FILE__, __LINE__);
        accumulateNoMatchCnt += noMatchCnt;
        if (accumulateNoMatchCnt > noMatchLimit)
            throw GAMSException("Unmatched record limit exceeded while processing modifier " + mod.gamsSymbol().name());
    }
    // Close Log and status file and remove
    void *lshandle = nullptr;
    if (mLogAvailable && output != nullptr) {
        gevSwitchLogStat(mGEV, 0, "", false, "", false, nullptr, nullptr, &lshandle);
        gevRestoreLogStatRewrite(mGEV, &lshandle);
    }

    if (output == &cout) {
        char buffer[GMS_SSSIZE], buffer1[GMS_SSSIZE];
        gevSwitchLogStat(mGEV, 3, gevGetStrOpt(mGEV, gevNameLogFile, buffer), false, gevGetStrOpt(mGEV, gevNameStaFile, buffer1), false, nullptr, nullptr, &lshandle);
    }

    {
        std::string tmpSolver = mSelectedSolver;
        if (!miOpt.solver().empty())
            tmpSolver = miOpt.solver();

        int tmpOptFile = gmoOptFile(mGMO);
        int saveOptFile = tmpOptFile;

        char buffer[GMS_SSSIZE];
        std::string saveNameOptFile = gmoNameOptFile(mGMO, buffer);

        if (miOpt.optFile() != -1)
            tmpOptFile = miOpt.optFile();

        gmoOptFileSet(mGMO, tmpOptFile);
        GAMSPath optFile(GAMSPath(mCheckpoint.workspace().workingDirectory()) / (tmpSolver + "." + mCheckpoint.workspace().optFileExtension(tmpOptFile)));
        gmoNameOptFileSet(mGMO, optFile.c_str());

        checkForGMDError(gmdCallSolver(gmd(), tmpSolver.c_str()), __FILE__, __LINE__);

        if (miOpt.debug()) {
            GAMSPath miDir(GAMSPath(mCheckpoint.workspace().workingDirectory()) / mModelInstanceName);
            string fName = miDir / "convert.opt";
            std::ofstream writer(fName);
            writer << "gams " << (miDir / "gams.gms").toStdString();
            writer << "dumpgdx " << (miDir / "dump.gdx").toStdString();
            writer << "dictmap " << (miDir / "dictmap.gdx").toStdString();
            writer.close();

            gmoOptFileSet(mGMO, 1);

            gmoNameOptFileSet(mGMO, (miDir / "convert.opt").toStdString().c_str());
            checkForGMDError(gmdCallSolver(gmd(), "convert"), __FILE__, __LINE__);
        }

        gmoOptFileSet(mGMO, saveOptFile);
        gmoNameOptFileSet(mGMO, saveNameOptFile.c_str());
    }
    if (output == &cout)
        gevRestoreLogStat(mGEV, &lshandle);


    if (output != nullptr && output != &cout)
    {
        if (mLogAvailable)
        {
            char buffer[GMS_SSSIZE];
            gevSwitchLogStat(mGEV, 0, "", false, "", false, nullptr, nullptr, &lshandle);
            ifstream logFile(gevGetStrOpt(mGEV, gevNameLogFile, buffer));
            string line;
            while (getline(logFile, line))
                *output << line << endl;
            gevRestoreLogStat(mGEV, &lshandle);
        }
        else
            *output << "No solver log available" << endl;
    }
}
}
