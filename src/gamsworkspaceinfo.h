/**
 *
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

#ifndef GAMSWORKSPACEINFO_H
#define GAMSWORKSPACEINFO_H

#include <string>
#include "gamsenum.h"

namespace gams {

class GAMSWorkspaceInfoImpl;

/// The GAMSWorkspaceInfo can be used input parameter for the GAMSWorkspace constructor.
class LIBSPEC GAMSWorkspaceInfo
{
public:
    /// Constructor
    /// \param workingDirectory GAMS working directory, anchor for all file-based operations
    ///  (determined automatically if omitted, in user's temporary folder)
    /// \param systemDirectory GAMS system directory (determined automatically if omitted)
    /// \param debug GAMS Debug Level.
    GAMSWorkspaceInfo(std::string workingDirectory = "", std::string systemDirectory = ""
            , GAMSEnum::DebugLevel debug = GAMSEnum::DebugLevel::Off);

    /// Constructor.
    /// \param debug GAMS Debug Level.
    GAMSWorkspaceInfo(GAMSEnum::DebugLevel debug);

    /// Compares two GAMSWorkspaceInfo objects.
    /// \param other Another GAMSWorkspaceInfo to compare to.
    /// \return Returns <c>true</c> if the two GAMSWorkspaceInfo are equal; otherwise <c>false</c>.
    bool operator==(const GAMSWorkspaceInfo& other) const;

    /// Compares two GAMSWorkspaceInfo objects.
    /// \param other Another GAMSWorkspaceInfo to compare to.
    /// \return Returns <c>true</c> if the two GAMSWorkspaceInfo are different; otherwise <c>false</c>.
    bool operator!=(const GAMSWorkspaceInfo& other) const;

    /// Get GAMS working directory, i.e. the anchor for all file-based operations.
    std::string workingDirectory() const;

    /// Set GAMS working directory, i.e. the anchor for all file-based operations.
    void setWorkingDirectory(std::string workingDir);

    /// Get GAMS system directory.
    std::string systemDirectory() const;

    /// Set GAMS system directory.
    void setSystemDirectory(std::string systemDir);

    /// Get GAMS debug level.
    GAMSEnum::DebugLevel debug() const;

    /// Set GAMS debug level.
    void setDebug(GAMSEnum::DebugLevel deb);

private:
    std::shared_ptr<GAMSWorkspaceInfoImpl> mImpl;
};

} // namespace gams

#endif // GAMSWORKSPACEINFO_H
