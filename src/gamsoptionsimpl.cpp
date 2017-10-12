/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
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

#include "gamsoptionsimpl.h"
#include "gamspath.h"
#include "gamsexception.h"
#include "gamsplatform.h"
#include "cfgmcc.h"
#include "gmomcc.h"

using namespace std;

namespace gams {
const char* GAMSOptionsImpl::cGamsBuild = __DATE__ " " __TIME__;

#if LOCAL_BUILD
const char* GAMSOptionsImpl::cGamsVersion = "0.0.0";
#else
const char* GAMSOptionsImpl::cGamsVersion = "25.0.0";
#endif

GAMSOptionsImpl::GAMSOptionsImpl(GAMSWorkspace& workspace, GAMSOptionsImpl* optFrom)
    : mWs(workspace)
{
    setupOptionsBase();
    if (optFrom) {
        GAMSPath pfPath = GAMSPath(mWs.workingDirectory()).tempFile(mWs.scratchFilePrefix());
        optWriteParameterFile(optFrom->mOPT, pfPath.c_str());
        optReadParameterFile(mOPT, pfPath.c_str());

        if (mWs.debug() == GAMSEnum::DebugLevel::Off)
            QFile(pfPath).remove();
    } else {
        setDefaultSolver();
    }
}

GAMSOptionsImpl::GAMSOptionsImpl(GAMSWorkspace& workspace, const std::string& optFile)
    : mWs(workspace)
{
    setupOptionsBase();
    if (!optFile.empty()) {
        GAMSPath optPath(optFile);
        if (optReadParameterFile(mOPT, optPath.c_str())) {
            char msg[GMS_SSSIZE];
            for (int i=1, itype=0; i <= optMessageCount(mOPT); i++) {
                optGetMessage(mOPT, i, msg, &itype);
                // TODO(AF) log msg!
            }
            throw GAMSException("Problem reading parameter file " + optFile);
        }

        for (int i = 1; i < gmoProc_nrofmodeltypes; i++) {
            char buf[GMS_SSSIZE], buf2[GMS_SSSIZE];
            if (!optGetDefinedStr(mOPT, cfgModelTypeName(mCFG, i, buf)))
                optSetStrStr(mOPT, cfgModelTypeName(mCFG, i, buf), cfgAlgName(mCFG, i, buf2));
        }
    } else {
        setDefaultSolver();
    }
}

void GAMSOptionsImpl::writeOptionFile(const std::string& optFileName)
{
    if (0 != optWriteParameterFile(mOPT, optFileName.c_str()))
        throw GAMSException("Could not write parameter file " + optFileName);
}

int GAMSOptionsImpl::inputDirSize()
{
    return mIDirSize;
}

void GAMSOptionsImpl::setInputDir(const string value, const int index)
{
    if (index > mIDirSize)
        throw GAMSException("No Gaps allowed. Index (" +to_string(index) + ") out of range [0-" + to_string(mIDirSize) + "]");
    if (mIDirSize >= 40)
        throw GAMSException("Maximum count of 40 input directories reached");
    bool append = (index < 0 || index == mIDirSize);
    auto dir = "InputDir" + to_string(append ? mIDirSize : index);
    optSetStrStr(mOPT, dir.c_str(), value.c_str());
    if (append) mIDirSize++;
}

string GAMSOptionsImpl::getInputDir(const int index)
{
    if (index < 0 || index >= mIDirSize)
        throw GAMSException("Requested index (" +to_string(index) + ") out of range [0-" + to_string(mIDirSize) + "]");
    char buf[GMS_SSSIZE];
    auto dir = "InputDir" + to_string(index);
    optGetStrStr(mOPT, dir.c_str(), buf);
    return buf;
}

void GAMSOptionsImpl::removeInputDir(const int index)
{
    if (index < 0 || index >= mIDirSize)
        throw GAMSException("Requested index (" +to_string(index) + ") out of range [0-" + to_string(mIDirSize) + "]");
    char buf[GMS_SSSIZE];
    for (int i = index+1; i < mIDirSize; ++i) {
        auto toDir = "InputDir" + to_string(i-1);
        auto fromDir = "InputDir" + to_string(i);
        optGetStrStr(mOPT, fromDir.c_str(), buf);
        optSetStrStr(mOPT, toDir.c_str(), buf);
    }
    mIDirSize--;
}

void GAMSOptionsImpl::setDefine(const string& key, const string& value)
{
    string gmsParam = "--" + key + "=" + value;
    optReadFromStr(mOPT, gmsParam.c_str());
}

string GAMSOptionsImpl::getDefine(const string& key)
{
    char buf[GMS_SSSIZE];
    if (1 != optReadFromListStr(mOPT, ("--"+key).c_str(), 1, buf))
        return "";
    return buf;
}

bool GAMSOptionsImpl::removeDefine(const string& key)
{
    char buf[GMS_SSSIZE];
    return 1 == optGetFromListStr(mOPT,("--"+key).c_str(), buf);
}

void GAMSOptionsImpl::setAllModelTypes(const string& solver)
{
    char buf[GMS_SSSIZE];
    if (0 == cfgAlgNumber(mCFG, solver.c_str()))
        throw GAMSException("Unknown solver " + solver);
    for (int i = 1; i < gmoProc_nrofmodeltypes; i++) {
        if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, solver.c_str()), i))
            optSetStrStr(mOPT, cfgModelTypeName(mCFG, i, buf), solver.c_str());
    }
}

string GAMSOptionsImpl::gdx()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "GDX", buf);
}

void GAMSOptionsImpl::setGdx(string value)
{
    GAMSPath gdxFile(value);
    optSetStrStr(mOPT, "GDX", gdxFile.suffix(".gdx").c_str());
}

std::string GAMSOptionsImpl::restart()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "Restart", buf);
}

void GAMSOptionsImpl::setRestart(const std::string& value)
{
    optSetStrStr(mOPT, "Restart", value.c_str());
}

std::string GAMSOptionsImpl::input()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "Input", buf);
}

