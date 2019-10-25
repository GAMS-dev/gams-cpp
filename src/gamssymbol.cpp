/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2019 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2019 GAMS Development Corp. <support@gams.com>
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

#include "gamslog.h"
#include "gamssymbolimpl.h"
#include "gamsexception.h"
#include "gamsdatabase.h"
#include "gamssymbolrecord.h"
#include "gamssetrecord.h"
#include "gamsdomain.h"

#include <iostream>

#include "gmdcc.h"
#include <sstream>

using namespace std;
namespace gams{

class GAMSVariableRecord;


GAMSSymbol::GAMSSymbol()
{ }


GAMSSymbol::GAMSSymbol(const GAMSSymbol &symbol)
    : mImpl(symbol.mImpl)
{ }


GAMSSymbol::GAMSSymbol(GAMSDatabase &database, void* symPtr, int dim, string name, string text, GAMSEnum::SymbolType symType
                       , gams::GAMSEnum::VarType varType, gams::GAMSEnum::EquType equType)
    : mImpl(make_shared<GAMSSymbolImpl>(database, symPtr, dim, name, text, symType, varType, equType))
{ }


GAMSSymbol::GAMSSymbol(const gams::GAMSDatabase& database, void* symPtr)
    : mImpl(make_shared<GAMSSymbolImpl>(database, symPtr))
{ }


GAMSSymbol::GAMSSymbol(GAMSDatabase& database, string name, string text, GAMSEnum::SymbolType symType, GAMSEnum::VarType varType
                       , GAMSEnum::EquType equType, const std::vector<GAMSDomain>& domains)
    : mImpl(make_shared<GAMSSymbolImpl>(database, name, text, symType, varType, equType, domains))
{ }


GAMSSymbol::GAMSSymbol(gams::GAMSDatabase& database, int dim, string name, string text, GAMSEnum::SymbolType symType
                       , GAMSEnum::VarType varType, GAMSEnum::EquType equType)
    : mImpl(make_shared<GAMSSymbolImpl>(database, dim, name, text, symType, varType, equType))
{ }


GAMSSymbol::~GAMSSymbol()
{ }

GAMSSymbol GAMSSymbol::operator=(const GAMSSymbol& other)
{
    mImpl = other.mImpl;
    return *this;
}

bool GAMSSymbol::operator!=(const GAMSSymbol& other) const
{
    return !operator==(other);
}

bool GAMSSymbol::operator==(const GAMSSymbol& other) const
{
    return mImpl == other.mImpl || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}

bool GAMSSymbol::isValid() const
{
    return bool(mImpl);
}

GAMSSymbolIter<GAMSSymbol> GAMSSymbol::begin()
{
    return GAMSSymbolIter<GAMSSymbol>(*this, 0);
}

GAMSSymbolIter<GAMSSymbol> GAMSSymbol::end()
{
    return GAMSSymbolIter<GAMSSymbol>(*this, numberRecords());
}


gams::GAMSDatabase& GAMSSymbol::database() const
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->mDatabase;
}

GAMSSymbolRecord GAMSSymbol::addRecord(const vector<string>& keys)
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->addRecord(*this, keys);
}

GAMSSymbolRecord GAMSSymbol::addRecord()
{
    vector<string> keys;
    return addRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::addRecord(const std::string& key1)
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->addRecord(*this, key1);
}

GAMSSymbolRecord GAMSSymbol::addRecord(const std::string& key1, const std::string& key2)
{
    vector<string> keys = { key1, key2 };
    return addRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::addRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    vector<string> keys = { key1, key2, key3 };
    return addRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::firstRecord()
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->firstRecord(*this);
}

GAMSSymbolRecord GAMSSymbol::firstRecord(const vector<string>& slice)
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->firstRecord(*this, slice);
}


GAMSSymbolRecord GAMSSymbol::firstRecord(const std::string& key1)
{
    vector<string> keys = { key1 };
    return firstRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::firstRecord(const std::string& key1, const std::string& key2)
{
    vector<string> keys = { key1, key2 };
    return firstRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::firstRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    vector<string> keys = { key1, key2, key3 };
    return firstRecord(keys);
}



GAMSSymbolRecord GAMSSymbol::lastRecord()
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->lastRecord(*this);
}

GAMSSymbolRecord GAMSSymbol::lastRecord(const vector<string>& slice)
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->lastRecord(*this, slice);
}

GAMSSymbolRecord GAMSSymbol::lastRecord(const std::string& key1)
{
    vector<string> keys = { key1 };
    return lastRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::lastRecord(const std::string& key1, const std::string& key2)
{
    vector<string> keys = { key1, key2 };
    return lastRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::lastRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    vector<string> keys = { key1, key2, key3 };
    return lastRecord(keys);
}


GAMSSymbolRecord GAMSSymbol::findRecord(const std::vector<std::string>& keys)
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->findRecord(*this, keys);
}

GAMSSymbolRecord GAMSSymbol::findRecord()
{
    vector<string> keys;
    return findRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::findRecord(const std::string& key1)
{
    vector<string> keys = { key1 };
    return findRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::findRecord(const std::string& key1, const std::string& key2)
{
    vector<string> keys = { key1, key2 };
    return findRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::findRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    vector<string> keys = { key1, key2, key3 };
    return findRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::mergeRecord(const std::vector<std::string>& keys)
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->mergeRecord(*this, keys);
}

GAMSSymbolRecord GAMSSymbol::mergeRecord()
{
    vector<string> keys;
    return mergeRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::mergeRecord(const std::string& key1)
{
    vector<string> keys = { key1 };
    return mergeRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::mergeRecord(const std::string& key1, const std::string& key2)
{
    vector<string> keys = { key1, key2 };
    return mergeRecord(keys);
}

GAMSSymbolRecord GAMSSymbol::mergeRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    vector<string> keys = { key1, key2, key3 };
    return mergeRecord(keys);
}

void GAMSSymbol::deleteRecord(const vector<string>& keys)
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    mImpl->deleteRecord(keys);
}

void GAMSSymbol::copySymbol(const GAMSSymbol& target)
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    if (!target.mImpl) throw GAMSException("GAMSSymbol: The target symbol has not been initialized.");
    mImpl->copySymbol(*target.mImpl.get());
}

void *GAMSSymbol::symPtr() const
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->symPtr();
}

bool GAMSSymbol::clear()
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return (1 == gmdClearSymbol(mImpl->gmd(), mImpl->symPtr()));
}

std::vector<GAMSDomain> GAMSSymbol::domains()
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->domains();
}

string& GAMSSymbol::name() const
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->mName;
}

int GAMSSymbol::dim() const
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->mDim;
}

string GAMSSymbol::text() const
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->mExplanatoryText;
}

bool GAMSSymbol::checkDomains()
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->checkDomains();
}

vector<GAMSSymbolDomainViolation> GAMSSymbol::getSymbolDVs(int maxViol)
{
    return getSymbolDVs(false, maxViol);
}

int GAMSSymbol::numberRecords() const
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->numberRecords();
}

gams::LogId GAMSSymbol::logID()
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->logID();
}

GAMSEnum::SymbolType GAMSSymbol::type() const
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->mSymType;
}

std::vector<GAMSSymbolDomainViolation> GAMSSymbol::getSymbolDVs(bool skipCleanup, int maxViol)
{
    if (!mImpl) throw GAMSException("GAMSSymbol: The symbol has not been initialized.");
    return mImpl->getSymbolDVs(*this, skipCleanup, maxViol);
}


} // namespace gams
