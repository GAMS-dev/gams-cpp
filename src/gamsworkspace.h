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

#ifndef GAMSWORKSPACE_H
#define GAMSWORKSPACE_H

#include <memory>
#include "gamslib_global.h"
#include "gamsenum.h"
#include "gamsworkspaceinfo.h"

namespace gams {

class GAMSDatabase;
class GAMSCheckpoint;
class GAMSOptions;
class GAMSWorkspaceImpl;
class GAMSJob;

/// <p>The GAMSWorkspace is the base class in the GAMS namespace. Most objects of the
/// GAMS namespace (e.g. GAMSDatabase and GAMSJob) have to be created by an "Add"
/// method of GAMSWorkspace.</p>
/// <p>Unless a GAMS system directory is specified during construction of
/// GAMSWorkspace, GAMSWorkspace determines the location of the GAMS installation
/// automatically. This is a source of potential  problems if more than one GAMS
/// installation exist on the machine (see below).</p>
/// <p>Furthermore, a working directory (the anchor into the file system) can be
/// provided when constructing the GAMSWorkspace instance. All file based
/// operation inside a GAMS model should be relative to this location (e.g. $GDXIN
/// and $include). There are options to add input search paths (e.g. addInputDir) and
/// output path (e.g. putDir) to specify other file system locations. If no working
/// directory is supplied, GAMSWorkspace creates a temporary folder and on
/// instance destruction removes this temporary folder.</p>
/// <p>In a typical C++ solution a single instance of GAMSWorkspace will
/// suffice, since the class is thread-safe.</p>
/// <p>Note: If you use multiple instances of the GAMSWorkspace in parallel, you should
/// avoid using the same WorkingDirectory. Otherwise you may end up with conflicting
/// file names.</p>
/// <h5>Working with different GAMS Versions on one Machine</h5>
/// <p>When creating a new instance of GAMSWorkspace, one way of defining the GAMS system
/// directory is setting the systemDirectory parameter of the constructor accordingly. If it
/// is not set, it is tried to be defined automatically. However, this can be tricky if there
/// is more than on version of GAMS installed on a machine and especially if there are
/// different applications running with different GAMS versions.</p>
/// <p>The automatic identification relies on information left in the Windows registry by the
/// GAMS installer. Hence the system directory of the last GAMS installation will be found in
/// this automatic identification step. One way of resetting the information in the registry
/// is running the executable "findthisgams.exe" from the directory that should be detected
/// automatically. While this can be done from the outside of the application it is not much
/// more convenient than the SystemDirectory argument in the GAMSWorkspace constructor. With MONO,
/// first the PATH and then the (DY)LD_LIBRARY_PATH instead of the registry is checked for a
/// valid GAMS system directory.</p>
/// <p>If one has a very structured way of organizing the GAMS installations (e.g. following
/// the GAMS default installation location) one can construct from the GAMS assembly information
/// enough information to point to the best matching GAMS system directory:</p>
/// \code{.cpp}
///   string sysdir; <br/>
///   if (IntPtr.Size == 4) <br/>
///       sysdir = @"c:\\GAMS\\win32\\" + GAMSWorkspace.apiMajorRelNumber + "." + GAMSWorkspace.apiMinorRelNumber; <br/>
///   else <br/>
///       sysdir = @"c:\\GAMS\\win64\\" + GAMSWorkspace.apiMajorRelNumber + "." + GAMSWorkspace.apiMinorRelNumber; <br/>
/// \endcode
/// <p>This avoids the automatic identification of the GAMS system directory but might be the
/// most convenient solution for systems running multiple applications using different versions
/// of the GAMS C++ API together with different versions of GAMS.</p>
class LIBSPEC GAMSWorkspace
{
public:
    /// Constructor
    GAMSWorkspace();

    /// Constructor.
    /// \param workingDir   GAMS working directory, anchor for all file-based operations (determined automatically
    ///                     if omitted, in user's temporary folder).
    /// \param systemDir    GAMS system directory (determined automatically if omitted).
    /// \param debug        GAMS debug level.
    GAMSWorkspace(const std::string& workingDir, const std::string& systemDir, GAMSEnum::DebugLevel debug);

    /// Constructor.
    /// \param workingDir   GAMS working directory, anchor for all file-based operations (determined automatically
    ///                     if omitted, in user's temporary folder).
    /// \param systemDir    GAMS system directory (determined automatically if omitted).
    GAMSWorkspace(const std::string& workingDir, const std::string& systemDir = "");

