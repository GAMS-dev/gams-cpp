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

#include "gamsequation.h"
#include "gamsexception.h"
#include "gmdcc.h"
#include "gamssymbolimpl.h"

namespace gams {

using namespace std;


GAMSEquation::GAMSEquation(const GAMSSymbol& other)
    : GAMSSymbol(other)
{
    if (other.isValid() && other.type() != GAMSEnum::SymTypeEqu) {
        throw GAMSException("GAMSEquation: invalid type conversion from type=" + to_string(other.type()));
    }
}


GAMSEquation::GAMSEquation(GAMSDatabase& database, void* symPtr, int dim, string name, string text, GAMSEnum::EquType eqType)
    : GAMSSymbol(database, symPtr, dim, name, text, GAMSEnum::SymTypeEqu, GAMSEnum::Unknown, eqType)
{}


GAMSEquation::GAMSEquation(GAMSDatabase& database, const string& name, const int dim, const string& text
                           , GAMSEnum::EquType equType)
    : GAMSSymbol(database, dim, name, text, GAMSEnum::SymTypeEqu, GAMSEnum::Unknown, equType)
{}

GAMSEquation::GAMSEquation(GAMSDatabase& database, const std::string& name, const std::string& text
                           , const std::vector<GAMSDomain>& domains, GAMSEnum::EquType equType)
    : GAMSSymbol(database, name, text, GAMSEnum::SymTypeEqu, GAMSEnum::Unknown, equType, domains)
{}


GAMSEquation::~GAMSEquation()
{}

GAMSEquation& GAMSEquation::operator=(const GAMSEquation& other)
{
    GAMSSymbol::operator =(other);
    return *this;
}

GAMSSymbolIter<GAMSEquation> GAMSEquation::begin()
{
    return GAMSSymbolIter<GAMSEquation>(*this, 0);
}

GAMSSymbolIter<GAMSEquation> GAMSEquation::end()
{
    return GAMSSymbolIter<GAMSEquation>(*this, numberRecords());
}

GAMSEquationRecord GAMSEquation::addRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::addRecord(keys));
}

GAMSEquationRecord GAMSEquation::addRecord()
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::addRecord());
}

GAMSEquationRecord GAMSEquation::addRecord(const std::string& key1)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::addRecord(key1));
}

GAMSEquationRecord GAMSEquation::addRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::addRecord(key1, key2));
}

GAMSEquationRecord GAMSEquation::addRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::addRecord(key1, key2, key3));
}

GAMSEquationRecord GAMSEquation::firstRecord(const std::vector<std::string>& slice)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::firstRecord(slice));
}

GAMSEquationRecord GAMSEquation::firstRecord()
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::firstRecord());
}

GAMSEquationRecord GAMSEquation::firstRecord(const std::string& key1)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::firstRecord(key1));
}

GAMSEquationRecord GAMSEquation::firstRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::firstRecord(key1, key2));
}

GAMSEquationRecord GAMSEquation::firstRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::firstRecord(key1, key2, key3));
}

GAMSEquationRecord GAMSEquation::lastRecord(const std::vector<std::string>& slice)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::lastRecord(slice));
}

GAMSEquationRecord GAMSEquation::lastRecord()
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::lastRecord());
}

GAMSEquationRecord GAMSEquation::lastRecord(const std::string& key1)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::lastRecord(key1));
}

GAMSEquationRecord GAMSEquation::lastRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::lastRecord(key1, key2));
}

GAMSEquationRecord GAMSEquation::lastRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::lastRecord(key1, key2, key3));
}

GAMSEquationRecord GAMSEquation::findRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::findRecord(keys));
}

GAMSEquationRecord GAMSEquation::findRecord()
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::findRecord());
}

GAMSEquationRecord GAMSEquation::findRecord(const std::string& key1)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::findRecord(key1));
}

GAMSEquationRecord GAMSEquation::findRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::findRecord(key1, key2));
}

GAMSEquationRecord GAMSEquation::findRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::findRecord(key1, key2, key3));
}

GAMSEquationRecord GAMSEquation::mergeRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::mergeRecord(keys));
}

GAMSEquationRecord GAMSEquation::mergeRecord()
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::mergeRecord());
}

GAMSEquationRecord GAMSEquation::mergeRecord(const std::string& key1)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::mergeRecord(key1));
}

GAMSEquationRecord GAMSEquation::mergeRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::mergeRecord(key1, key2));
}

GAMSEquationRecord GAMSEquation::mergeRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSEquationRecord>(GAMSSymbol::mergeRecord(key1, key2, key3));
}

GAMSEnum::EquType GAMSEquation::equType()
{
    if (!mImpl) throw GAMSException("GAMSEquation: The equation has not been initialized.");
    return this->mImpl->mEquType;
}

}
