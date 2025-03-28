/**
 * GAMS C++ API
 *
 * Copyright (c) 2017-2025 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2025 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMSDATABASEITER_H
#define GAMSDATABASEITER_H

#include "gamslib_global.h"

namespace gams {

class GAMSDatabaseImpl;
class GAMSSymbol;

/// Iterates through all GAMSSymbol objects within a GAMSDatabase.
/// \remark This class can be used with all GAMSSymbol types.
class LIBSPEC GAMSDatabaseIter
{
public:

    /// Constructs a GAMSDatabaseIter.
    /// \param database Iterator related GAMSDatabase.
    /// \param pos Position the iterator is related to.
    GAMSDatabaseIter(std::shared_ptr<GAMSDatabaseImpl> &database, int pos=0);

    /// Compares two GAMSDatabaseIter<T> objects.
    /// \param other Another GAMSDatabaseIter<T> to compare to.
    /// \return Returns <c>true</c> if the two GAMSDatabaseIter<T> are equal; otherwise <c>false</c>.
    bool operator==(const GAMSDatabaseIter& other);

    /// Compares two GAMSDatabaseIter<T>.
    /// \param other Another GAMSDatabaseIter<T> to compare to.
    /// \return Returns <c>true</c> if the two GAMSDatabaseIter<T> are different; otherwise <c>false</c>.
    bool operator!=(const GAMSDatabaseIter& other);

    /// Dereferences the iterator.
    /// \return Returns the actual object.
    GAMSSymbol operator*();

    /// Increments the iterator.
    /// \return Returns the incremented iterator.
    GAMSDatabaseIter& operator++();

private:
    std::shared_ptr<GAMSDatabaseImpl> mDatabase;
    int pos;
};

} // namespace gams

#endif // GAMSDATABASEITER_H
