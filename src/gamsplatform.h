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

#ifndef GAMSPLATFORM_H
#define GAMSPLATFORM_H

#include "gamslib_global.h"
#include <thread>

namespace gams {

extern LIBSPEC const char cEnvSep;
extern LIBSPEC const char *cLibEnv;
extern LIBSPEC const char *cPathSep;
extern LIBSPEC const char *cCmpFileName;
extern LIBSPEC const int cExitCodeDiv;
extern LIBSPEC const char *cExeSuffix;
extern LIBSPEC const char *cLibPrefix;
extern LIBSPEC const char *cLibSuffix;

/// Encapsulates all platform specific calls of the API.
struct LIBSPEC GAMSPlatform
{
    // TODO(RG): double check if this is accurate:
    /// find GAMS system directory from related environment.
    ///  - on windows : first search "PATH" if not found then search windows registry "gams.location",
    ///  - on Mac : first search "PATH" if not found then "DYLD_LIBRARAY_PATH",
    ///  - on other platforms : first search "PATH" if not found then "LD_LIBRARAY_PATH".
    /// After two attempts of search and GAMS directory is still not found, returns null String.
    /// \returns  GAMS directory if found, null otherwise.
    static std::string findGams(LogId logId = 0);

    /// Ensures that the directory name is contained in the environment path.
    /// \param dirName A directory name.
    static void ensureEnvPathContains(const char *dirName);

    /// Interrupt GAMS during processing.
    /// \param pid A process ID.
    /// \return Returns <c>true</c> if the interrupt was called successfully;
    ///         otherwise <c>false</c>.
    /// \remark On unix like platfoms this function calls gams to execute
    ///         the interrupt and waits unitl gams has finished.
    static bool interrupt(long pid);

    ///
    /// Runs a process in a specified location.
    /// \param where Directory of execution.
    /// \param what Name of process to run.
    /// \param args Process arguments.
    /// \param output Process output as string.
    /// \return Exit code.
    ///
    static int runProcess(const std::string where, const std::string what, const std::string args, std::string& output);

    ///
    /// Runs a process without waiting for it to end. Returns process id of started process.
    /// \brief runProcessParallel
    /// \param where Directory of execution.
    /// \param what Name of process to run.
    /// \param args Process arguments.
    /// \return Process ID
    ///
    static std::thread runProcessParallel(const std::string executable, const std::string args);
private:
    GAMSPlatform() {}

    static std::string findGamsOnUnix(LogId logId);
    static void ensureEnvPathSetOnUnix(const char *dirName);
    static bool interruptOnNonWindows(long pid);

    static std::string findGamsOnWindows(LogId logId);
    static void ensureEnvPathSetOnWindows(const char *dirName);
    static bool interruptOnWindows(long pid);
};

} // namespace gams

#endif // GAMSPLATFORM_H
