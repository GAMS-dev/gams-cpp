/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>
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

#include "gamsdatabaseimpl.h"
#include "gamsmodelinstanceimpl.h"

// This implementation contains only the methods that access GAMSDatabaseImpl directly.
// It avoids including "gamsdatabaseimpl.h" in the main implementation file,
// i.e. to avoid the direct usage of *Impl-classes within foreign classes.

using namespace std;

namespace gams {


void GAMSModelInstanceImpl::checkForGMDError(int retcode, const string& file, int linenr)
{
    if (!mSyncDb.isValid())
        throw GAMSException("GAMSModelInstanceImpl: the database has not been initialized");
    mSyncDb.mImpl->checkForGMDError(retcode, file, linenr);
}

gmdHandle_t GAMSModelInstanceImpl::gmd()
{
    if (!mSyncDb.isValid())
        throw GAMSException("GAMSModelInstanceImpl: the database has not been initialized");
    return mSyncDb.mImpl->gmd();
}

void GAMSModelInstanceImpl::dbLockRecords()
{
    if (!mSyncDb.isValid())
        throw GAMSException("GAMSModelInstanceImpl: the database has not been initialized");
    mSyncDb.mImpl->lockRecords();
}

void GAMSModelInstanceImpl::dbLockSymbols()
{
    if (!mSyncDb.isValid())
        throw GAMSException("GAMSModelInstanceImpl: the database has not been initialized");
    mSyncDb.mImpl->lockSymbols();
}

}