void GAMSOptionsImpl::setInput(const std::string& value)
{
    optSetStrStr(mOPT, "Input", value.c_str());
}

std::string GAMSOptionsImpl::output()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "Output", buf);
}

void GAMSOptionsImpl::setOutput(const std::string& value)
{
    optSetStrStr(mOPT, "Output", value.c_str());
}

std::string GAMSOptionsImpl::save()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "Save", buf);
}

void GAMSOptionsImpl::setSave(const std::string& value)
{
    optSetStrStr(mOPT, "Save", value.c_str());
}

std::string GAMSOptionsImpl::symbol()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "Symbol", buf);
}

void GAMSOptionsImpl::setSymbol(const std::string& value)
{
    optSetStrStr(mOPT, "Symbol", value.c_str());
}

std::string GAMSOptionsImpl::xSave()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "XSave", buf);
}

void GAMSOptionsImpl::setXSave(const std::string& value)
{
    optSetStrStr(mOPT, "XSave", value.c_str());
}

std::string GAMSOptionsImpl::expand()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "Expand", buf);
}

void GAMSOptionsImpl::setExpand(const std::string& value)
{
    optSetStrStr(mOPT, "Expand", value.c_str());
}

int GAMSOptionsImpl::pageWidth()
{
    return optGetIntStr(mOPT, "PageWidth");
}

void GAMSOptionsImpl::setPageWidth(const int value)
{
    optSetIntStr(mOPT, "PageWidth", value);
}

int GAMSOptionsImpl::pageSize()
{
    return optGetIntStr(mOPT, "PageSize");
}

void GAMSOptionsImpl::setPageSize(const int value)
{
    optSetIntStr(mOPT, "PageSize", value);
}

int GAMSOptionsImpl::pageContr()
{
    return optGetIntStr(mOPT, "PageContr");
}

void GAMSOptionsImpl::setPageContr(const int value)
{
    optSetIntStr(mOPT, "PageContr", value);
}

std::string GAMSOptionsImpl::action()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "Action", buf);
}

void GAMSOptionsImpl::setAction(const std::string& value)
{
    optSetStrStr(mOPT, "Action", value.c_str());
}

std::string GAMSOptionsImpl::subSys()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SubSys", buf);
}

void GAMSOptionsImpl::setSubSys(const std::string& value)
{
    optSetStrStr(mOPT, "SubSys", value.c_str());
}

std::string GAMSOptionsImpl::errNam()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "ErrNam", buf);
}

void GAMSOptionsImpl::setErrNam(const std::string& value)
{
    optSetStrStr(mOPT, "ErrNam", value.c_str());
}

std::string GAMSOptionsImpl::scrDir()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "ScrDir", buf);
}

void GAMSOptionsImpl::setScrDir(const std::string& value)
{
    optSetStrStr(mOPT, "ScrDir", value.c_str());
}

std::string GAMSOptionsImpl::scrNam()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "ScrNam", buf);
}

void GAMSOptionsImpl::setScrNam(const std::string& value)
{
    optSetStrStr(mOPT, "ScrNam", value.c_str());
}

std::string GAMSOptionsImpl::sysDir()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SysDir", buf);
}

void GAMSOptionsImpl::setSysDir(const std::string& value)
{
    optSetStrStr(mOPT, "SysDir", value.c_str());
}

std::string GAMSOptionsImpl::curDir()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "CurDir", buf);
}

void GAMSOptionsImpl::setCurDir(const std::string& value)
{
    optSetStrStr(mOPT, "CurDir", value.c_str());
}

std::string GAMSOptionsImpl::workDir()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "WorkDir", buf);
}

void GAMSOptionsImpl::setWorkDir(const std::string& value)
{
    optSetStrStr(mOPT, "WorkDir", value.c_str());
}

std::string GAMSOptionsImpl::putDir()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "PutDir", buf);
}

void GAMSOptionsImpl::setPutDir(const std::string& value)
{
    optSetStrStr(mOPT, "PutDir", value.c_str());
}

std::string GAMSOptionsImpl::gridDir()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "GridDir", buf);
}

void GAMSOptionsImpl::setGridDir(const std::string& value)
{
    optSetStrStr(mOPT, "GridDir", value.c_str());
}

std::string GAMSOptionsImpl::scriptNext()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "ScriptNext", buf);
}

void GAMSOptionsImpl::setScriptNext(const std::string& value)
{
    optSetStrStr(mOPT, "ScriptNext", value.c_str());
}

std::string GAMSOptionsImpl::scriptExit()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "ScriptExit", buf);
}

void GAMSOptionsImpl::setScriptExit(const std::string& value)
{
    optSetStrStr(mOPT, "ScriptExit", value.c_str());
}

std::string GAMSOptionsImpl::scriptFrst()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "ScriptFrst", buf);
}

void GAMSOptionsImpl::setScriptFrst(const std::string& value)
{
    optSetStrStr(mOPT, "ScriptFrst", value.c_str());
}

int GAMSOptionsImpl::logLine()
{
    return optGetIntStr(mOPT, "LogLine");
}

void GAMSOptionsImpl::setLogLine(const int value)
{
    optSetIntStr(mOPT, "LogLine", value);
}

int GAMSOptionsImpl::logOption()
{
    return optGetIntStr(mOPT, "LogOption");
}

void GAMSOptionsImpl::setLogOption(const int value)
{
    optSetIntStr(mOPT, "LogOption", value);
}

std::string GAMSOptionsImpl::logFile()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "LogFile", buf);
}

void GAMSOptionsImpl::setLogFile(const std::string& value)
{
    optSetStrStr(mOPT, "LogFile", value.c_str());
}

std::string GAMSOptionsImpl::trace()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "Trace", buf);
}

void GAMSOptionsImpl::setTrace(const std::string& value)
{
    optSetStrStr(mOPT, "Trace", value.c_str());
}

int GAMSOptionsImpl::appendOut()
{
    return optGetIntStr(mOPT, "AppendOut");
}

void GAMSOptionsImpl::setAppendOut(const int value)
{
    optSetIntStr(mOPT, "AppendOut", value);
}

