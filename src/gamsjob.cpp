/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2023 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2023 GAMS Development Corp. <support@gams.com>
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

#include "gamsjobimpl.h"
#include "gamsjob.h"
#include "gamscheckpoint.h"

using namespace std;

namespace gams {

string GAMSJob::name()
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    return mImpl->mJobName;
}

GAMSWorkspace GAMSJob::workspace()
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    return GAMSWorkspace(mImpl->mWs);
}


GAMSJob::GAMSJob(const std::shared_ptr<GAMSJobImpl>& impl)
    :mImpl(impl)
{
}

GAMSJob::GAMSJob(GAMSWorkspace& ws, const string& jobName, const string& fileName, const GAMSCheckpoint* checkpoint)
    : mImpl(make_shared<GAMSJobImpl>(ws, jobName, fileName, checkpoint))
{}

GAMSJob::GAMSJob()
{}

bool GAMSJob::isValid() const
{
    return bool(mImpl);
}

void GAMSJob::run()
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    mImpl->run();
}

void GAMSJob::run(std::ostream& outstream)
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    mImpl->run(nullptr, nullptr, &outstream, true);
}

void GAMSJob::run(GAMSOptions& gamsOptions)
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    mImpl->run(&gamsOptions);
}

void GAMSJob::run(GAMSOptions& gamsOptions, std::ostream& outstream)
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    mImpl->run(&gamsOptions, nullptr, &outstream, true);
}

void GAMSJob::run(GAMSOptions& gamsOptions, GAMSDatabase db)
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    mImpl->run(&gamsOptions, nullptr, nullptr, true, vector<GAMSDatabase>{db} );
}

void GAMSJob::run(GAMSCheckpoint gamsCheckpoint)
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    mImpl->run(nullptr, (gamsCheckpoint.isValid() ? &gamsCheckpoint : nullptr), nullptr, true);
}

void GAMSJob::run(GAMSOptions& gamsOptions, GAMSCheckpoint gamsCheckpoint)
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    mImpl->run(&gamsOptions, (gamsCheckpoint.isValid() ? &gamsCheckpoint : nullptr), nullptr, true);
}

void GAMSJob::run(GAMSOptions& gamsOptions, GAMSCheckpoint gamsCheckpoint, std::ostream& output, bool createOutDB
                  , std::vector<GAMSDatabase> databases)
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    mImpl->run(&gamsOptions, (gamsCheckpoint.isValid() ? &gamsCheckpoint : nullptr), &output, createOutDB, databases);
}

void GAMSJob::runEngine(GAMSEngineConfiguration engineConfiguration, GAMSOptions& gamsOptions,
                        GAMSCheckpoint* checkpoint, std::ostream* output,
                        std::set<std::string> extraModelFiles, std::unordered_map<std::string, std::string> engineOptions,
                        bool createOutDB, bool removeResults, std::vector<gams::GAMSDatabase> databases)
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    mImpl->runEngine(engineConfiguration, gamsOptions, checkpoint, output, extraModelFiles,
                     engineOptions, createOutDB, removeResults, databases);
}

GAMSJob::~GAMSJob()
{}

GAMSJob GAMSJob::operator=(const GAMSJob& other)
{
    mImpl = other.mImpl;
    return *this;
}

bool GAMSJob::operator!=(const GAMSJob& other) const
{
    return !operator==(other);
}

bool GAMSJob::operator==(const GAMSJob& other) const
{
    return mImpl == other.mImpl || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}

GAMSDatabase GAMSJob::outDB()
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    return mImpl->outDB();
}

bool GAMSJob::interrupt()
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    return mImpl->interrupt();
}

LogId GAMSJob::logID()
{
    if (!mImpl) throw GAMSException("GAMSJob: This job has not been initialized.");
    return mImpl->logID();
}

}