    /// Constructor.
    /// \param workingDir   GAMS working directory, anchor for all file-based operations (determined automatically
    ///                     if omitted, in user's temporary folder).
    /// \param debug        GAMS debug level.
    GAMSWorkspace(const std::string& workingDir, GAMSEnum::DebugLevel debug);

    /// Constructor.
    /// \param debug        GAMS debug level.
    GAMSWorkspace(GAMSEnum::DebugLevel debug);

    /// Copy Constructor.
    GAMSWorkspace(const GAMSWorkspace& other);

    /// Constructor.
    /// \param wsInfo <c>GAMSWorkspaceInfo</c> defining GAMS working directory,
    ///               GAMS system directory and GAMS debug level.
    GAMSWorkspace(const GAMSWorkspaceInfo& wsInfo);

    /// Destructor.
    ~GAMSWorkspace();

    /// Assigns a GAMSWorkspace.
    /// \param other Another GAMSWorkspace used as data source.
    /// \return Returns the assigned GAMSDatabase (*this).
    GAMSWorkspace& operator=(const GAMSWorkspace& other);

    /// Get the required GAMS version.
    std::string version();

    /// Get the required GAMS major version number.
    int majorRelNumber();

    /// Get the required GAMS minor version number.
    int minorRelNumber();

    /// Get the GAMS GOLD release number.
    int goldRelNumber();

    /// Get the detailed GAMS C++ API version.
    /// \return Returns the detailed API information, including major and minor
    ///         version number, build number as well as the build time stamp.
    static std::string apiVersionDetail();

    /// Get the GAMS C++ API version.
    /// \return Returns the API information, including major and
    ///         minor version number and build number.
    static std::string apiVersion();

    /// Get the GAMS C++ API major release number.
    static int apiMajorRelNumber();

    /// Get the GAMS C++ API minor release number.
    static int apiMinorRelNumber();

    /// Get the GAMS C++ API GOLD release number.
    static int apiGoldRelNumber();

    /// Get the string used to prefix automatically generated files.
    std::string scratchFilePrefix() const;

    /// Set the string used to prefix automatically generated files.
    /// \param prefix String used to prefix automatically generated files.
    void setScratchFilePrefix(std::string prefix);

    /// Get value to be stored in and read from GAMSDatabase for Epsilon.
    /// \remark Default is numeric_limits<double>::min().
    double myEPS();

    /// Set value to be stored in and read from GAMSDatabase for Epsilon.
    /// \remark Default is numeric_limits<double>::min().
    void setMyEPS(double eps);

    /// Database creation from an existing GDX file.
    /// \param gdxFileName  File to initialize Database from.
    /// \param databaseName Identifier of GAMSDatabase (determined automatically if null).
    /// \param inModelName  GAMS string constant that is used to access this database.
    /// \returns Returns the GAMSDatabase.
    /// \throws GAMSException if GAMSDatabase could not be successfully created.
    GAMSDatabase addDatabaseFromGDX(const std::string& gdxFileName, const std::string& databaseName
                                    , const std::string& inModelName);

    /// Database creation from an existing GDX file.
    /// \param gdxFileName  File to initialize Database from
    /// \param databaseName Identifier of GAMSDatabase (determined automatically if null).
    /// \returns Returns the GAMSDatabase.
    /// \throws GAMSException if GAMSDatabase could not be successfully created.
    GAMSDatabase addDatabaseFromGDX(const std::string& gdxFileName, const std::string& databaseName);

    /// Database creation from an existing GDX file.
    /// \param gdxFileName  File to initialize Database from.
    /// \returns Returns the GAMSDatabase.
    /// \throws GAMSException if GAMSDatabase could not be successfully created.
    GAMSDatabase addDatabaseFromGDX(const std::string& gdxFileName);

    // TODO(JM): experimental (keep this?)
    /// Database creation from an GMD handle.
    /// \param gmdPtr GMD low level handle
    /// \remark This is an experimental call.
    /// \return Returns the GAMSDatabase.
    GAMSDatabase addDatabaseFromGMD(void* gmdPtr);

    /// Empty database creation.
    /// \param databaseName Identifier of GAMSDatabase (determined automatically if omitted).
    /// \param inModelName  GAMS string constant that is used to access this database.
    /// \returns Returns the GAMSDatabase.
    GAMSDatabase addDatabase(const std::string& databaseName = "", const std::string& inModelName = "");

