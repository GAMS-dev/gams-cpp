/**
 * GAMS C++ API
 *
 * Copyright (c) 2017-2025 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2025 GAMS Development Corp. <support@gams.com>
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
#include "gamscheckpointimpl.h"
#include "gamscheckpoint.h"
#include "gamslog.h"
#include "gamspath.h"
#include "gamsexception.h"
#include <iostream>

namespace gams {

using namespace std;

GAMSCheckpointImpl::GAMSCheckpointImpl(const GAMSWorkspace& workspace, const string& checkpointName)
   : mWs(workspace), mName { checkpointName.empty() ? filesystem::path{workspace.registerCheckpoint()}.filename().string() : checkpointName }
{
    DEB << "---- Entering GAMSCheckpointImpl constructor ----";

    GAMSPath cpFilePath(mName);
    if (!cpFilePath.is_absolute())
      cpFilePath = GAMSPath(mWs.workingDirectory()) / mName;
    cpFilePath.setSuffix(".g00");
    mCheckpointFileName = cpFilePath.toStdString();
}

GAMSCheckpointImpl::~GAMSCheckpointImpl()
{
    DEB << "---- Entering GAMSCheckpointImpl destructor ----";
}

bool GAMSCheckpointImpl::operator!=(const GAMSCheckpointImpl& other) const
{
    return (mWs != other.mWs) || (mName != other.mName);
}

bool GAMSCheckpointImpl::operator==(const GAMSCheckpointImpl& other) const
{
    return !(operator!=(other));
}

GAMSModelInstance GAMSCheckpointImpl::addModelInstance(GAMSCheckpoint& cp, const string& modelInstanceName)
{
    std::string miName = mWs.registerModelInstance(modelInstanceName);
    if (miName.empty())
        throw GAMSException("ModelInstance with name " + modelInstanceName + " already exists");
    return GAMSModelInstance(cp, miName);
}

}
