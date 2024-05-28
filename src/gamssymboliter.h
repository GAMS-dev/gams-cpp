/*
 * GAMS - General Algebraic Modeling System C++ API
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

#ifndef GAMSSYMBOLITER_H
#define GAMSSYMBOLITER_H

#include "gamslib_global.h"

namespace gams {

template <class T>

/// Iterates through the records of a specific GAMSSymbol.
/// \remark This class can be used with all GAMSSymbol types.
class LIBSPEC GAMSSymbolIter
{
public:

    /// Constructs a GAMSSymbolIter.
    /// \param symbol Iterator related GAMSSymbol.
    /// \param pos Position the iterator is related to.
    GAMSSymbolIter(T& symbol, int pos);

    /// Destructor.
    ~GAMSSymbolIter();

    /// Compares two GAMSSymbolIter<T> objects.
    /// \param other Another GAMSSymbolIter<T> to compare to.
    /// \return Returns <c>true</c> if the two GAMSSymbolIter<T> are equal; otherwise <c>false</c>.
    bool operator==(const GAMSSymbolIter<T>& other) const;

    /// Compares two GAMSSymbolIter<T>.
    /// \param other Another GAMSSymbolIter<T> to compare to.
    /// \return Returns <c>true</c> if the two GAMSSymbolIter<T> are different; otherwise <c>false</c>.
    bool operator!=(const GAMSSymbolIter<T>& other) const;

    /// Dereferences the iterator.
    /// \return Returns the actual object.
    typename T::RecordType& operator*();

    /// Increments the iterator.
    /// \return Returns the incremented iterator.
    GAMSSymbolIter<T>& operator++();

private:
    T& mSymbol;
    typename T::RecordType* mSymRecord;
    int mPos;
    void* mSymIterPtr;
};

} // namespace gams

#endif // GAMSSYMBOLITER_H
