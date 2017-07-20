/**
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMSPARAMETERRECORD_H
#define GAMSPARAMETERRECORD_H

#include "gamssymbolrecord.h"

namespace gams{

class GAMSParameter;

/// This is the representation of a single record of a GAMSParameter.
class LIBSPEC GAMSParameterRecord : public GAMSSymbolRecord
{

public:
    /// The default constructor allows to create an uninitialized GAMSParameterRecord to be used later in the code.
    /// The method \ref isValid() shows if the instance is initialized.
    GAMSParameterRecord();

    /// Shallow copy constructor
    /// \param record the target GAMSParameterRecord
    GAMSParameterRecord(const GAMSSymbolRecord &record);

    /// Assigns a GAMSParameterRecord.
    /// \param other Another GAMSParameterRecord used as data source.
    /// \return Returns the assigned GAMSParameterRecord (*this).
    GAMSParameterRecord& operator =(const GAMSParameterRecord& other);

    /// Retrieve the value of this GAMSParameterRecord.
    double value();

    /// Set the value of this GAMSParameterRecord.
    void setValue(const double val);

private:
    friend class GAMSSymbolIter<GAMSParameter>;
    GAMSParameterRecord(const GAMSParameter& par, void* symIterPtr);
};

}
#endif // GAMSPARAMETERRECORD_H