    /// Database creation from existing database.
    /// \param sourceDatabase Source GAMSDatabase to initialize Database from.
    /// \param databaseName Identifier of GAMSDatabase (determined automatically if omitted).
    /// \param inModelName GAMS string constant that is used to access this database.
    /// \returns Returns the GAMSDatabase.
    GAMSDatabase addDatabase(const GAMSDatabase& sourceDatabase, const std::string& databaseName = ""
                             , const std::string& inModelName = "");

    /// Create GAMSCheckpoint.
    /// \param checkpointName Identifier of GAMSCheckpoint or filename for existing checkpoint (determined
    ///                       automatically if omitted).
    /// \returns Return the GAMSCheckpoint.
    GAMSCheckpoint addCheckpoint(const std::string& checkpointName = "");

    /// Create GAMSJob from model file.
    /// \param fileName GAMS source file name.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromFile(const std::string& fileName, const std::string& jobName = "");

    /// Create GAMSJob from model file.
    /// \param fileName GAMS source file name.
    /// \param checkpoint GAMSCheckpoint to initialize GAMSJob from.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromFile(const std::string& fileName, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    /// Create GAMSJob from string model source.
    /// \param gamsSource GAMS model as string.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromString(const std::string& gamsSource, const std::string& jobName = "");

    /// Create GAMSJob from string model source.
    /// \param gamsSource GAMS model as string.
    /// \param checkpoint GAMSCheckpoint to initialize GAMSJob from.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromString(const std::string& gamsSource, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    /// Create GAMSJob from GAMS Model Library.
    /// \param model Model name.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromGamsLib(const std::string& model, const std::string& jobName = "");

    /// Create GAMSJob from GAMS Model Library.
    /// \param model Model name.
    /// \param checkpoint GAMSCheckpoint to initialize GAMSJob from.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromGamsLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    /// Create GAMSJob from GAMS API Library.
    /// \param model Model name.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromApiLib(const std::string& model, const std::string& jobName = "");

    /// Create GAMSJob from GAMS API Library.
    /// \param model Model name.
    /// \param checkpoint GAMSCheckpoint to initialize GAMSJob from.
    /// \param jobName Job name (determined automatically if omitted.)
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromApiLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    /// Create GAMSJob from GAMS Test Library.
    /// \param model Model name.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromTestLib(const std::string& model, const std::string& jobName = "");

    /// Create GAMSJob from GAMS Test Library.
    /// \param model Model name.
    /// \param checkpoint GAMSCheckpoint to initialize GAMSJob from.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromTestLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    /// Create GAMSJob rom Extended Math Programming Library.
    /// \param model Model name.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromEmpLib(const std::string& model, const std::string& jobName = "");

    /// Create GAMSJob from Extended Math Programming Library.
    /// \param model Model name.
    /// \param checkpoint GAMSCheckpoint to initialize GAMSJob from.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromEmpLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    /// Create GAMSJob from GAMS Data Utilities Library.
    /// \param model Model name.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromDataLib(const std::string& model, const std::string& jobName = "");

    /// Create GAMSJob from GAMS Data Utilities Library.
    /// \param model Model name.
    /// \param checkpoint GAMSCheckpoint to initialize GAMSJob from.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromDataLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    /// Create GAMSJob from Practical Financial Optimization Library.
    /// \param model Model name.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromFinLib(const std::string& model, const std::string& jobName = "");

    /// Create GAMSJob from Practical Financial Optimization Library
    /// \param model Model name.
    /// \param checkpoint GAMSCheckpoint to initialize GAMSJob from.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromFinLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    /// Create GAMSJob from GAMS Non-linear Optimization Applications Library.
    /// \param model Model name.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromNoaLib(const std::string& model, const std::string& jobName = "");

    /// Create GAMSJob from GAMS Non-linear Optimization Applications Library.
    /// \param model Model name.
    /// \param checkpoint GAMSCheckpoint to initialize GAMSJob from.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromNoaLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    /// Create GAMSJob from GAMS Power System Optimization Modelling Library.
    /// \param model Model name.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromPsoptLib(const std::string& model, const std::string& jobName = "");

