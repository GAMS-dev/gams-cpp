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

#include "gamsmodifierimpl.h"
#include "gamsexception.h"
#include "gamsequation.h"
#include "gamsvariable.h"
#include "gamsdatabase.h"

using namespace std;

namespace gams {

GAMSModifierImpl::GAMSModifierImpl(const GAMSParameter &gamsPar, GAMSEnum::SymbolUpdateType updType)
    : mGamsSymbol(gamsPar), mDataSymbol(gamsPar), mUpdAction(GAMSEnum::SymbolUpdateAction::Param), mUpdType(updType)
{
   isParameter = true;
}

GAMSModifierImpl::GAMSModifierImpl(const GAMSVariable &gamsVar, GAMSEnum::SymbolUpdateAction updAction
                                   , const GAMSParameter &dataSymbol, GAMSEnum::SymbolUpdateType updType)
    : mGamsSymbol(gamsVar), mDataSymbol(dataSymbol), mUpdAction(updAction), mUpdType(updType)
{
   if (gamsVar.dim() != dataSymbol.dim())
      throw GAMSException("GAMS Symbol and Data must have same dimension, but saw " + to_string(gamsVar.dim())
                          + " (GAMS Symbol) and " + to_string(dataSymbol.dim()) + " (Data)");
   if (gamsVar.database() != dataSymbol.database())
      throw GAMSException("GAMS Symbol and Data must belong to same GAMSDatabase");

   isVariable = true;
}

GAMSModifierImpl::GAMSModifierImpl(const GAMSEquation &gamsEqu, GAMSEnum::SymbolUpdateAction updAction
                                   , const GAMSParameter &dataSymbol, GAMSEnum::SymbolUpdateType updType)
    : mGamsSymbol(gamsEqu), mDataSymbol(dataSymbol), mUpdAction(updAction), mUpdType(updType)
{
   if (gamsEqu.dim() != dataSymbol.dim())
      throw GAMSException("GAMS Symbol and Data must have same dimension, but saw " + to_string(gamsEqu.dim())
                          + " (GAMS Symbol) and " + to_string(dataSymbol.dim()) + " (Data)");
   if (gamsEqu.database() != dataSymbol.database())
      throw GAMSException("GAMS Symbol and Data must belong to same GAMSDatabase");
   if (GAMSEnum::SymbolUpdateAction::Upper == updAction || GAMSEnum::SymbolUpdateAction::Lower == updAction
           || GAMSEnum::SymbolUpdateAction::Fixed == updAction)
      throw GAMSException(("GAMS Symbol must be GAMSVariable for " + std::to_string(updAction)).c_str());

   isEquation = true;
}

bool GAMSModifierImpl::operator!=(const GAMSModifierImpl& other) const
{
    return (mGamsSymbol != other.mGamsSymbol) || (mDataSymbol != other.mDataSymbol)
            || (mUpdAction != other.mUpdAction) || (mUpdType != other.mUpdType);

}

bool GAMSModifierImpl::operator==(const GAMSModifierImpl& other) const
{
    return !(operator!=(other));
}

}