int GAMSOptionsImpl::appendLog()
{
    return optGetIntStr(mOPT, "AppendLog");
}

void GAMSOptionsImpl::setAppendLog(const int value)
{
    optSetIntStr(mOPT, "AppendLog", value);
}

int GAMSOptionsImpl::stepSum()
{
    return optGetIntStr(mOPT, "StepSum");
}

void GAMSOptionsImpl::setStepSum(const int value)
{
    optSetIntStr(mOPT, "StepSum", value);
}

int GAMSOptionsImpl::multiPass()
{
    return optGetIntStr(mOPT, "MultiPass");
}

void GAMSOptionsImpl::setMultiPass(const int value)
{
    optSetIntStr(mOPT, "MultiPass", value);
}

int GAMSOptionsImpl::suppress()
{
    return optGetIntStr(mOPT, "Suppress");
}

void GAMSOptionsImpl::setSuppress(const int value)
{
    optSetIntStr(mOPT, "Suppress", value);
}

int GAMSOptionsImpl::dFormat()
{
    return optGetIntStr(mOPT, "DFormat");
}

void GAMSOptionsImpl::setDFormat(const int value)
{
    optSetIntStr(mOPT, "DFormat", value);
}

int GAMSOptionsImpl::tFormat()
{
    return optGetIntStr(mOPT, "TFormat");
}

void GAMSOptionsImpl::setTFormat(const int value)
{
    optSetIntStr(mOPT, "TFormat", value);
}

int GAMSOptionsImpl::tabIn()
{
    return optGetIntStr(mOPT, "TabIn");
}

void GAMSOptionsImpl::setTabIn(const int value)
{
    optSetIntStr(mOPT, "TabIn", value);
}

std::string GAMSOptionsImpl::solverCntr()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SolverCntr", buf);
}

void GAMSOptionsImpl::setSolverCntr(const std::string& value)
{
    optSetStrStr(mOPT, "SolverCntr", value.c_str());
}

std::string GAMSOptionsImpl::solverMatr()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SolverMatr", buf);
}

void GAMSOptionsImpl::setSolverMatr(const std::string& value)
{
    optSetStrStr(mOPT, "SolverMatr", value.c_str());
}

std::string GAMSOptionsImpl::solverInst()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SolverInst", buf);
}

void GAMSOptionsImpl::setSolverInst(const std::string& value)
{
    optSetStrStr(mOPT, "SolverInst", value.c_str());
}

std::string GAMSOptionsImpl::solverStat()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SolverStat", buf);
}

void GAMSOptionsImpl::setSolverStat(const std::string& value)
{
    optSetStrStr(mOPT, "SolverStat", value.c_str());
}

std::string GAMSOptionsImpl::solverSolu()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SolverSolu", buf);
}

void GAMSOptionsImpl::setSolverSolu(const std::string& value)
{
    optSetStrStr(mOPT, "SolverSolu", value.c_str());
}

std::string GAMSOptionsImpl::solverDict()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SolverDict", buf);
}

void GAMSOptionsImpl::setSolverDict(const std::string& value)
{
    optSetStrStr(mOPT, "SolverDict", value.c_str());
}

int GAMSOptionsImpl::intVarUp()
{
    return optGetIntStr(mOPT, "IntVarUp");
}

void GAMSOptionsImpl::setIntVarUp(const int value)
{
    optSetIntStr(mOPT, "IntVarUp", value);
}

int GAMSOptionsImpl::lstCase()
{
   return optGetIntStr(mOPT, "Case");
}

void GAMSOptionsImpl::setLstCase(const int value)
{
   optSetIntStr(mOPT, "Case", value);
}

int GAMSOptionsImpl::g205()
{
    return optGetIntStr(mOPT, "G205");
}

void GAMSOptionsImpl::setG205(const int value)
{
    optSetIntStr(mOPT, "G205", value);
}

std::string GAMSOptionsImpl::license()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "License", buf);
}

void GAMSOptionsImpl::setLicense(const std::string& value)
{
    optSetStrStr(mOPT, "License", value.c_str());
}

int GAMSOptionsImpl::dumpParms()
{
    return optGetIntStr(mOPT, "DumpParms");
}

void GAMSOptionsImpl::setDumpParms(const int value)
{
    optSetIntStr(mOPT, "DumpParms", value);
}

int GAMSOptionsImpl::noCr()
{
    return optGetIntStr(mOPT, "NoCr");
}

void GAMSOptionsImpl::setNoCr(const int value)
{
    optSetIntStr(mOPT, "NoCr", value);
}

int GAMSOptionsImpl::profile()
{
    return optGetIntStr(mOPT, "Profile");
}

void GAMSOptionsImpl::setProfile(const int value)
{
    optSetIntStr(mOPT, "Profile", value);
}

std::string GAMSOptionsImpl::libIncDir()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "LibIncDir", buf);
}

void GAMSOptionsImpl::setLibIncDir(const std::string& value)
{
    optSetStrStr(mOPT, "LibIncDir", value.c_str());
}

std::string GAMSOptionsImpl::sysIncDir()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SysIncDir", buf);
}

void GAMSOptionsImpl::setSysIncDir(const std::string& value)
{
    optSetStrStr(mOPT, "SysIncDir", value.c_str());
}

std::string GAMSOptionsImpl::user1()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "User1", buf);
}

void GAMSOptionsImpl::setUser1(const std::string& value)
{
    optSetStrStr(mOPT, "User1", value.c_str());
}

std::string GAMSOptionsImpl::user2()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "User2", buf);
}

void GAMSOptionsImpl::setUser2(const std::string& value)
{
    optSetStrStr(mOPT, "User2", value.c_str());
}

std::string GAMSOptionsImpl::user3()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "User3", buf);
}

void GAMSOptionsImpl::setUser3(const std::string& value)
{
    optSetStrStr(mOPT, "User3", value.c_str());
}

std::string GAMSOptionsImpl::user4()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "User4", buf);
}

void GAMSOptionsImpl::setUser4(const std::string& value)
{
    optSetStrStr(mOPT, "User4", value.c_str());
}

