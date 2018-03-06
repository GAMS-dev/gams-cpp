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


#ifndef GAMSOPTIONS_H
#define GAMSOPTIONS_H
#include "gamslib_global.h"
#include <vector>
#include <map>
#include <string>
#include "gamsworkspace.h"

namespace gams {

class GAMSOptionsImpl;

/// <p>The GAMSOptions class manages GAMS options (sometimes also called GAMS
/// parameters since they correspond to the command line parameters of the GAMS
/// executable) for a GAMSJob and GAMSModelInstance. There are integer (e.g.
/// NodLim), double (e.g. ResLim), and string (e.g. PutDir) valued options. There
/// are also a few list options (setDefine() to set string macros inside GAMS and addInputDir()
/// provide multiple search paths for include files) and a power option to set a
/// solver for all suitable model types (setAllModelTypes()).</p>
/// <p>Some options known from other interfaces to GAMS that are of limited use or
/// could even create problematic situations in the C++ environment are not
/// settable through the GAMSOptions class.</p>
/// <p>For some options (e.g. Case) other GAMS interfaces use numeric values (e.g. 0,1)
/// while the GAMSOptions class has enumerated types with proper names (e.g.
/// MixedCase, UpperCase).</p>
class LIBSPEC GAMSOptions
{
    friend class GAMSModelInstanceImpl;
    friend class GAMSJobImpl;
    friend class GAMSWorkspaceImpl;

public:
    /// Output file page control option
    struct EPageContr
    {
        /// Output file page control option
        enum EPageContrEnum
        {
            NoPageContrWithPadding = 0,                        ///< No page control, with padding
            FortranStyle = 1,                                  ///< FORTRAN style line printer format
            NoPageContrNoPadding = 2,                          ///< No page control, no padding
            FormfeedCharNewPage = 3                            ///< Formfeed character for new page
        };
    };
    /// GAMS processing request
    struct EAction
    {
        /// GAMS processing request
        enum EActionEnum
        {
            RestartAfterSolve = 0,                             ///< Restart After Solve
            CompileOnly = 1,                                   ///< CompileOnly
            ExecuteOnly = 2,                                   ///< ExecuteOnly
            CompileAndExecute = 3,                             ///< Compile and Execute
            GlueCodeGeneration = 4,                            ///< Glue Code Generation
            TraceReport = 5                                    ///< Trace Report
        };
    };
    /// Amount of line tracing to the log file
    struct ELogLine
    {
        /// Amount of line tracing to the log file
        enum ELogLineEnum
        {
            NoTracing = 0,                                     ///< No line tracing
            MinimumTracing = 1,                                ///< Minimum line tracing
            Automatic = 2                                      ///< Automatic and visually pleasing
        };
    };
    /// Output file append option
    struct EAppendOut
    {
        /// Output file append option
        enum EAppendOutEnum
        {
            Reset = 0,                                         ///< Reset listing file
            Append = 1                                         ///< Append to listing file
        };
    };
    /// Summary of computing resources used by job steps
    struct EStepSum
    {
        /// Summary of computing resources used by job steps
        enum EStepSumEnum
        {
            NoStepSummmary = 0,                                ///< No step summary
            StepSummary = 1                                    ///< Step summary printed
        };
    };
    /// Compiler listing option
    struct ESuppress
    {
        /// Compiler listing option
        enum ESuppressEnum
        {
            StandardCompilerListing = 0,                       ///< Standard compiler listing
            SuppressCompilerListing = 1                        ///< Suppress compiler listing
        };
    };
    /// Date format
    struct EDFormat
    {
        /// Date format
        enum EDFormatEnum
        {
            Slash = 0,                                         ///< Date as mm/dd/yy
            Dot = 1,                                           ///< Date as dd.mm.yy
            Dash = 2                                           ///< Date as yy-mm-dy
        };
    };
    /// Time format
    struct ETFormat
    {
        /// Time format
        enum ETFormatEnum
        {
            Colon = 0,                                         ///< Time as hh:mm:ss
            Dot = 1                                            ///< Time as hh.mm.ss
        };
    };
    /// Set default upper bound on integer variables
    struct EIntVarUp
    {
        /// Set default upper bound on integer variables
        enum EIntVarUpEnum
        {
            INF = 0,                                           ///< Set default upper bound for integer variables to +INF
            Pass100ToSolver = 1,                               ///< Pass a value of 100 instead of +INF to the solver as upper bound for integer variables
            INFandLog = 2,                                     ///< Same as 0 but writes a message to the log if the level of an integer variable is greater than 100
            Pass100ToSolverAndError = 3                        ///< Same as 2 but issues an execution error if the level of an integer variable is greater than 100
        };
    };
    /// Output case option for LST file
    struct ECase
    {
        /// Output case option for LST file
        enum ECaseEnum
        {
            MixedCase = 0,                                     ///< Write listing file in mixed case
            UpperCase = 1                                      ///< Write listing file in upper case only
        };
    };
    /// GAMS parameter logging
    struct EDumpParms
    {
        /// GAMS parameter logging
        enum EDumpParmsEnum
        {
            No = 0,                                            ///< No logging
            AcceptedParameters = 1,                            ///< Lists accepted/set parameters
            FileOperationsAcceptedParameters = 2               ///< Log of file operations plus list of accepted/set parameters
        };
    };
    /// Force GAMS systems to process save files for example with an execution error
    struct EForceWork
    {
        /// Force GAMS systems to process save files for example with an execution error
        enum EForceWorkEnum
        {
            NoTranslation = 0,                                 ///< No translation
            TryTranslation = 1                                 ///< Try translation
        };
    };
    /// Writes preprocessed input to the file input.dmp
    struct EDumpOpt
    {
        /// Writes preprocessed input to the file input.dmp
        enum EDumpOptEnum
        {
            No = 0,                                            ///< No dumpfile
            RefDataOriginalSetElementNames = 1,                ///< Extract referenced data from the restart file using original set element names
            RefDataNewSetElementNames = 2,                     ///< Extract referenced data from the restart file using new set element names
            RefDataNewSetElementNamesDropSymbolText = 3,       ///< Extract referenced data from the restart file using new set element names and drop symbol text
            RefSymbol = 4,                                     ///< Extract referenced symbol declarations from the restart file
            Deprecated_10 = 10,                                ///< (Same as 11 and therefore hidden)
            InputFileWOComments = 11,                          ///< Write processed input file without comments
            Deprecated_12 = 12,                                ///< (Same as 11 and therefore hidden)
            Deprecated_19 = 19,                                ///< (Same as 21 and therefore hidden)
            Deprecated_20 = 20,                                ///< (Same as 21 and therefore hidden)
            InputFileWithComments = 21                         ///< Write processed input file with all comments
        };
    };
    /// Placing of compilation error messages
    struct EErrMsg
    {
        /// Placing of compilation error messages
        enum EErrMsgEnum
        {
            EndOfCompilerListing = 0,                          ///< Place error messages at the end of compiler listing
            FollowingError = 1,                                ///< Place error messages immediately following the line with the error
            Suppress = 2                                       ///< Suppress error messages
        };
    };
    /// String substitution options
    struct EStringChk
    {
        /// String substitution options
        enum EStringChkEnum
        {
            NoError = 0,                                       ///< No substitution if symbol undefined and no error
            Error = 1,                                         ///< Error if symbol undefined
            NoErrorRemoveSymbol = 2                            ///< Remove entire symbol reference if undefined and no error
        };
    };
    /// Character set flag
    struct ECharSet
    {
        /// Character set flag
        enum ECharSetEnum
        {
            LimitedGAMSCharSet = 0,                            ///< Use limited GAMS characters set
            AnyChar = 1                                        ///< Accept any character in comments and text items (foreign language characters)
        };
    };
    /// Controls keeping or deletion of process directory and scratch files
    struct EKeep
    {
        /// Controls keeping or deletion of process directory and scratch files
        enum EKeepEnum
        {
            DeleteProcDir = 0,                                 ///< Delete process directory
            KeepProcDir = 1                                    ///< Keep process directory
        };
    };
    /// Trace file format option
    struct ETraceOpt
    {
        /// Trace file format option
        enum ETraceOptEnum
        {
            SolverAndGAMSStepTraceWOHeaders = 0,               ///< Solver and GAMS step trace without headers
            SolverAndGAMSStepTrace = 1,                        ///< Solver and GAMS step trace
            SolverStepTraceOnly = 2,                           ///< Solver step trace only
            TraceFileFormatGAMSPerformanceWorld = 3,           ///< Trace file format used for GAMS performance world
            TraceFileFormatSupportingNLPEC = 4,                ///< Trace file format supporting NLPEC
            TraceFileWithAllAvailableTraceFields = 5           ///< Trace file with all available trace fields
        };
    };
    /// Solver Status file reporting option
    struct ESysOut
    {
        /// Solver Status file reporting option
        enum ESysOutEnum
        {
            SuppressAdditionalSolverOutput = 0,                ///< Suppress additional solver generated output
            IncludeAdditionalSolverOutput = 1                  ///< Include additional solver generated output
        };
    };
    /// Solution report print option
    struct ESolPrint
    {
        /// Solution report print option
        enum ESolPrintEnum
        {
            RemoveSolLstFollowingSolves = 0,                   ///< Remove solution listings following solves
            IncludeSolLstFollowingSolves = 1,                  ///< Include solution listings following solves
            SuppressAllSolInfo = 2                             ///< Suppress all solution information
        };
    };
    /// Treat fixed variables as constants
    struct EHoldFixed
    {
        /// Treat fixed variables as constants
        enum EHoldFixedEnum
        {
            FixedVarsNotTreatedAsConstants = 0,                ///< Fixed variables are not treated as constants
            FixedVarsTreatedAsConstants = 1                    ///< Fixed variables are treated as constants
        };
    };
    /// Generate errors for unknown unique element in an equation
    struct EOn115
    {
        /// Generate errors for unknown unique element in an equation
        enum EOn115Enum
        {
            NoMessages = 0,                                    ///< No error messages
            IssueMessages = 1                                  ///< Issue error messages
        };
    };
    /// Limits on external programs that are allowed to be executed
    struct EExecMode
    {
        /// Limits on external programs that are allowed to be executed
        enum EExecModeEnum
        {
            EverythingAllowed = 0,                             ///< Everything allowed
            InteractiveShellsProhibited = 1,                   ///< Interactive shells in $call and execute commands are prohibited
            CallAndExecuteProhibited = 2,                      ///< Embedded Code and all $call and execute commands are prohibited
            EchoAndPutOnlyToWorkdir = 3,                       ///< $echo or put commands can only write to directories in or below the working or scratchdir
            EchoAndPutProhibited = 4                           ///< $echo and put commands are not allowed
        };
    };
    /// Allow solver to interact via command line input
    struct EInteractiveSolver
    {
        /// Allow solver to interact via command line input
        enum EInteractiveSolverEnum
        {
            NoInteraction = 0,                                 ///< Interaction with solvelink 0 is not supported
            AllowInteraction = 1                               ///< Interaction with solvelink 0 is supported
        };
    };
    /// Casing of new file names (put, gdx, ref etc.)
    struct EFileCase
    {
        /// Casing of new file names (put, gdx, ref etc.)
        enum EFileCaseEnum
        {
            DefaultCase = 0,                                   ///< Causes GAMS to use default casing
            UpperCase = 1,                                     ///< Causes GAMS to upper case file names
            LowerCase = 2                                      ///< Causes GAMS to lower case file names
        };
    };
    /// Save solver point in GDX file
    struct ESavePoint
    {
        /// Save solver point in GDX file
        enum ESavePointEnum
        {
            NoPointFile = 0,                                   ///< No point GDX file is to be saved
            LastSolvePointFile = 1,                            ///< A point GDX file from the last solve is to be saved
            EverySolvePointFile = 2                            ///< A point GDX file from every solve is to be saved
        };
    };
    /// Solver link option
    struct ESolveLink
    {
        /// Solver link option
        enum ESolveLinkEnum
        {
            ChainScript = 0,                                   ///< GAMS operates as it has for years
            CallScript = 1,                                    ///< Solver is called from a shell and GAMS remains open
            CallModule = 2,                                    ///< Solver is called with a spawn (if possible) or a shell (if spawn is not possible) and GAMS remains open
            AsyncGrid = 3,                                     ///< GAMS starts the solution and continues in a Grid computing environment
            AsyncSimulate = 4,                                 ///< GAMS starts the solution and wait (same submission process as 3) in a Grid computing environment
            LoadLibrary = 5,                                   ///< The problem is passed to the solver in core without use of temporary files
            LoadLibraryAsync = 6,                              ///< The problem is passed to the solver in core without use of temporary files, GAMS does not wait for the solver to come back
            LoadLibraryAsyncSimulate = 7                       ///< The problem is passed to the solver in core without use of temporary files, GAMS waits for the solver to come back but uses same submission process as 6
        };
    };
    /// Changes rpower to ipower when the exponent is constant and within 1e-12 of an integer
    struct ESys10
    {
        /// Changes rpower to ipower when the exponent is constant and within 1e-12 of an integer
        enum ESys10Enum
        {
            Disable = 0,                                       ///< Disable conversion
            Enable = 1                                         ///< Enable conversion
        };
    };
    /// Dynamic resorting if indices in assignment/data statements are not in natural order
    struct ESys11
    {
        /// Dynamic resorting if indices in assignment/data statements are not in natural order
        enum ESys11Enum
        {
            AutomaticOptimization = 0,                         ///< Automatic optimization/restructuring of data
            NoOptimization = 1,                                ///< No optimization
            AlwaysOptimize = 2                                 ///< Always optimize/restructure
        };
    };
    /// Report underflow as a warning when abs(results) LE ZeroRes and result set to zero
    struct EZeroResRep
    {
        /// Report underflow as a warning when abs(results) LE ZeroRes and result set to zero
        enum EZeroResRepEnum
        {
            NoWarning = 0,                                     ///< No warning when a rounding occurs because of ZeroRes
            IssueWarning = 1                                   ///< Issue warnings whenever a rounding occurs because of ZeroRes
        };
    };
    /// Compression of generated GDX file
    struct EgdxCompress
    {
        /// Compression of generated GDX file
        enum EgdxCompressEnum
        {
            DoNotCompressGDX = 0,                              ///< Do not compress GDX files
            CompressGDX = 1                                    ///< Compress GDX files
        };
    };
    /// Version of GDX files generated (for backward compatibility)
    struct EgdxConvert
    {
        /// Version of GDX files generated (for backward compatibility)
        enum EgdxConvertEnum
        {
            Version5 = 0,                                      ///< Version 5 GDX file, does not support compression
            Version6 = 1,                                      ///< Version 6 GDX file
            Version7 = 2                                       ///< Version 7 GDX file
        };
    };
    /// Expand file append option
    struct EAppendExpand
    {
        /// Expand file append option
        enum EAppendExpandEnum
        {
            Reset = 0,                                         ///< Reset expand file
            Append = 1                                         ///< Append to expand file
        };
    };
    /// Triggers a compilation error when new equations or variable symbols are introduced
    struct ENoNewVarEqu
    {
        /// Triggers a compilation error when new equations or variable symbols are introduced
        enum ENoNewVarEquEnum
        {
            AllowNewVarEqu = 0,                                ///< AllowNewVarEqu
            DoNotAllowNewVarEqu = 1                            ///< DoNotAllowNewVarEqu
        };
    };
    /// Unload labels or UELs to GDX either squeezed or full
    struct EgdxUels
    {
        /// Unload labels or UELs to GDX either squeezed or full
        enum EgdxUelsEnum
        {
            squeezed = 0,                                      ///< Write only the UELs to Universe, that are used by the exported symbols
            full = 1                                           ///< Write all UELs to Universe
        };
    };
    /// Error if assignment to singleton set has multiple elements
    struct EstrictSingleton
    {
        /// Error if assignment to singleton set has multiple elements
        enum EstrictSingletonEnum
        {
            FirstRecord = 0,                                   ///< Take first record if assignment to singleton set has multiple elements
            Error = 1                                          ///< Error if assignment to singleton set has multiple elements
        };
    };
    /// Options for finite differences
    struct EFDOpt
    {
        /// Options for finite differences
        enum EFDOptEnum
        {
            GHAnalyticHNumericGradScale = 0,                   ///< All derivatives analytically, for numerical Hessian use gradient values, scale delta
            GHAnalyticHNumericFuncScale = 1,                   ///< All derivatives analytically, for numerical Hessian use function values, scale delta
            GAnalyticFHNumericGradScale = 2,                   ///< Gradient analytically, force Hessian numerically using gradient values, scale delta
            GAnalyticFHNumericFuncScale = 3,                   ///< Gradient analytically, force Hessian numerically using function values, scale delta
            FGHNumericScale = 4,                               ///< Force gradient and Hessian numerically, scale delta
            GHAnalyticHNumericGradNoScale = 10,                ///< Same as 0, but no scale of delta
            GHAnalyticHNumericFuncNoScale = 11,                ///< Same as 1, but no scale of delta
            GAnalyticFHNumericGradNoScale = 12,                ///< Same as 2, but no scale of delta
            GAnalyticFHNumericFuncNoScale = 13,                ///< Same as 3, but no scale of delta
            FGHNumericNoScale = 14                             ///< Same as 4, but no scale of delta
        };
    };
    /// Prepend GAMS system directory to library load path
    struct EPrefixLoadPath
    {
        /// Prepend GAMS system directory to library load path
        enum EPrefixLoadPathEnum
        {
            Off = 0,                                           ///< Do not set GAMS system directory at beginning of library load path
            On = 1                                             ///< Set GAMS system directory at beginning of library load path
        };
    };
    /// Print solution listing when asynchronous solve (Grid or Threads) is used
    struct EAsyncSolLst
    {
        /// Print solution listing when asynchronous solve (Grid or Threads) is used
        enum EAsyncSolLstEnum
        {
            Off = 0,                                           ///< Do not print solution listing into lst file for asynchronous solves
            On = 1                                             ///< Print solution listing into lst file for asynchronous solves
        };
    };
    /// GAMS/Python Setup
    struct EPySetup
    {
        /// GAMS/Python Setup
        enum EPySetupEnum
        {
            Off = 0,                                           ///< Rely on user setup of Python
            On = 1                                             ///< Use Python installation provided in GAMS system directory
        };
    };
    /// GAMS/Python Multiple Instance Interpreter
    struct EPyMultInst
    {
        /// GAMS/Python Multiple Instance Interpreter
        enum EPyMultInstEnum
        {
            Single = 0,                                        ///< Use a single-session Python interpreter
            Multi = 1                                          ///< Use a multi-session Python interpreter
        };
    };

