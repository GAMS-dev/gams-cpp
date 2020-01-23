/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2020 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2020 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMSEQUATION_H
#define GAMSEQUATION_H

#include "gamssymbol.h"
#include "gamsequationrecord.h"

namespace gams {

/// This is the representation of an equation symbol in GAMS. It exists in a
/// GAMSDatabase and contains GAMSEquationRecords which one can iterate through.
class LIBSPEC GAMSEquation : public GAMSSymbol
{
public:

    typedef GAMSEquationRecord RecordType;

    /// Standard constructor.
    GAMSEquation() {}

    /// Constructor
    /// \param other A GAMSSymbol used as data source.
    GAMSEquation(const GAMSSymbol &other);

    /// Destructor.
    ~GAMSEquation();

    /// Interator to the begining.
    /// \return Iterator to the first GAMSEquation.
    GAMSSymbolIter<GAMSEquation> begin();

    /// Iterator to end.
    /// \return Iterator to the element following the last GAMSEquation.
    GAMSSymbolIter<GAMSEquation> end();

    /// Assigns a GAMSEquation.
    /// \param other Another GAMSEquation used as data source.
    /// \return Returns the assigned GAMSCheckpoint (*this).
    GAMSEquation& operator=(const GAMSEquation& other);

    /// Retrieve the first record in GAMSEquation that meets the slice criteria.
    /// \param slice Define filter for elements whose record should be retrieved.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord firstRecord(const std::vector<std::string>& slice);

    /// Retrieve first record in GAMSEquation.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord firstRecord();

    /// Retrieve first record in GAMSEquation that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord firstRecord(const std::string& key1);

    /// Retrieve first record in GAMSEquation that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord firstRecord(const std::string& key1, const std::string& key2);

    /// Retrieve first record in GAMSEquation that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \param key3 Slicing criteria for third index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord firstRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Retrieve a slice of the last record in GAMSEquation.
    /// \param slice Define filter for elements whose record should be retrieved.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord lastRecord(const std::vector<std::string>& slice);

    /// Retrieve last record in GAMSEquation.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord lastRecord();

    /// Retrieve last record in GAMSEquation.
    /// \param key1 Slicing criteria for first index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord lastRecord(const std::string& key1);

    /// Retrieve last record in GAMSEquation.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord lastRecord(const std::string& key1, const std::string& key2);

    /// Retrieve last record in GAMSEquation.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \param key3 Slicing criteria for third index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord lastRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Add record to GAMSEquation.
    /// \param keys Vector of keys.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord addRecord(const std::vector<std::string>& keys);

    /// Add record to GAMSEquation.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord addRecord();

    /// Add record to GAMSEquation.
    /// \param key1 Key for first index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord addRecord(const std::string& key1);

    /// Add record to GAMSEquation.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord addRecord(const std::string& key1, const std::string& key2);

    /// Add record to GAMSEquation.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord addRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Find record in GAMSEquation.
    /// \param keys Vector of keys.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord findRecord(const std::vector<std::string>& keys);

    /// Find record in GAMSEquation.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord findRecord();

    /// Find record in GAMSEquation.
    /// \param key1 Key for first index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord findRecord(const std::string& key1);

    /// Find record in GAMSEquation.
    /// \param key1 Key for first index position.
    /// \param key2 key for second index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord findRecord(const std::string& key1, const std::string& key2);

    /// Find record in GAMSEquation.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of GAMSEquationRecord.
    GAMSEquationRecord findRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Finds record in GAMSEquation if it exists; otherwise add it.
    /// \param keys List of keys.
    /// \returns Instance of found or added GAMSEquationRecord.
    GAMSEquationRecord mergeRecord(const std::vector<std::string>& keys);

    /// Finds record in GAMSEquation if it exists, adds it if not
    /// \returns Instance of found or added GAMSEquationRecord.
    GAMSEquationRecord mergeRecord();

    /// Finds record in GAMSEquation if it exists; otherwise add it.
    /// \param key1 Key for first index position.
    /// \returns Instance of found or added GAMSEquationRecord.
    GAMSEquationRecord mergeRecord(const std::string& key1);

    /// Finds record in GAMSEquation if it exists; otherwise add it.
    /// \param key1 Key for first index position.
    /// \param key2 key for second index position.
    /// \returns Instance of found or added GAMSEquationRecord.
    GAMSEquationRecord mergeRecord(const std::string& key1, const std::string& key2);

    /// Finds record in GAMSEquation if it exists; otherwise add it.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of found or added GAMSEquationRecord
    GAMSEquationRecord mergeRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Retrieve subtype of Equation (E: Equal, G: Greater, L: Less, N: No specification, X: External defined, C: Conic).
    GAMSEnum::EquType equType();

private:
    friend class GAMSDatabaseImpl;
    friend class GAMSSymbolIter<GAMSEquation>;

    GAMSEquation(GAMSDatabase& database, void *symPtr, int dim, std::string name, std::string text
                 , GAMSEnum::EquType eqType);

    GAMSEquation(GAMSDatabase& database, const std::string& name, const int dim, const std::string& text
                 , GAMSEnum::EquType equType);

    GAMSEquation(GAMSDatabase& database, const std::string& name, const std::string& text
                 , const std::vector<GAMSDomain>& domains, GAMSEnum::EquType equType);
};

} // namespace gams

#endif // GAMSEQUATION_H