std::string GAMSOptionsImpl::user5()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "User5", buf);
}

void GAMSOptionsImpl::setUser5(const std::string& value)
{
    optSetStrStr(mOPT, "User5", value.c_str());
}

int GAMSOptionsImpl::forceWork()
{
    return optGetIntStr(mOPT, "ForceWork");
}

void GAMSOptionsImpl::setForceWork(const int value)
{
    optSetIntStr(mOPT, "ForceWork", value);
}

int GAMSOptionsImpl::cErr()
{
    return optGetIntStr(mOPT, "CErr");
}

void GAMSOptionsImpl::setCErr(const int value)
{
    optSetIntStr(mOPT, "CErr", value);
}

std::string GAMSOptionsImpl::fErr()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "FErr", buf);
}

void GAMSOptionsImpl::setFErr(const std::string& value)
{
    optSetStrStr(mOPT, "FErr", value.c_str());
}

int GAMSOptionsImpl::optFile()
{
    return optGetIntStr(mOPT, "OptFile");
}

void GAMSOptionsImpl::setOptFile(const int value)
{
    optSetIntStr(mOPT, "OptFile", value);
}

int GAMSOptionsImpl::fSave()
{
    return optGetIntStr(mOPT, "FSave");
}

void GAMSOptionsImpl::setFSave(const int value)
{
    optSetIntStr(mOPT, "FSave", value);
}

int GAMSOptionsImpl::dumpOpt()
{
    return optGetIntStr(mOPT, "DumpOpt");
}

void GAMSOptionsImpl::setDumpOpt(const int value)
{
    optSetIntStr(mOPT, "DumpOpt", value);
}

int GAMSOptionsImpl::errMsg()
{
    return optGetIntStr(mOPT, "ErrMsg");
}

void GAMSOptionsImpl::setErrMsg(const int value)
{
    optSetIntStr(mOPT, "ErrMsg", value);
}

int GAMSOptionsImpl::opt()
{
    return optGetIntStr(mOPT, "Opt");
}

void GAMSOptionsImpl::setOpt(const int value)
{
    optSetIntStr(mOPT, "Opt", value);
}

int GAMSOptionsImpl::relPath()
{
    return optGetIntStr(mOPT, "RelPath");
}

void GAMSOptionsImpl::setRelPath(const int value)
{
    optSetIntStr(mOPT, "RelPath", value);
}

int GAMSOptionsImpl::stringChk()
{
    return optGetIntStr(mOPT, "StringChk");
}

void GAMSOptionsImpl::setStringChk(const int value)
{
    optSetIntStr(mOPT, "StringChk", value);
}

std::string GAMSOptionsImpl::reference()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "Reference", buf);
}

void GAMSOptionsImpl::setReference(const std::string& value)
{
    optSetStrStr(mOPT, "Reference", value.c_str());
}

std::string GAMSOptionsImpl::optDir()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "OptDir", buf);
}

void GAMSOptionsImpl::setOptDir(const std::string& value)
{
    optSetStrStr(mOPT, "OptDir", value.c_str());
}

int GAMSOptionsImpl::execErr()
{
    return optGetIntStr(mOPT, "ExecErr");
}

void GAMSOptionsImpl::setExecErr(const int value)
{
    optSetIntStr(mOPT, "ExecErr", value);
}

int GAMSOptionsImpl::errorLog()
{
    return optGetIntStr(mOPT, "ErrorLog");
}

void GAMSOptionsImpl::setErrorLog(const int value)
{
    optSetIntStr(mOPT, "ErrorLog", value);
}

int GAMSOptionsImpl::limRow()
{
    return optGetIntStr(mOPT, "LimRow");
}

void GAMSOptionsImpl::setLimRow(const int value)
{
    optSetIntStr(mOPT, "LimRow", value);
}

int GAMSOptionsImpl::limCol()
{
    return optGetIntStr(mOPT, "LimCol");
}

void GAMSOptionsImpl::setLimCol(const int value)
{
    optSetIntStr(mOPT, "LimCol", value);
}

int GAMSOptionsImpl::charSet()
{
    return optGetIntStr(mOPT, "CharSet");
}

void GAMSOptionsImpl::setCharSet(const int value)
{
    optSetIntStr(mOPT, "CharSet", value);
}

int GAMSOptionsImpl::keep()
{
    return optGetIntStr(mOPT, "Keep");
}

void GAMSOptionsImpl::setKeep(const int value)
{
    optSetIntStr(mOPT, "Keep", value);
}

int GAMSOptionsImpl::iDE()
{
    return optGetIntStr(mOPT, "IDE");
}

void GAMSOptionsImpl::setIDE(const int value)
{
    optSetIntStr(mOPT, "IDE", value);
}

int GAMSOptionsImpl::traceOpt()
{
    return optGetIntStr(mOPT, "TraceOpt");
}

void GAMSOptionsImpl::setTraceOpt(const int value)
{
    optSetIntStr(mOPT, "TraceOpt", value);
}

int GAMSOptionsImpl::iterLim()
{
    return optGetIntStr(mOPT, "IterLim");
}

void GAMSOptionsImpl::setIterLim(const int value)
{
    optSetIntStr(mOPT, "IterLim", value);
}

int GAMSOptionsImpl::domLim()
{
    return optGetIntStr(mOPT, "DomLim");
}

void GAMSOptionsImpl::setDomLim(const int value)
{
    optSetIntStr(mOPT, "DomLim", value);
}

double GAMSOptionsImpl::resLim()
{
    return optGetDblStr(mOPT, "ResLim");
}

void GAMSOptionsImpl::setResLim(const double value)
{
    optSetDblStr(mOPT, "ResLim", value);
}

double GAMSOptionsImpl::optCR()
{
    return optGetDblStr(mOPT, "OptCR");
}

void GAMSOptionsImpl::setOptCR(const double value)
{
    optSetDblStr(mOPT, "OptCR", value);
}

double GAMSOptionsImpl::optCA()
{
    return optGetDblStr(mOPT, "OptCA");
}

