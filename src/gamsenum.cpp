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

#include "gamsenum.h"
#include "gamsexception.h"

namespace gams {

std::string GAMSEnum::text(SymbolType type)
{
    const char * res[] = {"GAMSEnum::SymTypeSet","GAMSEnum::SymTypePar","GAMSEnum::SymTypeVar","GAMSEnum::SymTypeEqu"};
    return res[type];
}

std::string GAMSEnum::text(GAMSEnum::EquType type)
{
    const char * res[] = {"GAMSEnum::E","GAMSEnum::G","GAMSEnum::L","GAMSEnum::N","GAMSEnum::X","GAMSEnum::C"};
    return res[type];
}

std::string GAMSEnum::text(GAMSEnum::SetType type)
{
    const char * res[] = {"Multi", "Singleton"};
    return res[type];
}

std::string GAMSEnum::text(GAMSEnum::VarType type)
{
    const char * res[] = {"Unknown","Binary","Integer","Positive","Negative","Free","SOS1","SOS2","SemiCont","SemiInt"};
    return res[type];
}

std::string GAMSEnum::text(GAMSEnum::GAMSExitCode type)
{
    if (type >= 1000) {
        int code = type/1000;
        if (code > 5 || (type / 1000.0 - type / 1000 > 0.0)) {
            return "InvalidReturnCode";
        }
        const char * res[] = {"MissingCommandLineParameter","CannotInstallInterrupt","CouldNotGetCurrentDir","CmexNotFound","OptNotFound"};
        return res[code-1];
    } else if (static_cast<int>(type) == 909) {
        return "CannotAddPath";
    } else if (type >= 400) {
        if (type > 412) {
            return "InvalidReturnCode";
        }
        const char * res[] = {"CouldNotSpawnGAMScmex"
                              ,"CurDirNotFound"
                              ,"CurDirNotSet"
                              ,"InvalidReturnCode"
                              ,"BlankInSysDir"
                              ,"BlankInCurDir"
                              ,"BlankInScrExt"
                              ,"UnexpectedCmexRC"
                              ,"ProcDirNotFound"
                              ,"CmexLibNotFound"
                              ,"CmexLibEPNotFound"
                              ,"BlankInProcDir"
                              ,"BlankInScrDir"};
        return res[type-400];
    } else if (type >= 109) {
        if (type > 115) {
            return "InvalidReturnCode";
        }
        const char * res[] = {"CouldNotCreateScratchDir"
                              ,"TooManyScratchDirs"
                              ,"InvalidReturnCode"
                              ,"CouldNotDeleteScratchDir"
                              ,"CouldNotWriteGamsNext"
                              ,"CouldNotWriteParamFile"
                              ,"CouldNotReadEnvVar"};
        return res[type-109];
    } else {
        int code = type;
        if (code < 0 || code > 11) {
            return "InvalidReturnCode";
        }
        const char * res[] = {"NormalReturn"
                              ,"SolverToBeCalled"
                              ,"CompilationError"
                              ,"ExecutionError"
                              ,"SystemLimitsReached"
                              ,"FileError"
                              ,"ParameterError"
                              ,"LicensingError"
                              ,"GAMSSystemError"
                              ,"GAMSCouldNotBeStarted"
                              ,"OutOfMemory"
                              ,"OutOfDisk"};
        return res[type];
    }
}

std::string GAMSEnum::text(GAMSEnum::EAction type)
{
    const char * res[] = {"RestartAfterSolve","CompileOnly","ExecuteOnly","CompileAndExecute","GlueCodeGeneration","TraceReport"};
    return res[type];
}

std::string GAMSEnum::code(GAMSEnum::EAction type)
{
    const char * res[] = { "R", "C", "E", "CE", "G", "GT" };
    return res[type];
}

GAMSEnum::EAction GAMSEnum::eActionFromCode(std::string str)
{
    for (unsigned i = RestartAfterSolve; i <= TraceReport+1; ++i) {
        EAction act = static_cast<EAction>(i);
        if (code(act) == str)
            return act;
    }
    throw GAMSException("GAMSEnum::eActionFromString: unknown EAction type '" +str+ "'");
}

}
