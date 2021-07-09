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

#include "gamssymboliter.h"
#include "gamsvariable.h"
#include "gamsequation.h"
#include "gamsset.h"
#include "gamsparameter.h"
#include "gamssymbolimpl.h"
#include "gamssymbolrecord.h"
#include "gamssymbolrecordimpl.h"

namespace gams {

template<class T>
GAMSSymbolIter<T>::GAMSSymbolIter(T& symbol, int pos) :
    mSymbol(symbol), mSymRecord(nullptr), mPos(pos), mSymIterPtr(nullptr)
{
    typedef typename T::RecordType TRecordType;
    if (!mSymbol.isValid())
        throw GAMSException("GAMSSymbolIter: The symbol has not been initialized");
    if (0 >= mPos) {
        gmdFindFirstRecord(mSymbol.mImpl->gmd(), mSymbol.mImpl->symPtr(), &mSymIterPtr);
        mSymbol.mImpl->checkForGMDError(gmdFindFirstRecord(mSymbol.mImpl->gmd(), mSymbol.mImpl->symPtr(), &mSymIterPtr)
                                        , __FILE__, __LINE__);
        mPos = 0;
    } else if (mPos >= mSymbol.numberRecords()) {
        mPos = mSymbol.numberRecords();
    } else {
        gmdFindFirstRecord(mSymbol.mImpl->gmd(), mSymbol.mImpl->symPtr(), &mSymIterPtr);
        for (int i = 0; i < mPos; i++) {
            mSymbol.mImpl->checkForGMDError(gmdRecordMoveNext(mSymbol.mImpl->gmd(), mSymIterPtr), __FILE__, __LINE__);
        }
    }
    mSymRecord = mSymIterPtr ? new TRecordType(mSymbol, mSymIterPtr) : new TRecordType;
}

template<class T>
GAMSSymbolIter<T>::~GAMSSymbolIter()
{
    if (mSymRecord) {
        delete mSymRecord;
    }
}

template<class T>
bool GAMSSymbolIter<T>::operator==(const GAMSSymbolIter<T>& other) const
{
    return !(operator !=(other));
}

template<class T>
bool GAMSSymbolIter<T>::operator!=(const GAMSSymbolIter<T>& other) const
{
    return (mSymbol.mImpl->symPtr() != other.mSymbol.mImpl->symPtr()) || (mPos != other.mPos);
}

template<class T>
typename T::RecordType& GAMSSymbolIter<T>::operator*()
{
    return *mSymRecord;
}

template<class T>
GAMSSymbolIter<T>& GAMSSymbolIter<T>::operator++()
{
    ++mPos;
    if (mPos < mSymbol.numberRecords()) {
        mSymbol.mImpl->checkForGMDError(gmdRecordMoveNext(mSymbol.mImpl->gmd(), mSymIterPtr), __FILE__, __LINE__);
    }

    return *this;
}

template
class GAMSSymbolIter < GAMSVariable >;
template
class GAMSSymbolIter < GAMSSet >;
template
class GAMSSymbolIter < GAMSParameter >;
template
class GAMSSymbolIter < GAMSEquation >;
template
class GAMSSymbolIter < GAMSSymbol >;

} // namespace gams
