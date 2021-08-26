/**
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2021 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2021 GAMS Development Corp. <support@gams.com>
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
   Use this command to compile the example (Windows):
   cl xp_dataWrite.cpp ../C/api/gdxcc.c ../C/api/gmdcc.c -I../C/api
   Use this command to compile the example (Unix):
   g++ xp_dataWrite.cpp ../C/api/gdxcc.c ../C/api/gmdcc.c -I../C/api -ldl
 */

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include "gdxcc.h"
#include "gmdcc.h"
#include "gdxcc.h"
#include "gmdcc.h"

#if defined(_WIN32)
#define snprintf _snprintf_s
#define strcpy strcpy_s
#endif


#define toStr( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;

enum writeMode {
	ordered,
	oneOOO,
	reversed
};

class worker {
	gdxHandle_t gdxHandle;
	gmdHandle_t gmdHandle;
	static const int size = 200;
	char iarrayC[size][GMS_SSSIZE]; // GMS_UEL_IDENT_SIZE?
	char jarrayC[size][GMS_SSSIZE];
	char karrayC[size][GMS_SSSIZE];
	clock_t tstart;
	int IndxI[GMS_MAX_INDEX_DIM];
	gdxStrIndex_t IndxC;
	gdxStrIndexPtrs_t IndxCPtrs;

	gdxValues_t Values;
	void ReportIOError(int N, const string &msg);
	void ReportGDXError();
	void ReportGMDError();
	void PrintDiff(const string &msg, clock_t tm);
public:
	void initC(string sysDir);
	void useStr(const string &fileName, const string &txt, writeMode mode);
	void useRaw(const string &fileName, const string &txt);
	void useMap(const string &fileName, const string &txt, writeMode mode);
	void useGmd(const string &fileName, const string &txt);
};

// private functions
void worker::ReportIOError(int N, const string &msg) {
	cout << "**** Fatal I/O Error = " << N << " when calling " << msg << endl;
	exit(1);
}

void worker::ReportGDXError() {
	char s[GMS_SSSIZE];

	cout << "**** Fatal GDX Error" << endl;
	gdxErrorStr(gdxHandle, gdxGetLastError(gdxHandle), s);
	cout << "**** " << s << endl;

	exit(1);
}

void worker::ReportGMDError() {
	char s[GMS_SSSIZE];

	cout << "**** Fatal GMD Error" << endl;
	gmdGetLastError(gmdHandle, s);
	cout << "**** " << s << endl;

	exit(1);
}

void worker::PrintDiff(const string &msg, clock_t tm) {
	float diff;
	diff = ((float)tm - (float)tstart) / CLOCKS_PER_SEC;
	cout << msg << diff << endl;
}

void worker::initC(string sysDir) {
	char msg[GMS_SSSIZE];

	if (!gdxCreateD(&gdxHandle, sysDir.c_str(), msg, sizeof(msg)))
	{
		cout << "**** Could not load GDX (c) library" << endl << "**** " << msg << endl;
		exit(1);
	}

	gdxGetDLLVersion(gdxHandle, msg);
	cout << "Using GDX DLL version: " << msg << endl;

	if (!gmdCreateD(&gmdHandle, sysDir.c_str(), msg, sizeof(msg)))
	{
		cout << "**** Could not load GMD (c) library" << endl << "**** " << msg << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++) {
		snprintf(iarrayC[i], GMS_UEL_IDENT_SIZE, "i%d", i);
		snprintf(jarrayC[i], GMS_UEL_IDENT_SIZE, "j%d", i);
		snprintf(karrayC[i], GMS_UEL_IDENT_SIZE, "k%d", i);
	}

	GDXSTRINDEXPTRS_INIT(IndxC, IndxCPtrs);
	Values[GMS_VAL_LEVEL] = 11;
}

