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

#ifndef GAMSSYMBOLIMPL_H
#define GAMSSYMBOLIMPL_H

#include "gamsenum.h"
#include <string>
#include <memory>
#include <vector>
#include "gamspath.h"
#include "gclgms.h"
#include "gmdcc.h"
#include "gamslib_global.h"
#include "gamsdatabase.h"

namespace gams{

class GAMSSymbol;
class GAMSSymbolRecord;

struct GdxStringArray {
    GdxStringArray() {
        GDXSTRINDEXPTRS_INIT(mIndxC, mIndxCPtrs);
    }

    const char **cPtrs() const {
        return const_cast<const char **>(mIndxCPtrs);
    }

    GdxStringArray& set(const std::vector<std::string>& input) {
        for (int i = 0; i < int(input.size()); i++) {
            strcpy(mIndxC[i], input[i].c_str());
        };
        return *this;
    }

    void set(const int index, const std::string& input) {
        strcpy(mIndxC[index], input.c_str());
    }

private:
    gdxStrIndex_t mIndxC;
    gdxStrIndexPtrs_t mIndxCPtrs;
};

class GAMSSymbolImpl
{
public:

    /// Constructor that gathers missing data from GMD.
    /// \param database Corresponding GAMSDatabase</param>
    /// \param symPtr Symbol Pointer</param>
    GAMSSymbolImpl(GAMSDatabase database, void* symPtr);

    GAMSSymbolImpl(GAMSDatabase database, int dim, const std::string& name, const std::string& text,
                   GAMSEnum::SymbolType symType, GAMSEnum::VarType varType, GAMSEnum::EquType equType);

    GAMSSymbolImpl(GAMSDatabase database, std::string name, std::string text, GAMSEnum::SymbolType symType,
                   GAMSEnum::VarType varType, GAMSEnum::EquType equType, const std::vector<GAMSDomain>& domains);

    GAMSSymbolImpl(GAMSDatabase database, void *symPtr, int dim, const std::string& name, const std::string& text,
                   GAMSEnum::SymbolType symType, GAMSEnum::VarType varType, GAMSEnum::EquType equType);

    /// Destructor
    virtual ~GAMSSymbolImpl();

    bool operator !=(const GAMSSymbolImpl& other);
    bool operator ==(const GAMSSymbolImpl& other);

    GAMSSymbolRecord addRecord(const GAMSSymbol& sym, const std::vector<std::string>& keys);

    /// Delete GAMSSymbol record
    /// \param keys List of keys
    /// \returns True if everything worked, else (record does not exist) false
    void deleteRecord(const std::vector<std::string>& keys);

    GAMSSymbolRecord mergeRecord(const GAMSSymbol& sym, const std::vector<std::string>& keys);

    bool clear();

    GAMSSymbolRecord firstRecord(const GAMSSymbol& sym);
    GAMSSymbolRecord firstRecord(const GAMSSymbol& sym, const std::vector<std::string>& slice);
    GAMSSymbolRecord lastRecord(const GAMSSymbol& sym);
    GAMSSymbolRecord lastRecord(const GAMSSymbol& sym, const std::vector<std::string>& slice);
    GAMSSymbolRecord findRecord(const GAMSSymbol& sym, const std::vector<std::string>& keys);

    void copySymbol(const GAMSSymbolImpl& target);
    int numberRecords();

    bool checkDomains();
    std::vector<GAMSSymbolDomainViolation> getSymbolDVs(GAMSSymbol& sym, bool skipCleanup, int maxViol = 0);

    LogId logID();

    std::vector<GAMSDomain> domains();
    void checkForGMDError(int retcode, const std::string& file, int linenr);
    bool recordLock() const;
    gmdHandle_t gmd() const;
    void* symPtr();

    GAMSDatabase mDatabase;
    std::string mName;
    int mDim;
    GAMSEnum::SymbolType mSymType;
    GAMSEnum::VarType mVarType;
    GAMSEnum::EquType mEquType;
    std::string mExplanatoryText;

private:
    void* mSymPtr = nullptr;
    std::vector<GAMSDomain> *mDomains = nullptr;
    GdxStringArray mIndexC;

    void checkKeys(const std::vector<std::string>& keys);
    GAMSSymbolRecord checkAndReturnRecord(const GAMSSymbol &sym, void* sumIterPtr);
    void vStrToCharArr(const std::vector<std::string>& input, gdxStrIndex_t& indexC);

};

} // namespace gams

#endif // GAMSSYMBOLIMPL_H
