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

#include "gamsdatabasedomainviolation.h"
#include "gamsdatabasedomainviolationimpl.h"
#include "gamsexception.h"

using namespace std;

namespace gams {

GAMSDatabaseDomainViolation::GAMSDatabaseDomainViolation()
{}

GAMSDatabaseDomainViolation& GAMSDatabaseDomainViolation::operator=(const GAMSDatabaseDomainViolation& other)
{
    mImpl = other.mImpl;
    return *this;
}

bool GAMSDatabaseDomainViolation::isValid() const
{
    return bool(mImpl);
}

bool GAMSDatabaseDomainViolation::operator!=(const GAMSDatabaseDomainViolation& other) const
{
    return !(operator==(other));
}

bool GAMSDatabaseDomainViolation::operator==(const GAMSDatabaseDomainViolation& other) const
{
    return mImpl == other.mImpl || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}

gams::GAMSSymbol GAMSDatabaseDomainViolation::violSym()
{
    if (!mImpl) throw GAMSException("GAMSDatabaseDomainViolation: The violation has not been initialized.");
    return mImpl->mViolSym;
}

const std::vector<GAMSSymbolDomainViolation> &GAMSDatabaseDomainViolation::violRecs() const
{
    if (!mImpl) throw GAMSException("GAMSDatabaseDomainViolation: The violation has not been initialized.");
    return mImpl->mViolRecs;
}

GAMSDatabaseDomainViolation::GAMSDatabaseDomainViolation(const std::shared_ptr<GAMSDatabaseDomainViolationImpl>& impl)
    : mImpl(impl)
{}

GAMSDatabaseDomainViolation::GAMSDatabaseDomainViolation(GAMSSymbol violSym, vector<GAMSSymbolDomainViolation> violRecs)
    : mImpl(make_shared<GAMSDatabaseDomainViolationImpl>(violSym, violRecs))
{}


} // namespace gams
