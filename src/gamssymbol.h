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

#ifndef GAMSSYMBOL_H
#define GAMSSYMBOL_H

#include "gamsenum.h"
#include "gamssymboliter.h"
#include <string>
#include <memory>
#include <vector>

namespace gams {

class GAMSSet;
class GAMSVariable;
class GAMSSymbolRecord;
class GAMSDatabase;
class GAMSDomain;
class GAMSSymbolImpl;
class GAMSSymbolDomainViolation;

/// This is the representation of a symbol in GAMS. It exists in a
/// GAMSDatabase and contains GAMSSymbolRecords which can be iterated through.
/// Derived classes are GAMSEquation, GAMSParameter, GAMSSet and GAMSVariable.
class LIBSPEC GAMSSymbol
{
public:
    typedef GAMSSymbolRecord RecordType;

    /// Standard constructor.
    GAMSSymbol();

    /// Shallow copy constructor that creates a new frame pointing to an existing GAMSSymbol
    /// \param symbol Another GAMSSymbol used as data source.
    GAMSSymbol(const GAMSSymbol &symbol);

    /// Destructor.
    virtual ~GAMSSymbol();

    /// Assigns a GAMSSymbol.
    /// \param other Another GAMSSymbol used as data source.
    /// \return Returns the assigned GAMSSymbol (*this).
    GAMSSymbol operator=(const GAMSSymbol& other);

    /// Compares two GAMSSymbol objects.
    /// \param other Another GAMSSymbol to compare to.
    /// \return Returns <c>true</c> if the two GAMSSymbol objects are different; otherwise <c>false</c>.
    bool operator!=(const GAMSSymbol& other) const;

    /// Compares two GAMSSymbol objects.
    /// \param other Another GAMSSymbol to compare to.
    /// \return Returns <c>true</c> if the two GAMSSymbol are equal; otherwise <c>false</c>.
    bool operator==(const GAMSSymbol& other) const;

    ///Checks if a GAMSSymbol is valid.
    /// \return Returns <c>true</c> if the GAMSSymbol is valid; otherwise <c>false</c>.
    bool isValid() const;

    /// Interator to the begining.
    /// \return Iterator to the first GAMSSymbol.
    GAMSSymbolIter<GAMSSymbol> begin();

    /// Iterator to end.
    /// \return Iterator to the element following the last GAMSSymbol.
    GAMSSymbolIter<GAMSSymbol> end();

    /// Add record to GAMSSymbol
    /// \param keys vector of keys
    /// \returns Instance of GAMSSymbolRecord
    GAMSSymbolRecord addRecord(const std::vector<std::string>& keys);

    /// Add record to GAMSSymbol
    /// \returns Instance of GAMSSymbolRecord
    GAMSSymbolRecord addRecord();

    /// Add record to GAMSSymbol
    /// \param key1 key for first index position
    /// \returns Instance of GAMSSymbolRecord
    GAMSSymbolRecord addRecord(const std::string& key1);

    /// Add record to GAMSSymbol
    /// \param key1 key for first index position
    /// \param key2 key for second index position
    /// \returns Instance of GAMSSymbolRecord
    GAMSSymbolRecord addRecord(const std::string& key1, const std::string& key2);

    /// Add record to GAMSSymbol
    /// \param key1 key for first index position
    /// \param key2 key for second index position
    /// \param key3 key for third index position
    /// \returns Instance of GAMSSymbolRecord
    GAMSSymbolRecord addRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Delete GAMSSymbol record.
    /// \param keys List of keys
    void deleteRecord(const std::vector<std::string>& keys);

    // TODO(JM) missing overloads for ::deleteRecord

    /// Clear symbol
    /// \returns Returns <c>true</c> on success; otherwise <c>false</c>
    bool clear();

    /// Get domains of a Symbol.
    /// \remark Each element is either a GAMSSet (real domain) or a string (relaxed domain).
    std::vector<GAMSDomain> domains();

    /// Check for domain violations.
    /// \returns Returns <c>true</c> if there is any domain violation; otherwise <c>false</c>.
    bool checkDomains();

    /// Get all <c>GAMSSymbolDomainViolations</c>.
    /// \param maxViol The maximum number of domain violations which should be stored (0 for no limit)
    /// \returns Returns a list of all <c>GAMSSymbolDomainViolations</c>.
    std::vector<GAMSSymbolDomainViolation> getSymbolDVs(int maxViol = 0);

    /// Retrieve the first record in GAMSSymbol that meets the slice criteria.
    /// \param slice Define filter for elements whose record should be retrieved.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord firstRecord(const std::vector<std::string>& slice);

    /// Retrieve first record in GAMSSymbol.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord firstRecord();

    /// Retrieve first record in GAMSSymbol that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \returns Instance of GAMSSymbolRecord
    GAMSSymbolRecord firstRecord(const std::string& key1);

    /// Retrieve first record in GAMSSymbol that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord firstRecord(const std::string& key1, const std::string& key2);

    /// Retrieve first record in GAMSSymbol that meets the slice criteria.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \param key3 Slicing criteria for third index position.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord firstRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Retrieve a slice of the last record in GAMSSymbol.
    /// \param slice Define filter for elements whose record should be retrieved.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord lastRecord(const std::vector<std::string>& slice);