void GAMSOptionsImpl::setOptCA(const double value)
{
    optSetDblStr(mOPT, "OptCA", value);
}

int GAMSOptionsImpl::sysOut()
{
    return optGetIntStr(mOPT, "SysOut");
}

void GAMSOptionsImpl::setSysOut(const int value)
{
    optSetIntStr(mOPT, "SysOut", value);
}

int GAMSOptionsImpl::solPrint()
{
    return optGetIntStr(mOPT, "SolPrint");
}

void GAMSOptionsImpl::setSolPrint(const int value)
{
    optSetIntStr(mOPT, "SolPrint", value);
}

double GAMSOptionsImpl::bratio()
{
    return optGetDblStr(mOPT, "Bratio");
}

void GAMSOptionsImpl::setBratio(const double value)
{
    optSetDblStr(mOPT, "Bratio", value);
}

int GAMSOptionsImpl::holdFixed()
{
    return optGetIntStr(mOPT, "HoldFixed");
}

void GAMSOptionsImpl::setHoldFixed(const int value)
{
    optSetIntStr(mOPT, "HoldFixed", value);
}

int GAMSOptionsImpl::nodLim()
{
    return optGetIntStr(mOPT, "NodLim");
}

void GAMSOptionsImpl::setNodLim(const int value)
{
    optSetIntStr(mOPT, "NodLim", value);
}

double GAMSOptionsImpl::workFactor()
{
    return optGetDblStr(mOPT, "WorkFactor");
}

void GAMSOptionsImpl::setWorkFactor(const double value)
{
    optSetDblStr(mOPT, "WorkFactor", value);
}

double GAMSOptionsImpl::workSpace()
{
    return optGetDblStr(mOPT, "WorkSpace");
}

void GAMSOptionsImpl::setWorkSpace(const double value)
{
    optSetDblStr(mOPT, "WorkSpace", value);
}

int GAMSOptionsImpl::forLim()
{
    return optGetIntStr(mOPT, "ForLim");
}

void GAMSOptionsImpl::setForLim(const int value)
{
    optSetIntStr(mOPT, "ForLim", value);
}

int GAMSOptionsImpl::seed()
{
    return optGetIntStr(mOPT, "Seed");
}

void GAMSOptionsImpl::setSeed(const int value)
{
    optSetIntStr(mOPT, "Seed", value);
}

int GAMSOptionsImpl::on115()
{
    return optGetIntStr(mOPT, "On115");
}

void GAMSOptionsImpl::setOn115(const int value)
{
    optSetIntStr(mOPT, "On115", value);
}

int GAMSOptionsImpl::execMode()
{
    return optGetIntStr(mOPT, "ExecMode");
}

void GAMSOptionsImpl::setExecMode(const int value)
{
    optSetIntStr(mOPT, "ExecMode", value);
}

int GAMSOptionsImpl::interactiveSolver()
{
    return optGetIntStr(mOPT, "InteractiveSolver");
}

void GAMSOptionsImpl::setInteractiveSolver(const int value)
{
    optSetIntStr(mOPT, "InteractiveSolver", value);
}

std::string GAMSOptionsImpl::pLicense()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "PLicense", buf);
}

void GAMSOptionsImpl::setPLicense(const std::string& value)
{
    optSetStrStr(mOPT, "PLicense", value.c_str());
}

std::string GAMSOptionsImpl::jobTrace()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "JobTrace", buf);
}

void GAMSOptionsImpl::setJobTrace(const std::string& value)
{
    optSetStrStr(mOPT, "JobTrace", value.c_str());
}

int GAMSOptionsImpl::warnings()
{
    return optGetIntStr(mOPT, "Warnings");
}

void GAMSOptionsImpl::setWarnings(const int value)
{
    optSetIntStr(mOPT, "Warnings", value);
}

int GAMSOptionsImpl::traceLevel()
{
    return optGetIntStr(mOPT, "TraceLevel");
}

void GAMSOptionsImpl::setTraceLevel(const int value)
{
    optSetIntStr(mOPT, "TraceLevel", value);
}

int GAMSOptionsImpl::timer()
{
    return optGetIntStr(mOPT, "Timer");
}

void GAMSOptionsImpl::setTimer(const int value)
{
    optSetIntStr(mOPT, "Timer", value);
}

int GAMSOptionsImpl::fileCase()
{
    return optGetIntStr(mOPT, "FileCase");
}

void GAMSOptionsImpl::setFileCase(const int value)
{
    optSetIntStr(mOPT, "FileCase", value);
}

int GAMSOptionsImpl::savePoint()
{
    return optGetIntStr(mOPT, "SavePoint");
}

void GAMSOptionsImpl::setSavePoint(const int value)
{
    optSetIntStr(mOPT, "SavePoint", value);
}

int GAMSOptionsImpl::solveLink()
{
    return optGetIntStr(mOPT, "SolveLink");
}

void GAMSOptionsImpl::setSolveLink(const int value)
{
    optSetIntStr(mOPT, "SolveLink", value);
}

int GAMSOptionsImpl::sys10()
{
    return optGetIntStr(mOPT, "Sys10");
}

void GAMSOptionsImpl::setSys10(const int value)
{
    optSetIntStr(mOPT, "Sys10", value);
}

int GAMSOptionsImpl::sys11()
{
    return optGetIntStr(mOPT, "Sys11");
}

void GAMSOptionsImpl::setSys11(const int value)
{
    optSetIntStr(mOPT, "Sys11", value);
}

int GAMSOptionsImpl::sys12()
{
    return optGetIntStr(mOPT, "Sys12");
}

void GAMSOptionsImpl::setSys12(const int value)
{
    optSetIntStr(mOPT, "Sys12", value);
}

double GAMSOptionsImpl::zeroRes()
{
    return optGetDblStr(mOPT, "ZeroRes");
}

void GAMSOptionsImpl::setZeroRes(const double value)
{
    optSetDblStr(mOPT, "ZeroRes", value);
}

int GAMSOptionsImpl::zeroResRep()
{
    return optGetIntStr(mOPT, "ZeroResRep");
}

