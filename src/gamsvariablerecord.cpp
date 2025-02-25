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

#include "gamsvariablerecord.h"
#include "gamssymbolrecordimpl.h"
#include "gamsvariable.h"
#include "gamssymbolimpl.h"
#include "gmdcc.h"

namespace gams{

GAMSVariableRecord::GAMSVariableRecord()
{ }

GAMSVariableRecord::GAMSVariableRecord(const GAMSSymbolRecord& record)
    : GAMSSymbolRecord(record)
{
    if (record.mImpl && record.type() != GAMSEnum::SymbolType::SymTypeVar)
        throw GAMSException("GAMSVariableRecord: invalid type conversion from type=" + std::to_string(record.type()));
}

GAMSVariableRecord::GAMSVariableRecord(const GAMSVariable& symbol, void* symIterPtr)
    : GAMSSymbolRecord(symbol, symIterPtr)
{ }

GAMSVariableRecord&GAMSVariableRecord::operator=(const GAMSVariableRecord& other)
{
    mImpl = other.mImpl;
    return *this;
}

double GAMSVariableRecord::level()
{
    if (!mImpl)
        throw GAMSException("GAMSVariableRecord: record has not been initialized");
    return mImpl->level();
}

void GAMSVariableRecord::setLevel(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSVariableRecord: record has not been initialized");
    mImpl->setLevel(val);
}

double GAMSVariableRecord::marginal()
{
    if (!mImpl)
        throw GAMSException("GAMSVariableRecord: record has not been initialized");
    return mImpl->marginal();
}

void GAMSVariableRecord::setMarginal(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSVariableRecord: record has not been initialized");
    mImpl->setMarginal(val);
}

double GAMSVariableRecord::upper()
{
    if (!mImpl)
        throw GAMSException("GAMSVariableRecord: record has not been initialized");
    return mImpl->upper();
}

void GAMSVariableRecord::setUpper(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSVariableRecord: record has not been initialized");
    mImpl->setUpper(val);
}

double GAMSVariableRecord::lower()
{
    if (!mImpl)
        throw GAMSException("GAMSVariableRecord: record has not been initialized");
    return mImpl->lower();
}

void GAMSVariableRecord::setLower(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSVariableRecord: record has not been initialized");
    mImpl->setLower(val);
}

double GAMSVariableRecord::scale()
{
    if (!mImpl)
        throw GAMSException("GAMSVariableRecord: record has not been initialized");
    return mImpl->scale();
}

void GAMSVariableRecord::setScale(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSVariableRecord: record has not been initialized");
    mImpl->setScale(val);
}

}
