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

#include "gamsdatabaseiter.h"
#include "gamsexception.h"
#include "gamsdatabase.h"
#include "gamsdatabaseimpl.h"
#include <memory>

using namespace std;

namespace gams {

GAMSDatabaseIter::GAMSDatabaseIter(shared_ptr<GAMSDatabaseImpl> database, int pos)
    :mDatabase(database), pos(pos)
{
    if (!mDatabase) throw GAMSException("GAMSDatabaseIter: The database has not been initialized.");
}

bool GAMSDatabaseIter::operator==(const GAMSDatabaseIter& other)
{
    return (mDatabase == other.mDatabase) && (pos == other.pos);
}

bool GAMSDatabaseIter::operator!=(const GAMSDatabaseIter& other)
{
    return (mDatabase != other.mDatabase) || (pos != other.pos);
}

GAMSSymbol GAMSDatabaseIter::operator*()
{
    GAMSDatabase db = GAMSDatabase(mDatabase);
    return mDatabase->getSymbolByIndex(db, pos);
}

GAMSDatabaseIter& GAMSDatabaseIter::operator++()
{
    pos++;
    return *this;
}

} // namespace gams