    /// Destructor
    ~GAMSOptions();

    /// Get GAMS build information.
    static const char* gamsBuild();

    /// Get GAMS version information.
    static const char* gamsVersion();

    /// Get the GAMS log ID.
    /// \return Returns the GAMS log ID.
    LogId logID();

    /// Export options to an option file
    void writeOptionFile(const std::string& optFileName);

    /// Set the solver for all model types the solver can handle
    void setAllModelTypes(const std::string& solver);

    /// Get the size of the inputdir list
    int inputDirSize();

    /// Add new inputdir at the end of the inputdir list
    void addInputDir(const std::string value);

    /// Get an inputdir at position index of the inputdir list
    std::string getInputDir(const int index);

    /// Replace inputdir at the specified index of the inputdir list
    void setInputDir(const std::string value, const int index);

    /// Remove an inputdir at position index of the inputdir list
    void removeInputDir(const int index);

    /// Set double-dash --key=value option pair
    void setDefine(const std::string &key, const std::string &value);

    /// Get value of previously set double-dash --key-value option pair
    std::string getDefine(const std::string& key);

    /// Remove a double-dash option
    bool removeDefine(const std::string& key);

    /// Get GDX file name
    std::string gdx();

    /// Set GDX file name
    void setGdx(const std::string& value);

