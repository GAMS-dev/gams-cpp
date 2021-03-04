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

#ifndef GAMSDATABASEIMPL_H
#define GAMSDATABASEIMPL_H

#include <vector>
#include <memory>
#include <string>
#include "gmdcc.h"
#include "gamsworkspace.h"
#include "gamsvariablerecord.h"
#include "gamslib_global.h"
#include "gamsexception.h"
#include "gamsdomain.h"

namespace gams {

class GAMSWorkspace;
class GAMSSymbol;
class GAMSSet;
class GAMSDatabaseDomainViolation;

class GAMSDatabaseImpl
{
public:
    // Constructor
    GAMSDatabaseImpl(const std::string& gdxFileName, const GAMSWorkspace& workspace, const double specValues[]
                     , const std::string& databaseName = "", const std::string& inModelName = "", bool forceName = false);

    GAMSDatabaseImpl(const GAMSWorkspace& workspace, const double specValues[], const std::string& databaseName = ""
                     , const std::string& inModelName = "", bool forceName = false);

    GAMSDatabaseImpl(const GAMSWorkspace& workspace, const double specValues[], std::shared_ptr<GAMSDatabaseImpl> sourceDb
                     , const std::string& dbName = "", const std::string& inModelName = "");

    GAMSDatabaseImpl(void* gmdPtr, const GAMSWorkspace &workspace);
    ~GAMSDatabaseImpl();

    bool operator!=(const GAMSDatabaseImpl& other) const;
    bool operator==(const GAMSDatabaseImpl& other) const;

    std::string name() { return mDatabaseName; }
    //GAMSSet addSet(const std::string& _identifier, const int _dimension, const std::string& _explanatoryText, const std::string& rDom0);

    void checkForGMDError(int retcode, const std::string& file, int linenr);
    LogId logID() { return mWs.logID(); }

    bool checkDomains();
    void doExport(const std::string& filePath = "");

    GAMSSymbol getSymbol(GAMSDatabase& database, const std::string& name);
    GAMSSymbol getSymbol(GAMSDatabase& database, void* symPtr);
    int getNrSymbols();

    GAMSEquation addEquation(GAMSDatabase& db, const std::string& name, const int dimension
                             , const GAMSEnum::EquType equType, const std::string& explanatoryText = "");

    GAMSEquation addEquation(GAMSDatabase& db, const std::string& name, const GAMSEnum::EquType equType
                             , const std::string& explanatoryText = ""
                             , const std::vector<GAMSDomain>& domains = std::vector<GAMSDomain>());

    GAMSSet addSet(GAMSDatabase& db, const std::string& name, const int dimension, const std::string& explanatoryText = "");

    GAMSSet addSet(GAMSDatabase& db, const std::string& name, const std::string& explanatoryText
                   , const std::vector<GAMSDomain>& domains = std::vector<GAMSDomain>());

    GAMSParameter addParameter(GAMSDatabase& db, const std::string& name, const int dimension
                               , const std::string& explanatoryText = "");

    GAMSParameter addParameter(GAMSDatabase& db, const std::string& name, const std::string& explanatoryText = ""
                               , const std::vector<GAMSDomain>& domains = std::vector<GAMSDomain>());

    GAMSVariable addVariable(GAMSDatabase& db, const std::string& name, const int dimension
                             , const GAMSEnum::VarType varType, const std::string& explanatoryText = "");

    GAMSVariable addVariable(GAMSDatabase& db, const std::string& name, const GAMSEnum::VarType varType
                             , const std::string& explanatoryText = ""
                             , const std::vector<GAMSDomain>& domains = std::vector<GAMSDomain>());

    gmdHandle_t gmd();

    std::vector<GAMSDatabaseDomainViolation> getDatabaseDVs(const GAMSDatabase& db, int maxViol = 0
                                                            , int maxViolPerSym = 0);

    GAMSSymbol getSymbolByIndex(GAMSDatabase& database, const int pos);

    void lockSymbols();
    void lockRecords();
    bool symbolLock();
    bool recordLock();

    GAMSWorkspace mWs;
    std::string mInModelName;
    std::string mDatabaseName;
    bool mSuppressAutoDomainChecking = false;

private:
    gmdHandle_t mGMD = nullptr;
    bool mOwnGMD = true;
    bool mRecordLock = false;
    bool mSymbolLock = false;
};

} // namespace gams

#endif // GAMSDATABASEIMPL_H
