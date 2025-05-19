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

#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "gams.h"

#include <ostream>
#include <string>

/// \file transport.h
/// \brief Wrapper class definition for GAMS trnsport model
class Transport
{
public:
    /// A Transportation Problem
    Transport(gams::GAMSWorkspace ws);

    /// Executes the trnsport model
    void run(gams::GAMSCheckpoint checkpoint, std::ostream &output);

    /// i(i): canning plants
    gams::GAMSSet i() const
    {
        return fi;
    }

    /// j(j): markets
    gams::GAMSSet j() const
    {
        return fj;
    }

    /// a(i): capacity of plant i in cases
    gams::GAMSParameter a() const
    {
        return fa;
    }

    /// b(i): demand at market j in cases
    gams::GAMSParameter b() const
    {
        return fb;
    }

    /// d(i,j): distance in thousands of miles
    gams::GAMSParameter d() const
    {
        return fd;
    }

    /// f: freight in dollars per case per thousand miles
    gams::GAMSParameter f() const
    {
        return ff;
    }

    /// x(i,j): shipment quantities in cases
    gams::GAMSVariable x() const
    {
        return fx;
    }

    /// z: total transportation costs in thousands of dollars
    gams::GAMSVariable z() const
    {
        return fz;
    }

    /// Options for the execution of the trnsport model
    gams::GAMSOptions opt() const
    {
        return fopt;
    }

private:
    /// Provide the model source of trnsport
    std::string getModelSource();

private:
    gams::GAMSWorkspace fws;
    gams::GAMSDatabase fDbIn1, fDbIn2, fDbOut1;
    gams::GAMSJob job;

    gams::GAMSSet fi, fj;
    gams::GAMSParameter fa, fb, fd, ff;
    gams::GAMSVariable fx, fz;
    gams::GAMSOptions fopt;
};

#endif // TRANSPORT_H