    /// Retrieve last record in GAMSSymbol.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord lastRecord();

    /// Retrieve last record in GAMSSymbol.
    /// \param key1 slicing criteria for first index position.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord lastRecord(const std::string& key1);

    /// Retrieve last record in GAMSSymbol.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord lastRecord(const std::string& key1, const std::string& key2);

    /// Retrieve last record in GAMSSymbol.
    /// \param key1 Slicing criteria for first index position.
    /// \param key2 Slicing criteria for second index position.
    /// \param key3 Slicing criteria for third index position.
    /// \returns Instance of GAMSSymbolRecord
    GAMSSymbolRecord lastRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Find record in GAMSSymbol.
    /// \param keys Vector of keys.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord findRecord(const std::vector<std::string>& keys);

    /// Find record in GAMSSymbol.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord findRecord();

    /// Find record in GAMSSymbol.
    /// \param key1 Key for first index position.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord findRecord(const std::string& key1);

    /// Find record in GAMSSymbol.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord findRecord(const std::string& key1, const std::string& key2);

    /// Find record in GAMSSymbol.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of GAMSSymbolRecord.
    GAMSSymbolRecord findRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Finds a record in a GAMSSymbol if it exists, otherwise the record gets added to the symbol.
    /// \param keys List of keys.
    /// \returns Instance of found or added GAMSSymbolRecord.
    GAMSSymbolRecord mergeRecord(const std::vector<std::string>& keys);

    /// Finds a record in a GAMSSymbol if it exists, otherwise the record gets added to the symbol.
    /// \returns Instance of found or added GAMSSymbolRecord.
    GAMSSymbolRecord mergeRecord();

    /// Finds a record in a GAMSSymbol if it exists, otherwise the record gets added to the symbol.
    /// \param key1 Key for first index position.
    /// \returns Instance of found or added GAMSSymbolRecord.
    GAMSSymbolRecord mergeRecord(const std::string& key1);

    /// Finds record in GAMSSymbol if it exists, otherwise the record gets added to the symbol.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \returns Instance of found or added GAMSSymbolRecord.
    GAMSSymbolRecord mergeRecord(const std::string& key1, const std::string& key2);

    /// Finds record in GAMSSymbol if it exists, otherwise the record gets added to the symbol.
    /// \param key1 Key for first index position.
    /// \param key2 Key for second index position.
    /// \param key3 Key for third index position.
    /// \returns Instance of found or added GAMSSymbolRecord.
    GAMSSymbolRecord mergeRecord(const std::string& key1, const std::string& key2, const std::string& key3);

    /// Copys all records of this GAMSSymbol to the target GAMSSymbol.
    /// \remark If the target symbol had records, they will be deleted.
    /// \param target Target GAMSSymbol.
    void copySymbol(const GAMSSymbol& target);

    /// Get GAMSDatabase containing the GAMSSymbol.
    gams::GAMSDatabase &database() const;

    /// Get explanatory text of GAMSSymbol.
    std::string text() const;

    /// Get GAMSSymbol name.
    std::string& name() const;

    /// Get GAMSSymbol dimension.
    int dim() const;

    /// Retrieve the number of records of the GAMSSymbol.
    int numberRecords() const;

    /// Get the GAMS log ID.
    /// \return Returns the GAMS log ID.
    LogId logID();

    /// Get the symbol type.
    GAMSEnum::SymbolType type() const;

protected:
    friend class GAMSSymbolRecordImpl;
    friend class GAMSModelInstanceImpl;
    friend class GAMSDatabaseImpl;
    friend class GAMSSymbolIter<GAMSSymbol>;

    GAMSSymbol(const std::shared_ptr<GAMSSymbolImpl> impl);

    GAMSSymbol(GAMSDatabase &database, int dim, std::string name, std::string text,
               GAMSEnum::SymbolType symType, GAMSEnum::VarType varType, GAMSEnum::EquType equType,
               GAMSEnum::SetType setType = GAMSEnum::SetType::Multi);

    GAMSSymbol(GAMSDatabase &database, std::string name, std::string text, GAMSEnum::SymbolType symType,
               GAMSEnum::VarType varType, GAMSEnum::EquType equType, const std::vector<GAMSDomain>& domains,
               GAMSEnum::SetType setType = GAMSEnum::SetType::Multi);

    GAMSSymbol(GAMSDatabase &database, void *symPtr, int dim, std::string name, std::string text,
               GAMSEnum::SymbolType symType, GAMSEnum::VarType varType, GAMSEnum::EquType equType,
               GAMSEnum::SetType setType = GAMSEnum::SetType::Multi);

    GAMSSymbol(const GAMSDatabase &database, void *symPtr);

    void *symPtr() const;

    std::vector<GAMSSymbolDomainViolation> getSymbolDVs(bool skipCleanup, int maxViol = 0);

protected:
    std::shared_ptr<GAMSSymbolImpl> mImpl;
};

} // namespace gams

#endif // GAMSSYMBOL_H
