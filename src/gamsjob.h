/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2024 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2024 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMSJOB_H
#define GAMSJOB_H

#include "gamslib_global.h"
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include "gamsengineconfiguration.h"

namespace gams {

class GAMSWorkspace;
class GAMSModelInstance;
class GAMSJobImpl;
class GAMSOptions;
class GAMSCheckpoint;
class GAMSDatabase;

/// <p>The GAMSJob class manages the execution of a GAMS program given by GAMS model
/// source. The GAMS source (or more precisely the root of a model source tree) of
/// the job can be provided as a string or by a filename (relative to the working
/// directory of the GAMSWorkspace) of a text file containing the GAMS model source.
/// The Run method organizes the export of the input GAMSDatabases, calls the GAMS
/// compiler and execution system with the supplied options and on successful
/// completion provides through the method OutDB (of type GAMSDatabase) the
/// results of the model run.</p>
/// <p>While the result data is captured in a GAMSDatabase, the Run method can also
/// create a GAMSCheckpoint that not only captures data but represents the state of
/// the entire GAMSJob and allows some other GAMSJob to continue from this state.
/// In case of a compilation or execution error, the Run method will throw an
/// exception. If the log output of GAMS is of interest, it can be captured by
/// providing a TextWriter instance.</p>
class LIBSPEC GAMSJob
{
public:
    /// Standard constructor.
    GAMSJob();

    /// Desctructor.
    ~GAMSJob();

    /// Assigns a GAMSJob.
    /// \param other Another GAMSJob used as data source.
    /// \return Returns the assigned GAMSJob (*this).
    GAMSJob operator=(const GAMSJob& other);

    /// Compares two GAMSJob objects.
    /// \param other Another GAMSJob to compare to.
    /// \return Returns <c>true</c> if the two GAMSJob are different; otherwise <c>false</c>.
    bool operator!=(const GAMSJob& other) const;

    /// Compares two GAMSJob objects.
    /// \param other Another GAMSJob to compare to.
    /// \return Returns <c>true</c> if the two GAMSJob are equal; otherwise <c>false</c>.
    bool operator==(const GAMSJob& other) const;

    ///Check if a GAMSDatabaseDomainViolation is valid.
    /// \return Returns <c>true</c> if the GAMSCheckpoint is valid; otherwise <c>false</c>.
    bool isValid() const;

    //TODO: complete run methods

    /// Run GAMSJob.
    void run();

    /// Run GAMSJob.
    /// \param outstream Stream to capture GAMS log.
    void run(std::ostream& outstream);

    /// Run GAMSJob.
    /// \param gamsOptions GAMSOptions to control the GAMSJob.
    void run(GAMSOptions& gamsOptions);

    /// Run GAMSJob.
    /// \param gamsOptions GAMSOptions to control the GAMSJob.
    /// \param outstream Stream to capture GAMS log.
    void run(GAMSOptions& gamsOptions, std::ostream& outstream);

    /// Run GAMSJob.
    /// \param gamsOptions GAMSOptions to control the GAMSJob.
    /// \param db GAMSDatabase read by the GAMSJob.
    void run(GAMSOptions& gamsOptions, GAMSDatabase db);

    /// Run GAMSJob.
    /// \param gamsCheckpoint GAMSCheckpoint to be created by GAMSJob.
    void run(GAMSCheckpoint gamsCheckpoint);

    /// Run GAMSJob.
    /// \param gamsOptions GAMSOptions to control the GAMSJob.
    /// \param gamsCheckpoint GAMSCheckpoint to be created by GAMSJob.
    void run(GAMSOptions& gamsOptions, GAMSCheckpoint gamsCheckpoint);

    /// Run GAMSJob.
    /// \param gamsOptions GAMSOptions to control the GAMSJob.
    /// \param gamsCheckpoint GAMSCheckpoint to be created by GAMSJob.
    /// \param output Stream to capture GAMS log.
    /// \param createOutDB Flag that activates writing to outDB.
    /// \param databases GAMSDatabases read by the GAMSJob.
    void run(GAMSOptions& gamsOptions, const GAMSCheckpoint &gamsCheckpoint, std::ostream& output,
             bool createOutDB, const std::vector<gams::GAMSDatabase> &databases);

    /// Run GAMSJob on GAMS Engine
    /// \param engineConfiguration GAMSEngineConfiguration object
    /// \param extraModelFiles Set of additional file paths (apart from main file) required to run the model (e.g. include files)
    /// \param engineOptions Dictionary of GAMS Engine options to control job execution
    /// \param gamsOptions GAMS options to control job
    /// \param checkpoint GAMSCheckpoint to be created by GAMSJob
    /// \param output Stream to capture GAMS log
    /// \param createOutDB Flag to define if OutDB should be created
    /// \param removeResults Remove results from GAMS Engine after downloading them
    /// \param databases GAMSDatabases read by the GAMSJob
    void runEngine(const GAMSEngineConfiguration &engineConfiguration, GAMSOptions* gamsOptions,
                   gams::GAMSCheckpoint *checkpoint, std::ostream* output = nullptr,
                   const std::vector<gams::GAMSDatabase> &databases = {},
                   const std::set<std::string> &extraModelFiles = {},
                   const std::unordered_map<std::string, std::string> &engineOptions = {},
                    bool createOutDB = true, bool removeResults = true);

    /// Get GAMSDatabase created by Run method
    GAMSDatabase outDB();

    /// Retrieve name of GAMSJob
    std::string name();

    /// Get GAMSWorkspace containing GAMSJob
    GAMSWorkspace workspace();

    /// Send interrupt signal to running GAMSJob
    /// Note: On Mac OS using Mono this call requires the tool pstree to be installed
    /// @returns Returns <c>true</c> when the interrupt signal was sent and <c>false</c> when there is no Job running
    bool interrupt();

    /// Get the GAMS log ID.
    /// \return Returns the GAMS log ID.
    LogId logID();

private:
    friend class GAMSWorkspaceImpl;
    GAMSJob(const std::shared_ptr<::gams::GAMSJobImpl>& impl);
    GAMSJob(gams::GAMSWorkspace &ws, const std::string& jobName = "", const std::string& fileName = ""
            , const GAMSCheckpoint *checkpoint = 0);

private:
    std::shared_ptr<::gams::GAMSJobImpl> mImpl;
};

} // namespace gams

#endif // GAMSJOB_H
