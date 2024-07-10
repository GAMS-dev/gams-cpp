/**
 * GAMS C++ API
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

#include "gamsparameter.h"
#include "gamssymbolimpl.h"

#include "gmdcc.h"
#include <memory>
#include <iostream>

namespace gams{
using namespace std;

GAMSParameter::GAMSParameter(const GAMSSymbol &other)
    : GAMSSymbol(other)
{
    if (other.isValid() && other.type() != GAMSEnum::SymTypePar) {
        throw GAMSException("GAMSParameter: invalid type conversion from type=" + to_string(other.type()));
    }
}

GAMSParameter::GAMSParameter(GAMSDatabase& database, void* symPtr, int dim, string name, string text)
    : GAMSSymbol(database, symPtr, dim, name, text, GAMSEnum::SymTypePar, GAMSEnum::Unknown, GAMSEnum::E)
{}


//GAMSParameter::GAMSParameter(GAMSDatabase& database, const std::string& name, const std::string& text, const std::vector<std::string>& relaxedDomains)
//    : GAMSSymbol(database, name, text, GAMSEnum::SymTypePar, GAMSEnum::Unknown, GAMSEnum::E, relaxedDomains)
//{}


GAMSParameter::GAMSParameter(GAMSDatabase& database, const std::string& name, const std::string& text, const std::vector<GAMSDomain>& domains)
    : GAMSSymbol(database, name, text, GAMSEnum::SymTypePar, GAMSEnum::Unknown, GAMSEnum::E, domains)
{}


GAMSParameter::GAMSParameter(GAMSDatabase& database, const string& name, const int dim, const string& text)
    : GAMSSymbol(database, dim, name, text, GAMSEnum::SymTypePar, GAMSEnum::Unknown, GAMSEnum::E)
{}


GAMSParameter::~GAMSParameter()
{}


GAMSParameter& GAMSParameter::operator=(const GAMSParameter& other)
{
    GAMSSymbol::operator =(other);
    return *this;
}


GAMSSymbolIter<GAMSParameter> GAMSParameter::begin()
{
    return GAMSSymbolIter<GAMSParameter>(*this, 0);
}

GAMSSymbolIter<GAMSParameter> GAMSParameter::end()
{
    return GAMSSymbolIter<GAMSParameter>(*this, numberRecords());
}

GAMSParameterRecord GAMSParameter::firstRecord(const std::vector<std::string>& slice)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::firstRecord(slice));
}

GAMSParameterRecord GAMSParameter::firstRecord()
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::firstRecord());
}

GAMSParameterRecord GAMSParameter::firstRecord(const std::string& key1)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::firstRecord(key1));
}

GAMSParameterRecord GAMSParameter::firstRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::firstRecord(key1, key2));
}

GAMSParameterRecord GAMSParameter::firstRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::firstRecord(key1, key2, key3));
}

GAMSParameterRecord GAMSParameter::lastRecord(const std::vector<std::string>& slice)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::lastRecord(slice));
}

GAMSParameterRecord GAMSParameter::lastRecord()
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::lastRecord());
}

GAMSParameterRecord GAMSParameter::lastRecord(const std::string& key1)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::lastRecord(key1));
}

GAMSParameterRecord GAMSParameter::lastRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::lastRecord(key1, key2));
}

GAMSParameterRecord GAMSParameter::lastRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::lastRecord(key1, key2, key3));
}

GAMSParameterRecord GAMSParameter::findRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::findRecord(keys));
}

GAMSParameterRecord GAMSParameter::findRecord()
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::findRecord());
}

GAMSParameterRecord GAMSParameter::findRecord(const std::string& key1)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::findRecord(key1));
}

GAMSParameterRecord GAMSParameter::findRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::findRecord(key1, key2));
}

GAMSParameterRecord GAMSParameter::findRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::findRecord(key1, key2, key3));
}

GAMSParameterRecord GAMSParameter::mergeRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::mergeRecord(keys));
}

GAMSParameterRecord GAMSParameter::mergeRecord()
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::mergeRecord());
}

GAMSParameterRecord GAMSParameter::mergeRecord(const std::string& key1)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::mergeRecord(key1));
}

GAMSParameterRecord GAMSParameter::mergeRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::mergeRecord(key1, key2));
}

GAMSParameterRecord GAMSParameter::mergeRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::mergeRecord(key1, key2, key3));
}

GAMSParameterRecord GAMSParameter::addRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::addRecord(keys));
}

GAMSParameterRecord GAMSParameter::addRecord()
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::addRecord());
}

GAMSParameterRecord GAMSParameter::addRecord(const std::string& key1)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::addRecord(key1));
}

GAMSParameterRecord GAMSParameter::addRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::addRecord(key1, key2));
}

GAMSParameterRecord GAMSParameter::addRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSParameterRecord>(GAMSSymbol::addRecord(key1, key2, key3));
}


}
