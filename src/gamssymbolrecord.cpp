/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2024 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2024 GAMS Development Corp. <support@gams.com>
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

#include "gamssymbolrecord.h"
#include "gamssymbolrecordimpl.h"
#include "gamssymbolimpl.h"

#include <stdio.h>
#include <vector>
#include "gmdcc.h"

using namespace std;
namespace gams {

GAMSSymbolRecord::GAMSSymbolRecord()
{}

GAMSSymbolRecord::GAMSSymbolRecord(const GAMSSymbolRecord& record)
    : mImpl(record.mImpl)
{}

GAMSSymbolRecord& GAMSSymbolRecord::operator =(const GAMSSymbolRecord& other)
{
    mImpl = other.mImpl;
    return *this;
}

bool GAMSSymbolRecord::operator!=(const GAMSSymbolRecord& other) const
{
    return !operator==(other);
}

bool GAMSSymbolRecord::operator==(const GAMSSymbolRecord& other) const
{
    return mImpl == other.mImpl || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}

GAMSSymbolRecord::GAMSSymbolRecord(GAMSSymbol symbol, void* symIterPtr)
    : mImpl(make_shared<GAMSSymbolRecordImpl>(symbol, symIterPtr))
{}

GAMSSymbolRecord::~GAMSSymbolRecord()
{}

bool GAMSSymbolRecord::isValid() const
{
    return (mImpl && mImpl->isValid());
}

bool GAMSSymbolRecord::moveNext()
{
    if (!mImpl)
        throw GAMSException("GAMSSymbolRecord: record has not been initialized");
    return mImpl->moveNext();
}

bool GAMSSymbolRecord::movePrevious()
{
    if (!mImpl)
        throw GAMSException("GAMSSymbolRecord: record has not been initialized");
    return mImpl->movePrevious();
}

vector<string> GAMSSymbolRecord::keys()
{
    if (!mImpl)
        throw GAMSException("GAMSSymbolRecord: record has not been initialized");
    return mImpl->keys();
}

std::string GAMSSymbolRecord::key(int index)
{
    if (!mImpl)
        throw GAMSException("GAMSSymbolRecord: record has not been initialized");
    return mImpl->key(index);
}

std::string GAMSSymbolRecord::operator[](int i)
{
    return key(i);
}

LogId GAMSSymbolRecord::logID()
{
    if (!mImpl)
        throw GAMSException("GAMSSymbolRecord: record has not been initialized");
    return mImpl->logID();
}

GAMSEnum::SymbolType GAMSSymbolRecord::type() const
{
    if (!mImpl) throw GAMSException("GAMSSymbolRecord: The record has not been initialized.");
    return mImpl->mSymbol.type();
}

} // namespace gams
