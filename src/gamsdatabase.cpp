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

#include "gamslog.h"
#include "gamsdatabaseimpl.h"
#include <iostream>
#include "gamsset.h"
#include "gamsparameter.h"
#include "gamsvariable.h"
#include "gamsequation.h"
#include "gamsdatabase.h"

using namespace std;

namespace gams {


GAMSDatabase::GAMSDatabase(const std::shared_ptr<GAMSDatabaseImpl>& impl)
    : mImpl(impl)
{}


GAMSDatabase::GAMSDatabase(void* gmdPtr, GAMSWorkspace& workspace)
    : mImpl(make_shared<GAMSDatabaseImpl>(gmdPtr, workspace))
{}


GAMSDatabase::GAMSDatabase(const string& gdxFileName, GAMSWorkspace& workspace, const double specValues[]
                           , const string& databaseName, const string& inModelName, bool forceName)
    : mImpl(make_shared<GAMSDatabaseImpl>(gdxFileName, workspace, specValues, databaseName, inModelName, forceName))
{}


GAMSDatabase::GAMSDatabase(GAMSWorkspace& workspace, const double specValues[], const string& databaseName
                           , const string& inModelName, bool forceName)
    : mImpl(make_shared<GAMSDatabaseImpl>(workspace, specValues, databaseName, inModelName, forceName))
{}


GAMSDatabase::GAMSDatabase(GAMSWorkspace& workspace, const double specValues[], const GAMSDatabase &sourceDb
                           , const string& dbName, const string& inModelName)
    : mImpl(make_shared<GAMSDatabaseImpl>(workspace, specValues, sourceDb.mImpl, dbName, inModelName))
{}


std::string GAMSDatabase::inModelName()
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->mInModelName;
}


GAMSDatabase::~GAMSDatabase()
{}


void GAMSDatabase::clear()
{
    for (GAMSDatabaseIter it = begin(); it != end(); ++it)
        if (!(*it).clear())
            throw GAMSException("Cannot clear symbol " + (*it).name());
}


bool GAMSDatabase::checkDomains()
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->checkDomains();
}


void GAMSDatabase::doExport(const std::string& filePath)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    mImpl->doExport(filePath);
}


GAMSSymbol GAMSDatabase::getSymbol(const string& name)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->getSymbol(*this, name);
}


GAMSSet GAMSDatabase::getSet(const std::string& name)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->getSymbol(*this, name);
}


GAMSParameter GAMSDatabase::getParameter(const std::string& name)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->getSymbol(*this, name);
}

GAMSVariable GAMSDatabase::getVariable(const std::string& name)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->getSymbol(*this, name);
}

GAMSEquation GAMSDatabase::getEquation(const std::string& name)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->getSymbol(*this, name);
}

GAMSSet GAMSDatabase::addSet(const string& name, const int dimension, const string& explanatoryText)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");
    return mImpl->addSet(*this, name, dimension, explanatoryText);
}

GAMSSet GAMSDatabase::addSet(const std::string& name, const std::string& explanatoryText
                             , const std::vector<GAMSDomain>& domains)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");
    return mImpl->addSet(*this, name, explanatoryText, domains);
}

GAMSSet GAMSDatabase::addSet(const std::string& name, const std::string& explanatoryText
                             , GAMSDomain domain1, GAMSDomain domain2, GAMSDomain domain3)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");

    vector<GAMSDomain> domains;
    if (!domain1.isValid())
        throw GAMSException("GAMSDatabase: Parameter domain1 has not been initialized");
    domains.push_back(domain1);

    if(domain2.isValid()) {
        domains.push_back(domain2);
        if(domain3.isValid())
            domains.push_back(domain3);
    }
    return addSet(name, explanatoryText, domains);
}

GAMSParameter GAMSDatabase::addParameter(const std::string& name, const int dimension, const std::string& explanatoryText)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");
    return mImpl->addParameter(*this, name, dimension, explanatoryText);
}


GAMSParameter GAMSDatabase::addParameter(const std::string& name, const std::string& explanatoryText
                                         , const std::vector<GAMSDomain>& domains)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");
    return mImpl->addParameter(*this, name, explanatoryText, domains);
}

GAMSParameter GAMSDatabase::addParameter(const std::string& name, const std::string& explanatoryText
                                         , GAMSDomain domain1, GAMSDomain domain2, GAMSDomain domain3)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");

    vector<GAMSDomain> domains;
    if (!domain1.isValid())
        throw GAMSException("GAMSDatabase: Parameter domain1 has not been initialized");
    domains.push_back(domain1);

    if(domain2.isValid())
    {
        domains.push_back(domain2);
        if(domain3.isValid())
            domains.push_back(domain3);
    }
    return addParameter(name, explanatoryText, domains);
}

