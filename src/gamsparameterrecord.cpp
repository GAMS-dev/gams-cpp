/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
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

#include "gamsparameterrecord.h"
#include "gamssymbolrecordimpl.h"
#include "gamsparameter.h"
#include "gamssymbolimpl.h"

#include <cassert>

#include "gmdcc.h"

namespace gams {


GAMSParameterRecord::GAMSParameterRecord()
{}

GAMSParameterRecord::GAMSParameterRecord(const GAMSSymbolRecord &record)
    : GAMSSymbolRecord(record)
{
    if (record.mImpl && record.type() != GAMSEnum::SymbolType::SymTypePar)
        throw GAMSException("GAMSParameterRecord: invalid type conversion from type=" + std::to_string(record.type()));
}

GAMSParameterRecord::GAMSParameterRecord(const GAMSParameter& par, void* symIterPtr)
    : GAMSSymbolRecord(par, symIterPtr)
{ }


GAMSParameterRecord&GAMSParameterRecord::operator=(const GAMSParameterRecord& other)
{
    mImpl = other.mImpl;
    return *this;
}

double GAMSParameterRecord::value()
{
    if (!mImpl)
        throw GAMSException("GAMSParameterRecord: record has not been initialized");
    return mImpl->value();
}


void GAMSParameterRecord::setValue(const double val)
{
    if (!mImpl)
        throw GAMSException("GAMSParameterRecord: record has not been initialized");
    mImpl->setValue(val);
}

}

