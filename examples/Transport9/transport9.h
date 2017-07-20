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

#ifndef TRANSPORT9_H
#define TRANSPORT9_H

#include "gams.h"
#include <queue>
#include <mutex>

//#include <atldbcli.h>
//#include <atldbsch.h>

/// TODO(AF) Decide whether we use MS Access or SQLite on Linux/Mac.
///
/// http://www.unixodbc.org/
/// http://www.easysoft.com/developer/interfaces/odbc/linux.html
class Transport9
{
public:
    static void becomes_main(int argc, char* argv[]);
//    static void ReadSet(CSession session, GAMSDatabase db, string strAccessSelect, string setName, int setDim, string setExp = "");
//    static void ReadParameter(CSession session, GAMSDatabase db, string strAccessSelect, string parName, int parDim, string parExp = "");
//    static GAMSDatabase ReadFromAccess(GAMSWorkspace ws);
//    static void WriteVariable(CSession session, GAMSDatabase db, string varName, vector<string> Domains);
//    static void WriteToAccess(GAMSWorkspace ws, GAMSDatabase db);

    static std::string getModelText();
};

#endif