void GAMSOptionsImpl::setZeroResRep(const int value)
{
    optSetIntStr(mOPT, "ZeroResRep", value);
}

double GAMSOptionsImpl::heapLimit()
{
    return optGetDblStr(mOPT, "HeapLimit");
}

void GAMSOptionsImpl::setHeapLimit(const double value)
{
    optSetDblStr(mOPT, "HeapLimit", value);
}

std::string GAMSOptionsImpl::scrExt()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "ScrExt", buf);
}

void GAMSOptionsImpl::setScrExt(const std::string& value)
{
    optSetStrStr(mOPT, "ScrExt", value.c_str());
}

std::string GAMSOptionsImpl::procDir()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "ProcDir", buf);
}

void GAMSOptionsImpl::setProcDir(const std::string& value)
{
    optSetStrStr(mOPT, "ProcDir", value.c_str());
}

int GAMSOptionsImpl::gdxCompress()
{
    return optGetIntStr(mOPT, "gdxCompress");
}

void GAMSOptionsImpl::setgdxCompress(const int value)
{
    optSetIntStr(mOPT, "gdxCompress", value);
}

std::string GAMSOptionsImpl::gdxConvert()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "gdxConvert", buf);
}

void GAMSOptionsImpl::setgdxConvert(const std::string& value)
{
    optSetStrStr(mOPT, "gdxConvert", value.c_str());
}

int GAMSOptionsImpl::appendExpand()
{
    return optGetIntStr(mOPT, "AppendExpand");
}

void GAMSOptionsImpl::setAppendExpand(const int value)
{
    optSetIntStr(mOPT, "AppendExpand", value);
}

double GAMSOptionsImpl::eTLim()
{
    return optGetDblStr(mOPT, "ETLim");
}

void GAMSOptionsImpl::setETLim(const double value)
{
    optSetDblStr(mOPT, "ETLim", value);
}

double GAMSOptionsImpl::profileTol()
{
    return optGetDblStr(mOPT, "ProfileTol");
}

void GAMSOptionsImpl::setProfileTol(const double value)
{
    optSetDblStr(mOPT, "ProfileTol", value);
}

std::string GAMSOptionsImpl::profileFile()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "ProfileFile", buf);
}

void GAMSOptionsImpl::setProfileFile(const std::string& value)
{
    optSetStrStr(mOPT, "ProfileFile", value.c_str());
}

int GAMSOptionsImpl::maxProcDir()
{
    return optGetIntStr(mOPT, "MaxProcDir");
}

void GAMSOptionsImpl::setMaxProcDir(const int value)
{
    optSetIntStr(mOPT, "MaxProcDir", value);
}

int GAMSOptionsImpl::integer1()
{
    return optGetIntStr(mOPT, "Integer1");
}

void GAMSOptionsImpl::setInteger1(const int value)
{
    optSetIntStr(mOPT, "Integer1", value);
}

int GAMSOptionsImpl::integer2()
{
    return optGetIntStr(mOPT, "Integer2");
}

void GAMSOptionsImpl::setInteger2(const int value)
{
    optSetIntStr(mOPT, "Integer2", value);
}

int GAMSOptionsImpl::integer3()
{
    return optGetIntStr(mOPT, "Integer3");
}

void GAMSOptionsImpl::setInteger3(const int value)
{
    optSetIntStr(mOPT, "Integer3", value);
}

int GAMSOptionsImpl::integer4()
{
    return optGetIntStr(mOPT, "Integer4");
}

void GAMSOptionsImpl::setInteger4(const int value)
{
    optSetIntStr(mOPT, "Integer4", value);
}

int GAMSOptionsImpl::integer5()
{
    return optGetIntStr(mOPT, "Integer5");
}

void GAMSOptionsImpl::setInteger5(const int value)
{
    optSetIntStr(mOPT, "Integer5", value);
}

std::string GAMSOptionsImpl::gridScript()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "GridScript", buf);
}

void GAMSOptionsImpl::setGridScript(const std::string& value)
{
    optSetStrStr(mOPT, "GridScript", value.c_str());
}

int GAMSOptionsImpl::threads()
{
    return optGetIntStr(mOPT, "Threads");
}

void GAMSOptionsImpl::setThreads(const int value)
{
    optSetIntStr(mOPT, "Threads", value);
}

int GAMSOptionsImpl::useDLL()
{
    return optGetIntStr(mOPT, "UseDLL");
}

void GAMSOptionsImpl::setUseDLL(const int value)
{
    optSetIntStr(mOPT, "UseDLL", value);
}

int GAMSOptionsImpl::debug()
{
    return optGetIntStr(mOPT, "Debug");
}

void GAMSOptionsImpl::setDebug(const int value)
{
    optSetIntStr(mOPT, "Debug", value);
}

int GAMSOptionsImpl::pID2Error()
{
    return optGetIntStr(mOPT, "PID2Error");
}

void GAMSOptionsImpl::setPID2Error(const int value)
{
    optSetIntStr(mOPT, "PID2Error", value);
}

int GAMSOptionsImpl::noNewVarEqu()
{
    return optGetIntStr(mOPT, "NoNewVarEqu");
}

void GAMSOptionsImpl::setNoNewVarEqu(const int value)
{
    optSetIntStr(mOPT, "NoNewVarEqu", value);
}

int GAMSOptionsImpl::forceOptFile()
{
    return optGetIntStr(mOPT, "ForceOptFile");
}

void GAMSOptionsImpl::setForceOptFile(const int value)
{
    optSetIntStr(mOPT, "ForceOptFile", value);
}

std::string GAMSOptionsImpl::symPrefix()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SymPrefix", buf);
}

void GAMSOptionsImpl::setSymPrefix(const std::string& value)
{
    optSetStrStr(mOPT, "SymPrefix", value.c_str());
}

int GAMSOptionsImpl::serverRun()
{
    return optGetIntStr(mOPT, "ServerRun");
}

void GAMSOptionsImpl::setServerRun(const int value)
{
    optSetIntStr(mOPT, "ServerRun", value);
}

std::string GAMSOptionsImpl::gdxUels()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "gdxUels", buf);
}

