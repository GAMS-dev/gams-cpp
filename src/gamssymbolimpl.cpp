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

#include "gamssymbolimpl.h"
#include "gamssymbol.h"
#include "gamslog.h"
#include "gamsvariable.h"
#include "gamsexception.h"
#include "gamsdatabase.h"
#include "gamssymbolrecord.h"
#include "gamssetrecord.h"
#include "gamsdomain.h"

using namespace std;

namespace gams {

GAMSSymbolImpl::GAMSSymbolImpl(GAMSDatabase database, void* symPtr, int dim, const string &name, const string &text,
                               GAMSEnum::SymbolType symType, GAMSEnum::VarType varType, GAMSEnum::EquType equType)
    : mDatabase(database), mName(name), mDim(dim), mSymType(symType), mVarType(varType), mEquType(equType), mExplanatoryText(text), mSymPtr(symPtr)
{}


GAMSSymbolImpl::GAMSSymbolImpl(GAMSDatabase database, void* symPtr)
    : mDatabase(database), mSymPtr(symPtr)
{
    DEB << "---- Entering GAMSSymbolImpl constructor ----";
    if (!symPtr)
        throw GAMSException("Symbol does not exist");

    int ival = 0;
    double dval = 0.0;
    char sval[GMS_SSSIZE];
    int type = 0;
    checkForGMDError(gmdSymbolType(gmd(), mSymPtr, &type), __FILE__, __LINE__);
    mSymType = GAMSEnum::SymbolType(type);
    checkForGMDError(gmdSymbolInfo(gmd(), mSymPtr, GMD_NAME, &ival, &dval, sval), __FILE__, __LINE__);
    mName = string(sval);
    int iDim;
    checkForGMDError(gmdSymbolInfo(gmd(), mSymPtr, GMD_DIM, &iDim, &dval, NULL), __FILE__, __LINE__);
    mDim = iDim;
    checkForGMDError(gmdSymbolInfo(gmd(), mSymPtr, GMD_EXPLTEXT, &ival, &dval, sval), __FILE__, __LINE__);
    mExplanatoryText = string(sval);
}


GAMSSymbolImpl::GAMSSymbolImpl(GAMSDatabase database, int dim, const std::string& name, const std::string& text,
                               GAMSEnum::SymbolType symType, GAMSEnum::VarType varType, GAMSEnum::EquType equType)
    : mDatabase(database), mName(name), mDim(dim), mSymType(symType), mVarType(varType), mEquType(equType), mExplanatoryText(text)
{
    if (mDim < 0 || mDim > GLOBAL_MAX_INDEX_DIM)
        throw GAMSException("Invalid dimension");
    int varEquType = 0;
    if (symType == GAMSEnum::SymTypeVar) varEquType = static_cast<int>(varType);
    if (symType == GAMSEnum::SymTypeEqu) varEquType = static_cast<int>(equType);

    checkForGMDError(gmdAddSymbol(gmd(), mName.c_str(), static_cast<int>(mDim), symType, varEquType, mExplanatoryText.c_str()
                                  , &mSymPtr), __FILE__, __LINE__);
}


GAMSSymbolImpl::GAMSSymbolImpl(GAMSDatabase database, std::string name, std::string text, GAMSEnum::SymbolType symType,
               GAMSEnum::VarType varType, GAMSEnum::EquType equType, const std::vector<GAMSDomain>& domains)
    : mDatabase(database), mName(name), mDim(static_cast<int>(domains.size())), mSymType(symType), mVarType(varType), mEquType(equType), mExplanatoryText(text)
{
    if (mDim > GLOBAL_MAX_INDEX_DIM)
        throw GAMSException("Invalid dimension");
    int varEquType = 0;
    if (symType == GAMSEnum::SymTypeVar) varEquType = static_cast<int>(varType);
    if (symType == GAMSEnum::SymTypeEqu) varEquType = static_cast<int>(equType);

    void* domPtr[GMS_MAX_INDEX_DIM];
    for (int i = 0; i < mDim; i++) {
        if (domains.at(i).isRelaxed()) {
            mIndexC.set(i, domains.at(i).name());
            domPtr[i] = nullptr;
        } else {
            domPtr[i] = domains.at(i).getSet().symPtr();
        }
    }

    checkForGMDError(gmdAddSymbolX(gmd(), mName.c_str(), mDim, symType, varEquType, mExplanatoryText.c_str()
                                   , domPtr, mIndexC.cPtrs(), &mSymPtr), __FILE__, __LINE__);
}


GAMSSymbolImpl::~GAMSSymbolImpl()
{
    DEB <<"---- Entering GAMSSymbol destructor ----";
    if (mDomains) delete mDomains;
}

bool GAMSSymbolImpl::operator !=(const GAMSSymbolImpl& other)
{
    return !operator==(other);
}

bool GAMSSymbolImpl::operator ==(const GAMSSymbolImpl& other)
{
    return mSymPtr == other.mSymPtr;
}

GAMSSymbolRecord GAMSSymbolImpl::addRecord(const GAMSSymbol& sym, const std::vector<string>& keys)
{
    if (recordLock())
        throw GAMSException("Cannot add data records to record-locked database");
    checkKeys(keys);
    void* symIterPtr = 0;

    checkForGMDError(gmdAddRecord(gmd(), mSymPtr, mIndexC.set(keys).cPtrs(), &symIterPtr), __FILE__, __LINE__);
    return checkAndReturnRecord(sym, symIterPtr);
}

GAMSSymbolRecord GAMSSymbolImpl::addRecord(const GAMSSymbol& sym, const string& key)
{
    if (recordLock())
        throw GAMSException("Cannot add data records to record-locked database");
    if (1 != mDim)
        throw GAMSException("Different dimensions: 1 vs. " + to_string(mDim));
    const char* cPtr[] { key.c_str()};
    void* symIterPtr = 0;

    checkForGMDError(gmdAddRecord(gmd(), mSymPtr, cPtr, &symIterPtr), __FILE__, __LINE__);
    return checkAndReturnRecord(sym, symIterPtr);
}

void GAMSSymbolImpl::deleteRecord(const std::vector<string>& keys)
{
    if (recordLock())
        throw GAMSException("Cannot remove data records to record-locked database");
    checkKeys(keys);
    void* symIterPtr = 0;
    if (!gmdFindRecord(gmd(), mSymPtr, mIndexC.set(keys).cPtrs(), &symIterPtr))
        throw GAMSException("Cannot identify record for deletion");
    checkForGMDError(gmdDeleteRecord(gmd(), symIterPtr), __FILE__, __LINE__);
    checkForGMDError(gmdFreeSymbolIterator(gmd(), symIterPtr), __FILE__, __LINE__);
}

GAMSSymbolRecord GAMSSymbolImpl::mergeRecord(const GAMSSymbol& sym, const std::vector<string>& keys)
{
    checkKeys(keys);
    mIndexC.set(keys);
    void *symIterPtr = 0;

    if (!gmdFindRecord(gmd(), mSymPtr, mIndexC.cPtrs(), &symIterPtr)) {
        if (recordLock())
            throw GAMSException("Cannot add data records to record-locked database");
        if (!gmdAddRecord(gmd(), mSymPtr, mIndexC.cPtrs(), &symIterPtr)) {
            string k = "";
            for (unsigned int i = 0; i < keys.size(); i++)
                k = keys[i] + ", ";
            if (k.size() > 0)
                k = k.substr(0, k.size() - 2);
            throw GAMSException("Record '" + k + "' could neither be found in nor added to symbol '" + mName + "'");
        }
    }
    return checkAndReturnRecord(sym, symIterPtr);
}

GAMSSymbolRecord GAMSSymbolImpl::firstRecord(const GAMSSymbol& sym)
{
    void* symIterPtr = 0;
    checkForGMDError(gmdFindFirstRecord(gmd(), mSymPtr, &symIterPtr), __FILE__, __LINE__);
    return checkAndReturnRecord(sym, symIterPtr);
}

GAMSSymbolRecord GAMSSymbolImpl::firstRecord(const GAMSSymbol& sym, const std::vector<string>& slice)
{
    checkKeys(slice);
    void *symIterPtr = 0;
    checkForGMDError(gmdFindFirstRecordSlice(gmd(), mSymPtr, mIndexC.set(slice).cPtrs(), &symIterPtr), __FILE__, __LINE__);
    return checkAndReturnRecord(sym, symIterPtr);

}

GAMSSymbolRecord GAMSSymbolImpl::lastRecord(const GAMSSymbol& sym)
{
    void* symIterPtr = 0;
    checkForGMDError(gmdFindLastRecord(gmd(), mSymPtr, &symIterPtr), __FILE__, __LINE__);
    return checkAndReturnRecord(sym, symIterPtr);
}

GAMSSymbolRecord GAMSSymbolImpl::lastRecord(const GAMSSymbol& sym, const std::vector<string>& slice)
{
    if (static_cast<int>(slice.size()) != mDim)
        throw GAMSException("Different dimensions : " + to_string(slice.size()) + " vs. " + to_string(mDim));
    void *symIterPtr = 0;
    checkForGMDError(gmdFindLastRecordSlice(gmd(), mSymPtr, mIndexC.set(slice).cPtrs(), &symIterPtr), __FILE__, __LINE__);
    return checkAndReturnRecord(sym, symIterPtr);
}

GAMSSymbolRecord GAMSSymbolImpl::findRecord(const GAMSSymbol& sym, const std::vector<string>& keys)
{
    checkKeys(keys);
    void* symIterPtr = 0;
    checkForGMDError(gmdFindRecord(gmd(), mSymPtr, mIndexC.set(keys).cPtrs(), &symIterPtr), __FILE__, __LINE__);
    return checkAndReturnRecord(sym, symIterPtr);
}

void GAMSSymbolImpl::copySymbol(const GAMSSymbolImpl& target)
{
    if (target.recordLock())
        throw GAMSException("Cannot add data records to record-locked database");
    checkForGMDError(gmdCopySymbol(gmd(), target.mSymPtr, mSymPtr), __FILE__, __LINE__);
}

int GAMSSymbolImpl::numberRecords()
{
    int i = 0;
    double d = 0.0;
    char buf[GMS_SSSIZE];
    checkForGMDError(gmdSymbolInfo(gmd(), mSymPtr, GMD_NRRECORDS, &i, &d, buf), __FILE__, __LINE__);
    return i;
}

bool GAMSSymbolImpl::checkDomains()
{
    int hasDV = 0;
    checkForGMDError(gmdCheckSymbolDV(gmd(), mSymPtr, &hasDV), __FILE__, __LINE__);
    return !hasDV;
}


void GAMSSymbolImpl::checkKeys(const vector<string>& keys)
{
    if (static_cast<int>(keys.size()) != mDim)
        throw GAMSException("Different dimensions: " + to_string(keys.size()) + " vs. " + to_string(mDim));
    if ((mDim > 0) && keys.empty())
        throw GAMSException("'keys' not allowed to be empty"); //TODO: error message
}


GAMSSymbolRecord GAMSSymbolImpl::checkAndReturnRecord(const GAMSSymbol &sym, void* symIterPtr)
{
    if (!mSymPtr)
        throw GAMSException("Invalid Pointer");
    // TODO: check for pointer type
    // if pointer->GAMSSet
    return GAMSSymbolRecord(sym, symIterPtr);
}


std::vector<GAMSDomain> GAMSSymbolImpl::domains()
{
    if (mDomains == nullptr) {
        mDomains = new vector<GAMSDomain>();

        if (mDim == 0)
            return *mDomains;

        void *domPtr[GMS_MAX_INDEX_DIM];
        gdxStrIndexPtrs_t domStr;
        gdxStrIndex_t     domStrXXX;
        GDXSTRINDEXPTRS_INIT(domStrXXX, domStr);
        checkForGMDError(gmdGetDomain(gmd(), mSymPtr, mDim, domPtr, domStr), __FILE__, __LINE__);

        void* universeSymPtr;
        checkForGMDError(gmdFindSymbol(gmd(), "*", &universeSymPtr), __FILE__, __LINE__);
        for (int i = 0; i < mDim; i++) {
            if (domPtr[i] == universeSymPtr) {
                mDomains->push_back("*");
            } else if (domPtr[i]) {
                mDomains->push_back(GAMSSet(mDatabase, domPtr[i]));
            } else {
                mDomains->push_back(domStr[i]);
            }
        }
    }
    return *mDomains;
}


/// converts a vector of strings to an array of char arrays
void GAMSSymbolImpl::vStrToCharArr(const vector<string>& input, gdxStrIndex_t &indexC)
{
    for (int i = 0; i < int(input.size()); i++) {
        strcpy(indexC[i], input[i].c_str());
    };
}

std::vector<GAMSSymbolDomainViolation> GAMSSymbolImpl::getSymbolDVs(GAMSSymbol& sym, bool skipCleanup, int maxViol)
{
    vector<GAMSSymbolDomainViolation> violations;
    void* dvHandle = nullptr;
    int hasNext = 0;

    checkForGMDError(gmdGetFirstDVInSymbol(gmd(), mSymPtr, &dvHandle), __FILE__, __LINE__);

    if (dvHandle)
        hasNext = 1;

    int *viol = new int[mDim];
    std::vector<bool> violTmp;
    while (hasNext && ((maxViol == 0) || (int(violations.size()) < maxViol))) {
        void* symIterPtr = nullptr;
        checkForGMDError(gmdGetDVSymbolRecord(gmd(), dvHandle, &symIterPtr), __FILE__, __LINE__);

        checkForGMDError(gmdGetDVIndicator(gmd(), dvHandle, viol), __FILE__, __LINE__);
        for (int i = 0; i < mDim; i++)
            violTmp.push_back(viol[i] != 0);

        violations.push_back(GAMSSymbolDomainViolation(violTmp, GAMSSymbolRecord(sym, symIterPtr)));
        checkForGMDError(gmdMoveNextDVInSymbol(gmd(), dvHandle, &hasNext), __FILE__, __LINE__);
        violTmp.clear();
    }
    delete[] viol;


    //Cleanup
    if (!skipCleanup) {
        checkForGMDError(gmdDomainCheckDone(gmd()), __FILE__, __LINE__);
        checkForGMDError(gmdFreeDVHandle(gmd(), dvHandle), __FILE__, __LINE__);
    }
    return violations;
}


/*
   //TODO: should we use templates for this or implementations in subclasses?
   vector<GAMSSymbolDomainViolation> GAMSSymbolImpl::getSymbolDVs(bool skipCleanup, int maxViol = 0)
   {
      vector<GAMSSymbolDomainViolation> violations;
      void *dvHandle = nullptr;
      bool hasNext = false;

      checkForGMDError(gmdGetFirstDVInSymbol(gmd(), this->symPtr, &dvHandle));

      if (dvHandle != nullptr)
         hasNext = true;

      while (hasNext && ((maxViol == 0) || (violations.size() < maxViol))) {
         int *viol = new int[this->dim];
         bool *violTmp = new bool[this->dim];

         void *symIterPtr;
         checkForGMDError(gmdGetDVSymbolRecord(gmd(), dvHandle, &symIterPtr), __FILE__, __LINE__);

         checkForGMDError(gmdGetDVIndicator(gmd(), dvHandle, &viol), __FILE__, __LINE__);
         for (int i = 0; i < this->dim; i++)
            violTmp[i] = viol[i] != 0;

         // TODO: complete implementation

         if (this is GAMSVariable)
            Violations.Add(new GAMSSymbolDomainViolation(violTmp, new GAMSVariableRecord((GAMSVariable)this, symIterPtr)));
         if (this is GAMSEquation)
            Violations.Add(new GAMSSymbolDomainViolation(violTmp, new GAMSEquationRecord((GAMSEquation)this, symIterPtr)));
         if (this is GAMSParameter)
            Violations.Add(new GAMSSymbolDomainViolation(violTmp, new GAMSParameterRecord((GAMSParameter)this, symIterPtr)));
         if (this is GAMSSet)
            Violations.Add(new GAMSSymbolDomainViolation(violTmp, new GAMSSetRecord((GAMSSet)this, symIterPtr)));

         checkForGMDError(fGAMSDatabase.GMD.gmdMoveNextDVInSymbol(dvHandle, ref hasNext));
      }


      //Cleanup
      if (!skipCleanup)
      {
         checkForGMDError(fGAMSDatabase.GMD.gmdDomainCheckDone());
         checkForGMDError(fGAMSDatabase.GMD.gmdFreeDVHandle(dvHandle));
      }
      return Violations;

   }      */

}
