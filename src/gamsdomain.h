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

#ifndef GAMSDOMAIN_H
#define GAMSDOMAIN_H

#include "gamsset.h"
#include <string>

namespace gams {

class GAMSDomainImpl;

/// This class represents a abstraction of GAMSSet objects and relaxed sets, i.e. strings.
class LIBSPEC GAMSDomain
{
public:
    /// Default constructor.
    GAMSDomain();

    /// Construtor for a GAMSSet.
    /// \param set The GAMSSet used as data source.
    GAMSDomain(const GAMSSet& set);

    /// Construtor for a relaxed set.
    /// \param relaxedName The relaxed set name used as data source.
    GAMSDomain(const std::string& relaxedName);

    /// Construtor for a relaxed set.
    /// \param relaxedName The relaxed set name used as data source.
    GAMSDomain(const char* relaxedName);

    /// Copy constructor.
    /// \param other Another GAMSDomain used as data source.
    GAMSDomain(const GAMSDomain& other);

    /// Assigns a GAMSSet.
    /// \param set A GAMSSet used as data source.
    /// \return Returns the assigned GAMSDomain (*this).
    GAMSDomain& operator=(const GAMSSet& set);

    /// Assigns a relaxed set.
    /// \param relaxedName A relaxed set used as data source.
    /// \return Returns the assigned GAMSDomain (*this).
    GAMSDomain& operator=(const std::string& relaxedName);

    /// Assigns a relaxed set.
    /// \param relaxedName A relaxed set used as data source.
    /// \return Returns the assigned GAMSDomain (*this).
    GAMSDomain& operator=(const char* relaxedName);

    /// Assigns a GAMSDomain.
    /// \param other Another GAMSDomain used as data source.
    /// \return Returns the assigned GAMSDomain (*this).
    GAMSDomain& operator=(const GAMSDomain& other);

    /// Compares two GAMSDomain objects.
    /// \param other Another GAMSDomain to compare to.
    /// \return Returns <c>true</c> if the two GAMSDomain are equal; otherwise <c>false</c>.
    bool operator!=(const GAMSDomain& other) const;

    /// Compares two GAMSDomain.
    /// \param other Another GAMSDomain to compare to.
    /// \return Returns <c>true</c> if the two GAMSDomain are different; otherwise <c>false</c>.
    bool operator==(const GAMSDomain& other) const;

    /// Get the GAMSSet.
    /// \remark This function throws an exception if it is called on an relaxed domain.
    /// \return  Returns the GAMSSet of a non relaxed domain.
    GAMSSet getSet() const;

    /// Get the set name.
    /// \return Returns the name.
    const std::string& name() const;

    /// Indicates if a GAMSDomain is a relaxed set.
    /// \return Returns <c>true</c> if the GAMSDomain is relaxed; otherwise <c>false</c>.
    bool isRelaxed() const;

    ///Check if a GAMSDomain is valid.
    /// \return Returns <c>true</c> if the GAMSCheckpoint is valid; otherwise <c>false</c>.
    bool isValid() const;

private:
    std::shared_ptr<GAMSDomainImpl> mImpl;
};

} // namespace gams

#endif // GAMSDOMAIN_H
