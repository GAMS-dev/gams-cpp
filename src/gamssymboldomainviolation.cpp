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

#include "gamssymboldomainviolation.h"
#include "gamssymboldomainviolationimpl.h"
#include "gamsexception.h"

namespace gams {

GAMSSymbolDomainViolation::GAMSSymbolDomainViolation()
{}

GAMSSymbolDomainViolation& GAMSSymbolDomainViolation::operator=(const GAMSSymbolDomainViolation& other)
{
    mImpl = other.mImpl;
    return *this;
}

bool GAMSSymbolDomainViolation::isValid() const
{
    return bool(mImpl);
}

bool GAMSSymbolDomainViolation::operator!=(const GAMSSymbolDomainViolation& other) const
{
    return !operator==(other);
}

bool GAMSSymbolDomainViolation::operator==(const GAMSSymbolDomainViolation& other) const
{
    return mImpl == other.mImpl || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}

GAMSSymbolRecord GAMSSymbolDomainViolation::violRec()
{
    if (!mImpl) throw GAMSException("GAMSSymbolDomainViolation: The violation has not been initialized.");
    return mImpl->mViolRec;
}

std::vector<bool> GAMSSymbolDomainViolation::violInd()
{
    if (!mImpl) throw GAMSException("GAMSSymbolDomainViolation: The violation has not been initialized.");
    return mImpl->mViolInd;
}

GAMSSymbolDomainViolation::GAMSSymbolDomainViolation(const std::shared_ptr<GAMSSymbolDomainViolationImpl> impl)
    : mImpl(impl)
{}

GAMSSymbolDomainViolation::GAMSSymbolDomainViolation(const std::vector<bool>& violInd, const GAMSSymbolRecord &violRec)
    : mImpl(std::make_shared<GAMSSymbolDomainViolationImpl>(violInd, violRec))
{}


} // namespace gams
