/*
 * GAMS - General Algebraic Modeling System C++ API
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

#ifndef GAMSCHECKPOINTIMPL_H
#define GAMSCHECKPOINTIMPL_H
#include <string>
#include "gamslib_global.h"
#include "gamsworkspace.h"
#include "gamsmodelinstance.h"

namespace gams {

class GAMSCheckpointImpl
{
public:
    GAMSCheckpointImpl(const GAMSWorkspace& workspace, const std::string& checkpointName = "");
    ~GAMSCheckpointImpl();

    bool operator!=(const GAMSCheckpointImpl& other) const;
    bool operator==(const GAMSCheckpointImpl& other) const;

    GAMSModelInstance addModelInstance(GAMSCheckpoint& cp, const std::string& modelInstanceName);

    LogId logID() { return mWs.logID(); }
    GAMSWorkspace mWs;
    std::string mName;
    std::string mCheckpointFileName;
};

}
#endif // GAMSCHECKPOINTIMPL_H
