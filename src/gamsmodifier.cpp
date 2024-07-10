/**
 * GAMS C++ API
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

#include "gamsmodifier.h"
#include "gamsmodifierimpl.h"
#include "gamsexception.h"
#include "gamssymbol.h"
#include "gamsparameter.h"
#include "gamsvariable.h"
#include "gamsset.h"
#include "gamsequation.h"
#include <iostream>

using namespace std;

namespace gams{

   GAMSModifier::GAMSModifier(const GAMSParameter &gamsPar, GAMSEnum::SymbolUpdateType updType)
      :mImpl(make_shared<GAMSModifierImpl>(gamsPar, updType))
   { }

   GAMSModifier::GAMSModifier(const GAMSVariable &gamsVar, GAMSEnum::SymbolUpdateAction updAction, const GAMSParameter &dataSym, GAMSEnum::SymbolUpdateType updType)
      : mImpl(make_shared<GAMSModifierImpl>(gamsVar, updAction, dataSym, updType))
   { }


   GAMSModifier::GAMSModifier(const GAMSEquation &gamsEqu, GAMSEnum::SymbolUpdateAction updAction, const GAMSParameter &dataSym, GAMSEnum::SymbolUpdateType updType)
       : mImpl(make_shared<GAMSModifierImpl>(gamsEqu, updAction, dataSym, updType))
   { }

   GAMSModifier&GAMSModifier::operator=(const GAMSModifier& other)
   {
       mImpl = other.mImpl;
       return *this;
   }

   bool GAMSModifier::operator!=(const GAMSModifier& other) const
   {
       return !operator==(other);
   }

   bool GAMSModifier::operator==(const GAMSModifier& other) const
   {
       return (mImpl == other.mImpl) || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
   }

   bool GAMSModifier::isValid() const
   {
       return bool(mImpl);
   }

   GAMSSymbol GAMSModifier::gamsSymbol()
   {
       if (!mImpl) throw GAMSException("GAMSModifier: The modifier has not been initialized.");
       return mImpl->mGamsSymbol;
   }

   GAMSEnum::SymbolUpdateAction GAMSModifier::updAction()
   {
       if (!mImpl) throw GAMSException("GAMSModifier: The modifier has not been initialized.");
      return mImpl->mUpdAction;
   }

   GAMSParameter GAMSModifier::dataSymbol()
   {
       if (!mImpl) throw GAMSException("GAMSModifier: The modifier has not been initialized.");
      return mImpl->mDataSymbol;
   }

   GAMSEnum::SymbolUpdateType GAMSModifier::updType()
   {
       if (!mImpl) throw GAMSException("GAMSModifier: The modifier has not been initialized.");
       return mImpl->mUpdType;
   }

   bool GAMSModifier::isParameter()
   {
       if (!mImpl) throw GAMSException("GAMSModifier: The modifier has not been initialized.");
       return mImpl->isParameter;
   }

   bool GAMSModifier::isVariable()
   {
       if (!mImpl) throw GAMSException("GAMSModifier: The modifier has not been initialized.");
       return mImpl->isVariable;
   }

   bool GAMSModifier::isEquation()
   {
       if (!mImpl) throw GAMSException("GAMSModifier: The modifier has not been initialized.");
       return mImpl->isEquation;
   }
}