    /// Get Listing file name
    std::string output();

    /// Set Listing file name
    void setOutput(const std::string& value);

    /// Get Symbol table file
    std::string symbol();

    /// Set Symbol table file
    void setSymbol(const std::string& value);

    /// Get Expanded (include) input file name
    std::string expand();

    /// Set Expanded (include) input file name
    void setExpand(const std::string& value);

    /// Get Output file page width
    int pageWidth();

    /// Set Output file page width
    void setPageWidth(const int value);

    /// Get Output file page size (=0 no paging)
    int pageSize();

    /// Set Output file page size (=0 no paging)
    void setPageSize(const int value);

    /// Get Output file page control option
    GAMSOptions::EPageContr::EPageContrEnum pageContr();

    /// Set Output file page control option
    void setPageContr(const GAMSOptions::EPageContr::EPageContrEnum value);

    /// Get GAMS processing request as text
    static std::string eActionText(GAMSOptions::EAction::EActionEnum type);

    /// Get GAMS processing request as code
    static std::string eActionCode(GAMSOptions::EAction::EActionEnum type);

    /// Get GAMS processing request from <c>std::string</c>
    static EAction::EActionEnum eActionFromCode(const std::string &str);

    /// Get GAMS processing request
    GAMSOptions::EAction::EActionEnum action();

