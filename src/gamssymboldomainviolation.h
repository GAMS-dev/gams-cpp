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

#ifndef GAMSSYMBOLDOMAINVIOLATION_H
#define GAMSSYMBOLDOMAINVIOLATION_H

#include "gamslib_global.h"
#include "gamssymbolrecord.h"

namespace gams {

/// This class describes a domain violation of a GAMSSymbol.
class LIBSPEC GAMSSymbolDomainViolation
{
public:
    /// Standard constructor.
    GAMSSymbolDomainViolation();

    /// Assigns a GAMSSymbolDomainViolation.
    /// \param other Another GAMSSymbolDomainViolation used as data source.
    /// \return Returns the assigned GAMSSymbolDomainViolation (*this).
    GAMSSymbolDomainViolation& operator=(const GAMSSymbolDomainViolation& other);

    ///Checks if a GAMSSymbolDomainViolation is valid.
    /// \return Returns <c>true</c> if the GAMSSymbolDomainViolation is valid; otherwise <c>false</c>.
    bool isValid() const;

    /// Compares two GAMSSymbolDomainViolation objects.
    /// \param other Another GAMSSymbolDomainViolation to compare to.
    /// \return Returns <c>true</c> if the two GAMSSymbolDomainViolation are different; otherwise <c>false</c>.
    bool operator!=(const GAMSSymbolDomainViolation& other) const;

    /// Compares two GAMSSymbolDomainViolation objects.
    /// \param other Another GAMSSymbolDomainViolation to compare to.
    /// \return Returns <c>true</c> if the two GAMSSymbolDomainViolation are equal; otherwise <c>false</c>.
    bool operator==(const GAMSSymbolDomainViolation& other) const;

    /// GAMSSymbolRecord that has a domain violation.
    /// \return Returns the GAMSSymbolRecord.
    GAMSSymbolRecord violRec();

    /// Array indicating which position of a record has a domain violation.
    std::vector<bool> violInd();

private:
    friend class GAMSSymbolImpl;
    GAMSSymbolDomainViolation(const std::shared_ptr<GAMSSymbolDomainViolationImpl> impl);
    GAMSSymbolDomainViolation(const std::vector<bool>& violInd, const GAMSSymbolRecord& violRec);
private:
    std::shared_ptr<GAMSSymbolDomainViolationImpl> mImpl;
};

} // namespace gams

#endif // GAMSSYMBOLDOMAINVIOLATION_H
