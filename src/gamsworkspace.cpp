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

#define NOMINMAX // we need this in order  for numerical_limits<double>::min() to work

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <stdio.h>
#include "gamslog.h"
#include "gamsworkspace.h"
#include "gamsworkspaceimpl.h"
#include "gamsoptions.h"
#include "gamsjob.h"

using namespace std;

namespace gams {


GAMSWorkspace::GAMSWorkspace(const std::shared_ptr<GAMSWorkspaceImpl>& impl)
    : mImpl(impl)
{
    DEB << "---- Entering GAMSWorkspace constructor ----";
}

GAMSDatabase GAMSWorkspace::addDatabaseFromGDXForcedName(string gdxFileName, string databaseName, string inModelName)
{
    return mImpl->addDatabaseFromGDXForcedName(*this, gdxFileName, databaseName, inModelName);
}

bool GAMSWorkspace::usingTmpWorkingDir() const
{
    return mImpl->usingTmpWorkingDir();
}

std::string GAMSWorkspace::registerDatabase(const string databaseName) const
{
    return mImpl->registerDatabase(databaseName);
}

string GAMSWorkspace::nextDatabaseName() const
{
    return mImpl->nextDatabaseName();
}

GAMSEnum::DebugLevel GAMSWorkspace::debug()
{
    return mImpl->debug();
}

LogId GAMSWorkspace::logID() const
{
    return mImpl->logID();
}

double* GAMSWorkspace::specValues() const
{
    return mImpl->specValues;
}

GAMSWorkspace::GAMSWorkspace(const string& workingDir, const string& systemDir, GAMSEnum::DebugLevel debug)
    : mImpl(make_shared<GAMSWorkspaceImpl>(workingDir, systemDir, debug))
{
    DEB << "---- Entering GAMSWorkspace constructor ----";
    cout << "GAMSWorkspace 0" << endl;
}

GAMSWorkspace::GAMSWorkspace(const string& workingDir, const string& systemDir)
    : GAMSWorkspace(make_shared<GAMSWorkspaceImpl>(workingDir, systemDir, GAMSEnum::DebugLevel::Off))
{
    cout << "GAMSWorkspace wdir, sysdir, db off" << endl;
}

GAMSWorkspace::GAMSWorkspace(const string& workingDir, GAMSEnum::DebugLevel debug)
    : GAMSWorkspace(make_shared<GAMSWorkspaceImpl>(workingDir, "", debug))
{
    cout << "GAMSWorkspace wdir + debug" << endl;
}

GAMSWorkspace::GAMSWorkspace(GAMSEnum::DebugLevel debug)
    : GAMSWorkspace(make_shared<GAMSWorkspaceImpl>("", "", debug))
{
    cout << "GAMSWorkspace debug" << endl;
}

GAMSWorkspace::GAMSWorkspace()
    : GAMSWorkspace(make_shared<GAMSWorkspaceImpl>("", "", GAMSEnum::DebugLevel::Off))
{
    cout << "GAMSWorkspace DebugLvl Off" << endl;
}

GAMSWorkspace::GAMSWorkspace(const GAMSWorkspace& other)
    : GAMSWorkspace(other.mImpl)
{
    cout << "GAMSWorkspace copy" << endl;
}

GAMSWorkspace::GAMSWorkspace(const GAMSWorkspaceInfo& wsInfo)
    : GAMSWorkspace(wsInfo.workingDirectory(), wsInfo.systemDirectory(), wsInfo.debug())
{}

GAMSWorkspace::~GAMSWorkspace()
{
    cout << "~GAMSWorkspace" << endl;
    DEB << "---- Entering GAMSWorkspace destructor ----";
    //CW: no delete anymore
    //delete ws_;
}

GAMSWorkspace&GAMSWorkspace::operator=(const GAMSWorkspace& other)
{
    mImpl = other.mImpl;
    return *this;
}

GAMSDatabase GAMSWorkspace::addDatabase(const string& databaseName, const string& inModelName)
{
    return mImpl->addDatabase(*this, databaseName, inModelName);
}

GAMSDatabase GAMSWorkspace::addDatabase(const GAMSDatabase& sourceDatabase, const std::string& databaseName, const std::string& inModelName)
{
    return mImpl->addDatabase(*this, sourceDatabase, databaseName, inModelName);
}

GAMSDatabase GAMSWorkspace::addDatabaseFromGDX(const string& gdxFileName)
{
    return mImpl->addDatabaseFromGDX(*this, gdxFileName);
}

GAMSDatabase GAMSWorkspace::addDatabaseFromGDX(const string& gdxFileName, const string& databaseName)
{
    return mImpl->addDatabaseFromGDX(*this, gdxFileName, databaseName);
}

GAMSDatabase GAMSWorkspace::addDatabaseFromGDX(const string& gdxFileName, const string& databaseName, const string& inModelName)
{
    return mImpl->addDatabaseFromGDX(*this, gdxFileName, databaseName, inModelName);
}

GAMSDatabase GAMSWorkspace::addDatabaseFromGMD(void* gmdPtr)
{
    return mImpl->addDatabaseFromGMD(*this, gmdPtr);
}

GAMSCheckpoint GAMSWorkspace::addCheckpoint(const string& checkpointName)
{
    return mImpl->addCheckpoint(*this, checkpointName);
}

GAMSOptions GAMSWorkspace::addOptions()
{
    return mImpl->addOptions(*this);
}

GAMSOptions GAMSWorkspace::addOptions(const std::string& optFile)
{
    return mImpl->addOptions(*this, optFile);
}

GAMSOptions GAMSWorkspace::addOptions(const GAMSOptions &optFrom)
{
    return mImpl->addOptions(*this, optFrom);
}

GAMSJob GAMSWorkspace::addJobFromFile(const std::string& fileName, const std::string& jobName)
{
    return mImpl->addJobFromFile(*this, fileName, jobName);
}

GAMSJob GAMSWorkspace::addJobFromFile(const string& fileName, const GAMSCheckpoint& checkpoint, const string& jobName)
{
    return mImpl->addJobFromFile(*this, fileName, checkpoint, jobName);
}

GAMSJob GAMSWorkspace::addJobFromString(const std::string& gamsSource, const std::string& jobName)
{
    return mImpl->addJobFromString(*this, gamsSource, jobName);
}

GAMSJob GAMSWorkspace::addJobFromString(const std::string& gamsSource, const GAMSCheckpoint& checkpoint, const std::string& jobName)
{
    return mImpl->addJobFromString(*this, gamsSource, checkpoint, jobName);
}

GAMSJob GAMSWorkspace::addJobFromGamsLib(const std::string& model, const std::string& jobName)
{
    gamsLib(model);
    return addJobFromFile(model + ".gms", jobName);
}

GAMSJob GAMSWorkspace::addJobFromGamsLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName)
{
    gamsLib(model);
    return addJobFromFile(model + ".gms", checkpoint, jobName);
}

