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

#ifndef GAMSDATABASEDOMAINVIOLATION_H
#define GAMSDATABASEDOMAINVIOLATION_H

#include "gamssymbol.h"
#include "gamssymboldomainviolation.h"

namespace gams {

/// This class describes a domain violation of a GAMSDatabase.
class LIBSPEC GAMSDatabaseDomainViolation
{
public:
    /// Standard constructor.
    GAMSDatabaseDomainViolation();

    /// Assigns a GAMSDatabaseDomainViolation.
    /// \param other Another GAMSDatabaseDomainViolation used as data source.
    /// \return Returns the assigned GAMSDatabaseDomainViolation (*this).
    GAMSDatabaseDomainViolation& operator=(const GAMSDatabaseDomainViolation& other);

    /// Check if a GAMSDatabaseDomainViolation is valid.
    /// \return Returns <c>true</c> if the GAMSCheckpoint is valid; otherwise <c>false</c>.
    bool isValid() const;

    /// Compares two GAMSDatabaseDomainViolation objects.
    /// \param other Another GAMSDatabaseDomainViolation to compare to.
    /// \return Returns <c>true</c> if the two GAMSDatabaseDomainViolation are different; otherwise <c>false</c>.
    bool operator!=(const GAMSDatabaseDomainViolation& other) const;

    /// Compares two GAMSDatabaseDomainViolation objects.
    /// \param other Another GAMSDatabaseDomainViolation to compare to.
    /// \return Returns <c>true</c> if the two GAMSDatabaseDomainViolation are equal; otherwise <c>false</c>.
    bool operator==(const GAMSDatabaseDomainViolation& other) const;

    /// GAMSSymbol that has a domain violation
    GAMSSymbol violSym();

    /// Vector of domain violations of symbol
    const std::vector<GAMSSymbolDomainViolation>& violRecs() const;

private:
    friend class GAMSDatabaseImpl;
    std::shared_ptr<GAMSDatabaseDomainViolationImpl> mImpl;
    GAMSDatabaseDomainViolation(const std::shared_ptr<GAMSDatabaseDomainViolationImpl>& impl);
    GAMSDatabaseDomainViolation(GAMSSymbol violSym, std::vector<GAMSSymbolDomainViolation> violRecs);
};


} // namespace gams

#endif // GAMSDATABASEDOMAINVIOLATION_H
