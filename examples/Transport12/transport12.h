/*
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

#ifndef TRANSPORT12_H
#define TRANSPORT12_H

#include "gams.h"
#include <string>

/// This is the 12th model in a series of tutorial examples. Here we show:
///   - How to implement a GUSS approach using the GAMS API
class Transport12
{
public:
    static void becomes_main(int argc, char* argv[]);
    static void GUSSCall(gams::GAMSSet dict, gams::GAMSModelInstance mi, std::string solveStatement
                        , gams::GAMSOptions* opt = 0
                        , gams::GAMSModelInstanceOpt miOpt = gams::GAMSModelInstanceOpt()
                        , std::ostream* output = 0);
    static std::string getModelText();
    static std::string toLower(const std::string &str);
};

#endif // TRANSPORT12_H