void GAMSOptionsImpl::setgdxUels(const std::string& value)
{
    optSetStrStr(mOPT, "gdxUels", value.c_str());
}

std::string GAMSOptionsImpl::dumpParmsLogPrefix()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "DumpParmsLogPrefix", buf);
}

void GAMSOptionsImpl::setDumpParmsLogPrefix(const std::string& value)
{
    optSetStrStr(mOPT, "DumpParmsLogPrefix", value.c_str());
}

int GAMSOptionsImpl::strictSingleton()
{
    return optGetIntStr(mOPT, "strictSingleton");
}

void GAMSOptionsImpl::setstrictSingleton(const int value)
{
    optSetIntStr(mOPT, "strictSingleton", value);
}

std::string GAMSOptionsImpl::saveObfuscate()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SaveObfuscate", buf);
}

void GAMSOptionsImpl::setSaveObfuscate(const std::string& value)
{
    optSetStrStr(mOPT, "SaveObfuscate", value.c_str());
}

std::string GAMSOptionsImpl::xSaveObfuscate()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "XSaveObfuscate", buf);
}

void GAMSOptionsImpl::setXSaveObfuscate(const std::string& value)
{
    optSetStrStr(mOPT, "XSaveObfuscate", value.c_str());
}

std::string GAMSOptionsImpl::restartNamed()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "RestartNamed", buf);
}

void GAMSOptionsImpl::setRestartNamed(const std::string& value)
{
    optSetStrStr(mOPT, "RestartNamed", value.c_str());
}

int GAMSOptionsImpl::sys14()
{
    return optGetIntStr(mOPT, "Sys14");
}

void GAMSOptionsImpl::setSys14(const int value)
{
    optSetIntStr(mOPT, "Sys14", value);
}

double GAMSOptionsImpl::fDDelta()
{
    return optGetDblStr(mOPT, "FDDelta");
}

void GAMSOptionsImpl::setFDDelta(const double value)
{
    optSetDblStr(mOPT, "FDDelta", value);
}

int GAMSOptionsImpl::fDOpt()
{
    return optGetIntStr(mOPT, "FDOpt");
}

void GAMSOptionsImpl::setFDOpt(const int value)
{
    optSetIntStr(mOPT, "FDOpt", value);
}

std::string GAMSOptionsImpl::solver()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "Solver", buf);
}

void GAMSOptionsImpl::setSolver(const std::string& value)
{
    optSetStrStr(mOPT, "Solver", value.c_str());
}

std::string GAMSOptionsImpl::docFile()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "DocFile", buf);
}

void GAMSOptionsImpl::setDocFile(const std::string& value)
{
    optSetStrStr(mOPT, "DocFile", value.c_str());
}

std::string GAMSOptionsImpl::sparseRun()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "SparseRun", buf);
}

void GAMSOptionsImpl::setSparseRun(const std::string& value)
{
    optSetStrStr(mOPT, "SparseRun", value.c_str());
}

int GAMSOptionsImpl::sys15()
{
    return optGetIntStr(mOPT, "Sys15");
}

void GAMSOptionsImpl::setSys15(const int value)
{
    optSetIntStr(mOPT, "Sys15", value);
}

int GAMSOptionsImpl::sys16()
{
    return optGetIntStr(mOPT, "Sys16");
}

void GAMSOptionsImpl::setSys16(const int value)
{
    optSetIntStr(mOPT, "Sys16", value);
}

int GAMSOptionsImpl::sys17()
{
    return optGetIntStr(mOPT, "Sys17");
}

void GAMSOptionsImpl::setSys17(const int value)
{
    optSetIntStr(mOPT, "Sys17", value);
}

int GAMSOptionsImpl::prefixLoadPath()
{
    return optGetIntStr(mOPT, "PrefixLoadPath");
}

void GAMSOptionsImpl::setPrefixLoadPath(const int value)
{
    optSetIntStr(mOPT, "PrefixLoadPath", value);
}

int GAMSOptionsImpl::threadsAsync()
{
    return optGetIntStr(mOPT, "ThreadsAsync");
}

void GAMSOptionsImpl::setThreadsAsync(const int value)
{
    optSetIntStr(mOPT, "ThreadsAsync", value);
}

int GAMSOptionsImpl::mCPRHoldfx()
{
    return optGetIntStr(mOPT, "MCPRHoldfx");
}

void GAMSOptionsImpl::setMCPRHoldfx(const int value)
{
    optSetIntStr(mOPT, "MCPRHoldfx", value);
}

int GAMSOptionsImpl::asyncSolLst()
{
    return optGetIntStr(mOPT, "AsyncSolLst");
}

void GAMSOptionsImpl::setAsyncSolLst(const int value)
{
    optSetIntStr(mOPT, "AsyncSolLst", value);
}

std::string GAMSOptionsImpl::fileStem()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "FileStem", buf);
}

void GAMSOptionsImpl::setFileStem(const std::string& value)
{
    optSetStrStr(mOPT, "FileStem", value.c_str());
}

std::string GAMSOptionsImpl::procDirPath()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "ProcDirPath", buf);
}

void GAMSOptionsImpl::setProcDirPath(const std::string& value)
{
    optSetStrStr(mOPT, "ProcDirPath", value.c_str());
}

int GAMSOptionsImpl::pySetup()
{
    return optGetIntStr(mOPT, "PySetup");
}

void GAMSOptionsImpl::setPySetup(const int value)
{
    optSetIntStr(mOPT, "PySetup", value);
}

int GAMSOptionsImpl::pyMultInst()
{
    return optGetIntStr(mOPT, "PyMultInst");
}

void GAMSOptionsImpl::setPyMultInst(const int value)
{
    optSetIntStr(mOPT, "PyMultInst", value);
}

std::string GAMSOptionsImpl::LP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "LP", buf);
}

void GAMSOptionsImpl::setLP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_lp))
        optSetStrStr(mOPT, "LP", value.c_str());
}

std::string GAMSOptionsImpl::MIP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "MIP", buf);
}