void worker::useStr(const string &fileName, const string &txt, writeMode mode) {
	int ErrNr, dummy;

	tstart = clock();

	gdxOpenWrite(gdxHandle, fileName.c_str(), "example", &ErrNr);

	if (ErrNr) ReportIOError(ErrNr, "gdxOpenWrite");


	if (!gdxUELRegisterStrStart(gdxHandle)) ReportGDXError();
	for (int i = 0; i < size; i++)
		if (!gdxUELRegisterStr(gdxHandle, iarrayC[i], &dummy)) ReportGDXError();
	for (int j = 0; j < size; j++)
		if (!gdxUELRegisterStr(gdxHandle, jarrayC[j], &dummy)) ReportGDXError();
	for (int k = 0; k < size; k++)
		if (!gdxUELRegisterStr(gdxHandle, karrayC[k], &dummy)) ReportGDXError();
	if (!gdxUELRegisterDone(gdxHandle)) ReportGDXError();

	if (!gdxDataWriteStrStart(gdxHandle, "Demand", "Demand data", 3, GMS_DT_PAR, 0)) ReportGDXError();

	switch (mode) {
	case ordered:
		for (int i = 0; i < size; i++) {
			strcpy(IndxC[0], iarrayC[i]); //this would be quicker, but "unfair" to compare it with the c++ approach: IndxCPtrs[0] = iarrayC[i];
			for (int j = 0; j < size; j++) {
				strcpy(IndxC[1], jarrayC[j]);
				for (int k = 0; k < size; k++) {
					strcpy(IndxC[2], karrayC[k]);
					if (!gdxDataWriteStr(gdxHandle, (const char **)IndxCPtrs, Values)) ReportGDXError();
				}
			}
		}
		break;
	case oneOOO:
		for (int i = 0; i < size; i++) {
			strcpy(IndxC[0], iarrayC[i]);
			for (int j = 0; j < size; j++) {
				strcpy(IndxC[1], jarrayC[j]);
				for (int k = 0; k < size; k++) {
					strcpy(IndxC[2], karrayC[k]);
					if (i + j + k > 0)
						if (!gdxDataWriteStr(gdxHandle, (const char **)IndxCPtrs, Values)) ReportGDXError();
				}
			}
		}
		strcpy(IndxC[0], iarrayC[0]);
		strcpy(IndxC[1], jarrayC[0]);
		strcpy(IndxC[2], karrayC[0]);
		if (!gdxDataWriteStr(gdxHandle, (const char **)IndxCPtrs, Values)) ReportGDXError();
		break;
	case reversed:
		for (int i = size - 1; i > -1; i--) {
			strcpy(IndxC[0], iarrayC[i]);
			for (int j = size - 1; j > -1; j--) {
				strcpy(IndxC[1], jarrayC[j]);
				for (int k = size - 1; k > -1; k--) {
					strcpy(IndxC[2], karrayC[k]);
					if (!gdxDataWriteStr(gdxHandle, (const char **)IndxCPtrs, Values)) ReportGDXError();
				}
			}
		}
		break;
	}

	PrintDiff("Before Done (" + txt + "): ", clock());

	if (!gdxDataWriteDone(gdxHandle)) ReportGDXError();
	if (ErrNr = gdxClose(gdxHandle)) ReportIOError(ErrNr, "gdxClose");

	PrintDiff("Running time (" + txt + "): ", clock());
}

void worker::useRaw(const string &fileName, const string &txt) {
	int ErrNr;

	tstart = clock();

	gdxOpenWrite(gdxHandle, fileName.c_str(), "example", &ErrNr);
	if (ErrNr) ReportIOError(ErrNr, "gdxOpenWrite");

	if (!gdxUELRegisterRawStart(gdxHandle)) ReportGDXError();
	for (int i = 0; i < size; i++)
		if (!gdxUELRegisterRaw(gdxHandle, iarrayC[i])) ReportGDXError();
	for (int j = 0; j < size; j++)
		if (!gdxUELRegisterRaw(gdxHandle, jarrayC[j])) ReportGDXError();
	for (int k = 0; k < size; k++)
		if (!gdxUELRegisterRaw(gdxHandle, karrayC[k])) ReportGDXError();
	if (!gdxUELRegisterDone(gdxHandle)) ReportGDXError();

	if (!gdxDataWriteRawStart(gdxHandle, "Demand", "Demand data", 3, GMS_DT_PAR, 0)) ReportGDXError();

	for (int i = 0; i < size; i++) {
		IndxI[0] = i + 1;
		for (int j = 0; j < size; j++) {
			IndxI[1] = size + j + 1;
			for (int k = 0; k < size; k++) {
				IndxI[2] = 2 * size + k + 1;
				if (!gdxDataWriteRaw(gdxHandle, IndxI, Values)) ReportGDXError();
			}
		}
	}

	PrintDiff("Before Done (" + txt + "): ", clock());

	if (!gdxDataWriteDone(gdxHandle)) ReportGDXError();
	if (ErrNr = gdxClose(gdxHandle)) ReportIOError(ErrNr, "gdxClose");

	PrintDiff("Running time (" + txt + "): ", clock());
}

