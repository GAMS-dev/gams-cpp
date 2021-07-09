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

#include "gamsdomain.h"
#include "gamsexception.h"
#include "gamsdomainimpl.h"

namespace gams {

GAMSDomain::GAMSDomain()
{}

GAMSDomain::GAMSDomain(const GAMSSet& set)
    : mImpl(std::make_shared<GAMSDomainImpl>(set))
{
    if (!set.isValid())
        throw GAMSException("GAMSDomain: domain can't be created with invalid GAMSSet");
}

GAMSDomain::GAMSDomain(const std::string& relaxedName)
    : mImpl(std::make_shared<GAMSDomainImpl>(relaxedName))
{}

GAMSDomain::GAMSDomain(const char* relaxedName)
    : GAMSDomain(std::string(relaxedName))
{}

GAMSDomain::GAMSDomain(const GAMSDomain& other)
    : mImpl(other.mImpl)
{}

GAMSDomain& GAMSDomain::operator=(const GAMSSet& set)
{
    if (!set.isValid())
        throw GAMSException("GAMSDomain: domain can't be created with invalid GAMSSet");
    mImpl = std::make_shared<GAMSDomainImpl>(set);
    return *this;
}

GAMSDomain& GAMSDomain::operator=(const std::string& relaxedName)
{
    mImpl = std::make_shared<GAMSDomainImpl>(relaxedName);
    return *this;
}

GAMSDomain&GAMSDomain::operator=(const char* relaxedName)
{
    return operator =(std::string(relaxedName));
}

GAMSDomain& GAMSDomain::operator=(const GAMSDomain& other)
{
    mImpl = other.mImpl;
    return *this;
}

bool GAMSDomain::operator!=(const GAMSDomain& other) const
{
    return !(operator==(other));
}

bool GAMSDomain::operator==(const GAMSDomain& other) const
{
    return mImpl == other.mImpl || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}

const std::string& GAMSDomain::name() const
{
    if (!isValid())
        throw GAMSException("GAMSDomain: domain has not been initialized");
    if (mImpl->mSet.isValid())
        return mImpl->mSet.name();
    else
        return mImpl->mDomain;
}

GAMSSet GAMSDomain::getSet() const
{
    if (!isValid())
        throw GAMSException("GAMSDomain: domain has not been initialized");
    if (!mImpl->mSet.isValid())
        throw GAMSException("GAMSDomain: can't convert a relaxed domain to GAMSSet");
    return mImpl->mSet;
}

bool GAMSDomain::isRelaxed() const
{
    if (!isValid())
        throw GAMSException("GAMSDomain: domain has not been initialized");
    return !mImpl->mSet.isValid();
}

bool GAMSDomain::isValid() const
{
    return bool(mImpl);
}

} // namespace gams
