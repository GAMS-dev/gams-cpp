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

#include "gamslog.h"
#include "gamsjob.h"
#include "gamsmodelinstanceimpl.h"
#include "gmdcc.h"
#include "gevmcc.h"
#include "gmomcc.h"
#include <iostream>
#include <memory>
#include <stdlib.h>

using namespace std;

namespace gams {

GAMSModelInstance::GAMSModelInstance(const std::shared_ptr<GAMSModelInstanceImpl>& impl) :mImpl(impl)
{}


GAMSModelInstance GAMSModelInstance::copyModelInstance(string modelInstanceName)
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    return GAMSModelInstance(make_shared<GAMSModelInstanceImpl>(mImpl, modelInstanceName));
}

gams::LogId GAMSModelInstance::logID()
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    return mImpl->logID();
}

GAMSModelInstance::GAMSModelInstance(GAMSCheckpoint& checkpoint, const string& modelInstanceName)
    : mImpl(make_shared<GAMSModelInstanceImpl>(checkpoint, modelInstanceName))
{ }


void GAMSModelInstance::instantiate(const std::string& modelDefinition, const GAMSOptions& options, const std::vector<GAMSModifier> &modifiers)
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    mImpl->instantiate(modelDefinition, &options, modifiers);
}


void GAMSModelInstance::instantiate(const std::string& modelDefinition, const GAMSOptions& options, const GAMSModifier& modifier1
                                    , const gams::GAMSModifier& modifier2, const gams::GAMSModifier& modifier3)
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    std::vector<GAMSModifier> modifiers;
    modifiers.push_back(modifier1);
    if (modifier2.isValid())
        modifiers.push_back(modifier2);
    if (modifier3.isValid())
        modifiers.push_back(modifier3);
    mImpl->instantiate(modelDefinition, &options, modifiers);
}

void GAMSModelInstance::instantiate(const string& modelDefinition, const std::vector<GAMSModifier>& modifiers)
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    mImpl->instantiate(modelDefinition, nullptr, modifiers);
}

void GAMSModelInstance::instantiate(const string& modelDefinition, const GAMSModifier& modifier1
                                    , const GAMSModifier& modifier2, const GAMSModifier& modifier3)
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    std::vector<GAMSModifier> modifiers;
    modifiers.push_back(modifier1);
    if (modifier2.isValid())
        modifiers.push_back(modifier2);
    if (modifier3.isValid())
        modifiers.push_back(modifier3);
    mImpl->instantiate(modelDefinition, nullptr, modifiers);
}


void GAMSModelInstance::solve(gams::GAMSEnum::SymbolUpdateType updateType, std::ostream& output, GAMSModelInstanceOpt miOpt)
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    mImpl->solve(updateType, &output, miOpt);
}

void GAMSModelInstance::solve(GAMSModelInstanceOpt miOpt)
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    mImpl->solve(GAMSEnum::SymbolUpdateType::BaseCase, nullptr, miOpt);
}

void GAMSModelInstance::solve(std::ostream& output)
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    mImpl->solve(GAMSEnum::SymbolUpdateType::BaseCase, &output, GAMSModelInstanceOpt());
}

void GAMSModelInstance::solve(GAMSEnum::SymbolUpdateType updateType)
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    mImpl->solve(updateType, nullptr, GAMSModelInstanceOpt());
}

GAMSEnum::ModelStat GAMSModelInstance::modelStatus()
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    return mImpl->modelStatus();
}

GAMSEnum::SolveStat GAMSModelInstance::solveStatus()
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    return mImpl->solveStatus();
}

std::string GAMSModelInstance::modelStatusAsString()
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    return mImpl->modelStatusAsString();
}

std::string GAMSModelInstance::solveStatusAsString()
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    return mImpl->solveStatusAsString();
}

GAMSModelInstance::GAMSModelInstance()
{}

GAMSModelInstance&GAMSModelInstance::operator=(const GAMSModelInstance& other)
{
    mImpl = other.mImpl;
    return *this;
}

bool GAMSModelInstance::operator!=(const GAMSModelInstance& other) const
{
    return !(operator==(other));
}

bool GAMSModelInstance::operator==(const GAMSModelInstance& other) const
{
    return (mImpl == other.mImpl) || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}

bool GAMSModelInstance::isValid() const
{
    return bool(mImpl);
}

GAMSDatabase GAMSModelInstance::syncDb()
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    return mImpl->mSyncDb;
}

void GAMSModelInstance::interrupt()
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    mImpl->interrupt();
}

//TODO(CW): getter methods sometimes return instances and sometimes references. make this consistent
GAMSCheckpoint GAMSModelInstance::checkpoint()
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    return mImpl->mCheckpoint;
}

std::string GAMSModelInstance::name()
{
    if (!mImpl) throw GAMSException("GAMSModelInstance: This model instance has not been initialized.");
    return mImpl->mModelInstanceName;
}

}
