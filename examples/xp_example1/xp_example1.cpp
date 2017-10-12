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

 /*
   Use this command to compile the example:
   cl xp_example1.cpp ../C/api/gdxcc.c -I../C/api
 */

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "gdxcc.h"

using namespace std;

static gdxStrIndexPtrs_t Indx;
static gdxStrIndex_t     IndxXXX;
static gdxValues_t       Values;

void ReportGDXError(gdxHandle_t PGX) {
	char S[GMS_SSSIZE];

	std::cout << "**** Fatal GDX Error" << endl;
	gdxErrorStr(PGX, gdxGetLastError(PGX), S);
	std::cout << "**** " << S << endl;
	exit(1);
}

void ReportIOError(int N, const std::string &msg) {
	std::cout << "**** Fatal I/O Error = " << N << " when calling " << msg << endl;
	exit(1);
}

void WriteData(gdxHandle_t PGX, const char *s, const double V) {
	strcpy(Indx[0], s);
	Values[GMS_VAL_LEVEL] = V;
	gdxDataWriteStr(PGX, (const char **)Indx, Values);
}

int main(int argc, char *argv[]) {
	gdxHandle_t PGX = NULL;
	char        Msg[GMS_SSSIZE], Producer[GMS_SSSIZE], Sysdir[GMS_SSSIZE], VarName[GMS_SSSIZE];
	int         ErrNr;
	int         VarNr;
	int         NrRecs;
	int         N;
	int         Dim;
	int         VarTyp;
	int         D;

	if (argc < 2 || argc > 3) {
		cout << "**** xp_Example1: incorrect number of parameters" << endl;
		exit(1);
	}

	strcpy(Sysdir, argv[1]);
	cout << "xp_Example1 using GAMS system directory: " << Sysdir << endl;

	if (!gdxCreateD(&PGX, Sysdir, Msg, sizeof(Msg))) {
		cout << "**** Could not load GDX library" << endl << "**** " << Msg << endl;
		exit(1);
	}

	gdxGetDLLVersion(PGX, Msg);
	cout << "Using GDX DLL version: " << Msg << endl;

	GDXSTRINDEXPTRS_INIT(IndxXXX, Indx);

	if (2 == argc) {
		/* Write demand data */
		gdxOpenWrite(PGX, "demanddata.gdx", "xp_example1", &ErrNr);
		if (ErrNr) ReportIOError(ErrNr, "gdxOpenWrite");
		if (!gdxDataWriteStrStart(PGX, "Demand", "Demand data", 1, GMS_DT_PAR, 0))
			ReportGDXError(PGX);
		WriteData(PGX, "New-York", 324.0);
		WriteData(PGX, "Chicago", 299.0);
		WriteData(PGX, "Topeka", 274.0);
		if (!gdxDataWriteDone(PGX)) ReportGDXError(PGX);
		cout << "Demand data written by example1" << endl;
	}
	else {
		gdxOpenRead(PGX, argv[2], &ErrNr);
		if (ErrNr) ReportIOError(ErrNr, "gdxOpenRead");
		gdxFileVersion(PGX, Msg, Producer);
		cout << "GDX file written using version: " << Msg << endl;
		cout << "GDX file written by: " << Producer << endl;

		if (!gdxFindSymbol(PGX, "x", &VarNr)) {
			cout << "**** Could not find variable X" << endl;
			exit(1);
		}

		gdxSymbolInfo(PGX, VarNr, VarName, &Dim, &VarTyp);
		if (Dim != 2 || GMS_DT_VAR != VarTyp) {
			cout << "**** X is not a two dimensional variable: "
				<< Dim << ":" << VarTyp << endl;
			exit(1);
		}

		if (!gdxDataReadStrStart(PGX, VarNr, &NrRecs)) ReportGDXError(PGX);
		cout << "Variable X has " << NrRecs << " records" << endl;
		while (gdxDataReadStr(PGX, Indx, Values, &N)) {
			if (0 == Values[GMS_VAL_LEVEL]) continue; /* skip level 0.0 is default */
			for (D = 0; D < Dim; D++) cout << (D ? '.' : ' ') << Indx[D];
			cout << " = " << Values[GMS_VAL_LEVEL] << endl;
		}
		cout << "All solution values shown" << endl;
		gdxDataReadDone(PGX);
	}

	if (ErrNr = gdxClose(PGX)) ReportIOError(ErrNr, "gdxClose");

	return 0;
} /* main */
