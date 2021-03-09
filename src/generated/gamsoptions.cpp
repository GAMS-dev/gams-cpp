/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2020 GAMS Development Corp. <support@gams.com>
 * Copyright (c) 2017-2020 GAMS Software GmbH <support@gams.com>
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
 *
 * This file was generated.
 */


#include <cstdio>
#include <algorithm>
#include "gamsoptions.h"
#include "gamsexception.h"
#include "gamsplatform.h"
#include "gamspath.h"
#include "gmdcc.h"
#include "gmomcc.h"
#include "gamsoptionsimpl.h"                    /*4impl*/

using namespace std;

namespace gams {

GAMSOptions::~GAMSOptions()
{ }

GAMSOptions::GAMSOptions(GAMSWorkspace &workspace,  const GAMSOptions* optFrom)
    : mImpl(make_shared<GAMSOptionsImpl>(workspace, optFrom ? optFrom->mImpl.get() : nullptr))
{}

GAMSOptions::GAMSOptions(GAMSWorkspace &workspace, const std::string& optFile)
    : mImpl(make_shared<GAMSOptionsImpl>(workspace, optFile))
{}

LogId GAMSOptions::logID()
{
    return mImpl->mWs.logID();
}

const char*GAMSOptions::gamsBuild()
{
    return GAMSOptionsImpl::cGamsBuild;
}

const char*GAMSOptions::gamsVersion()
{
    return GAMSOptionsImpl::cGamsVersion;
}

void GAMSOptions::writeOptionFile(const std::string& optFileName)
{
    mImpl->writeOptionFile(optFileName);
}

void GAMSOptions::setAllModelTypes(const std::string& solver)
{
    mImpl->setAllModelTypes(solver);
}

void GAMSOptions::setDefine(const std::string& key, const std::string& value)
{
    mImpl->setDefine(key,value);
}

std::string GAMSOptions::getDefine(const std::string& key)
{
    return mImpl->getDefine(key);
}

bool GAMSOptions::removeDefine(const std::string& key)
{
    return mImpl->removeDefine(key);
}

int GAMSOptions::inputDirSize()
{
    return mImpl->inputDirSize();
}

void GAMSOptions::addInputDir(const std::string value)
{
    mImpl->setInputDir(value);
}

void GAMSOptions::setInputDir(const std::string value, const int index)
{
    mImpl->setInputDir(value, index);
}

std::string GAMSOptions::getInputDir(const int index)
{
    return mImpl->getInputDir(index);
}

void GAMSOptions::removeInputDir(const int index)
{
    mImpl->removeInputDir(index);
}

std::string GAMSOptions::gdx()
{
   return mImpl->gdx();
}

void GAMSOptions::setGdx(const std::string& value)
{
    mImpl->setGdx(value);
}
std::string GAMSOptions::restart()
{
    return mImpl->restart();
}

void GAMSOptions::setRestart(const std::string& value)
{
    mImpl->setRestart(value);
}

std::string GAMSOptions::input()
{
    return mImpl->input();
}

void GAMSOptions::setInput(const std::string& value)
{
    mImpl->setInput(value);
}

std::string GAMSOptions::output()
{
    return mImpl->output();
}

void GAMSOptions::setOutput(const std::string& value)
{
    mImpl->setOutput(value);
}

std::string GAMSOptions::save()
{
    return mImpl->save();
}

void GAMSOptions::setSave(const std::string& value)
{
    mImpl->setSave(value);
}

std::string GAMSOptions::symbol()
{
    return mImpl->symbol();
}

void GAMSOptions::setSymbol(const std::string& value)
{
    mImpl->setSymbol(value);
}

std::string GAMSOptions::xSave()
{
    return mImpl->xSave();
}

void GAMSOptions::setXSave(const std::string& value)
{
    mImpl->setXSave(value);
}

std::string GAMSOptions::expand()
{
    return mImpl->expand();
}

void GAMSOptions::setExpand(const std::string& value)
{
    mImpl->setExpand(value);
}

int GAMSOptions::pageWidth()
{
    return mImpl->pageWidth();
}

void GAMSOptions::setPageWidth(const int value)
{
    mImpl->setPageWidth(value);
}

int GAMSOptions::pageSize()
{
    return mImpl->pageSize();
}

void GAMSOptions::setPageSize(const int value)
{
    mImpl->setPageSize(value);
}

GAMSOptions::EPageContr::EPageContrEnum GAMSOptions::pageContr()
{
    return static_cast<GAMSOptions::EPageContr::EPageContrEnum> (mImpl->pageContr());
}

void GAMSOptions::setPageContr(const GAMSOptions::EPageContr::EPageContrEnum value)
{
   mImpl->setPageContr(static_cast<int>(value));
}

std::string GAMSOptions::eActionText(GAMSOptions::EAction::EActionEnum type)
{
    const char * res[] = { "RestartAfterSolve", "CompileOnly", "ExecuteOnly", "CompileAndExecute", "GlueCodeGeneration", "TraceReport" };
    return res[type];
}

std::string GAMSOptions::eActionCode(GAMSOptions::EAction::EActionEnum type)
{
    const char * res[] = { "R", "C", "E", "CE", "G", "GT" };
    return res[type];
}

GAMSOptions::EAction::EActionEnum GAMSOptions::eActionFromCode(const std::string &str)
{
    for (unsigned i=0; i<6; i++) {
        EAction::EActionEnum act = static_cast<EAction::EActionEnum>(i);
        std::string up = str;
        std::transform(up.begin(), up.end(), up.begin(), ::toupper);
        if (eActionCode(act) == up)
            return act;
    };
    throw GAMSException("GAMSOptions::eActionFromString: unknown EAction::EActionEnum type '" +str+ "'");
}

GAMSOptions::EAction::EActionEnum GAMSOptions::action()
{
    return eActionFromCode(mImpl->action());
}

void GAMSOptions::setAction(const GAMSOptions::EAction::EActionEnum value)
{
   mImpl->setAction(eActionCode(value));
}

std::string GAMSOptions::subSys()
{
    return mImpl->subSys();
}

void GAMSOptions::setSubSys(const std::string& value)
{
    mImpl->setSubSys(value);
}

std::string GAMSOptions::errNam()
{
    return mImpl->errNam();
}

void GAMSOptions::setErrNam(const std::string& value)
{
    mImpl->setErrNam(value);
}

std::string GAMSOptions::scrDir()
{
    return mImpl->scrDir();
}

void GAMSOptions::setScrDir(const std::string& value)
{
    mImpl->setScrDir(value);
}

std::string GAMSOptions::scrNam()
{
    return mImpl->scrNam();
}

void GAMSOptions::setScrNam(const std::string& value)
{
    mImpl->setScrNam(value);
}

std::string GAMSOptions::sysDir()
{
    return mImpl->sysDir();
}

void GAMSOptions::setSysDir(const std::string& value)
{
    mImpl->setSysDir(value);
}

std::string GAMSOptions::curDir()
{
    return mImpl->curDir();
}

void GAMSOptions::setCurDir(const std::string& value)
{
    mImpl->setCurDir(value);
}

std::string GAMSOptions::workDir()
{
    return mImpl->workDir();
}

void GAMSOptions::setWorkDir(const std::string& value)
{
    mImpl->setWorkDir(value);
}

std::string GAMSOptions::putDir()
{
    return mImpl->putDir();
}

void GAMSOptions::setPutDir(const std::string& value)
{
    mImpl->setPutDir(value);
}

std::string GAMSOptions::gridDir()
{
    return mImpl->gridDir();
}

void GAMSOptions::setGridDir(const std::string& value)
{
    mImpl->setGridDir(value);
}

std::string GAMSOptions::scriptNext()
{
    return mImpl->scriptNext();
}

void GAMSOptions::setScriptNext(const std::string& value)
{
    mImpl->setScriptNext(value);
}

std::string GAMSOptions::scriptExit()
{
    return mImpl->scriptExit();
}

void GAMSOptions::setScriptExit(const std::string& value)
{
    mImpl->setScriptExit(value);
}

std::string GAMSOptions::scriptFrst()
{
    return mImpl->scriptFrst();
}

void GAMSOptions::setScriptFrst(const std::string& value)
{
    mImpl->setScriptFrst(value);
}

GAMSOptions::ELogLine::ELogLineEnum GAMSOptions::logLine()
{
    return static_cast<GAMSOptions::ELogLine::ELogLineEnum> (mImpl->logLine());
}

void GAMSOptions::setLogLine(const GAMSOptions::ELogLine::ELogLineEnum value)
{
   mImpl->setLogLine(static_cast<int>(value));
}

int GAMSOptions::logOption()
{
    return mImpl->logOption();
}

void GAMSOptions::setLogOption(const int value)
{
    mImpl->setLogOption(value);
}

std::string GAMSOptions::logFile()
{
    return mImpl->logFile();
}

void GAMSOptions::setLogFile(const std::string& value)
{
    mImpl->setLogFile(value);
}

std::string GAMSOptions::trace()
{
    return mImpl->trace();
}

void GAMSOptions::setTrace(const std::string& value)
{
    mImpl->setTrace(value);
}

GAMSOptions::EAppendOut::EAppendOutEnum GAMSOptions::appendOut()
{
    return static_cast<GAMSOptions::EAppendOut::EAppendOutEnum> (mImpl->appendOut());
}

void GAMSOptions::setAppendOut(const GAMSOptions::EAppendOut::EAppendOutEnum value)
{
   mImpl->setAppendOut(static_cast<int>(value));
}

int GAMSOptions::appendLog()
{
    return mImpl->appendLog();
}

void GAMSOptions::setAppendLog(const int value)
{
    mImpl->setAppendLog(value);
}

GAMSOptions::EStepSum::EStepSumEnum GAMSOptions::stepSum()
{
    return static_cast<GAMSOptions::EStepSum::EStepSumEnum> (mImpl->stepSum());
}

void GAMSOptions::setStepSum(const GAMSOptions::EStepSum::EStepSumEnum value)
{
   mImpl->setStepSum(static_cast<int>(value));
}

int GAMSOptions::multiPass()
{
    return mImpl->multiPass();
}

void GAMSOptions::setMultiPass(const int value)
{
    mImpl->setMultiPass(value);
}

GAMSOptions::ESuppress::ESuppressEnum GAMSOptions::suppress()
{
    return static_cast<GAMSOptions::ESuppress::ESuppressEnum> (mImpl->suppress());
}

void GAMSOptions::setSuppress(const GAMSOptions::ESuppress::ESuppressEnum value)
{
   mImpl->setSuppress(static_cast<int>(value));
}

GAMSOptions::EDFormat::EDFormatEnum GAMSOptions::dFormat()
{
    return static_cast<GAMSOptions::EDFormat::EDFormatEnum> (mImpl->dFormat());
}

void GAMSOptions::setDFormat(const GAMSOptions::EDFormat::EDFormatEnum value)
{
   mImpl->setDFormat(static_cast<int>(value));
}

GAMSOptions::ETFormat::ETFormatEnum GAMSOptions::tFormat()
{
    return static_cast<GAMSOptions::ETFormat::ETFormatEnum> (mImpl->tFormat());
}

void GAMSOptions::setTFormat(const GAMSOptions::ETFormat::ETFormatEnum value)
{
   mImpl->setTFormat(static_cast<int>(value));
}

int GAMSOptions::tabIn()
{
    return mImpl->tabIn();
}

void GAMSOptions::setTabIn(const int value)
{
    mImpl->setTabIn(value);
}

std::string GAMSOptions::solverCntr()
{
    return mImpl->solverCntr();
}

void GAMSOptions::setSolverCntr(const std::string& value)
{
    mImpl->setSolverCntr(value);
}

std::string GAMSOptions::solverMatr()
{
    return mImpl->solverMatr();
}

void GAMSOptions::setSolverMatr(const std::string& value)
{
    mImpl->setSolverMatr(value);
}

std::string GAMSOptions::solverInst()
{
    return mImpl->solverInst();
}

void GAMSOptions::setSolverInst(const std::string& value)
{
    mImpl->setSolverInst(value);
}

std::string GAMSOptions::solverStat()
{
    return mImpl->solverStat();
}

void GAMSOptions::setSolverStat(const std::string& value)
{
    mImpl->setSolverStat(value);
}

std::string GAMSOptions::solverSolu()
{
    return mImpl->solverSolu();
}

void GAMSOptions::setSolverSolu(const std::string& value)
{
    mImpl->setSolverSolu(value);
}

std::string GAMSOptions::solverDict()
{
    return mImpl->solverDict();
}

void GAMSOptions::setSolverDict(const std::string& value)
{
    mImpl->setSolverDict(value);
}

GAMSOptions::EIntVarUp::EIntVarUpEnum GAMSOptions::intVarUp()
{
    return static_cast<GAMSOptions::EIntVarUp::EIntVarUpEnum> (mImpl->intVarUp());
}

void GAMSOptions::setIntVarUp(const GAMSOptions::EIntVarUp::EIntVarUpEnum value)
{
   mImpl->setIntVarUp(static_cast<int>(value));
}

int GAMSOptions::lstCase()
{
   return mImpl->lstCase();
}

void GAMSOptions::setLstCase(const int value)
{
   mImpl->setLstCase(value);
}

int GAMSOptions::g205()
{
    return mImpl->g205();
}

void GAMSOptions::setG205(const int value)
{
    mImpl->setG205(value);
}

std::string GAMSOptions::license()
{
    return mImpl->license();
}

void GAMSOptions::setLicense(const std::string& value)
{
    mImpl->setLicense(value);
}

GAMSOptions::EDumpParms::EDumpParmsEnum GAMSOptions::dumpParms()
{
    return static_cast<GAMSOptions::EDumpParms::EDumpParmsEnum> (mImpl->dumpParms());
}

void GAMSOptions::setDumpParms(const GAMSOptions::EDumpParms::EDumpParmsEnum value)
{
   mImpl->setDumpParms(static_cast<int>(value));
}

int GAMSOptions::noCr()
{
    return mImpl->noCr();
}

void GAMSOptions::setNoCr(const int value)
{
    mImpl->setNoCr(value);
}

int GAMSOptions::profile()
{
    return mImpl->profile();
}

void GAMSOptions::setProfile(const int value)
{
    mImpl->setProfile(value);
}

std::string GAMSOptions::libIncDir()
{
    return mImpl->libIncDir();
}

void GAMSOptions::setLibIncDir(const std::string& value)
{
    mImpl->setLibIncDir(value);
}

std::string GAMSOptions::sysIncDir()
{
    return mImpl->sysIncDir();
}

void GAMSOptions::setSysIncDir(const std::string& value)
{
    mImpl->setSysIncDir(value);
}

std::string GAMSOptions::user1()
{
    return mImpl->user1();
}

void GAMSOptions::setUser1(const std::string& value)
{
    mImpl->setUser1(value);
}

std::string GAMSOptions::user2()
{
    return mImpl->user2();
}

void GAMSOptions::setUser2(const std::string& value)
{
    mImpl->setUser2(value);
}

std::string GAMSOptions::user3()
{
    return mImpl->user3();
}

void GAMSOptions::setUser3(const std::string& value)
{
    mImpl->setUser3(value);
}

std::string GAMSOptions::user4()
{
    return mImpl->user4();
}

void GAMSOptions::setUser4(const std::string& value)
{
    mImpl->setUser4(value);
}

std::string GAMSOptions::user5()
{
    return mImpl->user5();
}

void GAMSOptions::setUser5(const std::string& value)
{
    mImpl->setUser5(value);
}

GAMSOptions::EForceWork::EForceWorkEnum GAMSOptions::forceWork()
{
    return static_cast<GAMSOptions::EForceWork::EForceWorkEnum> (mImpl->forceWork());
}

void GAMSOptions::setForceWork(const GAMSOptions::EForceWork::EForceWorkEnum value)
{
   mImpl->setForceWork(static_cast<int>(value));
}

int GAMSOptions::cErr()
{
    return mImpl->cErr();
}

void GAMSOptions::setCErr(const int value)
{
    mImpl->setCErr(value);
}

std::string GAMSOptions::fErr()
{
    return mImpl->fErr();
}

void GAMSOptions::setFErr(const std::string& value)
{
    mImpl->setFErr(value);
}

int GAMSOptions::optFile()
{
    return mImpl->optFile();
}

void GAMSOptions::setOptFile(const int value)
{
    mImpl->setOptFile(value);
}

int GAMSOptions::fSave()
{
    return mImpl->fSave();
}

void GAMSOptions::setFSave(const int value)
{
    mImpl->setFSave(value);
}

GAMSOptions::EDumpOpt::EDumpOptEnum GAMSOptions::dumpOpt()
{
    return static_cast<GAMSOptions::EDumpOpt::EDumpOptEnum> (mImpl->dumpOpt());
}

void GAMSOptions::setDumpOpt(const GAMSOptions::EDumpOpt::EDumpOptEnum value)
{
   mImpl->setDumpOpt(static_cast<int>(value));
}

GAMSOptions::EErrMsg::EErrMsgEnum GAMSOptions::errMsg()
{
    return static_cast<GAMSOptions::EErrMsg::EErrMsgEnum> (mImpl->errMsg());
}

void GAMSOptions::setErrMsg(const GAMSOptions::EErrMsg::EErrMsgEnum value)
{
   mImpl->setErrMsg(static_cast<int>(value));
}

int GAMSOptions::opt()
{
    return mImpl->opt();
}

void GAMSOptions::setOpt(const int value)
{
    mImpl->setOpt(value);
}

int GAMSOptions::relPath()
{
    return mImpl->relPath();
}

void GAMSOptions::setRelPath(const int value)
{
    mImpl->setRelPath(value);
}

GAMSOptions::EStringChk::EStringChkEnum GAMSOptions::stringChk()
{
    return static_cast<GAMSOptions::EStringChk::EStringChkEnum> (mImpl->stringChk());
}

void GAMSOptions::setStringChk(const GAMSOptions::EStringChk::EStringChkEnum value)
{
   mImpl->setStringChk(static_cast<int>(value));
}

std::string GAMSOptions::reference()
{
    return mImpl->reference();
}

void GAMSOptions::setReference(const std::string& value)
{
    mImpl->setReference(value);
}

std::string GAMSOptions::optDir()
{
    return mImpl->optDir();
}

void GAMSOptions::setOptDir(const std::string& value)
{
    mImpl->setOptDir(value);
}

int GAMSOptions::execErr()
{
    return mImpl->execErr();
}

void GAMSOptions::setExecErr(const int value)
{
    mImpl->setExecErr(value);
}

int GAMSOptions::errorLog()
{
    return mImpl->errorLog();
}

void GAMSOptions::setErrorLog(const int value)
{
    mImpl->setErrorLog(value);
}

int GAMSOptions::limRow()
{
    return mImpl->limRow();
}

void GAMSOptions::setLimRow(const int value)
{
    mImpl->setLimRow(value);
}

int GAMSOptions::limCol()
{
    return mImpl->limCol();
}

void GAMSOptions::setLimCol(const int value)
{
    mImpl->setLimCol(value);
}

GAMSOptions::ECharSet::ECharSetEnum GAMSOptions::charSet()
{
    return static_cast<GAMSOptions::ECharSet::ECharSetEnum> (mImpl->charSet());
}

void GAMSOptions::setCharSet(const GAMSOptions::ECharSet::ECharSetEnum value)
{
   mImpl->setCharSet(static_cast<int>(value));
}

GAMSOptions::EKeep::EKeepEnum GAMSOptions::keep()
{
    return static_cast<GAMSOptions::EKeep::EKeepEnum> (mImpl->keep());
}

void GAMSOptions::setKeep(const GAMSOptions::EKeep::EKeepEnum value)
{
   mImpl->setKeep(static_cast<int>(value));
}

int GAMSOptions::iDE()
{
    return mImpl->iDE();
}

void GAMSOptions::setIDE(const int value)
{
    mImpl->setIDE(value);
}

GAMSOptions::ETraceOpt::ETraceOptEnum GAMSOptions::traceOpt()
{
    return static_cast<GAMSOptions::ETraceOpt::ETraceOptEnum> (mImpl->traceOpt());
}

void GAMSOptions::setTraceOpt(const GAMSOptions::ETraceOpt::ETraceOptEnum value)
{
   mImpl->setTraceOpt(static_cast<int>(value));
}

int GAMSOptions::iterLim()
{
    return mImpl->iterLim();
}

void GAMSOptions::setIterLim(const int value)
{
    mImpl->setIterLim(value);
}

int GAMSOptions::domLim()
{
    return mImpl->domLim();
}

void GAMSOptions::setDomLim(const int value)
{
    mImpl->setDomLim(value);
}

double GAMSOptions::resLim()
{
    return mImpl->resLim();
}

void GAMSOptions::setResLim(const double value)
{
    mImpl->setResLim(value);
}

double GAMSOptions::optCR()
{
    return mImpl->optCR();
}

void GAMSOptions::setOptCR(const double value)
{
    mImpl->setOptCR(value);
}

double GAMSOptions::optCA()
{
    return mImpl->optCA();
}

void GAMSOptions::setOptCA(const double value)
{
    mImpl->setOptCA(value);
}

GAMSOptions::ESysOut::ESysOutEnum GAMSOptions::sysOut()
{
    return static_cast<GAMSOptions::ESysOut::ESysOutEnum> (mImpl->sysOut());
}

void GAMSOptions::setSysOut(const GAMSOptions::ESysOut::ESysOutEnum value)
{
   mImpl->setSysOut(static_cast<int>(value));
}

GAMSOptions::ESolPrint::ESolPrintEnum GAMSOptions::solPrint()
{
    return static_cast<GAMSOptions::ESolPrint::ESolPrintEnum> (mImpl->solPrint());
}

void GAMSOptions::setSolPrint(const GAMSOptions::ESolPrint::ESolPrintEnum value)
{
   mImpl->setSolPrint(static_cast<int>(value));
}

double GAMSOptions::bratio()
{
    return mImpl->bratio();
}

void GAMSOptions::setBratio(const double value)
{
    mImpl->setBratio(value);
}

GAMSOptions::EHoldFixed::EHoldFixedEnum GAMSOptions::holdFixed()
{
    return static_cast<GAMSOptions::EHoldFixed::EHoldFixedEnum> (mImpl->holdFixed());
}

void GAMSOptions::setHoldFixed(const GAMSOptions::EHoldFixed::EHoldFixedEnum value)
{
   mImpl->setHoldFixed(static_cast<int>(value));
}

int GAMSOptions::nodLim()
{
    return mImpl->nodLim();
}

void GAMSOptions::setNodLim(const int value)
{
    mImpl->setNodLim(value);
}

double GAMSOptions::workFactor()
{
    return mImpl->workFactor();
}

void GAMSOptions::setWorkFactor(const double value)
{
    mImpl->setWorkFactor(value);
}

double GAMSOptions::workSpace()
{
    return mImpl->workSpace();
}

void GAMSOptions::setWorkSpace(const double value)
{
    mImpl->setWorkSpace(value);
}

int GAMSOptions::forLim()
{
    return mImpl->forLim();
}

void GAMSOptions::setForLim(const int value)
{
    mImpl->setForLim(value);
}

int GAMSOptions::seed()
{
    return mImpl->seed();
}

void GAMSOptions::setSeed(const int value)
{
    mImpl->setSeed(value);
}

GAMSOptions::EOn115::EOn115Enum GAMSOptions::on115()
{
    return static_cast<GAMSOptions::EOn115::EOn115Enum> (mImpl->on115());
}

void GAMSOptions::setOn115(const GAMSOptions::EOn115::EOn115Enum value)
{
   mImpl->setOn115(static_cast<int>(value));
}

GAMSOptions::EExecMode::EExecModeEnum GAMSOptions::execMode()
{
    return static_cast<GAMSOptions::EExecMode::EExecModeEnum> (mImpl->execMode());
}

void GAMSOptions::setExecMode(const GAMSOptions::EExecMode::EExecModeEnum value)
{
   mImpl->setExecMode(static_cast<int>(value));
}

GAMSOptions::EInteractiveSolver::EInteractiveSolverEnum GAMSOptions::interactiveSolver()
{
    return static_cast<GAMSOptions::EInteractiveSolver::EInteractiveSolverEnum> (mImpl->interactiveSolver());
}

void GAMSOptions::setInteractiveSolver(const GAMSOptions::EInteractiveSolver::EInteractiveSolverEnum value)
{
   mImpl->setInteractiveSolver(static_cast<int>(value));
}

std::string GAMSOptions::pLicense()
{
    return mImpl->pLicense();
}

void GAMSOptions::setPLicense(const std::string& value)
{
    mImpl->setPLicense(value);
}

std::string GAMSOptions::jobTrace()
{
    return mImpl->jobTrace();
}

void GAMSOptions::setJobTrace(const std::string& value)
{
    mImpl->setJobTrace(value);
}

int GAMSOptions::warnings()
{
    return mImpl->warnings();
}

void GAMSOptions::setWarnings(const int value)
{
    mImpl->setWarnings(value);
}

int GAMSOptions::traceLevel()
{
    return mImpl->traceLevel();
}

void GAMSOptions::setTraceLevel(const int value)
{
    mImpl->setTraceLevel(value);
}

int GAMSOptions::timer()
{
    return mImpl->timer();
}

void GAMSOptions::setTimer(const int value)
{
    mImpl->setTimer(value);
}

GAMSOptions::EFileCase::EFileCaseEnum GAMSOptions::fileCase()
{
    return static_cast<GAMSOptions::EFileCase::EFileCaseEnum> (mImpl->fileCase());
}

void GAMSOptions::setFileCase(const GAMSOptions::EFileCase::EFileCaseEnum value)
{
   mImpl->setFileCase(static_cast<int>(value));
}

GAMSOptions::ESavePoint::ESavePointEnum GAMSOptions::savePoint()
{
    return static_cast<GAMSOptions::ESavePoint::ESavePointEnum> (mImpl->savePoint());
}

void GAMSOptions::setSavePoint(const GAMSOptions::ESavePoint::ESavePointEnum value)
{
   mImpl->setSavePoint(static_cast<int>(value));
}

GAMSOptions::ESolveLink::ESolveLinkEnum GAMSOptions::solveLink()
{
    return static_cast<GAMSOptions::ESolveLink::ESolveLinkEnum> (mImpl->solveLink());
}

void GAMSOptions::setSolveLink(const GAMSOptions::ESolveLink::ESolveLinkEnum value)
{
   mImpl->setSolveLink(static_cast<int>(value));
}

GAMSOptions::ESys10::ESys10Enum GAMSOptions::sys10()
{
    return static_cast<GAMSOptions::ESys10::ESys10Enum> (mImpl->sys10());
}

void GAMSOptions::setSys10(const GAMSOptions::ESys10::ESys10Enum value)
{
   mImpl->setSys10(static_cast<int>(value));
}

GAMSOptions::ESys11::ESys11Enum GAMSOptions::sys11()
{
    return static_cast<GAMSOptions::ESys11::ESys11Enum> (mImpl->sys11());
}

void GAMSOptions::setSys11(const GAMSOptions::ESys11::ESys11Enum value)
{
   mImpl->setSys11(static_cast<int>(value));
}

int GAMSOptions::sys12()
{
    return mImpl->sys12();
}

void GAMSOptions::setSys12(const int value)
{
    mImpl->setSys12(value);
}

double GAMSOptions::zeroRes()
{
    return mImpl->zeroRes();
}

void GAMSOptions::setZeroRes(const double value)
{
    mImpl->setZeroRes(value);
}

GAMSOptions::EZeroResRep::EZeroResRepEnum GAMSOptions::zeroResRep()
{
    return static_cast<GAMSOptions::EZeroResRep::EZeroResRepEnum> (mImpl->zeroResRep());
}

void GAMSOptions::setZeroResRep(const GAMSOptions::EZeroResRep::EZeroResRepEnum value)
{
   mImpl->setZeroResRep(static_cast<int>(value));
}

double GAMSOptions::heapLimit()
{
    return mImpl->heapLimit();
}

void GAMSOptions::setHeapLimit(const double value)
{
    mImpl->setHeapLimit(value);
}

std::string GAMSOptions::scrExt()
{
    return mImpl->scrExt();
}

void GAMSOptions::setScrExt(const std::string& value)
{
    mImpl->setScrExt(value);
}

std::string GAMSOptions::procDir()
{
    return mImpl->procDir();
}

void GAMSOptions::setProcDir(const std::string& value)
{
    mImpl->setProcDir(value);
}

GAMSOptions::EgdxCompress::EgdxCompressEnum GAMSOptions::gdxCompress()
{
    return static_cast<GAMSOptions::EgdxCompress::EgdxCompressEnum> (mImpl->gdxCompress());
}

void GAMSOptions::setgdxCompress(const GAMSOptions::EgdxCompress::EgdxCompressEnum value)
{
   mImpl->setgdxCompress(static_cast<int>(value));
}

std::string GAMSOptions::egdxConvertText(GAMSOptions::EgdxConvert::EgdxConvertEnum type)
{
    const char * res[] = { "Version5", "Version6", "Version7" };
    return res[type];
}

std::string GAMSOptions::egdxConvertCode(GAMSOptions::EgdxConvert::EgdxConvertEnum type)
{
    const char * res[] = { "v5", "v6", "v7" };
    return res[type];
}

GAMSOptions::EgdxConvert::EgdxConvertEnum GAMSOptions::egdxConvertFromCode(const std::string &str)
{
    for (unsigned i=0; i<3; i++) {
        EgdxConvert::EgdxConvertEnum act = static_cast<EgdxConvert::EgdxConvertEnum>(i);
        std::string up = str;
        std::transform(up.begin(), up.end(), up.begin(), ::toupper);
        if (egdxConvertCode(act) == up)
            return act;
    };
    throw GAMSException("GAMSOptions::egdxConvertFromString: unknown EgdxConvert::EgdxConvertEnum type '" +str+ "'");
}

GAMSOptions::EgdxConvert::EgdxConvertEnum GAMSOptions::gdxConvert()
{
    return egdxConvertFromCode(mImpl->gdxConvert());
}

void GAMSOptions::setgdxConvert(const GAMSOptions::EgdxConvert::EgdxConvertEnum value)
{
   mImpl->setgdxConvert(egdxConvertCode(value));
}

GAMSOptions::EAppendExpand::EAppendExpandEnum GAMSOptions::appendExpand()
{
    return static_cast<GAMSOptions::EAppendExpand::EAppendExpandEnum> (mImpl->appendExpand());
}

void GAMSOptions::setAppendExpand(const GAMSOptions::EAppendExpand::EAppendExpandEnum value)
{
   mImpl->setAppendExpand(static_cast<int>(value));
}

double GAMSOptions::eTLim()
{
    return mImpl->eTLim();
}

void GAMSOptions::setETLim(const double value)
{
    mImpl->setETLim(value);
}

double GAMSOptions::profileTol()
{
    return mImpl->profileTol();
}

void GAMSOptions::setProfileTol(const double value)
{
    mImpl->setProfileTol(value);
}

std::string GAMSOptions::profileFile()
{
    return mImpl->profileFile();
}

void GAMSOptions::setProfileFile(const std::string& value)
{
    mImpl->setProfileFile(value);
}

int GAMSOptions::maxProcDir()
{
    return mImpl->maxProcDir();
}

void GAMSOptions::setMaxProcDir(const int value)
{
    mImpl->setMaxProcDir(value);
}

int GAMSOptions::integer1()
{
    return mImpl->integer1();
}

void GAMSOptions::setInteger1(const int value)
{
    mImpl->setInteger1(value);
}

int GAMSOptions::integer2()
{
    return mImpl->integer2();
}

void GAMSOptions::setInteger2(const int value)
{
    mImpl->setInteger2(value);
}

int GAMSOptions::integer3()
{
    return mImpl->integer3();
}

void GAMSOptions::setInteger3(const int value)
{
    mImpl->setInteger3(value);
}

int GAMSOptions::integer4()
{
    return mImpl->integer4();
}

void GAMSOptions::setInteger4(const int value)
{
    mImpl->setInteger4(value);
}

int GAMSOptions::integer5()
{
    return mImpl->integer5();
}

void GAMSOptions::setInteger5(const int value)
{
    mImpl->setInteger5(value);
}

std::string GAMSOptions::gridScript()
{
    return mImpl->gridScript();
}

void GAMSOptions::setGridScript(const std::string& value)
{
    mImpl->setGridScript(value);
}

int GAMSOptions::threads()
{
    return mImpl->threads();
}

void GAMSOptions::setThreads(const int value)
{
    mImpl->setThreads(value);
}

int GAMSOptions::useDLL()
{
    return mImpl->useDLL();
}

void GAMSOptions::setUseDLL(const int value)
{
    mImpl->setUseDLL(value);
}

int GAMSOptions::debug()
{
    return mImpl->debug();
}

void GAMSOptions::setDebug(const int value)
{
    mImpl->setDebug(value);
}

int GAMSOptions::pID2Error()
{
    return mImpl->pID2Error();
}

void GAMSOptions::setPID2Error(const int value)
{
    mImpl->setPID2Error(value);
}

GAMSOptions::ENoNewVarEqu::ENoNewVarEquEnum GAMSOptions::noNewVarEqu()
{
    return static_cast<GAMSOptions::ENoNewVarEqu::ENoNewVarEquEnum> (mImpl->noNewVarEqu());
}

void GAMSOptions::setNoNewVarEqu(const GAMSOptions::ENoNewVarEqu::ENoNewVarEquEnum value)
{
   mImpl->setNoNewVarEqu(static_cast<int>(value));
}

int GAMSOptions::forceOptFile()
{
    return mImpl->forceOptFile();
}

void GAMSOptions::setForceOptFile(const int value)
{
    mImpl->setForceOptFile(value);
}

std::string GAMSOptions::symPrefix()
{
    return mImpl->symPrefix();
}

void GAMSOptions::setSymPrefix(const std::string& value)
{
    mImpl->setSymPrefix(value);
}

int GAMSOptions::serverRun()
{
    return mImpl->serverRun();
}

void GAMSOptions::setServerRun(const int value)
{
    mImpl->setServerRun(value);
}

std::string GAMSOptions::egdxUelsText(GAMSOptions::EgdxUels::EgdxUelsEnum type)
{
    const char * res[] = { "squeezed", "full" };
    return res[type];
}

std::string GAMSOptions::egdxUelsCode(GAMSOptions::EgdxUels::EgdxUelsEnum type)
{
    const char * res[] = { "Squeezed", "Full" };
    return res[type];
}

GAMSOptions::EgdxUels::EgdxUelsEnum GAMSOptions::egdxUelsFromCode(const std::string &str)
{
    for (unsigned i=0; i<2; i++) {
        EgdxUels::EgdxUelsEnum act = static_cast<EgdxUels::EgdxUelsEnum>(i);
        std::string up = str;
        std::transform(up.begin(), up.end(), up.begin(), ::toupper);
        if (egdxUelsCode(act) == up)
            return act;
    };
    throw GAMSException("GAMSOptions::egdxUelsFromString: unknown EgdxUels::EgdxUelsEnum type '" +str+ "'");
}

GAMSOptions::EgdxUels::EgdxUelsEnum GAMSOptions::gdxUels()
{
    return egdxUelsFromCode(mImpl->gdxUels());
}

void GAMSOptions::setgdxUels(const GAMSOptions::EgdxUels::EgdxUelsEnum value)
{
   mImpl->setgdxUels(egdxUelsCode(value));
}

std::string GAMSOptions::dumpParmsLogPrefix()
{
    return mImpl->dumpParmsLogPrefix();
}

void GAMSOptions::setDumpParmsLogPrefix(const std::string& value)
{
    mImpl->setDumpParmsLogPrefix(value);
}

GAMSOptions::EstrictSingleton::EstrictSingletonEnum GAMSOptions::strictSingleton()
{
    return static_cast<GAMSOptions::EstrictSingleton::EstrictSingletonEnum> (mImpl->strictSingleton());
}

void GAMSOptions::setstrictSingleton(const GAMSOptions::EstrictSingleton::EstrictSingletonEnum value)
{
   mImpl->setstrictSingleton(static_cast<int>(value));
}

std::string GAMSOptions::saveObfuscate()
{
    return mImpl->saveObfuscate();
}

void GAMSOptions::setSaveObfuscate(const std::string& value)
{
    mImpl->setSaveObfuscate(value);
}

std::string GAMSOptions::xSaveObfuscate()
{
    return mImpl->xSaveObfuscate();
}

void GAMSOptions::setXSaveObfuscate(const std::string& value)
{
    mImpl->setXSaveObfuscate(value);
}

std::string GAMSOptions::restartNamed()
{
    return mImpl->restartNamed();
}

void GAMSOptions::setRestartNamed(const std::string& value)
{
    mImpl->setRestartNamed(value);
}

int GAMSOptions::sys14()
{
    return mImpl->sys14();
}

void GAMSOptions::setSys14(const int value)
{
    mImpl->setSys14(value);
}

double GAMSOptions::fDDelta()
{
    return mImpl->fDDelta();
}

void GAMSOptions::setFDDelta(const double value)
{
    mImpl->setFDDelta(value);
}

GAMSOptions::EFDOpt::EFDOptEnum GAMSOptions::fDOpt()
{
    return static_cast<GAMSOptions::EFDOpt::EFDOptEnum> (mImpl->fDOpt());
}

void GAMSOptions::setFDOpt(const GAMSOptions::EFDOpt::EFDOptEnum value)
{
   mImpl->setFDOpt(static_cast<int>(value));
}

std::string GAMSOptions::solver()
{
    return mImpl->solver();
}

void GAMSOptions::setSolver(const std::string& value)
{
    mImpl->setSolver(value);
}

std::string GAMSOptions::docFile()
{
    return mImpl->docFile();
}

void GAMSOptions::setDocFile(const std::string& value)
{
    mImpl->setDocFile(value);
}

std::string GAMSOptions::sparseRun()
{
    return mImpl->sparseRun();
}

void GAMSOptions::setSparseRun(const std::string& value)
{
    mImpl->setSparseRun(value);
}

int GAMSOptions::sys15()
{
    return mImpl->sys15();
}

void GAMSOptions::setSys15(const int value)
{
    mImpl->setSys15(value);
}

int GAMSOptions::sys16()
{
    return mImpl->sys16();
}

void GAMSOptions::setSys16(const int value)
{
    mImpl->setSys16(value);
}

int GAMSOptions::sys17()
{
    return mImpl->sys17();
}

void GAMSOptions::setSys17(const int value)
{
    mImpl->setSys17(value);
}

GAMSOptions::EPrefixLoadPath::EPrefixLoadPathEnum GAMSOptions::prefixLoadPath()
{
    return static_cast<GAMSOptions::EPrefixLoadPath::EPrefixLoadPathEnum> (mImpl->prefixLoadPath());
}

void GAMSOptions::setPrefixLoadPath(const GAMSOptions::EPrefixLoadPath::EPrefixLoadPathEnum value)
{
   mImpl->setPrefixLoadPath(static_cast<int>(value));
}

int GAMSOptions::threadsAsync()
{
    return mImpl->threadsAsync();
}

void GAMSOptions::setThreadsAsync(const int value)
{
    mImpl->setThreadsAsync(value);
}

int GAMSOptions::mCPRHoldfx()
{
    return mImpl->mCPRHoldfx();
}

void GAMSOptions::setMCPRHoldfx(const int value)
{
    mImpl->setMCPRHoldfx(value);
}

GAMSOptions::EAsyncSolLst::EAsyncSolLstEnum GAMSOptions::asyncSolLst()
{
    return static_cast<GAMSOptions::EAsyncSolLst::EAsyncSolLstEnum> (mImpl->asyncSolLst());
}

void GAMSOptions::setAsyncSolLst(const GAMSOptions::EAsyncSolLst::EAsyncSolLstEnum value)
{
   mImpl->setAsyncSolLst(static_cast<int>(value));
}

std::string GAMSOptions::fileStem()
{
    return mImpl->fileStem();
}

void GAMSOptions::setFileStem(const std::string& value)
{
    mImpl->setFileStem(value);
}

std::string GAMSOptions::procDirPath()
{
    return mImpl->procDirPath();
}

void GAMSOptions::setProcDirPath(const std::string& value)
{
    mImpl->setProcDirPath(value);
}

GAMSOptions::EPyMultInst::EPyMultInstEnum GAMSOptions::pyMultInst()
{
    return static_cast<GAMSOptions::EPyMultInst::EPyMultInstEnum> (mImpl->pyMultInst());
}

void GAMSOptions::setPyMultInst(const GAMSOptions::EPyMultInst::EPyMultInstEnum value)
{
   mImpl->setPyMultInst(static_cast<int>(value));
}

std::string GAMSOptions::fileStemApFromEnv()
{
    return mImpl->fileStemApFromEnv();
}

void GAMSOptions::setFileStemApFromEnv(const std::string& value)
{
    mImpl->setFileStemApFromEnv(value);
}

GAMSOptions::EShowOSMemory::EShowOSMemoryEnum GAMSOptions::showOSMemory()
{
    return static_cast<GAMSOptions::EShowOSMemory::EShowOSMemoryEnum> (mImpl->showOSMemory());
}

void GAMSOptions::setShowOSMemory(const GAMSOptions::EShowOSMemory::EShowOSMemoryEnum value)
{
   mImpl->setShowOSMemory(static_cast<int>(value));
}

GAMSOptions::ELstTitleLeftAligned::ELstTitleLeftAlignedEnum GAMSOptions::lstTitleLeftAligned()
{
    return static_cast<GAMSOptions::ELstTitleLeftAligned::ELstTitleLeftAlignedEnum> (mImpl->lstTitleLeftAligned());
}

void GAMSOptions::setLstTitleLeftAligned(const GAMSOptions::ELstTitleLeftAligned::ELstTitleLeftAlignedEnum value)
{
   mImpl->setLstTitleLeftAligned(static_cast<int>(value));
}

GAMSOptions::EFreeEmbeddedPython::EFreeEmbeddedPythonEnum GAMSOptions::freeEmbeddedPython()
{
    return static_cast<GAMSOptions::EFreeEmbeddedPython::EFreeEmbeddedPythonEnum> (mImpl->freeEmbeddedPython());
}

void GAMSOptions::setFreeEmbeddedPython(const GAMSOptions::EFreeEmbeddedPython::EFreeEmbeddedPythonEnum value)
{
   mImpl->setFreeEmbeddedPython(static_cast<int>(value));
}

GAMSOptions::ECaptureModelInstance::ECaptureModelInstanceEnum GAMSOptions::captureModelInstance()
{
    return static_cast<GAMSOptions::ECaptureModelInstance::ECaptureModelInstanceEnum> (mImpl->captureModelInstance());
}

void GAMSOptions::setCaptureModelInstance(const GAMSOptions::ECaptureModelInstance::ECaptureModelInstanceEnum value)
{
   mImpl->setCaptureModelInstance(static_cast<int>(value));
}

std::string GAMSOptions::iDCJSON()
{
    return mImpl->iDCJSON();
}

void GAMSOptions::setIDCJSON(const std::string& value)
{
    mImpl->setIDCJSON(value);
}

std::string GAMSOptions::iDCGenerateJSON()
{
    return mImpl->iDCGenerateJSON();
}

void GAMSOptions::setIDCGenerateJSON(const std::string& value)
{
    mImpl->setIDCGenerateJSON(value);
}

std::string GAMSOptions::iDCGenerateGDX()
{
    return mImpl->iDCGenerateGDX();
}

void GAMSOptions::setIDCGenerateGDX(const std::string& value)
{
    mImpl->setIDCGenerateGDX(value);
}

std::string GAMSOptions::iDCGenerateGDXInput()
{
    return mImpl->iDCGenerateGDXInput();
}

void GAMSOptions::setIDCGenerateGDXInput(const std::string& value)
{
    mImpl->setIDCGenerateGDXInput(value);
}

std::string GAMSOptions::iDCGenerateGDXOutput()
{
    return mImpl->iDCGenerateGDXOutput();
}

void GAMSOptions::setIDCGenerateGDXOutput(const std::string& value)
{
    mImpl->setIDCGenerateGDXOutput(value);
}

std::string GAMSOptions::iDCGDXInput()
{
    return mImpl->iDCGDXInput();
}

void GAMSOptions::setIDCGDXInput(const std::string& value)
{
    mImpl->setIDCGDXInput(value);
}

std::string GAMSOptions::iDCGDXOutput()
{
    return mImpl->iDCGDXOutput();
}

void GAMSOptions::setIDCGDXOutput(const std::string& value)
{
    mImpl->setIDCGDXOutput(value);
}

GAMSOptions::ECheckErrorLevel::ECheckErrorLevelEnum GAMSOptions::checkErrorLevel()
{
    return static_cast<GAMSOptions::ECheckErrorLevel::ECheckErrorLevelEnum> (mImpl->checkErrorLevel());
}

void GAMSOptions::setCheckErrorLevel(const GAMSOptions::ECheckErrorLevel::ECheckErrorLevelEnum value)
{
   mImpl->setCheckErrorLevel(static_cast<int>(value));
}

int GAMSOptions::iDCProtect()
{
    return mImpl->iDCProtect();
}

void GAMSOptions::setIDCProtect(const int value)
{
    mImpl->setIDCProtect(value);
}

std::string GAMSOptions::encryptKey()
{
    return mImpl->encryptKey();
}

void GAMSOptions::setEncryptKey(const std::string& value)
{
    mImpl->setEncryptKey(value);
}

std::string GAMSOptions::decryptKey()
{
    return mImpl->decryptKey();
}

void GAMSOptions::setDecryptKey(const std::string& value)
{
    mImpl->setDecryptKey(value);
}

int GAMSOptions::sys18()
{
    return mImpl->sys18();
}

void GAMSOptions::setSys18(const int value)
{
    mImpl->setSys18(value);
}

std::string GAMSOptions::eSuffixDLVarsText(GAMSOptions::ESuffixDLVars::ESuffixDLVarsEnum type)
{
    const char * res[] = { "OffSuffixDLVars", "OnSuffixDLVars" };
    return res[type];
}

std::string GAMSOptions::eSuffixDLVarsCode(GAMSOptions::ESuffixDLVars::ESuffixDLVarsEnum type)
{
    const char * res[] = { "off", "on" };
    return res[type];
}

GAMSOptions::ESuffixDLVars::ESuffixDLVarsEnum GAMSOptions::eSuffixDLVarsFromCode(const std::string &str)
{
    for (unsigned i=0; i<2; i++) {
        ESuffixDLVars::ESuffixDLVarsEnum act = static_cast<ESuffixDLVars::ESuffixDLVarsEnum>(i);
        std::string up = str;
        std::transform(up.begin(), up.end(), up.begin(), ::toupper);
        if (eSuffixDLVarsCode(act) == up)
            return act;
    };
    throw GAMSException("GAMSOptions::eSuffixDLVarsFromString: unknown ESuffixDLVars::ESuffixDLVarsEnum type '" +str+ "'");
}

GAMSOptions::ESuffixDLVars::ESuffixDLVarsEnum GAMSOptions::suffixDLVars()
{
    return eSuffixDLVarsFromCode(mImpl->suffixDLVars());
}

void GAMSOptions::setSuffixDLVars(const GAMSOptions::ESuffixDLVars::ESuffixDLVarsEnum value)
{
   mImpl->setSuffixDLVars(eSuffixDLVarsCode(value));
}

std::string GAMSOptions::eDigitText(GAMSOptions::EDigit::EDigitEnum type)
{
    const char * res[] = { "OffDigit", "OnDigit" };
    return res[type];
}

std::string GAMSOptions::eDigitCode(GAMSOptions::EDigit::EDigitEnum type)
{
    const char * res[] = { "off", "on" };
    return res[type];
}

GAMSOptions::EDigit::EDigitEnum GAMSOptions::eDigitFromCode(const std::string &str)
{
    for (unsigned i=0; i<2; i++) {
        EDigit::EDigitEnum act = static_cast<EDigit::EDigitEnum>(i);
        std::string up = str;
        std::transform(up.begin(), up.end(), up.begin(), ::toupper);
        if (eDigitCode(act) == up)
            return act;
    };
    throw GAMSException("GAMSOptions::eDigitFromString: unknown EDigit::EDigitEnum type '" +str+ "'");
}

GAMSOptions::EDigit::EDigitEnum GAMSOptions::digit()
{
    return eDigitFromCode(mImpl->digit());
}

void GAMSOptions::setDigit(const GAMSOptions::EDigit::EDigitEnum value)
{
   mImpl->setDigit(eDigitCode(value));
}

std::string GAMSOptions::eEmptyText(GAMSOptions::EEmpty::EEmptyEnum type)
{
    const char * res[] = { "OffEmpty", "OnEmpty" };
    return res[type];
}

std::string GAMSOptions::eEmptyCode(GAMSOptions::EEmpty::EEmptyEnum type)
{
    const char * res[] = { "off", "on" };
    return res[type];
}

GAMSOptions::EEmpty::EEmptyEnum GAMSOptions::eEmptyFromCode(const std::string &str)
{
    for (unsigned i=0; i<2; i++) {
        EEmpty::EEmptyEnum act = static_cast<EEmpty::EEmptyEnum>(i);
        std::string up = str;
        std::transform(up.begin(), up.end(), up.begin(), ::toupper);
        if (eEmptyCode(act) == up)
            return act;
    };
    throw GAMSException("GAMSOptions::eEmptyFromString: unknown EEmpty::EEmptyEnum type '" +str+ "'");
}

GAMSOptions::EEmpty::EEmptyEnum GAMSOptions::empty()
{
    return eEmptyFromCode(mImpl->empty());
}

void GAMSOptions::setEmpty(const GAMSOptions::EEmpty::EEmptyEnum value)
{
   mImpl->setEmpty(eEmptyCode(value));
}

std::string GAMSOptions::eolCom()
{
    return mImpl->eolCom();
}

void GAMSOptions::setEolCom(const std::string& value)
{
    mImpl->setEolCom(value);
}

std::string GAMSOptions::eFilteredText(GAMSOptions::EFiltered::EFilteredEnum type)
{
    const char * res[] = { "OffFiltered", "OnFiltered" };
    return res[type];
}

std::string GAMSOptions::eFilteredCode(GAMSOptions::EFiltered::EFilteredEnum type)
{
    const char * res[] = { "off", "on" };
    return res[type];
}

GAMSOptions::EFiltered::EFilteredEnum GAMSOptions::eFilteredFromCode(const std::string &str)
{
    for (unsigned i=0; i<2; i++) {
        EFiltered::EFilteredEnum act = static_cast<EFiltered::EFilteredEnum>(i);
        std::string up = str;
        std::transform(up.begin(), up.end(), up.begin(), ::toupper);
        if (eFilteredCode(act) == up)
            return act;
    };
    throw GAMSException("GAMSOptions::eFilteredFromString: unknown EFiltered::EFilteredEnum type '" +str+ "'");
}

GAMSOptions::EFiltered::EFilteredEnum GAMSOptions::filtered()
{
    return eFilteredFromCode(mImpl->filtered());
}

void GAMSOptions::setFiltered(const GAMSOptions::EFiltered::EFilteredEnum value)
{
   mImpl->setFiltered(eFilteredCode(value));
}

std::string GAMSOptions::inlineCom()
{
    return mImpl->inlineCom();
}

void GAMSOptions::setInlineCom(const std::string& value)
{
    mImpl->setInlineCom(value);
}

std::string GAMSOptions::eListingText(GAMSOptions::EListing::EListingEnum type)
{
    const char * res[] = { "OffListing", "OnListing" };
    return res[type];
}

std::string GAMSOptions::eListingCode(GAMSOptions::EListing::EListingEnum type)
{
    const char * res[] = { "off", "on" };
    return res[type];
}

GAMSOptions::EListing::EListingEnum GAMSOptions::eListingFromCode(const std::string &str)
{
    for (unsigned i=0; i<2; i++) {
        EListing::EListingEnum act = static_cast<EListing::EListingEnum>(i);
        std::string up = str;
        std::transform(up.begin(), up.end(), up.begin(), ::toupper);
        if (eListingCode(act) == up)
            return act;
    };
    throw GAMSException("GAMSOptions::eListingFromString: unknown EListing::EListingEnum type '" +str+ "'");
}

GAMSOptions::EListing::EListingEnum GAMSOptions::listing()
{
    return eListingFromCode(mImpl->listing());
}

void GAMSOptions::setListing(const GAMSOptions::EListing::EListingEnum value)
{
   mImpl->setListing(eListingCode(value));
}

int GAMSOptions::putND()
{
    return mImpl->putND();
}

void GAMSOptions::setPutND(const int value)
{
    mImpl->setPutND(value);
}

GAMSOptions::EPutNR::EPutNREnum GAMSOptions::putNR()
{
    return static_cast<GAMSOptions::EPutNR::EPutNREnum> (mImpl->putNR());
}

void GAMSOptions::setPutNR(const GAMSOptions::EPutNR::EPutNREnum value)
{
   mImpl->setPutNR(static_cast<int>(value));
}

int GAMSOptions::putPS()
{
    return mImpl->putPS();
}

void GAMSOptions::setPutPS(const int value)
{
    mImpl->setPutPS(value);
}

int GAMSOptions::putPW()
{
    return mImpl->putPW();
}

void GAMSOptions::setPutPW(const int value)
{
    mImpl->setPutPW(value);
}

std::string GAMSOptions::LP()
{
    return mImpl->LP();
}

void GAMSOptions::setLP(const std::string& value)
{
     mImpl->setLP(value);
}

std::string GAMSOptions::MIP()
{
    return mImpl->MIP();
}

void GAMSOptions::setMIP(const std::string& value)
{
     mImpl->setMIP(value);
}

std::string GAMSOptions::RMIP()
{
    return mImpl->RMIP();
}

void GAMSOptions::setRMIP(const std::string& value)
{
     mImpl->setRMIP(value);
}

std::string GAMSOptions::NLP()
{
    return mImpl->NLP();
}

void GAMSOptions::setNLP(const std::string& value)
{
     mImpl->setNLP(value);
}

std::string GAMSOptions::MCP()
{
    return mImpl->MCP();
}

void GAMSOptions::setMCP(const std::string& value)
{
     mImpl->setMCP(value);
}

std::string GAMSOptions::MPEC()
{
    return mImpl->MPEC();
}

void GAMSOptions::setMPEC(const std::string& value)
{
     mImpl->setMPEC(value);
}

std::string GAMSOptions::RMPEC()
{
    return mImpl->RMPEC();
}

void GAMSOptions::setRMPEC(const std::string& value)
{
     mImpl->setRMPEC(value);
}

std::string GAMSOptions::CNS()
{
    return mImpl->CNS();
}

void GAMSOptions::setCNS(const std::string& value)
{
     mImpl->setCNS(value);
}

std::string GAMSOptions::DNLP()
{
    return mImpl->DNLP();
}

void GAMSOptions::setDNLP(const std::string& value)
{
     mImpl->setDNLP(value);
}

std::string GAMSOptions::RMINLP()
{
    return mImpl->RMINLP();
}

void GAMSOptions::setRMINLP(const std::string& value)
{
     mImpl->setRMINLP(value);
}

std::string GAMSOptions::MINLP()
{
    return mImpl->MINLP();
}

void GAMSOptions::setMINLP(const std::string& value)
{
     mImpl->setMINLP(value);
}

std::string GAMSOptions::QCP()
{
    return mImpl->QCP();
}

void GAMSOptions::setQCP(const std::string& value)
{
     mImpl->setQCP(value);
}

std::string GAMSOptions::MIQCP()
{
    return mImpl->MIQCP();
}

void GAMSOptions::setMIQCP(const std::string& value)
{
     mImpl->setMIQCP(value);
}

std::string GAMSOptions::RMIQCP()
{
    return mImpl->RMIQCP();
}

void GAMSOptions::setRMIQCP(const std::string& value)
{
     mImpl->setRMIQCP(value);
}

std::string GAMSOptions::EMP()
{
    return mImpl->EMP();
}

void GAMSOptions::setEMP(const std::string& value)
{
     mImpl->setEMP(value);
}

string GAMSOptions::getSolver(const string& modelType)
{
    return mImpl->getSolver(modelType);
}

void GAMSOptions::setSolver(const string& modelType, const string& solver)
{
    mImpl->setSolver(modelType, solver);
}
}
