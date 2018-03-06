/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
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


#ifndef GAMSOPTIONSIMPL_H
#define GAMSOPTIONSIMPL_H
#include <map>
#include <vector>
#include <string>
#include <tuple>
#include <memory>
#include "optcc.h"
#include "cfgmcc.h"
#include "gclgms.h"
#include "gamsworkspace.h"

namespace gams {

class GAMSOptionsImpl
{
public:
    GAMSOptionsImpl(GAMSWorkspace &workspace, GAMSOptionsImpl* optFrom);
    GAMSOptionsImpl(GAMSWorkspace &workspace, const std::string& optFile = "");

    void setupOptionsBase();
    void setDefaultSolver();

    void writeOptionFile(const std::string& optFileName);

    void setAllModelTypes(const std::string& solver);

    int inputDirSize();
    void setInputDir(const std::string value, const int index = -1);
    std::string getInputDir(const int index);
    void removeInputDir(const int index);

    void setDefine(const std::string &key, const std::string &value);
    std::string getDefine(const std::string& key);
    bool removeDefine(const std::string& key);

    std::string gdx();
    void setGdx(std::string value);

    std::string restart();
    void setRestart(const std::string& value);

    std::string input();
    void setInput(const std::string& value);

    std::string output();
    void setOutput(const std::string& value);

    std::string save();
    void setSave(const std::string& value);

    std::string symbol();
    void setSymbol(const std::string& value);

    std::string xSave();
    void setXSave(const std::string& value);

    std::string expand();
    void setExpand(const std::string& value);

    int pageWidth();
    void setPageWidth(const int value);

    int pageSize();
    void setPageSize(const int value);

    int pageContr();
    void setPageContr(const int value);

    std::string action();
    void setAction(const std::string& value);

    std::string subSys();
    void setSubSys(const std::string& value);

    std::string errNam();
    void setErrNam(const std::string& value);

    std::string scrDir();
    void setScrDir(const std::string& value);

    std::string scrNam();
    void setScrNam(const std::string& value);

    std::string sysDir();
    void setSysDir(const std::string& value);

    std::string curDir();
    void setCurDir(const std::string& value);

    std::string workDir();
    void setWorkDir(const std::string& value);

    std::string putDir();
    void setPutDir(const std::string& value);

    std::string gridDir();
    void setGridDir(const std::string& value);

    std::string scriptNext();
    void setScriptNext(const std::string& value);

    std::string scriptExit();
    void setScriptExit(const std::string& value);

    std::string scriptFrst();
    void setScriptFrst(const std::string& value);

    int logLine();
    void setLogLine(const int value);

    int logOption();
    void setLogOption(const int value);

    std::string logFile();
    void setLogFile(const std::string& value);

    std::string trace();
    void setTrace(const std::string& value);

    int appendOut();
    void setAppendOut(const int value);

    int appendLog();
    void setAppendLog(const int value);

    int stepSum();
    void setStepSum(const int value);

    int multiPass();
    void setMultiPass(const int value);

    int suppress();
    void setSuppress(const int value);

    int dFormat();
    void setDFormat(const int value);

    int tFormat();
    void setTFormat(const int value);

    int tabIn();
    void setTabIn(const int value);

    std::string solverCntr();
    void setSolverCntr(const std::string& value);

    std::string solverMatr();
    void setSolverMatr(const std::string& value);

    std::string solverInst();
    void setSolverInst(const std::string& value);

    std::string solverStat();
    void setSolverStat(const std::string& value);

    std::string solverSolu();
    void setSolverSolu(const std::string& value);

    std::string solverDict();
    void setSolverDict(const std::string& value);

    int intVarUp();
    void setIntVarUp(const int value);

    int lstCase();

    void setLstCase(const int value);

    int g205();
    void setG205(const int value);

    std::string license();
    void setLicense(const std::string& value);

    int dumpParms();
    void setDumpParms(const int value);

    int noCr();
    void setNoCr(const int value);

    int profile();
    void setProfile(const int value);

    std::string libIncDir();
    void setLibIncDir(const std::string& value);

    std::string sysIncDir();
    void setSysIncDir(const std::string& value);

    std::string user1();
    void setUser1(const std::string& value);

    std::string user2();
    void setUser2(const std::string& value);

    std::string user3();
    void setUser3(const std::string& value);

    std::string user4();
    void setUser4(const std::string& value);

    std::string user5();
    void setUser5(const std::string& value);

    int forceWork();
    void setForceWork(const int value);

    int cErr();
    void setCErr(const int value);

    std::string fErr();
    void setFErr(const std::string& value);

    int optFile();
    void setOptFile(const int value);

    int fSave();
    void setFSave(const int value);

    int dumpOpt();
    void setDumpOpt(const int value);

    int errMsg();
    void setErrMsg(const int value);

    int opt();
    void setOpt(const int value);

    int relPath();
    void setRelPath(const int value);

    int stringChk();
    void setStringChk(const int value);

    std::string reference();
    void setReference(const std::string& value);

    std::string optDir();
    void setOptDir(const std::string& value);

    int execErr();
    void setExecErr(const int value);

    int errorLog();
    void setErrorLog(const int value);

    int limRow();
    void setLimRow(const int value);

    int limCol();
    void setLimCol(const int value);

    int charSet();
    void setCharSet(const int value);

    int keep();
    void setKeep(const int value);

    int iDE();
    void setIDE(const int value);

    int traceOpt();
    void setTraceOpt(const int value);

    int iterLim();
    void setIterLim(const int value);

    int domLim();
    void setDomLim(const int value);

    double resLim();
    void setResLim(const double value);

    double optCR();
    void setOptCR(const double value);

    double optCA();
    void setOptCA(const double value);

