/**
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
 */

#ifndef TESTGAMSOPTIONS_H
#define TESTGAMSOPTIONS_H

#include "testgamsobject.h"

class TestGAMSOptions: public TestGAMSObject
{
  Q_OBJECT
  private slots:
    void testDestructor();
    void testIDir();
    void testDefines();
    void testSetAllModelTypes();
    void testGetGdx();
    void testSetGdx();
    void testGetOutput();
    void testSetOutput();
    void testGetSymbol();
    void testSetSymbol();
    void testGetExpand();
    void testSetExpand();
    void testGetPageWidth();
    void testSetPageWidth();
    void testGetPageSize();
    void testSetPageSize();
    void testGetPageContr();
    void testSetPageContr();
    void testGetAction();
    void testSetAction();
    void testGetPutDir();
    void testSetPutDir();
    void testGetGridDir();
    void testSetGridDir();
    void testGetScriptExit();
    void testSetScriptExit();
    void testGetLogLine();
    void testSetLogLine();
    void testGetTrace();
    void testSetTrace();
    void testGetAppendOut();
    void testSetAppendOut();
    void testGetStepSum();
    void testSetStepSum();
    void testGetSuppress();
    void testSetSuppress();
    void testGetDFormat();
    void testSetDFormat();
    void testGetTFormat();
    void testSetTFormat();
    void testGetTabIn();
    void testSetTabIn();
    void testGetIntVarUp();
    void testSetIntVarUp();
    void testGetCase();
    void testSetCase();
    void testGetLicense();
    void testSetLicense();
    void testGetDumpParms();
    void testSetDumpParms();
    void testGetProfile();
    void testSetProfile();
    void testGetLibIncDir();
    void testSetLibIncDir();
    void testGetSysIncDir();
    void testSetSysIncDir();
    void testGetUser1();
    void testSetUser1();
    void testGetUser2();
    void testSetUser2();
    void testGetUser3();
    void testSetUser3();
    void testGetUser4();
    void testSetUser4();
    void testGetUser5();
    void testSetUser5();
    void testGetForceWork();
    void testSetForceWork();
    void testGetCErr();
    void testSetCErr();
    void testGetFErr();
    void testSetFErr();
    void testGetOptFile();
    void testSetOptFile();
    void testGetDumpOpt();
    void testSetDumpOpt();
    void testGetErrMsg();
    void testSetErrMsg();
    void testGetOpt();
    void testSetOpt();
    void testGetStringChk();
    void testSetStringChk();
    void testGetReference();
    void testSetReference();
    void testGetOptDir();
    void testSetOptDir();
    void testGetExecErr();
    void testSetExecErr();
    void testGetErrorLog();
    void testSetErrorLog();
    void testGetLimRow();
    void testSetLimRow();
    void testGetLimCol();
    void testSetLimCol();
    void testGetCharSet();
    void testSetCharSet();
    void testGetKeep();
    void testSetKeep();
    void testGetTraceOpt();
    void testSetTraceOpt();
    void testGetIterLim();
    void testSetIterLim();
    void testGetDomLim();
    void testSetDomLim();
    void testGetResLim();
    void testSetResLim();
    void testGetOptCR();
    void testSetOptCR();
    void testGetOptCA();
    void testSetOptCA();
    void testGetSysOut();
    void testSetSysOut();
    void testGetSolPrint();
    void testSetSolPrint();
    void testGetBratio();
    void testSetBratio();
    void testGetHoldFixed();
    void testSetHoldFixed();
    void testGetNodLim();
    void testSetNodLim();
    void testGetWorkFactor();
    void testSetWorkFactor();
    void testGetWorkSpace();
    void testSetWorkSpace();
    void testGetForLim();
    void testSetForLim();
    void testGetSeed();
    void testSetSeed();
    void testGetOn115();
    void testSetOn115();
    void testGetExecMode();
    void testSetExecMode();
    void testGetInteractiveSolver();
    void testSetInteractiveSolver();
    void testGetPLicense();
    void testSetPLicense();
    void testGetJobTrace();
    void testSetJobTrace();
    void testGetWarnings();
    void testSetWarnings();
    void testGetTraceLevel();
    void testSetTraceLevel();
    void testGetTimer();
    void testSetTimer();
    void testGetFileCase();
    void testSetFileCase();
    void testGetSavePoint();
    void testSetSavePoint();
    void testGetSolveLink();
    void testSetSolveLink();
    void testGetSys10();
    void testSetSys10();
    void testGetSys11();
    void testSetSys11();
    void testGetSys12();
    void testSetSys12();
    void testGetZeroRes();
    void testSetZeroRes();
    void testGetZeroResRep();
    void testSetZeroResRep();
    void testGetHeapLimit();
    void testSetHeapLimit();
    void testGetgdxCompress();
    void testSetgdxCompress();
    void testGetgdxConvert();
    void testSetgdxConvert();
    void testGetAppendExpand();
    void testSetAppendExpand();
    void testGetETLim();
    void testSetETLim();
    void testGetProfileTol();
    void testSetProfileTol();
    void testGetProfileFile();
    void testSetProfileFile();
    void testGetMaxProcDir();
    void testSetMaxProcDir();
    void testGetInteger1();
    void testSetInteger1();
    void testGetInteger2();
    void testSetInteger2();
    void testGetInteger3();
    void testSetInteger3();
    void testGetInteger4();
    void testSetInteger4();
    void testGetInteger5();
    void testSetInteger5();
    void testGetGridScript();
    void testSetGridScript();
    void testGetThreads();
    void testSetThreads();
    void testGetNoNewVarEqu();
    void testSetNoNewVarEqu();
    void testGetForceOptFile();
    void testSetForceOptFile();
    void testGetSymPrefix();
    void testSetSymPrefix();
    void testGetgdxUels();
    void testSetgdxUels();
    void testGetDumpParmsLogPrefix();
    void testSetDumpParmsLogPrefix();
    void testGetstrictSingleton();
    void testSetstrictSingleton();
    void testGetFDDelta();
    void testSetFDDelta();
    void testGetFDOpt();
    void testSetFDOpt();
    void testGetPrefixLoadPath();
    void testSetPrefixLoadPath();
    void testGetThreadsAsync();
    void testSetThreadsAsync();
    void testGetAsyncSolLst();
    void testSetAsyncSolLst();
    void testGetFileStem();
    void testSetFileStem();
    void testGetLP();
    void testSetLP();
    void testGetMIP();
    void testSetMIP();
    void testGetRMIP();
    void testSetRMIP();
    void testGetNLP();
    void testSetNLP();
    void testGetMCP();
    void testSetMCP();
    void testGetMPEC();
    void testSetMPEC();
    void testGetRMPEC();
    void testSetRMPEC();
    void testGetCNS();
    void testSetCNS();
    void testGetDNLP();
    void testSetDNLP();
    void testGetRMINLP();
    void testSetRMINLP();
    void testGetMINLP();
    void testSetMINLP();
    void testGetQCP();
    void testSetQCP();
    void testGetMIQCP();
    void testSetMIQCP();
    void testGetRMIQCP();
    void testSetRMIQCP();
    void testGetEMP();
    void testSetEMP();
  protected:
    QString classname();
};
#endif // TESTGAMSOPTIONS_H
