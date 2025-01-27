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

#include "gamscheckpoint.h"
#include "gamscheckpointimpl.h"
#include "gamsmodelinstance.h"
#include "gamsexception.h"

namespace gams {

using namespace std;

GAMSCheckpoint::GAMSCheckpoint()
{}

GAMSCheckpoint::GAMSCheckpoint(const gams::GAMSCheckpoint &other)
    : mImpl(other.mImpl)
{}

GAMSCheckpoint& GAMSCheckpoint::operator=(const GAMSCheckpoint &other)
{
    mImpl = other.mImpl;
    return *this;
}

GAMSCheckpoint::GAMSCheckpoint(const GAMSWorkspace &ws, const string &checkpointName)
    : mImpl(make_shared<GAMSCheckpointImpl>(ws, checkpointName))
{}

GAMSModelInstance GAMSCheckpoint::addModelInstance(const std::string &modelInstanceName)
{
    if (!mImpl) throw GAMSException("GAMSCheckpoint: The checkpoint has not been initialized.");
    return mImpl->addModelInstance(*this, modelInstanceName);
}

std::string GAMSCheckpoint::name()
{
    if (!mImpl) throw GAMSException("GAMSCheckpoint: The checkpoint has not been initialized.");
    return mImpl->mName;
}

GAMSWorkspace GAMSCheckpoint::workspace()
{
    if (!mImpl) throw GAMSException("GAMSCheckpoint: The checkpoint has not been initialized.");
    return mImpl->mWs;
}

LogId GAMSCheckpoint::logID()
{
    if (!mImpl) throw GAMSException("GAMSCheckpoint: The checkpoint has not been initialized.");
    return mImpl->logID();
}

bool GAMSCheckpoint::isValid() const
{
    return bool(mImpl);
}


bool GAMSCheckpoint::operator!=(const GAMSCheckpoint &other) const
{
    return !operator==(other);
}

bool GAMSCheckpoint::operator==(const GAMSCheckpoint &other) const
{
    return mImpl == other.mImpl || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}


string GAMSCheckpoint::fileName() const
{
    if (!mImpl) throw GAMSException("GAMSCheckpoint: The checkpoint has not been initialized.");
    return mImpl->mCheckpointFileName;
}

}