    /// Set GAMS processing request
    void setAction(const GAMSOptions::EAction::EActionEnum value);

    /// Get Put file directory
    std::string putDir();

    /// Set Put file directory
    void setPutDir(const std::string& value);

    /// Get Grid file directory
    std::string gridDir();

    /// Set Grid file directory
    void setGridDir(const std::string& value);

    /// Get Program or script to be executed at the end of a GAMS run
    std::string scriptExit();

    /// Set Program or script to be executed at the end of a GAMS run
    void setScriptExit(const std::string& value);

    /// Get Amount of line tracing to the log file
    GAMSOptions::ELogLine::ELogLineEnum logLine();

    /// Set Amount of line tracing to the log file
    void setLogLine(const GAMSOptions::ELogLine::ELogLineEnum value);

    /// Get Trace file name
    std::string trace();

    /// Set Trace file name
    void setTrace(const std::string& value);

    /// Get Output file append option
    GAMSOptions::EAppendOut::EAppendOutEnum appendOut();

    /// Set Output file append option
    void setAppendOut(const GAMSOptions::EAppendOut::EAppendOutEnum value);

    /// Get Summary of computing resources used by job steps
    GAMSOptions::EStepSum::EStepSumEnum stepSum();

    /// Set Summary of computing resources used by job steps
    void setStepSum(const GAMSOptions::EStepSum::EStepSumEnum value);

