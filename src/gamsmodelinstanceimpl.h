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

#ifndef GAMSMODELINSTANCEIMPL_H
#define GAMSMODELINSTANCEIMPL_H

#include <string>
#include <vector>
#include "gamsenum.h"
#include "gamsoptions.h"
#include "gamsmodelinstance.h"
#include "gamscheckpoint.h"
#include "gevmcc.h"
#include "gmomcc.h"
#include "gmdcc.h"
#include "gamspath.h"
#include "gamsdatabase.h"


namespace gams {

class GAMSModelInstanceOpt;

class GAMSModelInstanceImpl
{
public:
    /// Constructor
    /// \param checkpoint GAMSCheckpoint
    /// \param modelInstanceName Identifier of GAMSModelInstance (determined automatically if omitted)
    GAMSModelInstanceImpl(GAMSCheckpoint& checkpoint, const std::string& modelInstanceName = "");

    GAMSModelInstanceImpl(std::shared_ptr<GAMSModelInstanceImpl> sourceMI, const std::string& modelInstanceName = "");

    ~GAMSModelInstanceImpl();

    bool operator!=(const GAMSModelInstanceImpl& other) const;
    bool operator==(const GAMSModelInstanceImpl& other) const;

    GAMSEnum::ModelStat modelStatus();
    std::string modelStatusAsString();
    GAMSEnum::SolveStat solveStatus();
    std::string solveStatusAsString();

    void interrupt();

    GAMSEnum::DebugLevel debug();
    LogId logID() { return mCheckpoint.logID(); }
    void checkForGMDError(int retcode, const std::string& file, int linenr);
    gmdHandle_t gmd();

    /// Don't allow records to be added, we need an empty uel list when synchronizing with dictionary
    void dbLockRecords();

    /// Lock syncDB symbols so user can't add new symbols which would result in other exceptions
    /// Unlock syncDB record so user can add data for modifiers
    void dbLockSymbols();

    void instantiate(const std::string& modelDefinition, const GAMSOptions* options, const std::vector<GAMSModifier>& modifiers);
    void solve(GAMSEnum::SymbolUpdateType updateType, std::ostream* output, GAMSModelInstanceOpt miOpt);

public:
    gevHandle_t mGEV;
    gmoHandle_t mGMO;
    GAMSCheckpoint mCheckpoint;
    std::string mModelInstanceName;
    GAMSDatabase mSyncDb;

private:
    GAMSPath mScrDir;
    std::vector<GAMSModifier> mModifiers;
    bool mInstantiated;
    bool mLogAvailable;
    std::string mSelectedSolver = "";
    void solve(GAMSEnum::SymbolUpdateType updateType, std::string output, GAMSModelInstanceOpt miOpt);

};

}
#endif // GAMSMODELINSTANCEIMPL_H