void GAMSOptionsImpl::setMIP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_mip))
        optSetStrStr(mOPT, "MIP", value.c_str());
}

std::string GAMSOptionsImpl::RMIP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "RMIP", buf);
}

void GAMSOptionsImpl::setRMIP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_rmip))
        optSetStrStr(mOPT, "RMIP", value.c_str());
}

std::string GAMSOptionsImpl::NLP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "NLP", buf);
}

void GAMSOptionsImpl::setNLP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_nlp))
        optSetStrStr(mOPT, "NLP", value.c_str());
}

std::string GAMSOptionsImpl::MCP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "MCP", buf);
}

void GAMSOptionsImpl::setMCP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_mcp))
        optSetStrStr(mOPT, "MCP", value.c_str());
}

std::string GAMSOptionsImpl::MPEC()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "MPEC", buf);
}

void GAMSOptionsImpl::setMPEC(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_mpec))
        optSetStrStr(mOPT, "MPEC", value.c_str());
}

std::string GAMSOptionsImpl::RMPEC()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "RMPEC", buf);
}

void GAMSOptionsImpl::setRMPEC(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_rmpec))
        optSetStrStr(mOPT, "RMPEC", value.c_str());
}

std::string GAMSOptionsImpl::CNS()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "CNS", buf);
}

void GAMSOptionsImpl::setCNS(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_cns))
        optSetStrStr(mOPT, "CNS", value.c_str());
}

std::string GAMSOptionsImpl::DNLP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "DNLP", buf);
}

void GAMSOptionsImpl::setDNLP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_dnlp))
        optSetStrStr(mOPT, "DNLP", value.c_str());
}

std::string GAMSOptionsImpl::RMINLP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "RMINLP", buf);
}

void GAMSOptionsImpl::setRMINLP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_rminlp))
        optSetStrStr(mOPT, "RMINLP", value.c_str());
}

std::string GAMSOptionsImpl::MINLP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "MINLP", buf);
}

void GAMSOptionsImpl::setMINLP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_minlp))
        optSetStrStr(mOPT, "MINLP", value.c_str());
}

std::string GAMSOptionsImpl::QCP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "QCP", buf);
}

void GAMSOptionsImpl::setQCP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_qcp))
        optSetStrStr(mOPT, "QCP", value.c_str());
}

std::string GAMSOptionsImpl::MIQCP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "MIQCP", buf);
}

void GAMSOptionsImpl::setMIQCP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_miqcp))
        optSetStrStr(mOPT, "MIQCP", value.c_str());
}

std::string GAMSOptionsImpl::RMIQCP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "RMIQCP", buf);
}

void GAMSOptionsImpl::setRMIQCP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_rmiqcp))
        optSetStrStr(mOPT, "RMIQCP", value.c_str());
}

std::string GAMSOptionsImpl::EMP()
{
    char buf[GMS_SSSIZE];
    return optGetStrStr(mOPT, "EMP", buf);
}

void GAMSOptionsImpl::setEMP(const std::string& value)
{
    if (0 == cfgAlgNumber(mCFG, value.c_str()))
        throw GAMSException("Unknown solver " + value);
    if (cfgAlgCapability(mCFG, cfgAlgNumber(mCFG, value.c_str()), gmoProc_emp))
        optSetStrStr(mOPT, "EMP", value.c_str());
}

string GAMSOptionsImpl::getSolver(const string& modelType)
{
    char buf[GMS_SSSIZE];
    std::string up = modelType;
    std::transform(up.begin(), up.end(), up.begin(), ::toupper);
    int procSelect = cfgModelTypeNumber (this->mCFG, modelType.c_str());
    if (!procSelect)
        throw GAMSException("Unknown model type " + modelType);

    optGetStrStr(mOPT, modelType.c_str(), buf);
    return buf;
}

void GAMSOptionsImpl::setSolver(const string& modelType, const string& solver)
{
    std::string up = modelType;
    std::transform(up.begin(), up.end(), up.begin(), ::toupper);
    int procSelect = cfgModelTypeNumber (this->mCFG, modelType.c_str());
    if (!procSelect)
        throw GAMSException("Unknown model type " + modelType);
    if (0 == cfgAlgNumber(mCFG, solver.c_str()))
        throw GAMSException("Unknown solver " + solver);
    optSetStrStr(mOPT, modelType.c_str(), solver.c_str());
}

void GAMSOptionsImpl::setupOptionsBase()
{
    char msg[GMS_SSSIZE];
    optCreateD(&mOPT, mWs.systemDirectory().c_str(), msg, sizeof(msg));
    if (msg[0] != '\0')
        throw GAMSException(msg);

    // TODO: check if sys/workDir are always absolute paths
    GAMSPath defFile = GAMSPath(mWs.systemDirectory()) / "optgams.def";

    // TODO: lock(optLock); (c++11 added support for lock, find alternative for older versions)
    if (optReadDefinition(mOPT, defFile.c_str())) {
        for (int i = 1, itype=0; i <= optMessageCount(mOPT); i++) {
            optGetMessage(mOPT, i, msg, &itype);
            // TODO(AF) log mMsg!
        }
        throw GAMSException("Problem reading definition file " + defFile.toStdString());
    }

    cfgCreateD(&mCFG, mWs.systemDirectory().c_str(), msg, sizeof(msg));
    if (msg[0] != '\0')
        throw GAMSException(msg);

    GAMSPath cmpFileFullPath = GAMSPath(mWs.systemDirectory()) / cCmpFileName;
    if (cfgReadConfig(mCFG, cmpFileFullPath.c_str()) != 0) {
        throw GAMSException("Error reading config file " + cmpFileFullPath.toStdString());
    }
}

void GAMSOptionsImpl::setDefaultSolver()
{
    char buf[GMS_SSSIZE], buf2[GMS_SSSIZE];
    for (int i = 1; i < gmoProc_nrofmodeltypes; i++)
        optSetStrStr(mOPT, cfgModelTypeName(mCFG, i, buf), cfgAlgName(mCFG, cfgDefaultAlg(mCFG, i), buf2));
}
} // namespace gams