    /// Get Compiler listing option
    GAMSOptions::ESuppress::ESuppressEnum suppress();

    /// Set Compiler listing option
    void setSuppress(const GAMSOptions::ESuppress::ESuppressEnum value);

    /// Get Date format
    GAMSOptions::EDFormat::EDFormatEnum dFormat();

    /// Set Date format
    void setDFormat(const GAMSOptions::EDFormat::EDFormatEnum value);

    /// Get Time format
    GAMSOptions::ETFormat::ETFormatEnum tFormat();

    /// Set Time format
    void setTFormat(const GAMSOptions::ETFormat::ETFormatEnum value);

    /// Get Tab spacing
    int tabIn();

    /// Set Tab spacing
    void setTabIn(const int value);

    /// Get Set default upper bound on integer variables
    GAMSOptions::EIntVarUp::EIntVarUpEnum intVarUp();

    /// Set Set default upper bound on integer variables
    void setIntVarUp(const GAMSOptions::EIntVarUp::EIntVarUpEnum value);

    /// Get Output case option for LST file
    int lstCase();

    /// Set Output case option for LST file
    void setLstCase(const int value);

    /// Get Use alternative license file
    std::string license();

    /// Set Use alternative license file
    void setLicense(const std::string& value);

    /// Get GAMS parameter logging
    GAMSOptions::EDumpParms::EDumpParmsEnum dumpParms();

    /// Set GAMS parameter logging
    void setDumpParms(const GAMSOptions::EDumpParms::EDumpParmsEnum value);

    /// Get Execution profiling
    int profile();

    /// Set Execution profiling
    void setProfile(const int value);

    /// Get LibInclude directory
    std::string libIncDir();

    /// Set LibInclude directory
    void setLibIncDir(const std::string& value);

    /// Get SysInclude directory
    std::string sysIncDir();

    /// Set SysInclude directory
    void setSysIncDir(const std::string& value);

    /// Get User string N
    std::string user1();

    /// Set User string N
    void setUser1(const std::string& value);

    /// Get User string N
    std::string user2();

    /// Set User string N
    void setUser2(const std::string& value);

    /// Get User string N
    std::string user3();

    /// Set User string N
    void setUser3(const std::string& value);

    /// Get User string N
    std::string user4();

    /// Set User string N
    void setUser4(const std::string& value);

    /// Get User string N
    std::string user5();

    /// Set User string N
    void setUser5(const std::string& value);

    /// Get Force GAMS systems to process save files for example with an execution error
    GAMSOptions::EForceWork::EForceWorkEnum forceWork();

    /// Set Force GAMS systems to process save files for example with an execution error
    void setForceWork(const GAMSOptions::EForceWork::EForceWorkEnum value);

    /// Get Compile time error limit
    int cErr();

    /// Set Compile time error limit
    void setCErr(const int value);

    /// Get Alternative error message file
    std::string fErr();

    /// Set Alternative error message file
    void setFErr(const std::string& value);

    /// Get Default option file
    int optFile();

    /// Set Default option file
    void setOptFile(const int value);

    /// Get Writes preprocessed input to the file input.dmp
    GAMSOptions::EDumpOpt::EDumpOptEnum dumpOpt();

    /// Set Writes preprocessed input to the file input.dmp
    void setDumpOpt(const GAMSOptions::EDumpOpt::EDumpOptEnum value);

    /// Get Placing of compilation error messages
    GAMSOptions::EErrMsg::EErrMsgEnum errMsg();

    /// Set Placing of compilation error messages
    void setErrMsg(const GAMSOptions::EErrMsg::EErrMsgEnum value);

    /// Get String substitution options
    GAMSOptions::EStringChk::EStringChkEnum stringChk();

    /// Set String substitution options
    void setStringChk(const GAMSOptions::EStringChk::EStringChkEnum value);

    /// Get Symbol reference file
    std::string reference();

    /// Set Symbol reference file
    void setReference(const std::string& value);

    /// Get Option file directory
    std::string optDir();

    /// Set Option file directory
    void setOptDir(const std::string& value);

    /// Get Execution time error limit
    int execErr();

    /// Set Execution time error limit
    void setExecErr(const int value);

    /// Get Max error message lines written to the log for each error
    int errorLog();

    /// Set Max error message lines written to the log for each error
    void setErrorLog(const int value);

    /// Get Maximum number of rows listed in one equation block
    int limRow();

    /// Set Maximum number of rows listed in one equation block
    void setLimRow(const int value);

    /// Get Maximum number of columns listed in one variable block
    int limCol();

    /// Set Maximum number of columns listed in one variable block
    void setLimCol(const int value);

    /// Get Character set flag
    GAMSOptions::ECharSet::ECharSetEnum charSet();

    /// Set Character set flag
    void setCharSet(const GAMSOptions::ECharSet::ECharSetEnum value);

    /// Get Controls keeping or deletion of process directory and scratch files
    GAMSOptions::EKeep::EKeepEnum keep();

    /// Set Controls keeping or deletion of process directory and scratch files
    void setKeep(const GAMSOptions::EKeep::EKeepEnum value);

    /// Get Trace file format option
    GAMSOptions::ETraceOpt::ETraceOptEnum traceOpt();

    /// Set Trace file format option
    void setTraceOpt(const GAMSOptions::ETraceOpt::ETraceOptEnum value);

    /// Get Iteration limit of solver
    int iterLim();

    /// Set Iteration limit of solver
    void setIterLim(const int value);

    /// Get Domain violation limit solver default
    int domLim();

    /// Set Domain violation limit solver default
    void setDomLim(const int value);