void worker::useMap(const string &fileName, const string &txt, writeMode mode) {
	int ErrNr;

	tstart = clock();

	gdxOpenWrite(gdxHandle, fileName.c_str(), "example", &ErrNr);
	if (ErrNr) ReportIOError(ErrNr, "gdxOpenWrite");

	if (!gdxUELRegisterMapStart(gdxHandle)) ReportGDXError();
	for (int i = 0; i < size; i++)
		if (!gdxUELRegisterMap(gdxHandle, i, iarrayC[i])) ReportGDXError();
	for (int j = 0; j < size; j++)
		if (!gdxUELRegisterMap(gdxHandle, size + j, jarrayC[j])) ReportGDXError();
	for (int k = 0; k < size; k++)
		if (!gdxUELRegisterMap(gdxHandle, 2 * size + k, karrayC[k])) ReportGDXError();
	if (!gdxUELRegisterDone(gdxHandle)) ReportGDXError();

	if (!gdxDataWriteMapStart(gdxHandle, "Demand", "Demand data", 3, GMS_DT_PAR, 0)) ReportGDXError();

	switch (mode) {
	case ordered:
		for (int i = 0; i < size; i++) {
			IndxI[0] = i;
			for (int j = 0; j < size; j++) {
				IndxI[1] = size + j;
				for (int k = 0; k < size; k++) {
					IndxI[2] = 2 * size + k;
					if (!gdxDataWriteMap(gdxHandle, IndxI, Values)) ReportGDXError();
				}
			}

		}
		break;
	case oneOOO:
		for (int i = 0; i < size; i++) {
			IndxI[0] = i;
			for (int j = 0; j < size; j++) {
				IndxI[1] = size + j;
				for (int k = 0; k < size; k++) {
					IndxI[2] = 2 * size + k;
					if (i + j + k > 0)
						if (!gdxDataWriteMap(gdxHandle, IndxI, Values)) ReportGDXError();
				}
			}
		}

		IndxI[0] = 0;
		IndxI[1] = 0;
		IndxI[2] = 0;
		if (!gdxDataWriteMap(gdxHandle, IndxI, Values)) ReportGDXError();
		break;
	case reversed:
		for (int i = size - 1; i > -1; i--) {
			IndxI[0] = i;
			for (int j = size - 1; j > -1; j--) {
				IndxI[1] = size + j;
				for (int k = size - 1; k > -1; k--) {
					IndxI[2] = 2 * size + k;
					if (!gdxDataWriteMap(gdxHandle, IndxI, Values)) ReportGDXError();
				}
			}
		}
		break;
	}

	PrintDiff("Before Done (" + txt + "): ", clock());

	if (!gdxDataWriteDone(gdxHandle)) ReportGDXError();
	if (ErrNr = gdxClose(gdxHandle)) ReportIOError(ErrNr, "gdxClose");

	PrintDiff("Running time (" + txt + "): ", clock());
}

void worker::useGmd(const string &fileName, const string &txt) {
	void *symPtr, *symRecPtr;

	tstart = clock();

	if (!gmdAddSymbol(gmdHandle, "Demand", 3, GMS_DT_PAR, 0, "", &symPtr)) ReportGMDError();

	for (int i = 0; i < size; i++) {
		strcpy(IndxC[0], iarrayC[i]);
		for (int j = 0; j < size; j++) {
			strcpy(IndxC[1], jarrayC[j]);
			for (int k = 0; k < size; k++) {
				strcpy(IndxC[2], karrayC[k]);
				if (!gmdAddRecord(gmdHandle, symPtr, (const char **)IndxCPtrs, &symRecPtr)) ReportGMDError();
				if (!gmdSetLevel(gmdHandle, symRecPtr, 11.0)) ReportGMDError();
			}
		}
	}

	PrintDiff("Before Done (" + txt + "): ", clock());

	if (!gmdWriteGDX(gmdHandle, fileName.c_str(), true)) ReportGMDError();

	PrintDiff("Running time (" + txt + "): ", clock());
}

int main(int argc, char *argv[]) {
	string Sysdir;

	if (argc != 2) {
		cout << "**** DataWrite: incorrect number of parameters" << endl;
		exit(1);
	}

	Sysdir = argv[1];
	//Sysdir = "C:\\GAMS\\win32\\24.7";
	cout << "DataWrite using GAMS system directory: " << Sysdir << endl;

	worker w;

	cout << "Start using C API (aka use *char all the time)" << endl;
	w.initC(Sysdir);

	w.useStr("stringOrderedC.gdx", "strOrdered", ordered);
	w.useStr("string1oooC.gdx", "str1ooo", oneOOO);
	w.useStr("stringRevOrdC.gdx", "strRevOrd", reversed);

	w.useRaw("raw.gdx", "raw");

	w.useMap("mapOrdered.gdx", "mapOrdered", ordered);
	w.useMap("map1ooo.gdx", "map1ooo", oneOOO);
	w.useMap("mapRevOrd.gdx", "mapRevOrd", reversed);

	w.useGmd("gmd.gdx", "gmd");

	cout << "All data written" << endl;
	return 0;
} /* main */
