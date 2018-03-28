/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMSWORKSPACEPOOL_H
#define GAMSWORKSPACEPOOL_H

#include <string>
#include <set>

namespace gams {

/// Manages a collection of all GAMS workspaces.
class GAMSWorkspacePool
{
public:

    /// Register a GAMS workspace path.
    /// \param path A GAMS workspace path.
    static void registerWorkspacePath(std::string path);

    /// Unregister a GAMS workspace path.
    /// \param path A GAMS workspace path.
    static void unregisterWorkspacePath(std::string path);

private:
    GAMSWorkspacePool();
    static std::set<std::string> mWorspacePaths;
};

} // namespace gams

#endif // GAMSWORKSPACEPOOL_H