    /// Get Wall-clock time limit for solver
    double resLim();

    /// Set Wall-clock time limit for solver
    void setResLim(const double value);

    /// Get Relative Optimality criterion solver default
    double optCR();

    /// Set Relative Optimality criterion solver default
    void setOptCR(const double value);

    /// Get Absolute Optimality criterion solver default
    double optCA();

    /// Set Absolute Optimality criterion solver default
    void setOptCA(const double value);

    /// Get Solver Status file reporting option
    GAMSOptions::ESysOut::ESysOutEnum sysOut();

    /// Set Solver Status file reporting option
    void setSysOut(const GAMSOptions::ESysOut::ESysOutEnum value);

    /// Get Solution report print option
    GAMSOptions::ESolPrint::ESolPrintEnum solPrint();

    /// Set Solution report print option
    void setSolPrint(const GAMSOptions::ESolPrint::ESolPrintEnum value);

    /// Get Basis acceptance threshold
    double bratio();

    /// Set Basis acceptance threshold
    void setBratio(const double value);

    /// Get Treat fixed variables as constants
    GAMSOptions::EHoldFixed::EHoldFixedEnum holdFixed();

    /// Set Treat fixed variables as constants
    void setHoldFixed(const GAMSOptions::EHoldFixed::EHoldFixedEnum value);

    /// Get Node limit in branch and bound tree
    int nodLim();

    /// Set Node limit in branch and bound tree
    void setNodLim(const int value);

    /// Get Memory Estimate multiplier for some solvers
    double workFactor();

    /// Set Memory Estimate multiplier for some solvers
    void setWorkFactor(const double value);

    /// Get Work space for some solvers in MB
    double workSpace();

    /// Set Work space for some solvers in MB
    void setWorkSpace(const double value);

    /// Get GAMS looping limit
    int forLim();

    /// Set GAMS looping limit
    void setForLim(const int value);

    /// Get Random number seed
    int seed();

    /// Set Random number seed
    void setSeed(const int value);

    /// Get Generate errors for unknown unique element in an equation
    GAMSOptions::EOn115::EOn115Enum on115();

    /// Set Generate errors for unknown unique element in an equation
    void setOn115(const GAMSOptions::EOn115::EOn115Enum value);

    /// Get Limits on external programs that are allowed to be executed
    GAMSOptions::EExecMode::EExecModeEnum execMode();

    /// Set Limits on external programs that are allowed to be executed
    void setExecMode(const GAMSOptions::EExecMode::EExecModeEnum value);

    /// Get Allow solver to interact via command line input
    GAMSOptions::EInteractiveSolver::EInteractiveSolverEnum interactiveSolver();

    /// Set Allow solver to interact via command line input
    void setInteractiveSolver(const GAMSOptions::EInteractiveSolver::EInteractiveSolverEnum value);

    /// Get Privacy license file name
    std::string pLicense();

    /// Set Privacy license file name
    void setPLicense(const std::string& value);

    /// Get Job trace string to be written to the trace file at the end of a GAMS job
    std::string jobTrace();

    /// Set Job trace string to be written to the trace file at the end of a GAMS job
    void setJobTrace(const std::string& value);

    /// Get Number of warnings permitted before a run terminates
    int warnings();

    /// Set Number of warnings permitted before a run terminates
    void setWarnings(const int value);

    /// Get Solvestat threshold used in conjunction with action=GT
    int traceLevel();

    /// Set Solvestat threshold used in conjunction with action=GT
    void setTraceLevel(const int value);

    /// Get Instruction timer threshold in milli seconds
    int timer();

    /// Set Instruction timer threshold in milli seconds
    void setTimer(const int value);

    /// Get Casing of new file names (put, gdx, ref etc.)
    GAMSOptions::EFileCase::EFileCaseEnum fileCase();

    /// Set Casing of new file names (put, gdx, ref etc.)
    void setFileCase(const GAMSOptions::EFileCase::EFileCaseEnum value);

    /// Get Save solver point in GDX file
    GAMSOptions::ESavePoint::ESavePointEnum savePoint();

    /// Set Save solver point in GDX file
    void setSavePoint(const GAMSOptions::ESavePoint::ESavePointEnum value);

    /// Get Solver link option
    GAMSOptions::ESolveLink::ESolveLinkEnum solveLink();

    /// Set Solver link option
    void setSolveLink(const GAMSOptions::ESolveLink::ESolveLinkEnum value);

    /// Get Changes rpower to ipower when the exponent is constant and within 1e-12 of an integer
    GAMSOptions::ESys10::ESys10Enum sys10();

    /// Set Changes rpower to ipower when the exponent is constant and within 1e-12 of an integer
    void setSys10(const GAMSOptions::ESys10::ESys10Enum value);

    /// Get Dynamic resorting if indices in assignment/data statements are not in natural order
    GAMSOptions::ESys11::ESys11Enum sys11();

    /// Set Dynamic resorting if indices in assignment/data statements are not in natural order
    void setSys11(const GAMSOptions::ESys11::ESys11Enum value);

    /// Get Pass model with generation errors to solver
    int sys12();

    /// Set Pass model with generation errors to solver
    void setSys12(const int value);

    /// Get The results of certain operations will be set to zero if abs(result) LE ZeroRes
    double zeroRes();

    /// Set The results of certain operations will be set to zero if abs(result) LE ZeroRes
    void setZeroRes(const double value);

    /// Get Report underflow as a warning when abs(results) LE ZeroRes and result set to zero
    GAMSOptions::EZeroResRep::EZeroResRepEnum zeroResRep();

    /// Set Report underflow as a warning when abs(results) LE ZeroRes and result set to zero
    void setZeroResRep(const GAMSOptions::EZeroResRep::EZeroResRepEnum value);

