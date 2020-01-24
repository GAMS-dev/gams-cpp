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
    /// Get the GAMS path.
    /// \param logId GAMS logging ID.
    /// \return Returns the path to GAMS.
    static std::string findGams(LogId logId);

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

private:
    GAMSPlatform() {}

    static std::string findGamsOnApple(LogId logId);
    static void ensureEnvPathSetOnApple(const char *dirName);
    static bool interruptOnNonWindows(long pid);

    static std::string findGamsOnLinux(LogId logId);
    static void ensureEnvPathSetOnLinux(const char *dirName);

    static std::string findGamsOnWindows(LogId logId);
    static void ensureEnvPathSetOnWindows(const char *dirName);
    static bool interruptOnWindows(long pid);
};

} // namespace gams

#endif // GAMSPLATFORM_H
