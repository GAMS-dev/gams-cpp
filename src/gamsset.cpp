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

#include "gamsset.h"
#include "gamsexception.h"
#include "gmdcc.h"
#include "gamssymbolimpl.h"
#include <typeinfo>

namespace gams {
using namespace std;

GAMSSet::GAMSSet()
    : GAMSSymbol()
{}

GAMSSet::GAMSSet(const GAMSSymbol& other)
    : GAMSSymbol(other)
{
    if (other.isValid() && other.type() != GAMSEnum::SymTypeSet) {
        throw GAMSException("GAMSSet: invalid type conversion from type=" + GAMSEnum::text(other.type()));
    }
}

GAMSSet::GAMSSet(GAMSDatabase& database, void* symPtr)
    : GAMSSymbol(database, symPtr)
{}


GAMSSet::GAMSSet(GAMSDatabase& database, void* symPtr, int dim, string name, string text)
    : GAMSSymbol(database, symPtr, dim, name, text, GAMSEnum::SymTypeSet, GAMSEnum::Unknown,
                 GAMSEnum::E)
{}


GAMSSet::GAMSSet(GAMSDatabase& database, const string& name, const int dim, const string& text,
                 GAMSEnum::SetType setType)
    : GAMSSymbol(database, dim, name, text, GAMSEnum::SymTypeSet, GAMSEnum::Unknown, GAMSEnum::E,
                 setType)
{}

GAMSSet::GAMSSet(GAMSDatabase& database, const std::string& name, const std::string& text,
                 const std::vector<GAMSDomain>& domains, GAMSEnum::SetType setType)
    : GAMSSymbol(database, name, text, GAMSEnum::SymTypeSet, GAMSEnum::Unknown, GAMSEnum::E,
                 domains, setType)
{}

GAMSSet::~GAMSSet()
{}

GAMSEnum::SetType GAMSSet::setType()
{
    if (!mImpl) throw GAMSException("GAMSSet: The set has not been initialized.");
    return mImpl->mSetType;
}

GAMSSet& GAMSSet::operator=(const GAMSSet& other)
{
    GAMSSymbol::operator =(other);
    return *this;
}

GAMSSymbolIter<GAMSSet> GAMSSet::begin()
{
    return GAMSSymbolIter<GAMSSet>(*this, 0);
}

GAMSSymbolIter<GAMSSet> GAMSSet::end()
{
    return GAMSSymbolIter<GAMSSet>(*this, numberRecords());
}

GAMSSetRecord GAMSSet::firstRecord(const std::vector<std::string>& slice)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::firstRecord(slice));
}

GAMSSetRecord GAMSSet::firstRecord()
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::firstRecord());
}

GAMSSetRecord GAMSSet::firstRecord(const std::string& key1)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::firstRecord(key1));
}

GAMSSetRecord GAMSSet::firstRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::firstRecord(key1, key2));
}

GAMSSetRecord GAMSSet::firstRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::firstRecord(key1, key2, key3));
}

GAMSSetRecord GAMSSet::lastRecord(const std::vector<std::string>& slice)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::lastRecord(slice));
}

GAMSSetRecord GAMSSet::lastRecord()
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::lastRecord());
}

GAMSSetRecord GAMSSet::lastRecord(const std::string& key1)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::lastRecord(key1));
}

GAMSSetRecord GAMSSet::lastRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::lastRecord(key1, key2));
}

GAMSSetRecord GAMSSet::lastRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::lastRecord(key1, key2, key3));
}

GAMSSetRecord GAMSSet::findRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::findRecord(keys));
}

GAMSSetRecord GAMSSet::findRecord()
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::findRecord());
}

GAMSSetRecord GAMSSet::findRecord(const std::string& key1)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::findRecord(key1));
}

GAMSSetRecord GAMSSet::findRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::findRecord(key1, key2));
}

GAMSSetRecord GAMSSet::findRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::findRecord(key1, key2, key3));
}

GAMSSetRecord GAMSSet::mergeRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::mergeRecord(keys));
}

GAMSSetRecord GAMSSet::mergeRecord()
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::mergeRecord());
}

GAMSSetRecord GAMSSet::mergeRecord(const std::string& key1)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::mergeRecord(key1));
}

GAMSSetRecord GAMSSet::mergeRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::mergeRecord(key1, key2));
}

GAMSSetRecord GAMSSet::mergeRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::mergeRecord(key1, key2, key3));
}

GAMSSetRecord GAMSSet::addRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::addRecord(keys));
}

GAMSSetRecord GAMSSet::addRecord()
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::addRecord());
}

GAMSSetRecord GAMSSet::addRecord(const std::string& key1)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::addRecord(key1));
}

GAMSSetRecord GAMSSet::addRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::addRecord(key1, key2));
}

GAMSSetRecord GAMSSet::addRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSSetRecord>(GAMSSymbol::addRecord(key1, key2, key3));
}

}