    /// Get Maximum Heap size allowed in MB
    double heapLimit();

    /// Set Maximum Heap size allowed in MB
    void setHeapLimit(const double value);

    /// Get Compression of generated GDX file
    GAMSOptions::EgdxCompress::EgdxCompressEnum gdxCompress();

    /// Set Compression of generated GDX file
    void setgdxCompress(const GAMSOptions::EgdxCompress::EgdxCompressEnum value);

    /// Get Version of GDX files generated (for backward compatibility) as text
    static std::string egdxConvertText(GAMSOptions::EgdxConvert::EgdxConvertEnum type);

    /// Get Version of GDX files generated (for backward compatibility) as code
    static std::string egdxConvertCode(GAMSOptions::EgdxConvert::EgdxConvertEnum type);

    /// Get Version of GDX files generated (for backward compatibility) from <c>std::string</c>
    static EgdxConvert::EgdxConvertEnum egdxConvertFromCode(const std::string &str);

    /// Get Version of GDX files generated (for backward compatibility)
    GAMSOptions::EgdxConvert::EgdxConvertEnum gdxConvert();

    /// Set Version of GDX files generated (for backward compatibility)
    void setgdxConvert(const GAMSOptions::EgdxConvert::EgdxConvertEnum value);

    /// Get Expand file append option
    GAMSOptions::EAppendExpand::EAppendExpandEnum appendExpand();

    /// Set Expand file append option
    void setAppendExpand(const GAMSOptions::EAppendExpand::EAppendExpandEnum value);

    /// Get Elapsed time limit in seconds
    double eTLim();

    /// Set Elapsed time limit in seconds
    void setETLim(const double value);

    /// Get Minimum time a statement must use to appear in profile generated output
    double profileTol();

    /// Set Minimum time a statement must use to appear in profile generated output
    void setProfileTol(const double value);

    /// Get Write profile information to this file
    std::string profileFile();

    /// Set Write profile information to this file
    void setProfileFile(const std::string& value);

    /// Get Maximum number of 225* process directories
    int maxProcDir();

    /// Set Maximum number of 225* process directories
    void setMaxProcDir(const int value);

    /// Get Integer communication cell N
    int integer1();

    /// Set Integer communication cell N
    void setInteger1(const int value);

    /// Get Integer communication cell N
    int integer2();

    /// Set Integer communication cell N
    void setInteger2(const int value);

    /// Get Integer communication cell N
    int integer3();

    /// Set Integer communication cell N
    void setInteger3(const int value);

    /// Get Integer communication cell N
    int integer4();

    /// Set Integer communication cell N
    void setInteger4(const int value);

    /// Get Integer communication cell N
    int integer5();

    /// Set Integer communication cell N
    void setInteger5(const int value);

    /// Get Grid submission script
    std::string gridScript();

    /// Set Grid submission script
    void setGridScript(const std::string& value);

    /// Get Number of threads to be used by a solver
    int threads();

    /// Set Number of threads to be used by a solver
    void setThreads(const int value);

    /// Get Triggers a compilation error when new equations or variable symbols are introduced
    GAMSOptions::ENoNewVarEqu::ENoNewVarEquEnum noNewVarEqu();

    /// Set Triggers a compilation error when new equations or variable symbols are introduced
    void setNoNewVarEqu(const GAMSOptions::ENoNewVarEqu::ENoNewVarEquEnum value);

    /// Get Overwrites other option file section mechanism
    int forceOptFile();

    /// Set Overwrites other option file section mechanism
    void setForceOptFile(const int value);

    /// Get Prefix all symbols encountered during compilation by the specified string in work file
    std::string symPrefix();

    /// Set Prefix all symbols encountered during compilation by the specified string in work file
    void setSymPrefix(const std::string& value);

    /// Get Unload labels or UELs to GDX either squeezed or full as text
    static std::string egdxUelsText(GAMSOptions::EgdxUels::EgdxUelsEnum type);

    /// Get Unload labels or UELs to GDX either squeezed or full as code
    static std::string egdxUelsCode(GAMSOptions::EgdxUels::EgdxUelsEnum type);

    /// Get Unload labels or UELs to GDX either squeezed or full from <c>std::string</c>
    static EgdxUels::EgdxUelsEnum egdxUelsFromCode(const std::string &str);

    /// Get Unload labels or UELs to GDX either squeezed or full
    GAMSOptions::EgdxUels::EgdxUelsEnum gdxUels();

    /// Set Unload labels or UELs to GDX either squeezed or full
    void setgdxUels(const GAMSOptions::EgdxUels::EgdxUelsEnum value);

    /// Get Prefix of lines triggered by DumpParms>1
    std::string dumpParmsLogPrefix();

    /// Set Prefix of lines triggered by DumpParms>1
    void setDumpParmsLogPrefix(const std::string& value);

    /// Get Error if assignment to singleton set has multiple elements
    GAMSOptions::EstrictSingleton::EstrictSingletonEnum strictSingleton();

    /// Set Error if assignment to singleton set has multiple elements
    void setstrictSingleton(const GAMSOptions::EstrictSingleton::EstrictSingletonEnum value);

    /// Get Step size for finite differences
    double fDDelta();

    /// Set Step size for finite differences
    void setFDDelta(const double value);

    /// Get Options for finite differences
    GAMSOptions::EFDOpt::EFDOptEnum fDOpt();

    /// Set Options for finite differences
    void setFDOpt(const GAMSOptions::EFDOpt::EFDOptEnum value);

    /// Get Prepend GAMS system directory to library load path
    GAMSOptions::EPrefixLoadPath::EPrefixLoadPathEnum prefixLoadPath();

