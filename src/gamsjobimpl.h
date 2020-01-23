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

#ifndef GAMSJOBIMPL_H
#define GAMSJOBIMPL_H

#include <string>
#include <memory>
#include <QProcess>
#include <vector>
#include "gamsenum.h"
#include "gamsworkspace.h"
#include "gamsdatabase.h"
#include "gamsoptions.h"

namespace gams
{
class GAMSCheckpoint;

class GAMSJobImpl
{
public:

    /// Destructor
    ~GAMSJobImpl();

    //TODO: implement the overload functions with default arguments
    /// Retrieve GAMSJob by string
    /// \param name workspace GAMSWorkspace containing to GAMSJob
    /// \param name gamsSource GAMS model as string
    /// \param name checkpoint GAMSCheckpoint to initialize GAMSJob from
    /// \param name jobName Job name (determined automatically if omitted)
    /// @returns
    //      static shared_ptr<GAMSJobImpl> fromString(GAMSWorkspace workspace, std::string gamsSource, const GAMSCheckpoint *checkpoint, string mJobName = "");

    //TODO: implement the overload functions with default arguments
    /// Retrieve GAMSJob by file name
    /// \param workspace GAMSWorkspace containing to GAMSJob
    /// \param fileName GAMS source file name
    /// \param checkpoint GAMSCheckpoint
    /// \param jobName Job name (determined automatically if omitted)
    /// @returns></returns>
    //      static shared_ptr<GAMSJobImpl> fromFile(GAMSWorkspace workspace, std::string mFileName, const GAMSCheckpoint *checkpoint, string mJobName = "");

    /// Constructor
    /// \param workspace GAMSWorkspace containing GAMSJob
    /// \param jobName Identifier of GAMSJob
    /// \param fileName File name
    /// \param checkpoint GAMSCheckpoint
    GAMSJobImpl(GAMSWorkspace workspace, const std::string& mJobName, const std::string& mFileName,
                const GAMSCheckpoint *checkpoint);

    bool operator!=(const GAMSJobImpl& other) const;
    bool operator==(const GAMSJobImpl& other) const;

    void run(GAMSOptions* gamsOptions = nullptr, GAMSCheckpoint* checkpoint = nullptr, std::ostream* output = nullptr,
             bool createOutDb = true, std::vector<GAMSDatabase> databases = std::vector<GAMSDatabase>());

    GAMSDatabase outDB();

    bool interrupt();
    LogId logID() { return mWs.logID(); }

public:
    GAMSWorkspace mWs;
    std::string mJobName;

private:
    GAMSDatabase mOutDb;
    std::string mFileName;
    GAMSCheckpoint* mCheckpointStart = nullptr;
    QProcess mProc;
};
}
#endif // GAMSJOBIMPL_H
