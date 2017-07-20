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

#ifndef GAMSMODIFIER_H
#define GAMSMODIFIER_H

#include "gamslib_global.h"
#include <memory>
#include "gamsenum.h"

namespace gams {

class GAMSSymbol;
class GAMSParameter;
class GAMSVariable;
class GAMSEquation;
class GAMSModifierImpl;

/// Instances of this class are input to GAMSModelInstance.instatiate. A GAMSModifier consists either
/// of a GAMSParameter or a triple: A GAMSVariable or GAMSEquation to be modified, the modification
/// action (e.g. . Upper, Lower or Fixed for updating bounds of a variable, or Primal/Dual for updating
/// the level/marginal of a variable or equation mainly used for starting non-linear models from different
/// starting points), and a GAMSParameter that holds the data for modification. In addition the UpdateType
/// can be defined (if ommitted the type defined in the Solve call is used).
class LIBSPEC GAMSModifier
{
public:
    /// Default constructor
    GAMSModifier() {}

    /// Constructor
    /// \param gamsPar Parameter in the GAMS model to be modified with data for modification
    /// \param updType Symbol Update Type
    GAMSModifier(const GAMSParameter& gamsPar, GAMSEnum::SymbolUpdateType updType = GAMSEnum::SymbolUpdateType::Inherit);

    /// Constructor
    /// \param gamsVar GAMS Variable in the GAMS model to be modified
    /// \param updAction Modification action
    /// \param dataSym Parameter containing the data for the modification
    /// \param updType Symbol Update Type
    GAMSModifier(const GAMSVariable& gamsVar,  GAMSEnum::SymbolUpdateAction updAction, const GAMSParameter& dataSym
                 , GAMSEnum::SymbolUpdateType updType = GAMSEnum::SymbolUpdateType::Inherit);

    /// Constructor
    /// \param gamsEqu GAMS Equation in the GAMS model to be modified
    /// \param updAction Modification action
    /// \param dataSym Parameter containing the data for the modification
    /// \param updType Symbol Update Type
    GAMSModifier(const GAMSEquation& gamsEqu, GAMSEnum::SymbolUpdateAction updAction, const GAMSParameter& dataSym
                 , GAMSEnum::SymbolUpdateType updType = GAMSEnum::SymbolUpdateType::Inherit);

    /// Assigns a GAMSModifier.
    /// \param other Another GAMSModifier used as data source.
    /// \return Returns the assigned GAMSModifier (*this).
    GAMSModifier& operator=(const GAMSModifier& other);

    /// Compares two GAMSModifier objects.
    /// \param other Another GAMSModifier to compare to.
    /// \return Returns <c>true</c> if the two GAMSModifier are different; otherwise <c>false</c>.
    bool operator!=(const GAMSModifier& other) const;

    /// Compares two GAMSModifier objects.
    /// \param other Another GAMSModifier to compare to.
    /// \return Returns <c>true</c> if the two GAMSModifier are equal; otherwise <c>false</c>.
    bool operator==(const GAMSModifier& other) const;

    /// The GAMSModifier is invalid, if it has no assignment to a GAMSSymbol.
    /// \return true, if the GAMModifier has been initialized.
    bool isValid() const;

    /// Symbol in the GAMS model to be modified
    GAMSSymbol gamsSymbol();

    /// Type of modification
    GAMSEnum::SymbolUpdateAction updAction();

    /// Symbol containing the data for the modification
    GAMSParameter dataSymbol();

    /// Symbol Update Type
    GAMSEnum::SymbolUpdateType updType();

    // TODO(jm) new getter in API
    /// True, if the associated symbol is a parameter
    bool isParameter();

    // TODO(jm) new getter in API
    /// True, if the associated symbol is a variable
    bool isVariable();

    // TODO(jm) new getter in API
    /// True, if the associated symbol is ae equation
    bool isEquation();

private:
    std::shared_ptr<GAMSModifierImpl> mImpl;
};

} // namespace gams

#endif // GAMSMODIFIER_H
