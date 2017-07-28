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

#include "gamsworkspaceinfo.h"
#include "gamsworkspaceinfoimpl.h"

namespace gams {

GAMSWorkspaceInfo::GAMSWorkspaceInfo(std::string workingDirectory, std::string systemDirectory, GAMSEnum::DebugLevel debug)
    : mImpl(std::make_shared<GAMSWorkspaceInfoImpl>(workingDirectory, systemDirectory, debug))
{ }

GAMSWorkspaceInfo::GAMSWorkspaceInfo(GAMSEnum::DebugLevel debug)
    : GAMSWorkspaceInfo("","",debug)
{ }

bool GAMSWorkspaceInfo::operator==(const GAMSWorkspaceInfo& other) const
{
    return mImpl->mWorkingDirectory == other.mImpl->mWorkingDirectory
            && mImpl->mSystemDirectory == other.mImpl->mSystemDirectory
            && mImpl->mDebug == other.mImpl->mDebug;
}

bool GAMSWorkspaceInfo::operator!=(const GAMSWorkspaceInfo& other) const
{
    return !operator==(other);
}

std::string GAMSWorkspaceInfo::workingDirectory() const
{
    return mImpl->mWorkingDirectory;
}

void GAMSWorkspaceInfo::setWorkingDirectory(std::string workingDir)
{
    mImpl->mWorkingDirectory = workingDir;
}

std::string GAMSWorkspaceInfo::systemDirectory() const
{
    return mImpl->mSystemDirectory;
}

void GAMSWorkspaceInfo::setSystemDirectory(std::string systemDir)
{
    mImpl->mSystemDirectory = systemDir;
}

GAMSEnum::DebugLevel GAMSWorkspaceInfo::debug() const
{
    return mImpl->mDebug;
}

void GAMSWorkspaceInfo::setDebug(GAMSEnum::DebugLevel deb)
{
    mImpl->mDebug = deb;
}

} // namespace gams