GAMSVariable GAMSDatabase::addVariable(const std::string& name, const int dimension, const GAMSEnum::VarType varType
                                       , const std::string& explanatoryText)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");
    return mImpl->addVariable(*this, name, dimension, varType, explanatoryText);
}

GAMSVariable GAMSDatabase::addVariable(const string& name, const GAMSEnum::VarType varType, const string& explanatoryText
                                       , const std::vector<GAMSDomain>& domains)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");

    return mImpl->addVariable(*this, name, varType, explanatoryText, domains);
}

GAMSVariable GAMSDatabase::addVariable(const std::string& name, const GAMSEnum::VarType varType
                                       , const std::string& explanatoryText, GAMSDomain domain1
                                       , GAMSDomain domain2, GAMSDomain domain3)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");

    vector<GAMSDomain> domains;
    if (!domain1.isValid())
        throw GAMSException("GAMSDatabase: Parameter domain1 has not been initialized");
    domains.push_back(domain1);

    if(domain2.isValid())
    {
        domains.push_back(domain2);
        if(domain3.isValid())
            domains.push_back(domain3);
    }
    return addVariable(name, varType, explanatoryText, domains);
}

GAMSEquation GAMSDatabase::addEquation(const std::string& name, const int dimension, const GAMSEnum::EquType equType
                                       , const std::string& explanatoryText)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");

    return mImpl->addEquation(*this, name, dimension, equType, explanatoryText);
}

GAMSEquation GAMSDatabase::addEquation(const std::string& name, const GAMSEnum::EquType equType
                                       , const std::string& explanatoryText, const std::vector<GAMSDomain>& domains)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");

    return mImpl->addEquation(*this, name, equType, explanatoryText, domains);
}

GAMSEquation GAMSDatabase::addEquation(const std::string& name, const GAMSEnum::EquType equType
                                       , const std::string& explanatoryText, GAMSDomain domain1, GAMSDomain domain2
                                       , GAMSDomain domain3)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    if (mImpl->symbolLock()) throw GAMSException("Cannot add symbols to symbol-locked database");

    vector<GAMSDomain> domains;
    if (!domain1.isValid())
        throw GAMSException("GAMSDatabase: Parameter domain1 has not been initialized");
    domains.push_back(domain1);

    if(domain2.isValid())
    {
        domains.push_back(domain2);
        if(domain3.isValid())
            domains.push_back(domain3);
    }
    return addEquation(name, equType, explanatoryText, domains);
}

std::string GAMSDatabase::name()
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->mDatabaseName;
}


void GAMSDatabase::setSuppressAutoDomainChecking(bool value)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    mImpl->mSuppressAutoDomainChecking = value;
}


bool GAMSDatabase::suppressAutoDomainChecking()
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->mSuppressAutoDomainChecking;
}


int GAMSDatabase::getNrSymbols()
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->getNrSymbols();
}


GAMSWorkspace GAMSDatabase::workspace()
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return GAMSWorkspace(mImpl->mWs);
}


bool GAMSDatabase::operator!=(const GAMSDatabase& other) const
{
    return !operator==(other);
}

bool GAMSDatabase::operator==(const GAMSDatabase& other) const
{
    return mImpl == other.mImpl || (mImpl && other.mImpl && *mImpl.get() == *other.mImpl.get());
}

std::vector<GAMSDatabaseDomainViolation> GAMSDatabase::getDatabaseDVs(int maxViol, int maxViolPerSym)
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->getDatabaseDVs(*this, maxViol, maxViolPerSym);
}


LogId GAMSDatabase::logID()
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    return mImpl->logID();
}

GAMSDatabase::GAMSDatabase()
{}

GAMSDatabase& GAMSDatabase::operator=(const GAMSDatabase& other)
{
    mImpl = other.mImpl;
    return *this;
}

bool GAMSDatabase::isValid() const
{
    return bool(mImpl);
}

GAMSDatabaseIter GAMSDatabase::begin()
{
    return GAMSDatabaseIter(mImpl);
}

GAMSDatabaseIter GAMSDatabase::end()
{
    if (!mImpl) throw GAMSException("GAMSDatabase: The database has not been initialized.");
    int nrSymbols = 0;
    double dval = 0.0;
    char sval;
    gmdInfo(mImpl->gmd(), GMD_NRSYMBOLS, &nrSymbols, &dval, &sval);
    return GAMSDatabaseIter(mImpl, nrSymbols);
}
} // namespace gams
