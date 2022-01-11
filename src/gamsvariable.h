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

#ifndef GAMSVARIABLE_H
#define GAMSVARIABLE_H

#include "gamssymbol.h"
#include "gamsvariablerecord.h"

namespace gams {

/// This is the representation of a variable symbol in GAMS. It exists in a
/// GAMSDatabase and contains GAMSVariableRecords which one can iterate through.
class LIBSPEC GAMSVariable : public GAMSSymbol
{
public:
    typedef GAMSVariableRecord RecordType;

    /// Standard constructor.
    GAMSVariable();

    /// Constructor based on GAMSSymbol.
    /// \param other Another GAMSSymbol used as data source.
    GAMSVariable(const GAMSSymbol& other);

    /// Destructor.
    ~GAMSVariable();

    /// Interator to the begining.
    /// \return Iterator to the first GAMSVariable.
    GAMSSymbolIter<GAMSVariable> begin();

    /// Iterator to end.
    /// \return Iterator to the element following the last GAMSVariable.
    GAMSSymbolIter<GAMSVariable> end();

    /// Assigns a GAMSVariable.
    /// \param other Another GAMSVariable used as data source.
    /// \return Returns the assigned GAMSVariable (*this).
    GAMSVariable& operator=(const GAMSVariable& other);

    /// Retrieve the first record in GAMSVariable that meets the slice criteria.
    /// \param slice Define filter for elements whose record should be retrieved.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord firstRecord(const std::vector<std::string>& slice);

    /// Retrieve first record in GAMSVariable.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord firstRecord();

    /// Retrieve first record in GAMSVariable that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord firstRecord(const std::string& key1);

    /// Retrieve first record in GAMSVariable that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord firstRecord(const std::string& key1, const std::string& key2);

    /// Retrieve first record in GAMSVariable that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \param key3 Slicing criteria for third index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord firstRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Retrieve a slice of the last record in GAMSVariable.
    /// \param slice Define filter for elements whose record should be retrieved.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord lastRecord(const std::vector<std::string>& slice);

    /// Retrieve last record in GAMSVariable.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord lastRecord();

    /// Retrieve last record in GAMSVariable.
    /// \param key1 Slicing criteria for first index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord lastRecord(const std::string& key1);

    /// Retrieve last record in GAMSVariable.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord lastRecord(const std::string& key1, const std::string& key2);

    /// Retrieve last record in GAMSVariable.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \param key3 Slicing criteria for third index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord lastRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Find record in GAMSVariable.
    /// \param keys Vector of keys.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord findRecord(const std::vector<std::string>& keys);

    /// Find record in GAMSVariable.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord findRecord();

    /// Find record in GAMSVariable.
    /// \param key1 Key for first index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord findRecord(const std::string& key1);

    /// Find record in GAMSVariable.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord findRecord(const std::string& key1, const std::string& key2);

    /// Find record in GAMSVariable.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord findRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Finds record in GAMSVariable if it exists; otherwise add it.
    /// \param keys List of keys.
    /// \returns Instance of found or added GAMSVariableRecord.
    GAMSVariableRecord mergeRecord(const std::vector<std::string>& keys);

    /// Finds record in GAMSVariable if it exists; otherwise add it.
    /// \returns Instance of found or added GAMSVariableRecord.
    GAMSVariableRecord mergeRecord();

    /// Finds record in GAMSVariable if it exists; otherwise add it.
    /// \param key1 Key for first index position.
    /// \returns Instance of found or added GAMSVariableRecord.
    GAMSVariableRecord mergeRecord(const std::string& key1);

    /// Finds record in GAMSVariable if it exists; otherwise add it.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \returns Instance of found or added GAMSVariableRecord.
    GAMSVariableRecord mergeRecord(const std::string& key1, const std::string& key2);

    /// Finds record in GAMSVariable if it exists; otherwise add it.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of found or added GAMSVariableRecord.
    GAMSVariableRecord mergeRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Add record to GAMSVariable.
    /// \param keys Vector of keys.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord addRecord(const std::vector<std::string>& keys);

    /// Add record to GAMSVariable.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord addRecord();

    /// Add record to GAMSVariable.
    /// \param key1 Key for first index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord addRecord(const std::string& key1);

    /// Add record to GAMSVariable.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord addRecord(const std::string& key1, const std::string& key2);

    /// Add record to GAMSVariable.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of GAMSVariableRecord.
    GAMSVariableRecord addRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Retrieve subtype of variable (Binary, Integer, Positive, Negative, Free, SOS1, SOS2, SemiCont, SemiInt).
    GAMSEnum::VarType varType();

private:
    friend class GAMSDatabaseImpl;
    friend class GAMSSymbolIter<GAMSVariable>;
    GAMSVariable(GAMSDatabase& database, void* symPtr, int dim, std::string name, std::string text, GAMSEnum::VarType varType);
    GAMSVariable(GAMSDatabase& database, const std::string& name, const int dim, const std::string& text, GAMSEnum::VarType varType);
    GAMSVariable(GAMSDatabase& database, const std::string& name, const std::string& text, const std::vector<GAMSDomain>& domains, GAMSEnum::VarType varType);
};

} // namespace gams

#endif // GAMSVARIABLE_H