    /// Set Prepend GAMS system directory to library load path
    void setPrefixLoadPath(const GAMSOptions::EPrefixLoadPath::EPrefixLoadPathEnum value);

    /// Get Number of threads to be used for asynchronous solve (solveLink=6)
    int threadsAsync();

    /// Set Number of threads to be used for asynchronous solve (solveLink=6)
    void setThreadsAsync(const int value);

    /// Get Print solution listing when asynchronous solve (Grid or Threads) is used
    GAMSOptions::EAsyncSolLst::EAsyncSolLstEnum asyncSolLst();

    /// Set Print solution listing when asynchronous solve (Grid or Threads) is used
    void setAsyncSolLst(const GAMSOptions::EAsyncSolLst::EAsyncSolLstEnum value);

    /// Get Sets the file stem for output files which use the input file name as stem by default
    std::string fileStem();

    /// Set Sets the file stem for output files which use the input file name as stem by default
    void setFileStem(const std::string& value);

    /// Get GAMS/Python Setup
    GAMSOptions::EPySetup::EPySetupEnum pySetup();

    /// Set GAMS/Python Setup
    void setPySetup(const GAMSOptions::EPySetup::EPySetupEnum value);

    /// Get GAMS/Python Multiple Instance Interpreter
    GAMSOptions::EPyMultInst::EPyMultInstEnum pyMultInst();

    /// Set GAMS/Python Multiple Instance Interpreter
    void setPyMultInst(const GAMSOptions::EPyMultInst::EPyMultInstEnum value);

    /// Get Append a string read from an environment variable to the "FileStem"
    std::string fileStemApFromEnv();

    /// Set Append a string read from an environment variable to the "FileStem"
    void setFileStemApFromEnv(const std::string& value);

    /// Get default LP solver
    std::string LP();

    /// Set default LP solver
    void setLP(const std::string& value);

    /// Get default MIP solver
    std::string MIP();

    /// Set default MIP solver
    void setMIP(const std::string& value);

    /// Get default RMIP solver
    std::string RMIP();

    /// Set default RMIP solver
    void setRMIP(const std::string& value);

    /// Get default NLP solver
    std::string NLP();

    /// Set default NLP solver
    void setNLP(const std::string& value);

    /// Get default MCP solver
    std::string MCP();

    /// Set default MCP solver
    void setMCP(const std::string& value);

    /// Get default MPEC solver
    std::string MPEC();

    /// Set default MPEC solver
    void setMPEC(const std::string& value);

    /// Get default RMPEC solver
    std::string RMPEC();

    /// Set default RMPEC solver
    void setRMPEC(const std::string& value);

    /// Get default CNS solver
    std::string CNS();

    /// Set default CNS solver
    void setCNS(const std::string& value);

    /// Get default DNLP solver
    std::string DNLP();

    /// Set default DNLP solver
    void setDNLP(const std::string& value);

    /// Get default RMINLP solver
    std::string RMINLP();

    /// Set default RMINLP solver
    void setRMINLP(const std::string& value);

    /// Get default MINLP solver
    std::string MINLP();

    /// Set default MINLP solver
    void setMINLP(const std::string& value);

    /// Get default QCP solver
    std::string QCP();

    /// Set default QCP solver
    void setQCP(const std::string& value);

    /// Get default MIQCP solver
    std::string MIQCP();

    /// Set default MIQCP solver
    void setMIQCP(const std::string& value);

    /// Get default RMIQCP solver
    std::string RMIQCP();

    /// Set default RMIQCP solver
    void setRMIQCP(const std::string& value);

    /// Get default EMP solver
    std::string EMP();

    /// Set default EMP solver
    void setEMP(const std::string& value);

    /// Get solver by model type
    std::string getSolver(const std::string& modelType);

    /// Get solver by model type
    void setSolver(const std::string& modelType, const std::string& solver);

private:
    std::shared_ptr<GAMSOptionsImpl> mImpl;
    GAMSOptions(GAMSWorkspace workspace, const GAMSOptions* optFrom);
    GAMSOptions(GAMSWorkspace workspace, const std::string& optFile = "");
    std::string restart();
    void setRestart(const std::string& value);
    std::string input();
    void setInput(const std::string& value);
    std::string save();
    void setSave(const std::string& value);
    std::string xSave();
    void setXSave(const std::string& value);
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
    std::string scriptNext();
    void setScriptNext(const std::string& value);
    std::string scriptFrst();
    void setScriptFrst(const std::string& value);
    int logOption();
    void setLogOption(const int value);
    std::string logFile();
    void setLogFile(const std::string& value);
    int appendLog();
    void setAppendLog(const int value);
    int multiPass();
    void setMultiPass(const int value);
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
    int g205();
    void setG205(const int value);
    int noCr();
    void setNoCr(const int value);
    int fSave();
    void setFSave(const int value);
    int opt();
    void setOpt(const int value);
    int relPath();
    void setRelPath(const int value);
    int iDE();
    void setIDE(const int value);
    std::string scrExt();
    void setScrExt(const std::string& value);
    std::string procDir();
    void setProcDir(const std::string& value);
    int useDLL();
    void setUseDLL(const int value);
    int debug();
    void setDebug(const int value);
    int pID2Error();
    void setPID2Error(const int value);
    int serverRun();
    void setServerRun(const int value);
    std::string saveObfuscate();
    void setSaveObfuscate(const std::string& value);
    std::string xSaveObfuscate();
    void setXSaveObfuscate(const std::string& value);
    std::string restartNamed();
    void setRestartNamed(const std::string& value);
    int sys14();
    void setSys14(const int value);
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
    int mCPRHoldfx();
    void setMCPRHoldfx(const int value);
    std::string procDirPath();
    void setProcDirPath(const std::string& value);

};
}
#endif // GAMSOPTIONS_H
