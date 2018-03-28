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

#include "gamsvariablerecord.h"
#include <iostream>
#include "gamsvariable.h"
#include "gamssymbolimpl.h"

#include "gmdcc.h"

namespace gams {
using namespace std;

// --------------------------------------------------------------------------------------------
//   GAMSVariable

GAMSVariable::GAMSVariable() : GAMSSymbol()
{}


GAMSVariable::GAMSVariable(const GAMSSymbol& other)
    : GAMSSymbol(other)
{
    if (other.isValid() && other.type() != GAMSEnum::SymTypeVar) {
        throw GAMSException("GAMSVariable: invalid type conversion from type=" + to_string(other.type()));
    }
}


GAMSVariable::GAMSVariable(GAMSDatabase& database, void* symPtr, int dim, string name, string text, GAMSEnum::VarType varType)
    : GAMSSymbol(database, symPtr, dim, name, text, GAMSEnum::SymTypeVar, varType, GAMSEnum::E)
{}


GAMSVariable::GAMSVariable(GAMSDatabase& database, const string& name, const int dim, const string& text, GAMSEnum::VarType varType)
    : GAMSSymbol(database, dim, name, text, GAMSEnum::SymTypeVar, varType, GAMSEnum::E)
{}


//GAMSVariable::GAMSVariable(GAMSDatabase& database, const std::string& name, const std::string& text, const std::vector<std::string>& relaxedDomains, GAMSEnum::VarType varType)
//    : GAMSSymbol(database, name, text, GAMSEnum::SymTypeVar, varType, GAMSEnum::E, relaxedDomains)
//{}


GAMSVariable::GAMSVariable(GAMSDatabase& database, const std::string& name, const std::string& text, const std::vector<GAMSDomain>& domains, GAMSEnum::VarType varType)
    : GAMSSymbol(database, name, text, GAMSEnum::SymTypeVar, varType, GAMSEnum::E, domains)
{}


GAMSVariable::~GAMSVariable()
{}


GAMSVariable& GAMSVariable::operator=(const GAMSVariable& other)
{
    GAMSSymbol::operator =(other);
    return *this;
}


GAMSSymbolIter<GAMSVariable> GAMSVariable::begin()
{
    return GAMSSymbolIter<GAMSVariable>(*this, 0);
}


GAMSSymbolIter<GAMSVariable> GAMSVariable::end()
{
    return GAMSSymbolIter<GAMSVariable>(*this, numberRecords());
}


// --------------------------------------------------------------------------------------------
//   GAMSVariableRecord


GAMSVariableRecord GAMSVariable::firstRecord(const std::vector<std::string>& slice)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::firstRecord(slice));
}

GAMSVariableRecord GAMSVariable::firstRecord()
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::firstRecord());
}

GAMSVariableRecord GAMSVariable::firstRecord(const std::string& key1)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::firstRecord(key1));
}

GAMSVariableRecord GAMSVariable::firstRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::firstRecord(key1, key2));
}

GAMSVariableRecord GAMSVariable::firstRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::firstRecord(key1, key2, key3));
}

GAMSVariableRecord GAMSVariable::lastRecord(const std::vector<std::string>& slice)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::lastRecord(slice));
}

GAMSVariableRecord GAMSVariable::lastRecord()
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::lastRecord());
}

GAMSVariableRecord GAMSVariable::lastRecord(const std::string& key1)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::lastRecord(key1));
}

GAMSVariableRecord GAMSVariable::lastRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::lastRecord(key1, key2));
}

GAMSVariableRecord GAMSVariable::lastRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::lastRecord(key1, key2, key3));
}

GAMSVariableRecord GAMSVariable::findRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::findRecord(keys));
}

GAMSVariableRecord GAMSVariable::findRecord()
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::findRecord());
}

GAMSVariableRecord GAMSVariable::findRecord(const std::string& key1)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::findRecord(key1));
}

GAMSVariableRecord GAMSVariable::findRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::findRecord(key1, key2));
}

GAMSVariableRecord GAMSVariable::findRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::findRecord(key1, key2, key3));
}

GAMSVariableRecord GAMSVariable::mergeRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::mergeRecord(keys));
}

GAMSVariableRecord GAMSVariable::mergeRecord()
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::mergeRecord());
}

GAMSVariableRecord GAMSVariable::mergeRecord(const std::string& key1)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::mergeRecord(key1));
}

GAMSVariableRecord GAMSVariable::mergeRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::mergeRecord(key1, key2));
}

GAMSVariableRecord GAMSVariable::mergeRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::mergeRecord(key1, key2, key3));
}

GAMSVariableRecord GAMSVariable::addRecord(const std::vector<std::string>& keys)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::addRecord(keys));
}

GAMSVariableRecord GAMSVariable::addRecord()
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::addRecord());
}

GAMSVariableRecord GAMSVariable::addRecord(const std::string& key1)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::addRecord(key1));
}

GAMSVariableRecord GAMSVariable::addRecord(const std::string& key1, const std::string& key2)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::addRecord(key1, key2));
}

GAMSVariableRecord GAMSVariable::addRecord(const std::string& key1, const std::string& key2, const std::string& key3)
{
    return static_cast<GAMSVariableRecord>(GAMSSymbol::addRecord(key1, key2, key3));
}

GAMSEnum::VarType GAMSVariable::varType()
{
    if (!mImpl) throw GAMSException("GAMSVariable: The variable has not been initialized.");
    return this->mImpl->mVarType;
}

}
