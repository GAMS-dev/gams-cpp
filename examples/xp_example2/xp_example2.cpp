/**
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>
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

 /*
   Use this command to compile the example:
   cl xp_example2.cpp ../C/api/gdxcc.c ../C/api/optcc.c -I../C/api
 */

 /*
  This program performs the following steps:
     1. Generate a gdx file with demand data
     2. Calls GAMS to solve a simple transportation model
        (The GAMS model writes the solution to a gdx file)
     3. The solution is read from the gdx file
 */

#include "gclgms.h"
#include "gdxcc.h"
#include "optcc.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <cassert>
#include <iostream>

using namespace std;

/* Handles to the GDX and Option objects */
gdxHandle_t   gdx = NULL;
optHandle_t   opt = NULL;

#define gdxerror(i, s) { gdxErrorStr(gdx, i, msg); \
                         cout << s << "% failed: " << msg << endl; return 1; }

int WriteModelData(const char * fngdxfile)
{
    int                status;
    char               msg[GMS_SSSIZE];
    gdxStrIndex_t      strIndex;
    gdxStrIndexPtrs_t  sp;
    gdxValues_t        v;

    gdxOpenWrite(gdx, fngdxfile, "xp_Example2", &status);
    if (status)
        gdxerror(status, "gdxOpenWrite");

    if (0 == gdxDataWriteStrStart(gdx, "Demand", "Demand Data", 1, GMS_DT_PAR, 0))
        gdxerror(gdxGetLastError(gdx), "gdxDataWriteStrStart");

    /* Initalize some GDX data structure */
    GDXSTRINDEXPTRS_INIT(strIndex, sp);

    strcpy(sp[0], "New-York"); v[GMS_VAL_LEVEL] = 324.0; gdxDataWriteStr(gdx, (const char **)sp, v);
    strcpy(sp[0], "Chicago"); v[GMS_VAL_LEVEL] = 299.0; gdxDataWriteStr(gdx, (const char **)sp, v);
    strcpy(sp[0], "Topeka"); v[GMS_VAL_LEVEL] = 274.0; gdxDataWriteStr(gdx, (const char **)sp, v);

    if (0 == gdxDataWriteDone(gdx))
        gdxerror(gdxGetLastError(gdx), "gdxDataWriteDone");

    if (gdxClose(gdx))
        gdxerror(gdxGetLastError(gdx), "gdxClose");

    return 0;
}

int CallGams(const char *sysdir, const char *model)
{
    char msg[GMS_SSSIZE], deffile[GMS_SSSIZE], cmdLine[2*GMS_SSSIZE];

    strncpy(deffile, sysdir, sizeof(deffile));
    strncat(deffile, "/optgams.def", sizeof(deffile));

    if (optReadDefinition(opt, deffile)) {
        int i, itype;
        for (i = 1; i <= optMessageCount(opt); i++) {
            optGetMessage(opt, i, msg, &itype);
            cout << msg << endl;
        }
        return 1;
    }

    optSetStrStr(opt, "input", model);
    optSetIntStr(opt, "logoption", 2);
    optWriteParameterFile(opt,"gams.opt");

    sprintf(cmdLine, "\"%s/gams\" dummy pf=gams.opt", sysdir);
    return system(cmdLine);
}

int ReadSolutionData(const char *fngdxfile)
{
    int                status, VarNr, NrRecs, FDim, dim, vartype;
    char               msg[GMS_SSSIZE], VarName[GMS_SSSIZE];
    gdxStrIndex_t      strIndex;
    gdxStrIndexPtrs_t  sp;
    gdxValues_t        v;

    gdxOpenRead(gdx, fngdxfile, &status);
    if (status)
        gdxerror(status, "gdxOpenRead");

    strcpy(VarName, "result");
    if (0 == gdxFindSymbol(gdx, VarName, &VarNr)) {
        cout << "Could not find variable >" << VarName << "<" << endl;
        return 1;
    }

    gdxSymbolInfo(gdx, VarNr, VarName, &dim, &vartype);
    if (2 != dim || GMS_DT_VAR != vartype) {
        cout << VarName << " is not a two dimensional variable" << endl;
        return 1;
    }

    if (0 == gdxDataReadStrStart(gdx, VarNr, &NrRecs))
        gdxerror(gdxGetLastError(gdx), "gdxDataReadStrStart");

    /* Initalize some GDX data structure */
    GDXSTRINDEXPTRS_INIT(strIndex, sp);

    while (gdxDataReadStr(gdx, sp, v, &FDim)) {
        int i;

        if (0.0 == v[GMS_VAL_LEVEL]) continue; /* skip level = 0.0 is default */
        for (i = 0; i < dim; i++)
            cout << sp[i] << ((i < dim - 1) ? "." : "");
        cout << " = " << v[GMS_VAL_LEVEL] << endl;
    }
    cout << "All solution values shown" << endl;

    gdxDataReadDone(gdx);

    if ((status = gdxGetLastError(gdx)))
        gdxerror(status, "GDX");

    if (gdxClose(gdx))
        gdxerror(gdxGetLastError(gdx), "gdxClose");

    return 0;
}

int main(int argc, char *argv[])
{

    int status;
    char       sysdir[GMS_SSSIZE], model[GMS_SSSIZE], msg[GMS_SSSIZE];
    const char defModel[] = "../GAMS/model2.gms";

    if (argc < 2 || argc > 3) {
        cout << "xp_Example2: Incorrect number of parameters, first arg is sysDir, second arg is model to execute (optional)" << endl;
        return 1;
    }

    if (argc > 2)
        strncpy(model, argv[2], sizeof(model));
    else
        strncpy(model, defModel, sizeof(model));

    strncpy(sysdir, argv[1], sizeof(sysdir));

    cout << "Loading objects from GAMS system directory: " << sysdir << endl;

    /* Create objects */
    if (!gdxCreateD(&gdx, sysdir, msg, sizeof(msg))) {
        cout << "Could not create gdx object: " << msg << endl;
        return 1;
    }

    if (!optCreateD(&opt, sysdir, msg, sizeof(msg))) {
        cout << "Could not create opt object: " << msg << endl;
        return 1;
    }

    if ((status = WriteModelData("demanddata.gdx"))) {
        cout << "Model data not written" << endl;
        goto TERMINATE;
    }

    if ((status = CallGams(sysdir, model))) {
        cout << "Call to GAMS failed" << endl;
        goto TERMINATE;
    }

    if ((status = ReadSolutionData("results.gdx"))) {
        cout << "Could not read solution back" << endl;
        goto TERMINATE;
    }

TERMINATE:
    return status;
}
