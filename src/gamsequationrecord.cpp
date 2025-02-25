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

#include "gamsequationrecord.h"
#include "gamssymbolrecordimpl.h"
#include "gamssymbolimpl.h"
#include "gamsequation.h"
#include "gmdcc.h"

namespace gams {

GAMSEquationRecord::GAMSEquationRecord()
{ }

GAMSEquationRecord::GAMSEquationRecord(const GAMSSymbolRecord& record)
    : GAMSSymbolRecord(record)
{
    if (record.mImpl && record.type() != GAMSEnum::SymbolType::SymTypeEqu)
        throw GAMSException("GAMSEquationRecord: invalid type conversion from type=" + std::to_string(record.type()));
}

GAMSEquationRecord::GAMSEquationRecord(const GAMSEquation& equ, void* symIterPtr)
    : GAMSSymbolRecord(equ, symIterPtr)
{ }

GAMSEquationRecord& GAMSEquationRecord::operator=(const GAMSEquationRecord& other)
{
    mImpl = other.mImpl;
    return *this;
}

double GAMSEquationRecord::level()
{
    if (!mImpl)
        throw GAMSException("GAMSEquationRecord: record has not been initialized");
    return mImpl->level();
}

void GAMSEquationRecord::setLevel(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSEquationRecord: record has not been initialized");
    mImpl->setLevel(val);
}

double GAMSEquationRecord::marginal()
{
    if (!mImpl)
        throw GAMSException("GAMSEquationRecord: record has not been initialized");
    return mImpl->marginal();
}

void GAMSEquationRecord::setMarginal(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSEquationRecord: record has not been initialized");
    mImpl->setMarginal(val);
}

double GAMSEquationRecord::upper()
{
    if (!mImpl)
        throw GAMSException("GAMSEquationRecord: record has not been initialized");
    return mImpl->upper();
}

void GAMSEquationRecord::setUpper(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSEquationRecord: record has not been initialized");
    mImpl->setUpper(val);
}

double GAMSEquationRecord::lower()
{
    if (!mImpl)
        throw GAMSException("GAMSEquationRecord: record has not been initialized");
    return mImpl->lower();
}

void GAMSEquationRecord::setLower(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSEquationRecord: record has not been initialized");
    mImpl->setLower(val);
}

double GAMSEquationRecord::scale()
{
    if (!mImpl)
        throw GAMSException("GAMSEquationRecord: record has not been initialized");
    return mImpl->scale();
}

void GAMSEquationRecord::setScale(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSEquationRecord: record has not been initialized");
    mImpl->setScale(val);
}

}
