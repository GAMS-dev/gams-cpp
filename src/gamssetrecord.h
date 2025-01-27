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

#ifndef GAMSSETRECORD_H
#define GAMSSETRECORD_H

#include "gamssymbolrecord.h"

namespace gams {

class GAMSSet;

/// This is the representation of a single record of a GAMSSet.
class LIBSPEC GAMSSetRecord : public GAMSSymbolRecord
{
public:
    /// The default constructor allows to create an uninitialized GAMSSetRecord to be used later in the code.
    /// The method \ref isValid() shows if the instance is initialized.
    GAMSSetRecord();

    /// Copy constructor.
    /// \param record Another GAMSSymbolRecord used as data source.
    GAMSSetRecord(const GAMSSymbolRecord& record);

    /// Assigns a GAMSSetRecord.
    /// \param other Another GAMSSetRecord used as data source.
    /// \return Returns the assigned GAMSSetRecord (*this).
    GAMSSetRecord& operator =(const GAMSSetRecord& other);

    /// Retrieve the explanatory text of this GAMSSetRecord.
    /// \return Returns the explanatory text.
    std::string text();

    /// Set the explanatory text of this GAMSSetRecord.
    /// \param text Explanatory text.
    void setText(const std::string& text);

private:
    friend class GAMSSymbolIter<GAMSSet>;
    GAMSSetRecord(const GAMSSet &set, void* symIterPtr);
};

}
#endif // GAMSSETRECORD_H