GAMSJob GAMSWorkspace::addJobFromApiLib(const std::string& model, const std::string& jobName)
{
    apiLib(model);
    return addJobFromFile(model + ".gms", jobName);
}

GAMSJob GAMSWorkspace::addJobFromApiLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName)
{
    apiLib(model);
    return addJobFromFile(model + ".gms", checkpoint, jobName);
}

GAMSJob GAMSWorkspace::addJobFromTestLib(const std::string& model, const std::string& jobName)
{
    testLib(model);
    return addJobFromFile(model + ".gms", jobName);
}

GAMSJob GAMSWorkspace::addJobFromTestLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName)
{
    testLib(model);
    return addJobFromFile(model + ".gms", checkpoint, jobName);
}

GAMSJob GAMSWorkspace::addJobFromEmpLib(const std::string& model, const std::string& jobName)
{
    empLib(model);
    return addJobFromFile(model + ".gms", jobName);
}

GAMSJob GAMSWorkspace::addJobFromEmpLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName)
{
    empLib(model);
    return addJobFromFile(model + ".gms", checkpoint, jobName);
}

GAMSJob GAMSWorkspace::addJobFromDataLib(const std::string& model, const std::string& jobName)
{
    dataLib(model);
    return addJobFromFile(model + ".gms", jobName);
}

GAMSJob GAMSWorkspace::addJobFromDataLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName)
{
    dataLib(model);
    return addJobFromFile(model + ".gms", checkpoint, jobName);
}

