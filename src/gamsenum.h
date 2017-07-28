/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMSENUM_H
#define GAMSENUM_H
// TODO(jm) generator-change-request: #pragma once in some cases behaves differently to standard IFDEF guard
// TODO(JM) generator-change-request: XML-Tags in comments -> no c++ style
// TODO(JM) generator-change-request: add static functions to get enums as string

#include <string>
#include "gamslib_global.h" // LIBSPEC

namespace gams {

/// The collection of all common GAMS enumerations.
class LIBSPEC GAMSEnum
{
public:

    /// This enum defines the type of a symbol (The type Alias is not supported and shows as Set).
    enum SymbolType
    {
        SymTypeSet = 0, ///< Symbol type is a Set (or an Alias)
        SymTypePar,     ///< Symbol type is a Parameter
        SymTypeVar,     ///< Symbol type is a Variable
        SymTypeEqu      ///< Symbol type is a Equation
    };

    /// Get a <c>GAMSEnum::SymbolType</c> as string.
    static std::string text(GAMSEnum::SymbolType type);


    // TODO(AF) difference gmomcc.h/gmoEquType(=> B) and gclgms.h/GMS_EQUTYPE_MAX(=> MAX)?
    /// Equation subtype
    enum EquType
    {
        E = 0,          ///< Equality - =E=
        G = 1,          ///< Greater or equal than inequality - =G=
        L = 2,          ///< Less or equal than inequality - =L=
        N = 3,          ///< Non-binding equation - =N=
        X = 4,          ///< External equation - =X=
        C = 5,          ///< Cone equation - =C=
        MAX = 6
    };

    /// Get a <c>GAMSEnum::EquType</c> as string.
    static std::string text(GAMSEnum::EquType type);

    /// Variable subtype
    enum VarType
    {
        Unknown,        ///< Unknown variable type
        Binary,         ///< Binary variable
        Integer,        ///< Integer Variable
        Positive,       ///< Positive variable
        Negative,       ///< Negative variable
        Free,           ///< Free variable
        SOS1,           ///< Special Ordered Set 1
        SOS2,           ///< Special Ordered Set 2
        SemiCont,       ///< Semi-continuous variable
        SemiInt         ///< Semi-integer variable
    };

    /// Get a <c>GAMSEnum::VarType</c> as string.
    static std::string text(GAMSEnum::VarType type);

    /// Solver termination condition
    enum SolveStat
    {
        Normal = 1,     ///< Normal termination
        Iteration,      ///< Solver ran out of iterations
        Resource,       ///< Solver exceeded time limit
        Solver,         ///< Solver quit with a problem
        EvalError,      ///< Solver quit with nonlinear term evaluation errors
        Capability,     ///< Solver terminated because the model is beyond the solvers capabilities
        License,        ///< Solver terminated with a license error
        User,           ///< Solver terminated on users request (e.g. Ctrl-C)
        SetupErr,       ///< Solver terminated on setup error
        SolverErr,      ///< Solver terminated with error
        InternalErr,    ///< Solver terminated with error
        Skipped,        ///< Solve skipped
        SystemErr       ///< Other error
    };

    /// Model Solution Status
    enum ModelStat
    {
        OptimalGlobal = 1,      ///< Optimal solution achieved
        OptimalLocal,           ///< Local optimal solution achieved
        Unbounded,              ///< Unbounded model found
        InfeasibleGlobal,       ///< Infeasible model found
        InfeasibleLocal,        ///< Locally infeasible model found
        InfeasibleIntermed,     ///< Solver terminated early and model was still infeasible
        Feasible,               ///< Feasible solution
        IntegerSolution,        ///< Integer solution found
        NonIntegerIntermed,     ///< Solver terminated early with a non integer solution found
        IntegerInfeasible,      ///< No feasible integer solution could be found
        LicenseError,           ///< Licensing problem
        ErrorUnknown,           ///< Error - No cause known
        ErrorNoSolution,        ///< Error - No solution attained
        NoSolutionReturned,     ///< No solution returned
        SolvedUnique,           ///< Unique solution in a CNS models
        Solved,                 ///< Feasible solution in a CNS models
        SolvedSingular,         ///< Singular in a CNS models
        UnboundedNoSolution,    ///< Unbounded - no solution
        InfeasibleNoSolution    ///< Infeasible - no solution
    };

