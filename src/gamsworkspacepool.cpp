/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2021 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2021 GAMS Development Corp. <support@gams.com>
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

#include "gamsworkspacepool.h"
#include <algorithm>
#include "gamsexception.h"
#include "gamspath.h"

namespace gams {

std::set<std::string> GAMSWorkspacePool::mWorspacePaths;

void GAMSWorkspacePool::registerWorkspacePath(std::string path)
{
    // Create unique lockfile in path (throw on missing write access).
    GAMSPath lockPath = GAMSPath(path).tempFile("gams.lock");
    if (!lockPath.exists())
        throw GAMSException("GAMSWorkspacePool: failed to create lockfile in path " + path);

    // Iterate through the paths for the lockfile (throw if exists).
    for (auto iPath = mWorspacePaths.cbegin(); iPath != mWorspacePaths.cend(); ++iPath) {
        GAMSPath checkFile = GAMSPath(*iPath) / lockPath.string();
        if (checkFile.exists()) {
            lockPath.remove();
            if (path != *iPath)
                throw GAMSException("GAMSWorkspacePool: path " + path + " is already in use as " + *iPath);
            else
                throw GAMSException("GAMSWorkspacePool: path " + path + " is already in use.");
        }
    }
    mWorspacePaths.insert(path);
    // Remove lockfile and add path to paths.
    lockPath.remove();
}

void GAMSWorkspacePool::unregisterWorkspacePath(std::string path)
{
    auto iPath = mWorspacePaths.find(path);
    if (iPath != mWorspacePaths.end()) {
        mWorspacePaths.erase(iPath);
    }
}

GAMSWorkspacePool::GAMSWorkspacePool()
{}

} // namespace gams
