/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2023 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2023 GAMS Development Corp. <support@gams.com>
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

#include "gamslog.h"
#include "gamsdatabaseimpl.h"
#include <iostream>
#include <algorithm>
#include "gamsset.h"
#include "gamsparameter.h"
#include "gamsvariable.h"
#include "gamsequation.h"
#include "gamspath.h"
#include "gamsdatabasedomainviolation.h"

using namespace std;

namespace gams {


GAMSDatabaseImpl::GAMSDatabaseImpl(const string& gdxFileName, GAMSWorkspace& workspace, const double specValues[]
                                   , const string& databaseName, const string& inModelName, bool forceName)
    : GAMSDatabaseImpl::GAMSDatabaseImpl(workspace, specValues, databaseName, inModelName, forceName)
{
    DEB << "---- Entering GAMSDatabaseImpl constructor ----";
    // assigning databaseName catching nullptr
    string databaseNameTmp = databaseName;
    std::transform(databaseNameTmp.begin(), databaseNameTmp.end(), databaseNameTmp.begin(), ::tolower);
    GAMSPath gdxFile(gdxFileName);
    if (databaseNameTmp == gdxFile.suffix("").toStdString())
        throw GAMSException("GAMSDatabase name and gdx file name for initialization must be different (saw " + databaseName + " for both)");
    if (gdxFile.is_absolute())
        checkForGMDError(gmdInitFromGDX(mGMD, gdxFile.c_str()), __FILE__, __LINE__);
    else
        checkForGMDError(gmdInitFromGDX(mGMD, (GAMSPath(workspace.workingDirectory()) / gdxFileName).c_str()), __FILE__, __LINE__);
}


GAMSDatabaseImpl::GAMSDatabaseImpl(GAMSWorkspace& workspace, const double specValues[], const std::string& databaseName
                                   , const std::string& inModelName, bool forceName)
    : mWs(workspace), mDatabaseName(databaseName)
{
    DEB << "---- Entering GAMSDatabaseImpl constructor ----";
    mInModelName = inModelName;
    if (mDatabaseName == "") {
        mDatabaseName = mWs.registerDatabase();
    } else {
        string extensionTmp = GAMSPath(mDatabaseName).suffix();
        std::transform(extensionTmp.begin(), extensionTmp.end(), extensionTmp.begin(), ::tolower);
        if (extensionTmp == ".gdx")
            mDatabaseName = GAMSPath(mDatabaseName).suffix("").toStdString();
        if (mWs.registerDatabase(mDatabaseName).empty() && !forceName)
            throw GAMSException("Database with name " + mDatabaseName + " already exists");
    }

    char msg[GMS_SSSIZE];
    if (!gmdCreateD(&mGMD, mWs.systemDirectory().c_str(), msg, sizeof(msg))){
        throw GAMSException(msg);
    }
    //set debug mode
    if (mWs.debug() == GAMSEnum::DebugLevel::Verbose)
        checkForGMDError(gmdSetDebug(mGMD, 10), __FILE__, __LINE__);
    //we have to register our special values first
    checkForGMDError(gmdSetSpecialValues(mGMD, specValues), __FILE__, __LINE__);

    // TODO: set member universe if we need it
    //universe = getSet("*");
    //TODO: getSet is unavailable from GAMSDatabaseImpl, implement it again here:
}


GAMSDatabaseImpl::GAMSDatabaseImpl(GAMSWorkspace& workspace, const double specValues[]
                                   , std::shared_ptr<GAMSDatabaseImpl> sourceDb, const string& databaseName
                                   , const string& inModelName)
    : GAMSDatabaseImpl(workspace, specValues, databaseName, inModelName)
{
    //TODO: disposed mechanism from C# is not implemented. DO we need this in C++?
    checkForGMDError(gmdInitFromDB(mGMD, sourceDb->mGMD), __FILE__, __LINE__);
}


GAMSDatabaseImpl::GAMSDatabaseImpl(void* gmdPtr, GAMSWorkspace& workspace)
    :mWs(workspace)
{
    DEB << "---- Entering GAMSDatabaseImpl constructor ----";
    mDatabaseName = mWs.registerDatabase();
    if (gmdPtr) {
        mGMD = static_cast<gmdHandle_t>(gmdPtr);
        mOwnGMD = false;
    } else {
        throw GAMSException("no valid GMD pointer");
    }
}


GAMSDatabaseImpl::~GAMSDatabaseImpl()
{
    if (mGMD && mOwnGMD) {
        gmdFree(&mGMD);
    }
}

bool GAMSDatabaseImpl::operator!=(const GAMSDatabaseImpl& other) const
{
    return (mWs != other.mWs) || (mDatabaseName != other.mDatabaseName);
}

bool GAMSDatabaseImpl::operator==(const GAMSDatabaseImpl& other) const
{
    return !(operator!=(other));
}


void GAMSDatabaseImpl::checkForGMDError(int retcode, const string &file, int linenr)
{
    if (retcode != 1) {
        char msg[GMS_SSSIZE];
        gmdGetLastError(mGMD, msg);
        throw GAMSException(msg + (" at [" + file) + ", " + std::to_string(linenr) + "]");
    }
}

bool GAMSDatabaseImpl::checkDomains()
{
    int hasDV = 0;
    checkForGMDError(gmdCheckDBDV(mGMD, &hasDV), __FILE__, __LINE__);
    return hasDV == 0;
}

void GAMSDatabaseImpl::doExport(const string& filePath)
{
    if (!mSuppressAutoDomainChecking) {
        if (!checkDomains())
            throw GAMSException("Domain vioaltions in GAMSDatabase " + mDatabaseName);
    }
    if (filePath == "") {
        checkForGMDError(gmdWriteGDX(mGMD, (GAMSPath(mWs.workingDirectory()) / (mDatabaseName + ".gdx")).c_str()
                                     , mSuppressAutoDomainChecking), __FILE__, __LINE__);
    } else {
        GAMSPath filePathTmp(filePath);
        filePathTmp.setSuffix(".gdx");
        if (filePathTmp.is_absolute())
            checkForGMDError(gmdWriteGDX(mGMD, filePathTmp.c_str(), mSuppressAutoDomainChecking), __FILE__, __LINE__);
        else
            checkForGMDError(gmdWriteGDX(mGMD, (GAMSPath(mWs.workingDirectory()) / filePathTmp).c_str()
                                         , mSuppressAutoDomainChecking), __FILE__, __LINE__);
    }
}

GAMSSymbol GAMSDatabaseImpl::getSymbol(GAMSDatabase& database, const string& name)
{
    void* symPtr;
    checkForGMDError(gmdFindSymbol(mGMD, name.c_str(), &symPtr), __FILE__, __LINE__);
    if (!symPtr)
        throw GAMSException("GAMSDatabase: Cannot find symbol " + name);
    return getSymbol(database, symPtr);
}

GAMSSymbol GAMSDatabaseImpl::getSymbol(GAMSDatabase& database, void* symPtr)
{
    if (!symPtr)
        throw GAMSException("Symbol does not exist");
    GAMSEnum::VarType varType = GAMSEnum::Unknown;
    GAMSEnum::EquType eqType = GAMSEnum::E;

    int type = 0;
    checkForGMDError(gmdSymbolType(mGMD, symPtr, &type), __FILE__, __LINE__);
    GAMSEnum::SymbolType symType = GAMSEnum::SymbolType(type);

    char sval[GMS_SSSIZE];
    int ival = 0;
    double dval = 0.0;
    checkForGMDError(gmdSymbolInfo(mGMD, symPtr, GMD_NAME, &ival, &dval, sval), __FILE__, __LINE__);
    std::string name = std::string(sval);

    int dim;
    checkForGMDError(gmdSymbolInfo(mGMD, symPtr, GMD_DIM, &dim, &dval, NULL), __FILE__, __LINE__);
    checkForGMDError(gmdSymbolInfo(mGMD, symPtr, GMD_EXPLTEXT, &ival, &dval, sval), __FILE__, __LINE__);

    std::string text = std::string(sval);
    if (symType == GAMSEnum::SymTypeVar) {
        int subType = 0;
        checkForGMDError(gmdSymbolInfo(mGMD, symPtr, GMD_USERINFO, &subType, NULL, NULL), __FILE__, __LINE__);
        varType = static_cast<GAMSEnum::VarType>(subType);
    }
    if (symType == GAMSEnum::SymTypeEqu) {
        int subType = 0;
        checkForGMDError(gmdSymbolInfo(mGMD, symPtr, GMD_USERINFO, &subType, NULL, NULL), __FILE__, __LINE__);
        eqType = static_cast<GAMSEnum::EquType>(subType);
    }

    switch (symType) {
    case GAMSEnum::SymTypePar:
        return GAMSParameter(database, symPtr, dim, name, text);
    case GAMSEnum::SymTypeVar:
        return GAMSVariable(database, symPtr, dim, name, text, varType);
    case GAMSEnum::SymTypeEqu:
        return GAMSEquation(database, symPtr, dim, name, text, eqType);
    default: // GAMSEnum::SymTypeSet:
        return GAMSSet(database, symPtr, dim, name, text);
    }
}

int GAMSDatabaseImpl::getNrSymbols()
{
    int nrSymbols = 0;
    checkForGMDError(gmdInfo(mGMD, GMD_NRSYMBOLS, &nrSymbols, NULL, NULL), __FILE__, __LINE__);
    return nrSymbols;
}

GAMSEquation GAMSDatabaseImpl::addEquation(GAMSDatabase& db, const string& name, const int dimension
                                           , const GAMSEnum::EquType equType, const string& explanatoryText)
{
    return GAMSEquation(db, name, dimension, explanatoryText, equType);
}

GAMSEquation GAMSDatabaseImpl::addEquation(GAMSDatabase& db, const string& name, const GAMSEnum::EquType equType
                                           , const string& explanatoryText, const std::vector<GAMSDomain>& domains)
{
    return GAMSEquation(db, name, explanatoryText, domains, equType);
}

GAMSSet GAMSDatabaseImpl::addSet(GAMSDatabase& db, const string& name, const int dimension, const string& explanatoryText, GAMSEnum::SetType setType)
{
    return GAMSSet(db, name, dimension, explanatoryText, setType);
}

GAMSSet GAMSDatabaseImpl::addSet(GAMSDatabase& db, const string& name, const string& explanatoryText
                                 , const std::vector<GAMSDomain>& domains, GAMSEnum::SetType setType)
{
    if(domains.size() == 0)
        return GAMSSet(db, name, 1, explanatoryText);
    return GAMSSet(db, name, explanatoryText, domains, setType);
}

GAMSParameter GAMSDatabaseImpl::addParameter(GAMSDatabase& db, const string& name, const int dimension
                                             , const string& explanatoryText)
{
    return GAMSParameter(db, name, dimension, explanatoryText);
}

GAMSParameter GAMSDatabaseImpl::addParameter(GAMSDatabase& db, const string& name, const string& explanatoryText
                                             , const std::vector<GAMSDomain>& domains)
{
    return GAMSParameter(db, name, explanatoryText, domains);
}

GAMSVariable GAMSDatabaseImpl::addVariable(GAMSDatabase& db, const string& name, const int dimension
                                           , const GAMSEnum::VarType varType, const string& explanatoryText)
{
    return GAMSVariable(db, name, dimension, explanatoryText, varType);
}

GAMSVariable GAMSDatabaseImpl::addVariable(GAMSDatabase& db, const string& name, const GAMSEnum::VarType varType
                                           , const string& explanatoryText, const std::vector<GAMSDomain>& domains)
{
    return GAMSVariable(db, name, explanatoryText, domains, varType);
}

gmdHandle_t GAMSDatabaseImpl::gmd()
{
    return mGMD;
}

std::vector<GAMSDatabaseDomainViolation> GAMSDatabaseImpl::getDatabaseDVs(const GAMSDatabase &db, int maxViol
                                                                          , int maxViolPerSym)
{
    std::vector<GAMSDatabaseDomainViolation> violations;
    int violAll = 0;
    int type = 0;

    void* dvHandle = nullptr;
    int hasNextDV = 0;

    checkForGMDError(gmdGetFirstDBDV(mGMD, &dvHandle), __FILE__, __LINE__);

    if (dvHandle)
        hasNextDV = 1;

    while (hasNextDV && ((maxViol == 0) || (violAll < maxViol))) {
        void* symPtr = nullptr;
        checkForGMDError(gmdGetDVSymbol(mGMD, dvHandle, &symPtr), __FILE__, __LINE__);
        checkForGMDError(gmdSymbolType(mGMD, symPtr, &type), __FILE__, __LINE__);

        int maxViolNextSym = maxViolPerSym;
        if (maxViol > 0) {
            if (maxViolNextSym > 0)
                maxViolNextSym = min(maxViol - violAll, maxViolPerSym);
            else
                maxViolNextSym = maxViol - violAll;
        }

        GAMSSymbol violSym = GAMSSymbol(db, symPtr);
        GAMSDatabaseDomainViolation dbViol = GAMSDatabaseDomainViolation(violSym, violSym.getSymbolDVs(true, maxViolNextSym));
        violations.push_back(dbViol);
        violAll += static_cast<int>(dbViol.violRecs().size());

        checkForGMDError(gmdGetFirstDVInNextSymbol(mGMD, dvHandle, &hasNextDV), __FILE__, __LINE__);
    }

    //Cleanup
    checkForGMDError(gmdDomainCheckDone(mGMD), __FILE__, __LINE__);
    checkForGMDError(gmdFreeDVHandle(mGMD, dvHandle), __FILE__, __LINE__);
    return violations;

}

GAMSSymbol GAMSDatabaseImpl::getSymbolByIndex(GAMSDatabase& database, const int pos)
{
    void* symPtr = nullptr;
    checkForGMDError(gmdGetSymbolByIndex(mGMD, pos, &symPtr), __FILE__, __LINE__);
    if (!symPtr) {
        throw GAMSException("GAMSDatabase: Cannot find symbol at position " + to_string(pos));
    }
    return getSymbol(database, symPtr);
}

void GAMSDatabaseImpl::lockSymbols()
{
    mSymbolLock = true;
    mRecordLock = false;
}

void GAMSDatabaseImpl::lockRecords()
{
    mSymbolLock = false;
    mRecordLock = true;
}

bool GAMSDatabaseImpl::symbolLock()
{
    return mSymbolLock;
}

bool GAMSDatabaseImpl::recordLock()
{
    return mRecordLock;
}

//GAMSSet GAMSDatabaseImpl::AddSet(const std::string& _identifier, const int _dimension, const std::string& _explanatoryText, const std::vector<std::string>& relaxedDomain)
//{
//   return GAMSSet(*this, _identifier, _dimension, _explanatoryText, relaxedDomain);
//}

//GAMSSet GAMSDatabaseImpl::AddSet(const std::string& _identifier, const int _dimension, const std::string& _explanatoryText, const std::vector<GAMSSet>& Domain)
//{
//   return GAMSSet(*this, _identifier, _dimension, _explanatoryText, Domain);
//}

//GAMSSet GAMSDatabaseImpl::AddSet(const std::string& _identifier, const int _dimension, const std::string& _explanatoryText, const GAMSSet& Dom0)
//{
//   return GAMSSet(*this, _identifier, _dimension, _explanatoryText, Dom0);
//}


} // namespace gams