    int sysOut();
    void setSysOut(const int value);

    int solPrint();
    void setSolPrint(const int value);

    double bratio();
    void setBratio(const double value);

    int holdFixed();
    void setHoldFixed(const int value);

    int nodLim();
    void setNodLim(const int value);

    double workFactor();
    void setWorkFactor(const double value);

    double workSpace();
    void setWorkSpace(const double value);

    int forLim();
    void setForLim(const int value);

    int seed();
    void setSeed(const int value);

    int on115();
    void setOn115(const int value);

    int execMode();
    void setExecMode(const int value);

    int interactiveSolver();
    void setInteractiveSolver(const int value);

    std::string pLicense();
    void setPLicense(const std::string& value);

    std::string jobTrace();
    void setJobTrace(const std::string& value);

    int warnings();
    void setWarnings(const int value);

    int traceLevel();
    void setTraceLevel(const int value);

    int timer();
    void setTimer(const int value);

    int fileCase();
    void setFileCase(const int value);

    int savePoint();
    void setSavePoint(const int value);

    int solveLink();
    void setSolveLink(const int value);

    int sys10();
    void setSys10(const int value);

    int sys11();
    void setSys11(const int value);

    int sys12();
    void setSys12(const int value);

    double zeroRes();
    void setZeroRes(const double value);

    int zeroResRep();
    void setZeroResRep(const int value);

    double heapLimit();
    void setHeapLimit(const double value);

    std::string scrExt();
    void setScrExt(const std::string& value);

    std::string procDir();
    void setProcDir(const std::string& value);

    int gdxCompress();
    void setgdxCompress(const int value);

    std::string gdxConvert();
    void setgdxConvert(const std::string& value);

    int appendExpand();
    void setAppendExpand(const int value);

    double eTLim();
    void setETLim(const double value);

    double profileTol();
    void setProfileTol(const double value);

    std::string profileFile();
    void setProfileFile(const std::string& value);

    int maxProcDir();
    void setMaxProcDir(const int value);

    int integer1();
    void setInteger1(const int value);

    int integer2();
    void setInteger2(const int value);

    int integer3();
    void setInteger3(const int value);

    int integer4();
    void setInteger4(const int value);

    int integer5();
    void setInteger5(const int value);

    std::string gridScript();
    void setGridScript(const std::string& value);

    int threads();
    void setThreads(const int value);

    int useDLL();
    void setUseDLL(const int value);

    int debug();
    void setDebug(const int value);

    int pID2Error();
    void setPID2Error(const int value);

    int noNewVarEqu();
    void setNoNewVarEqu(const int value);

    int forceOptFile();
    void setForceOptFile(const int value);

    std::string symPrefix();
    void setSymPrefix(const std::string& value);

    int serverRun();
    void setServerRun(const int value);

    std::string gdxUels();
    void setgdxUels(const std::string& value);

    std::string dumpParmsLogPrefix();
    void setDumpParmsLogPrefix(const std::string& value);

    int strictSingleton();
    void setstrictSingleton(const int value);

    std::string saveObfuscate();
    void setSaveObfuscate(const std::string& value);

    std::string xSaveObfuscate();
    void setXSaveObfuscate(const std::string& value);

    std::string restartNamed();
    void setRestartNamed(const std::string& value);

    int sys14();
    void setSys14(const int value);

    double fDDelta();
    void setFDDelta(const double value);

    int fDOpt();
    void setFDOpt(const int value);

    std::string solver();
    void setSolver(const std::string& value);

    std::string docFile();
    void setDocFile(const std::string& value);

    std::string sparseRun();
    void setSparseRun(const std::string& value);

    int sys15();
    void setSys15(const int value);

    int sys16();
    void setSys16(const int value);

    int sys17();
    void setSys17(const int value);

    int prefixLoadPath();
    void setPrefixLoadPath(const int value);

    int threadsAsync();
    void setThreadsAsync(const int value);

    int mCPRHoldfx();
    void setMCPRHoldfx(const int value);

    int asyncSolLst();
    void setAsyncSolLst(const int value);

    std::string fileStem();
    void setFileStem(const std::string& value);

    std::string procDirPath();
    void setProcDirPath(const std::string& value);

    int pySetup();
    void setPySetup(const int value);

    int pyMultInst();
    void setPyMultInst(const int value);

    std::string fileStemApFromEnv();
    void setFileStemApFromEnv(const std::string& value);

    std::string LP();
    void setLP(const std::string& value);

    std::string MIP();
    void setMIP(const std::string& value);

    std::string RMIP();
    void setRMIP(const std::string& value);

    std::string NLP();
    void setNLP(const std::string& value);

    std::string MCP();
    void setMCP(const std::string& value);

    std::string MPEC();
    void setMPEC(const std::string& value);

    std::string RMPEC();
    void setRMPEC(const std::string& value);

    std::string CNS();
    void setCNS(const std::string& value);

    std::string DNLP();
    void setDNLP(const std::string& value);

    std::string RMINLP();
    void setRMINLP(const std::string& value);

    std::string MINLP();
    void setMINLP(const std::string& value);

    std::string QCP();
    void setQCP(const std::string& value);

    std::string MIQCP();
    void setMIQCP(const std::string& value);

    std::string RMIQCP();
    void setRMIQCP(const std::string& value);

    std::string EMP();
    void setEMP(const std::string& value);

    std::string getSolver(const std::string& modelType);
    void setSolver(const std::string& modelType, const std::string& solver);

public:
    static const char* cGamsBuild;
    static const char* cGamsVersion;
    int mIDirSize = 0;
    optHandle_t mOPT;
    cfgHandle_t mCFG;
    GAMSWorkspace mWs;

};
} // namespace gams
#endif // GAMSOPTIONSIMPL_H
