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

#include "gamsmodelinstanceopt.h"
#include "gamsmodelinstanceoptimpl.h"

using namespace std;

namespace gams {


GAMSModelInstanceOpt::GAMSModelInstanceOpt(string solver, int optfile, int noMatchLimit, bool debug)
    : mImpl(make_shared<GAMSModelInstanceOptImpl>(solver, optfile, noMatchLimit, debug))
{}

GAMSModelInstanceOpt::~GAMSModelInstanceOpt()
{}

GAMSModelInstanceOpt& GAMSModelInstanceOpt::operator=(const GAMSModelInstanceOpt& other)
{
    mImpl = other.mImpl;
    return *this;
}

bool GAMSModelInstanceOpt::operator!=(const GAMSModelInstanceOpt& other) const
{
    return !operator==(other);
}

bool GAMSModelInstanceOpt::operator==(const GAMSModelInstanceOpt& other) const
{
    return (mImpl == other.mImpl) || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}

string GAMSModelInstanceOpt::solver()
{
    return mImpl->mSolver;
}

void GAMSModelInstanceOpt::setSolver(const std::string& solver)
{
    mImpl->mSolver = solver;
}

int GAMSModelInstanceOpt::optFile()
{
    return mImpl->mOptFile;
}

void GAMSModelInstanceOpt::setOptFile(const int optfile)
{
    mImpl->mOptFile = optfile;
}

int GAMSModelInstanceOpt::noMatchLimit()
{
    return mImpl->mNoMatchLimit;
}

void GAMSModelInstanceOpt::setNoMatchLimit(const int noMatchLimit)
{
    mImpl->mNoMatchLimit = noMatchLimit;
}

bool GAMSModelInstanceOpt::debug()
{
    return mImpl->mDebug;
}

void GAMSModelInstanceOpt::setDebug(const bool debug)
{
    mImpl->mDebug = debug;
}

} // namespace gams