GAMSJob GAMSWorkspace::addJobFromFinLib(const std::string& model, const std::string& jobName)
{
    finLib(model);
    return addJobFromFile(model + ".gms", jobName);
}

GAMSJob GAMSWorkspace::addJobFromFinLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName)
{
    finLib(model);
    return addJobFromFile(model + ".gms", checkpoint, jobName);
}

GAMSJob GAMSWorkspace::addJobFromNoaLib(const std::string& model, const std::string& jobName)
{
    noaLib(model);
    return addJobFromFile(model + ".gms", jobName);
}

GAMSJob GAMSWorkspace::addJobFromNoaLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName)
{
    noaLib(model);
    return addJobFromFile(model + ".gms", checkpoint, jobName);
}

GAMSJob GAMSWorkspace::addJobFromPsoptLib(const std::string& model, const std::string& jobName)
{
    psoptLib(model);
    return addJobFromFile(model + ".gms", jobName);
}

GAMSJob GAMSWorkspace::addJobFromPsoptLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName)
{
    psoptLib(model);
    return addJobFromFile(model + ".gms", checkpoint, jobName);
}

string GAMSWorkspace::optFileExtension(int optfile)
{
    return mImpl->optFileExtension(optfile);
}

string GAMSWorkspace::registerModelInstance(const string modelInstanceName)
{
    return mImpl->registerModelInstance(modelInstanceName);
}


void GAMSWorkspace::gamsLib(string model)
{
    mImpl->xxxLib("gams", model);
}

void GAMSWorkspace::testLib(string model)
{
    mImpl->xxxLib("test", model);
}

void GAMSWorkspace::empLib(string model)
{
    mImpl->xxxLib("emp", model);
}

void GAMSWorkspace::dataLib(string model)
{
    mImpl->xxxLib("data", model);
}

void GAMSWorkspace::apiLib(string model)
{
    mImpl->xxxLib("api", model);
}

void GAMSWorkspace::finLib(string model)
{
    mImpl->xxxLib("fin", model);
}

void GAMSWorkspace::noaLib(string model)
{
    mImpl->xxxLib("noa", model);
}

void GAMSWorkspace::psoptLib(string model)
{
    mImpl->xxxLib("psopt", model);
}

string GAMSWorkspace::systemDirectory() const
{
    return mImpl->systemDir().toStdString();
}

bool GAMSWorkspace::operator!=(const GAMSWorkspace& other) const
{
    return !operator==(other);
}

bool GAMSWorkspace::operator==(const GAMSWorkspace& other) const
{
    return mImpl == other.mImpl || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}

string GAMSWorkspace::workingDirectory() const
{
    return mImpl->workingDir().toStdString();
}

string GAMSWorkspace::version()
{
    return mImpl->gamsVersion();
}

int GAMSWorkspace::majorRelNumber()
{
    return mImpl->gamsRelease();
}

int GAMSWorkspace::minorRelNumber()
{
    return mImpl->gamsSubRel();
}

int GAMSWorkspace::goldRelNumber()
{
    return mImpl->gamsGoldRel();
}

string GAMSWorkspace::apiVersionDetail()
{
    return GAMSWorkspaceImpl::apiVersionDetail();
}

string GAMSWorkspace::apiVersion()
{
    return GAMSWorkspaceImpl::apiVersion();
}

int GAMSWorkspace::apiMajorRelNumber()
{
    return GAMSWorkspaceImpl::apiRelease();
}

int GAMSWorkspace::apiMinorRelNumber()
{
    return GAMSWorkspaceImpl::apiSubRel();
}

int GAMSWorkspace::apiGoldRelNumber()
{
    return GAMSWorkspaceImpl::apiGoldRel();
}


string GAMSWorkspace::scratchFilePrefix() const
{
    return mImpl->scratchFilePrefix();
}

void GAMSWorkspace::setScratchFilePrefix(string prefix)
{
    mImpl->setScratchFilePrefix(prefix);
}

double GAMSWorkspace::myEPS()
{
    return mImpl->specValues[GMS_SVIDX_EPS];
}

void GAMSWorkspace::setMyEPS(double eps)
{
    mImpl->specValues[GMS_SVIDX_EPS] = eps;
}


} // namespace gams
