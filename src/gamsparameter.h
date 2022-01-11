/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>s
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

#ifndef GAMSPARAMETER_H
#define GAMSPARAMETER_H

#include "gamssymbol.h"
#include "gamsparameterrecord.h"

namespace gams {

/// This is the representation of a parameter symbol in GAMS. It exists in a
/// GAMSDatabase and contains GAMSParameterRecords which one can iterate through.
class LIBSPEC GAMSParameter : public GAMSSymbol
{
public:
    typedef GAMSParameterRecord RecordType;

    /// Standard constructor.
    GAMSParameter() {}

    /// Copy constructor.
    /// \param other Another GAMSSymbol used as data source.
    GAMSParameter(const GAMSSymbol& other);

    /// Destructor.
    ~GAMSParameter();

    /// Interator to the begining.
    /// \return Iterator to the first GAMSParameter.
    GAMSSymbolIter<GAMSParameter> begin();

    /// Iterator to end.
    /// \return Iterator to the element following the last GAMSParameter.
    GAMSSymbolIter<GAMSParameter> end();

    /// Assigns a GAMSParameter.
    /// \param other Another GAMSParameter used as data source.
    /// \return Returns the assigned GAMSParameter (*this).
    GAMSParameter& operator=(const GAMSParameter& other);

    /// Retrieve the first record in GAMSParameter that meets the slice criteria.
    /// \param slice Define filter for elements whose record should be retrieved.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord firstRecord(const std::vector<std::string>& slice);

    /// Retrieve first record in GAMSParameter.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord firstRecord();

    /// Retrieve first record in GAMSParameter that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord firstRecord(const std::string& key1);

    /// Retrieve first record in GAMSParameter that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord firstRecord(const std::string& key1, const std::string& key2);

    /// Retrieve first record in GAMSParameter that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \param key3 Slicing criteria for third index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord firstRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Retrieve a slice of the last record in GAMSParameter.
    /// \param slice Define filter for elements whose record should be retrieved.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord lastRecord(const std::vector<std::string>& slice);

    /// Retrieve last record in GAMSParameter.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord lastRecord();

    /// Retrieve last record in GAMSParameter.
    /// \param key1 Slicing criteria for first index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord lastRecord(const std::string& key1);

    /// Retrieve last record in GAMSParameter.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord lastRecord(const std::string& key1, const std::string& key2);

    /// Retrieve last record in GAMSParameter.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \param key3 Slicing criteria for third index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord lastRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Find record in GAMSParameter.
    /// \param keys Vector of keys.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord findRecord(const std::vector<std::string>& keys);

    /// Find record in GAMSParameter.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord findRecord();

    /// Find record in GAMSParameter.
    /// \param key1 Key for first index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord findRecord(const std::string& key1);

    /// Find record in GAMSParameter.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord findRecord(const std::string& key1, const std::string& key2);

    /// Find record in GAMSParameter.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord findRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Finds record in GAMSParameter if it exists; otherwise add it.
    /// \param keys List of keys.
    /// \returns Instance of found or added GAMSParameterRecord.
    GAMSParameterRecord mergeRecord(const std::vector<std::string>& keys);

    /// Finds record in GAMSParameter if it exists; otherwise add it.
    /// \returns Instance of found or added GAMSParameterRecord
    GAMSParameterRecord mergeRecord();

    /// Finds record in GAMSParameter if it exists; otherwise add it.
    /// \param key1 Key for first index position.
    /// \returns Instance of found or added GAMSParameterRecord.
    GAMSParameterRecord mergeRecord(const std::string& key1);

    /// Finds record in GAMSParameter if it exists; otherwise add it.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \returns Instance of found or added GAMSParameterRecord.
    GAMSParameterRecord mergeRecord(const std::string& key1, const std::string& key2);

    /// Finds record in GAMSParameter if it exists; otherwise add it.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of found or added GAMSParameterRecord.
    GAMSParameterRecord mergeRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Add record to GAMSParameter.
    /// \param keys Vector of keys.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord addRecord(const std::vector<std::string>& keys);

    /// Add record to GAMSParameter.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord addRecord();

    /// Add record to GAMSParameter.
    /// \param key1 Key for first index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord addRecord(const std::string& key1);

    /// Add record to GAMSParameter.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord addRecord(const std::string& key1, const std::string& key2);

    /// Add record to GAMSParameter.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of GAMSParameterRecord.
    GAMSParameterRecord addRecord(const std::string& key1, const std::string& key2, const std::string& key3);

private:
    friend class GAMSDatabaseImpl;
    friend class GAMSSymbolIter<GAMSParameter>;
    GAMSParameter(GAMSDatabase& database, void *symPtr, int dim, std::string name, std::string text);
    GAMSParameter(GAMSDatabase& database, const std::string& name, const int dim, const std::string& text);
    GAMSParameter(GAMSDatabase& database, const std::string& name, const std::string& text, const std::vector<GAMSDomain>& domains);

};


} // namespace gams

#endif // GAMSPARAMETER_H