    /// Create GAMSJob from GAMS Power System Optimization Modelling Library.
    /// \param model Model name.
    /// \param checkpoint GAMSCheckpoint to initialize GAMSJob from.
    /// \param jobName Job name (determined automatically if omitted).
    /// \returns Returns the GAMSJob.
    GAMSJob addJobFromPsoptLib(const std::string& model, const GAMSCheckpoint& checkpoint, const std::string& jobName = "");

    /// Create a default GAMSOptions instance.
    /// \returns Returns default GAMSOptions.
    /// \throws  GAMSException   If GAMSOptions could not be successfully created.
    GAMSOptions addOptions();

    /// Create GAMSOptions.
    /// \param optFrom GAMSOptions used to initialize the new options object.
    /// \returns Returns the GAMSOptions.
    /// \throws  GAMSException   If GAMSOptions could not be successfully created.
    GAMSOptions addOptions(const GAMSOptions& optFrom);

    /// Create GAMSOptions from an option file.
    /// \param   optFile Option file name.
    /// \returns Returns the GAMSOptions.
    /// \throws  GAMSException   If GAMSOptions could not be successfully created.
    GAMSOptions addOptions(const std::string& optFile);

    /// Retrieves model from GAMS Model Library.
    /// \param model Model name.
    void gamsLib(std::string model);

    /// Retrieves model from GAMS Test Library.
    /// \param model Model name.
    void testLib(std::string model);

    /// Retrieves model from Extended Math Programming Library.
    /// \param model Model name.
    void empLib(std::string model);

    /// Retrieves model from GAMS Data Utilities Library.
    /// \param model Model name.
    void dataLib(std::string model);

    /// Retrieves model from GAMS API Library.
    /// \param model Model name.
    void apiLib(std::string model);

    /// Retrieves model from Practical Financial Optimization Library.
    /// \param model Model name.
    void finLib(std::string model);

    /// Retrieves model from Nonlinear Optimization Applications Library.
    /// \param model Model name.
    void noaLib(std::string model);

    /// Retrieves model from Power System Optimization Modelling Library.
    /// \param model Model name.
    void psoptLib(std::string model);

    /// Get GAMS working directory, anchor for all file-based operations.
    /// \returns GAMS working directory.
    std::string workingDirectory() const;

    /// Get GAMS system directory.
    /// \returns GAMS system directory.
    std::string systemDirectory() const;

    /// Compares two GAMSWorkspace objects.
    /// \param other Another GAMSWorkspace to compare to.
    /// \return Returns <c>true</c> if the two GAMSWorkspace are equal; otherwise <c>false</c>.
    bool operator==(const GAMSWorkspace& other) const;

    /// Compares two GAMSWorkspace objects.
    /// \param other Another GAMSWorkspace to compare to.
    /// \return Returns <c>true</c> if the two GAMSWorkspace are different; otherwise <c>false</c>.
    bool operator!=(const GAMSWorkspace& other) const;

    /// Get GAMS debug level.
    GAMSEnum::DebugLevel debug();

    /// Get the GAMS log ID.
    /// \return Returns the GAMS log ID.
    LogId logID() const;

    /// Get all special values related to a system.
    double* specValues() const;

private:
    friend class GAMSModelInstanceImpl;
    friend class GAMSJobImpl;
    friend class GAMSDatabaseImpl;
    friend class GAMSCheckpointImpl;

    GAMSDatabase addDatabaseFromGDXForcedName(std::string gdxFileName, std::string databaseName, std::string inModelName);

    bool usingTmpWorkingDir() const;

    /// Add GAMSDatabase with given name to workspace
    /// \param databaseName Database name to add
    /// \returns True if everything worked, else false
    std::string registerDatabase(const std::string databaseName = "") const;
    std::string nextDatabaseName() const;

    /// Registers a GAMSCheckpoint at the workspace
    /// \param checkpointName Checkpoint name to add (empty: generate default name)
    /// \returns Checkpoint name if all worked well, else an empty string
    std::string registerCheckpoint(std::string checkpointName = "") const;

    /// Registers a ModelInstance to workspace
    /// \param databaseName Database name to add
    /// \returns True if everything worked, else false
    std::string registerModelInstance(const std::string modelInstanceName = "");

    std::string optFileExtension(int optfile); // TODO(JM) check if this method can stay here

private:
    std::shared_ptr<GAMSWorkspaceImpl> mImpl;
};

} // namespace gams

#endif // GAMSWORKSPACE_H
