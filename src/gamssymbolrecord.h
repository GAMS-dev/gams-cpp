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

#ifndef GAMSSYMBOLRECORD_H
#define GAMSSYMBOLRECORD_H

#include <string>
#include <vector>
#include "gamsenum.h"
#include "gamssymboliter.h"

namespace gams {

class GAMSSet;
class GAMSParameter;
class GAMSVariable;
class GAMSEquation;
class GAMSSymbolRecordImpl;
class GAMSSymbol;

/// This is the representation of a single record of a GAMSSymbol.
/// \remark Derived classes are GAMSEquationRecord, GAMSParameterRecord, GAMSSetRecord and GAMSVariableRecord.
class LIBSPEC GAMSSymbolRecord
{
public:
    /// The default constructor allows to create an uninitialized GAMSSymbolRecord to be used later in the code.
    /// The method \ref isValid() shows if the instance is initialized.
    GAMSSymbolRecord();

    /// Copy constructor
    /// \param record GAMSSymbolRecord used to initialize the new GAMSSymbolRecord.
    GAMSSymbolRecord(const GAMSSymbolRecord& record);

    /// Destructor.
    virtual ~GAMSSymbolRecord();

    /// Assigns a GAMSSymbolRecord.
    /// \param other Another GAMSSymbolRecord used as data source.
    /// \return Returns the assigned GAMSSymbolRecord (*this).
    GAMSSymbolRecord& operator =(const GAMSSymbolRecord& other);

    /// Compares two GAMSSymbolRecord objects.
    /// \param other Another GAMSSymbolRecord to compare to.
    /// \return Returns <c>true</c> if the two GAMSSymbolRecord are different; otherwise <c>false</c>.
    bool operator!=(const GAMSSymbolRecord& other) const;

    /// Compares two GAMSSymbolRecord objects.
    /// \param other Another GAMSSymbolRecord to compare to.
    /// \return Returns <c>true</c> if the two GAMSSymbolRecord are equal; otherwise <c>false</c>.
    bool operator==(const GAMSSymbolRecord& other) const;

    ///Checks if a GAMSSymbolRecord is valid.
    /// \return Returns <c>true</c> if the GAMSSymbolRecord is valid; otherwise <c>false</c>.
    bool isValid() const;

    /// Iterate to next GAMSSymbolRecord of GAMSSymbol.
    /// \return Returns <c>true</c> if everything worked; otherwise <c>false</c>.
    bool moveNext();

    /// Iterate to previous GAMSSymbolRecord of GAMSSymbol
    /// \return Returns <c>true</c> if everything worked; otherwise <c>false</c>.
    bool movePrevious();

    /// Retrieve keys of GAMSSymbolRecord
    std::vector<std::string> keys();

    /// Retrieve key of GAMSSymbolRecord on position index.
    /// \param index Index position of key to be returned.
    /// \return Returns the key at position index.
    std::string key(int index);

    /// Access specified GAMSSymbolRecord.
    /// \param i Index of the element to return.
    /// \return Returns the requested element.
    std::string operator[](int i);

    /// Get the GAMS log ID.
    /// \return Returns the GAMS log ID.
    LogId logID();

    /// Get the GAMSSymbol type.
    GAMSEnum::SymbolType type() const;

private:
    friend class GAMSSymbolImpl;
    friend class GAMSSetRecord;
    friend class GAMSParameterRecord;
    friend class GAMSVariableRecord;
    friend class GAMSEquationRecord;
    friend class GAMSSymbolIter<GAMSSymbol>;
    friend class GAMSSymbolIter<GAMSSet>;
    friend class GAMSSymbolIter<GAMSParameter>;
    friend class GAMSSymbolIter<GAMSVariable>;
    friend class GAMSSymbolIter<GAMSEquation>;
    GAMSSymbolRecord(GAMSSymbol symbol, void* symIterPtr);
private:
    std::shared_ptr<GAMSSymbolRecordImpl> mImpl;

}; // class GAMSSymbolRecord

} // namespace gams
#endif // GAMSSYMBOLRECORD_H
