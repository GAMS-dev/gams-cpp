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

#ifndef GAMSEQUATIONRECORD_H
#define GAMSEQUATIONRECORD_H

#include "gamssymbolrecord.h"

namespace gams{

class GAMSEquation;

/// This class represents a single record of a GAMSEquation.
class LIBSPEC GAMSEquationRecord : public GAMSSymbolRecord
{
public:
    /// The default constructor allows to create an uninitialized GAMSEquationRecord to be used later in the code.
    /// The method \ref isValid() shows if the instance is initialized.
    GAMSEquationRecord();

    /// Shallow copy constructor
    /// \param record the target GAMSEquationRecord
    GAMSEquationRecord(const GAMSSymbolRecord& record);

    /// Assigns a GAMSEquationRecord.
    /// \param other Another GAMSEquationRecord used as data source.
    /// \return Returns the assigned GAMSEquationRecord (*this).
    GAMSEquationRecord& operator =(const GAMSEquationRecord& other);

    /// Retrieve the the level of this GAMSEquationRecord.
    double level();

    /// Set the level of this GAMSEquationRecord.
    void setLevel(const double val);

    /// Retrieve the marginal of this GAMSEquationRecord.
    double marginal();

    /// Set the marginal of this GAMSEquationRecord.
    void setMarginal(const double val);

    /// Retrieve the upper bound of this GAMSEquationRecord.
    double upper();

    /// Set the upper bound of this GAMSEquationRecord.
    void setUpper(const double val);

    /// Retrieve the lower bound of this GAMSEquationRecord.
    double lower();

    /// Set the lower bound of this GAMSEquationRecord.
    void setLower(const double val);

    /// Retrieve the scale factor of this GAMSEquationRecord.
    double scale();

    /// Set the scale factor of this GAMSEquationRecord.
    void setScale(const double val);

private:
    friend class GAMSSymbolIter<GAMSEquation>;
    GAMSEquationRecord(const GAMSEquation& equ, void* symIterPtr);
};

}
#endif // GAMSEQUATIONRECORD_H
