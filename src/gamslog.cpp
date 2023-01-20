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

#include <stdio.h>

#include "gamslog.h"

using namespace std;
namespace gams {

LoggerPool *LoggerPool::mInstance = 0;

LoggerPool &LoggerPool::instance() {
    if (!mInstance) mInstance = new LoggerPool();
    return *mInstance;
}

void LoggerPool::registerLogger(const LogId logId, const GAMSEnum::DebugLevel debug, FILE *target) {
    mBinds[logId] = TargetSet(debug, target);
}

void LoggerPool::unregisterLogger(const LogId logId)
{
    mBinds.erase(logId);
}

Logger::~Logger()
{
    for (FILE* f: mTargets)
        fprintf(f, "[%30s] %s\n", mWhere.c_str(), mBuffer.c_str());
}

}