    /// GAMS exit code
    enum GAMSExitCode
    {
        NormalReturn = 0,                   ///< Normal Return
        SolverToBeCalled,                   ///< Solver is to be called, the system should never return this number
        CompilationError,                   ///< There was a compilation error
        ExecutionError,                     ///< There was an execution error
        SystemLimitsReached,                ///< System limits were reached
        FileError,                          ///< There was a file error
        ParameterError,                     ///< There was a parameter error
        LicensingError,                     ///< There was a licensing error
        GAMSSystemError,                    ///< There was a GAMS system error
        GAMSCouldNotBeStarted,              ///< GAMS cold not be started
        OutOfMemory,                        ///< Out of memory
        OutOfDisk,                          ///< Out of disk
        CouldNotCreateScratchDir = 109,     ///< Could not create process/scratch directory
        TooManyScratchDirs = 110,           ///< Too many process/scratch directories
        CouldNotDeleteScratchDir = 112,     ///< Could not delete process/scratch directory
        CouldNotWriteGamsNext = 113,        ///< Could not write the "gamsnext" script
        CouldNotWriteParamFile = 114,       ///< Could not write the "parameter" file
        CouldNotSpawnGAMScmex = 400,        ///< Could not spawn the GAMS language compiler (gamscmex)
        CurDirNotFound = 401,               ///< Current directory (curdir) does not exist
        CurDirNotSet = 402,                 ///< Cannot set current directory (curdir)
        BlankInSysDir = 404,                ///< Blank in system directory (UNIX only)
        BlankInCurDir = 405,                ///< Blank in current directory (UNIX only)
        BlankInScrExt = 406,                ///< Blank in scratch extension (scrext)
        UnexpectedCmexRC = 407,             ///< Unexpected cmexRC
        ProcDirNotFound = 408,              ///< Could not find the process directory (procdir)
        CmexLibNotFound = 409,              ///< CMEX library not found (experimental)
        CmexLibEPNotFound = 410,            ///< Entry point in CMEX library not found (experimental)
        BlankInProcDir = 411,               ///< Blank in process directory (UNIX only)
        BlankInScrDir = 412,                ///< Blank in scratch directory (UNIX only)
        CannotAddPath = 909,                ///< Cannot add path / Unknown UNIX environment / Cannot set environment variable
        MissingCommandLineParameter = 1000, ///< Driver error: Missing command line parameter for gams.exe
        CannotInstallInterrupt = 2000,      ///< Driver error: Internal error: Cannot install interrupt handler
        CouldNotGetCurrentDir = 3000,       ///< Driver error: Problems getting current directory (sometimes caused by specifying the current directory in Microsoft UNC format)
        CmexNotFound = 4000,                ///< Driver error: Internal error: GAMS compile and execute module not found
        OptNotFound = 5000                  ///< Driver error: Internal error: Cannot load option handling library
    };

    /// What field to update
    enum SymbolUpdateAction
    {
        Param = 0,          ///< Supplies paramter value
        Upper = 1,          ///< Supplies upper bounds for a variable
        Lower = 2,          ///< Supplies lower bounds for a variable
        Fixed = 3,          ///< Supplies fixed bounds for a variable
        Primal = 4,         ///< Supplies level for a variable or equation
        Dual = 5            ///< Supplies marginal for a variable or equation
    };

    /// GAMS Debug Level
    enum DebugLevel
    {
        Off = 0,            /// No Debug
        KeepFiles,          /// Keep temporary files
        ShowLog,            /// Send GAMS log to stdout and keep temporary files
        Verbose             /// Send highly technical info and GAMS log to stdout and keep temporary file
    };

    /// Symbol update type
    enum SymbolUpdateType
    {
        Default = 0,        /// If record does not exist use 0 (Zero)
        BaseCase = 1,       /// If record does not exist use values from instantiation
        Accumulate = 2,     /// If record does not exist use value from previous solve

        Inherit = 3         /// Inherit setting from solve statement
        //                   // TODO(AF): this is not part of the C API (gmdUpdateType)!
    };

    /// GAMS processing requests
    enum EAction
    {
        RestartAfterSolve = 0,  ///< Restart After Solve
        CompileOnly,            ///< CompileOnly
        ExecuteOnly,            ///< ExecuteOnly
        CompileAndExecute,      ///< Compile and Execute
        GlueCodeGeneration,     ///< Glue Code Generation
        TraceReport             ///< Trace Report
    };

    /// Get a <c>GAMSEnum::VarType</c> as text.
    static std::string text(GAMSEnum::EAction type);

    /// Get a <c>GAMSEnum::VarType</c> as code.
    static std::string code(GAMSEnum::EAction type);

    /// Get a <c>GAMSEnum::VarType</c> from a <c>std::string</c>.
    static EAction eActionFromCode(std::string str);

};

}
#endif // GAMSENUM_H
