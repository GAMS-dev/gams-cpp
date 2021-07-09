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

#include "gamssymbolrecordimpl.h"
#include "gamssymbolimpl.h"
#include "gmdcc.h"

using namespace std;

namespace gams {


GAMSSymbolRecordImpl::GAMSSymbolRecordImpl(GAMSSymbol symbol, void* symIterPtr)
    : mSymbol(symbol), mSymIterPtr(symIterPtr)
{}

GAMSSymbolRecordImpl::~GAMSSymbolRecordImpl()
{
    if (isValid())
        gmdFreeSymbolIterator(mSymbol.mImpl->gmd(), mSymIterPtr);
}

bool GAMSSymbolRecordImpl::isValid() const
{
    return mSymbol.isValid();
}

bool GAMSSymbolRecordImpl::operator!=(const GAMSSymbolRecordImpl& other) const
{
    return !operator==(other);
}

bool GAMSSymbolRecordImpl::operator==(const GAMSSymbolRecordImpl& other) const
{
    return 1 == gmdSameRecord(mSymbol.mImpl->gmd(), mSymIterPtr, other.mSymIterPtr);
}

bool GAMSSymbolRecordImpl::moveNext()
{
    return 1 == gmdRecordMoveNext(mSymbol.mImpl->gmd(), mSymIterPtr);
}

bool GAMSSymbolRecordImpl::movePrevious()
{
    return 1 == gmdRecordHasPrev(mSymbol.mImpl->gmd(), mSymIterPtr);
}

std::string GAMSSymbolRecordImpl::text()
{
    char result[GMS_SSSIZE];
    mSymbol.mImpl->checkForGMDError(gmdGetElemText(mSymbol.mImpl->gmd(), mSymIterPtr, result), __FILE__, __LINE__);
    return result;
}

void GAMSSymbolRecordImpl::setText(const std::string& text)
{
    mSymbol.mImpl->checkForGMDError(gmdSetElemText(mSymbol.mImpl->gmd(), mSymIterPtr, text.c_str()),
                                    __FILE__, __LINE__);
}

double GAMSSymbolRecordImpl::level()
{
    double val;
    mSymbol.mImpl->checkForGMDError(gmdGetLevel(mSymbol.mImpl->gmd(), mSymIterPtr, &val), __FILE__, __LINE__);
    return val;
}

void GAMSSymbolRecordImpl::setLevel(const double val)
{
    mSymbol.mImpl->checkForGMDError(gmdSetLevel(mSymbol.mImpl->gmd(), mSymIterPtr, val), __FILE__, __LINE__);
}

double GAMSSymbolRecordImpl::marginal()
{
    double val;
    mSymbol.mImpl->checkForGMDError(gmdGetMarginal(mSymbol.mImpl->gmd(), mSymIterPtr, &val), __FILE__, __LINE__);
    return val;
}

void GAMSSymbolRecordImpl::setMarginal(const double val)
{
    mSymbol.mImpl->checkForGMDError(gmdSetMarginal(mSymbol.mImpl->gmd(), mSymIterPtr, val), __FILE__, __LINE__);
}

double GAMSSymbolRecordImpl::upper()
{
    double val;
    mSymbol.mImpl->checkForGMDError(gmdGetUpper(mSymbol.mImpl->gmd(), mSymIterPtr, &val), __FILE__, __LINE__);
    return val;
}

void GAMSSymbolRecordImpl::setUpper(const double val)
{
    mSymbol.mImpl->checkForGMDError(gmdSetUpper(mSymbol.mImpl->gmd(), mSymIterPtr, val), __FILE__, __LINE__);
}

double GAMSSymbolRecordImpl::lower()
{
    double val;
    mSymbol.mImpl->checkForGMDError(gmdGetLower(mSymbol.mImpl->gmd(), mSymIterPtr, &val), __FILE__, __LINE__);
    return val;
}

void GAMSSymbolRecordImpl::setLower(const double val)
{
    mSymbol.mImpl->checkForGMDError(gmdSetLower(mSymbol.mImpl->gmd(), mSymIterPtr, val), __FILE__, __LINE__);
}

double GAMSSymbolRecordImpl::scale()
{
    double val;
    mSymbol.mImpl->checkForGMDError(gmdGetScale(mSymbol.mImpl->gmd(), mSymIterPtr, &val), __FILE__, __LINE__);
    return val;
}

void GAMSSymbolRecordImpl::setScale(const double val)
{
    mSymbol.mImpl->checkForGMDError(gmdSetScale(mSymbol.mImpl->gmd(), mSymIterPtr, val), __FILE__, __LINE__);
}

double GAMSSymbolRecordImpl::value()
{
    double val;
    int rc = gmdGetLevel(mSymbol.mImpl->gmd(),mSymIterPtr, &val);
    mSymbol.mImpl->checkForGMDError(rc, __FILE__, __LINE__);
    return val;
}


void GAMSSymbolRecordImpl::setValue(const double val)
{
    int rc = gmdSetLevel(mSymbol.mImpl->gmd(),mSymIterPtr, val);
    mSymbol.mImpl->checkForGMDError(rc, __FILE__, __LINE__);
}


vector<string> GAMSSymbolRecordImpl::keys()
{
    gdxStrIndex_t resData;
    gdxStrIndexPtrs_t result;
    GDXSTRINDEXPTRS_INIT(resData, result);

    vector<string> vresult;
    mSymbol.mImpl->checkForGMDError(gmdGetKeys(mSymbol.mImpl->gmd(), mSymIterPtr, static_cast<int>(mSymbol.dim()), result)
                                    , __FILE__, __LINE__);

    for (int i = 0; i < mSymbol.dim(); i++) {
        if (result[i] != nullptr)
            vresult.push_back(string(result[i]));
    }
    return vresult;
}

std::string GAMSSymbolRecordImpl::key(int index)
{
    char result[GMS_SSSIZE];
    mSymbol.mImpl->checkForGMDError(gmdGetKey(mSymbol.mImpl->gmd(), mSymIterPtr, index, result), __FILE__, __LINE__);
    return result;
}

std::string GAMSSymbolRecordImpl::operator[](int i)
{
    return key(i);
}

LogId GAMSSymbolRecordImpl::logID()
{
    return mSymbol.logID();
}


} // namespace gams
