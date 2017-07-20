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
  Use this command to compile the example (Windows):
  cl xp_dataWrite.cpp api/gdxco.cpp api/gmdco.cpp ../C/api/gdxcc.c ../C/api/gmdcc.c -Iapi -I../C/api
  Use this command to compile the example (Unix):
  g++ xp_dataWrite.cpp api/gdxco.cpp api/gmdco.cpp ../C/api/gdxcc.c ../C/api/gmdcc.c -Iapi -I../C/api -ldl
*/

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include "gdxco.hpp"
#include "gmdco.hpp"
#include "gdxcc.h"
#include "gmdcc.h"

#if defined(_WIN32)
#define snprintf _snprintf_s
#define strcpy strcpy_s
#endif


#define toStr( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;
using namespace GAMS;

enum writeMode { 
	ordered, 
	oneOOO, 
	reversed 
};

enum apiMode {
	C,
	CPP
};

class worker {
	GDX PGX;
	GMD PGD;
	gdxHandle_t gdxHandle;
	gmdHandle_t gmdHandle;
	apiMode api;
	static const int size = 200;
	string iarray[size];
	string jarray[size];
	string karray[size];
	char iarrayC[size][GMS_SSSIZE]; // GMS_UEL_IDENT_SIZE?
	char jarrayC[size][GMS_SSSIZE];
	char karrayC[size][GMS_SSSIZE];
	clock_t tstart;
    string Indx[GMS_MAX_INDEX_DIM];
    int IndxI[GMS_MAX_INDEX_DIM];
    gdxStrIndex_t IndxC;
    gdxStrIndexPtrs_t IndxCPtrs;

    gdxValues_t Values;
	void ReportIOError(int N, const string &msg);
	void ReportGDXError();
	void ReportGMDError();
	void PrintDiff(const string &msg, clock_t tm);
public:
	void initCPP(string sysDir);
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
	std::string S;
	char s[GMS_SSSIZE];

	cout << "**** Fatal GDX Error" << endl;
	switch(api) {
  		case CPP:
			PGX.ErrorStr(PGX.GetLastError(), S);
			cout << "**** " << S << endl;
			break;
		case C:
			gdxErrorStr(gdxHandle, gdxGetLastError(gdxHandle), s);
			cout << "**** " << s << endl;
			break;
	}

	exit(1);
}

void worker::ReportGMDError() {
	std::string S;
	char s[GMS_SSSIZE];
	
	cout << "**** Fatal GMD Error" << endl;
	switch(api) {
  		case CPP:
			PGD.GetLastError(S);
			cout << "**** " << S << endl;
			break;
		case C:
			gmdGetLastError(gmdHandle, s);
			cout << "**** " << s << endl;
			break;
	}
	
	exit(1);
}

void worker::PrintDiff(const string &msg, clock_t tm) {
	float diff;
	diff = ((float)tm-(float)tstart)/CLOCKS_PER_SEC;
    cout << msg << diff << endl;
}

// public functions
void worker::initCPP(string sysDir) {
	string Msg;

	api = CPP;

	PGX.Init(sysDir, Msg);
	if (Msg != "") {
		cout << "**** Could not load GDX (cpp) library" << endl << "**** " << Msg << endl;
		exit(1);
	}
	
	PGX.GetDLLVersion(Msg);
	cout << "Using GDX DLL version: " << Msg << endl;

	PGD.Init(sysDir, Msg, true); 
    if (Msg != "") {
        cout << "**** Could not load GMD (cpp) library" << endl << "**** " << Msg << endl;
        exit(1);
     }
	
	for(int i=0; i<size; i++) {
		iarray[i] = "i" + toStr(i);
		jarray[i] = "j" + toStr(i);
		karray[i] = "k" + toStr(i);
	}

	Values[GMS_VAL_LEVEL] = 11;
}

void worker::initC(string sysDir) {
	char msg[GMS_SSSIZE];

	api = C;

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
	
	for(int i=0; i<size; i++) {
		snprintf(iarrayC[i], GMS_UEL_IDENT_SIZE, "i%d", i);
		snprintf(jarrayC[i], GMS_UEL_IDENT_SIZE, "j%d", i);
		snprintf(karrayC[i], GMS_UEL_IDENT_SIZE, "k%d", i);
	}

    GDXSTRINDEXPTRS_INIT(IndxC,IndxCPtrs);
	Values[GMS_VAL_LEVEL] = 11;
}

void worker::useStr(const string &fileName, const string &txt, writeMode mode) {
	int ErrNr, dummy;

	tstart = clock();

	switch(api) {
		case CPP:
			PGX.OpenWrite(fileName, "example", ErrNr);
			break;
		case C:
			gdxOpenWrite(gdxHandle, fileName.c_str(), "example", &ErrNr);
			break;
	}
    if (ErrNr) ReportIOError(ErrNr,"gdxOpenWrite");

	switch(api) {
		case CPP:
			if (!PGX.UELRegisterStrStart()) ReportGDXError();
			for(int i=0; i<size; i++)
				if (!PGX.UELRegisterStr(iarray[i], dummy)) ReportGDXError();
			for(int j=0; j<size; j++)
				if (!PGX.UELRegisterStr(jarray[j], dummy)) ReportGDXError();
			for(int k=0; k<size; k++)
				if (!PGX.UELRegisterStr(karray[k], dummy)) ReportGDXError();
			if (!PGX.UELRegisterDone()) ReportGDXError();

			if (!PGX.DataWriteStrStart("Demand","Demand data",3,GMS_DT_PAR ,0)) ReportGDXError();

			switch(mode) {
				case ordered: 
					for(int i=0; i<size; i++) {
						Indx[0] = iarray[i];
						for(int j=0; j<size; j++) {
							Indx[1] = jarray[j];
							for(int k=0; k<size; k++) {
								Indx[2] = karray[k];
								if (!PGX.DataWriteStr(Indx,Values)) ReportGDXError();
							}
						}
					}
					break;
				case oneOOO: 
					for(int i=0; i<size; i++) {
						Indx[0] = iarray[i];
						for(int j=0; j<size; j++) {
							Indx[1] = jarray[j];
							for(int k=0; k<size; k++) {
								Indx[2] = karray[k];
								if(i+j+k>0)
									if (!PGX.DataWriteStr(Indx,Values)) ReportGDXError();
							}
						}
					}
					Indx[0] = iarray[0];
					Indx[1] = jarray[0];
					Indx[2] = karray[0];
					if (!PGX.DataWriteStr(Indx,Values)) ReportGDXError();
					break;
				case reversed: 
					for(int i=size-1; i>-1; i--) {
						Indx[0] = iarray[i];
						for(int j=size-1; j>-1; j--) {
							Indx[1] = jarray[j];
							for(int k=size-1; k>-1; k--) {
								Indx[2] = karray[k];
								if (!PGX.DataWriteStr(Indx,Values)) ReportGDXError();
							}
						}
					}
					break;
			}
			break;
		case C:
			if (!gdxUELRegisterStrStart(gdxHandle)) ReportGDXError();
			for(int i=0; i<size; i++)
				if (!gdxUELRegisterStr(gdxHandle, iarrayC[i], &dummy)) ReportGDXError();
			for(int j=0; j<size; j++)
				if (!gdxUELRegisterStr(gdxHandle, jarrayC[j], &dummy)) ReportGDXError();
			for(int k=0; k<size; k++)
				if (!gdxUELRegisterStr(gdxHandle, karrayC[k], &dummy)) ReportGDXError();
			if (!gdxUELRegisterDone(gdxHandle)) ReportGDXError();

			if (!gdxDataWriteStrStart(gdxHandle, "Demand","Demand data",3,GMS_DT_PAR ,0)) ReportGDXError();

			switch(mode) {
				case ordered: 
					for(int i=0; i<size; i++) {
						strcpy(IndxC[0],iarrayC[i]); //this would be quicker, but "unfair" to compare it with the c++ approach: IndxCPtrs[0] = iarrayC[i];
						for(int j=0; j<size; j++) {
							strcpy(IndxC[1],jarrayC[j]);
							for(int k=0; k<size; k++) {
								strcpy(IndxC[2],karrayC[k]);
								if (!gdxDataWriteStr(gdxHandle, (const char **)IndxCPtrs,Values)) ReportGDXError();
							}
						}
					}
					break;
				case oneOOO: 
					for(int i=0; i<size; i++) {
						strcpy(IndxC[0],iarrayC[i]);
						for(int j=0; j<size; j++) {
							strcpy(IndxC[1],jarrayC[j]);
							for(int k=0; k<size; k++) {
								strcpy(IndxC[2],karrayC[k]);
								if(i+j+k>0)
									if (!gdxDataWriteStr(gdxHandle, (const char **)IndxCPtrs,Values)) ReportGDXError();
							}
						}
					}
					strcpy(IndxC[0],iarrayC[0]);
					strcpy(IndxC[1],jarrayC[0]);
					strcpy(IndxC[2],karrayC[0]);
					if (!gdxDataWriteStr(gdxHandle, (const char **)IndxCPtrs,Values)) ReportGDXError();
					break;
				case reversed: 
					for(int i=size-1; i>-1; i--) {
						strcpy(IndxC[0],iarrayC[i]);
						for(int j=size-1; j>-1; j--) {
							strcpy(IndxC[1],jarrayC[j]);
							for(int k=size-1; k>-1; k--) {
								strcpy(IndxC[2],karrayC[k]);
								if (!gdxDataWriteStr(gdxHandle, (const char **)IndxCPtrs,Values)) ReportGDXError();
							}
						}
					}
					break;
			}
			break;
	}

	PrintDiff("Before Done ("+ txt +"): ", clock());

	switch(api) {
		case CPP:
			if (!PGX.DataWriteDone()) ReportGDXError();
			if (ErrNr = PGX.Close()) ReportIOError(ErrNr,"gdxClose");
			break;
		case C:
			if (!gdxDataWriteDone(gdxHandle)) ReportGDXError();
			if (ErrNr = gdxClose(gdxHandle)) ReportIOError(ErrNr,"gdxClose");
			break;
	}

	PrintDiff("Running time ("+ txt +"): ", clock());
}

void worker::useRaw(const string &fileName, const string &txt) {
	int ErrNr;

	tstart = clock();

	PGX.OpenWrite(fileName, "example", ErrNr);
    if (ErrNr) ReportIOError(ErrNr,"gdxOpenWrite");

	if (!PGX.UELRegisterRawStart()) ReportGDXError();
	for(int i=0; i<size; i++)
		if (!PGX.UELRegisterRaw(iarray[i])) ReportGDXError();
	for(int j=0; j<size; j++)
		if (!PGX.UELRegisterRaw(jarray[j])) ReportGDXError();
	for(int k=0; k<size; k++)
		if (!PGX.UELRegisterRaw(karray[k])) ReportGDXError();
    if (!PGX.UELRegisterDone()) ReportGDXError();

    if (!PGX.DataWriteRawStart("Demand","Demand data",3,GMS_DT_PAR ,0)) ReportGDXError();

	for(int i=0; i<size; i++) {
		IndxI[0] = i+1;
		for(int j=0; j<size; j++) {
		    IndxI[1] = size+j+1;
			for(int k=0; k<size; k++) {
				IndxI[2] = 2*size+k+1;
				if (!PGX.DataWriteRaw(IndxI,Values)) ReportGDXError();
			}
		}
	}

	PrintDiff("Before Done ("+ txt +"): ", clock());

	if (!PGX.DataWriteDone()) ReportGDXError();
	if (ErrNr = PGX.Close()) ReportIOError(ErrNr,"gdxClose");

	PrintDiff("Running time ("+ txt +"): ", clock());
}

void worker::useMap(const string &fileName, const string &txt, writeMode mode) {
	int ErrNr;

	tstart = clock();

	PGX.OpenWrite(fileName, "example", ErrNr);
    if (ErrNr) ReportIOError(ErrNr,"gdxOpenWrite");

	if (!PGX.UELRegisterMapStart()) ReportGDXError();
	for(int i=0; i<size; i++)
		if (!PGX.UELRegisterMap(i, iarray[i])) ReportGDXError();
	for(int j=0; j<size; j++)
		if (!PGX.UELRegisterMap(size+j, jarray[j])) ReportGDXError();
	for(int k=0; k<size; k++)
		if (!PGX.UELRegisterMap(2*size+k, karray[k])) ReportGDXError();
	if (!PGX.UELRegisterDone()) ReportGDXError();

    if (!PGX.DataWriteMapStart("Demand","Demand data",3,GMS_DT_PAR ,0)) ReportGDXError();

	switch(mode) {
	    case ordered:
			for(int i=0; i<size; i++) {
				IndxI[0] = i;
				for(int j=0; j<size; j++) {
					IndxI[1] = size+j;
					for(int k=0; k<size; k++) {
						IndxI[2] = 2*size+k;
						if (!PGX.DataWriteMap(IndxI,Values)) ReportGDXError();
					}
				}
			
			}
			break;
		case oneOOO: 
			for(int i=0; i<size; i++) {
				IndxI[0] = i;
				for(int j=0; j<size; j++) {
					IndxI[1] = size+j;
					for(int k=0; k<size; k++) {
						IndxI[2] = 2*size+k;
						if(i+j+k>0)
							if (!PGX.DataWriteMap(IndxI,Values)) ReportGDXError();
					}
				}
			}

			IndxI[0] = 0;
			IndxI[1] = 0;
			IndxI[2] = 0;
			if (!PGX.DataWriteMap(IndxI,Values)) ReportGDXError();
			break;
		case reversed: 
			for(int i=size-1; i>-1; i--) {
				IndxI[0] = i;
				for(int j=size-1; j>-1; j--) {
					IndxI[1] = size+j;
					for(int k=size-1; k>-1; k--) {
						IndxI[2] = 2*size+k;
						if (!PGX.DataWriteMap(IndxI,Values)) ReportGDXError();
					}
				}
			}
			break;
	}

	PrintDiff("Before Done ("+ txt +"): ", clock());

	if (!PGX.DataWriteDone()) ReportGDXError();
	if (ErrNr = PGX.Close()) ReportIOError(ErrNr,"gdxClose");

	PrintDiff("Running time ("+ txt +"): ", clock());
}

void worker::useGmd(const string &fileName, const string &txt) {
	void *symPtr, *symRecPtr;

	tstart = clock();

	switch(api) {
		case CPP:
			if(!PGD.AddSymbol("Demand", 3, GMS_DT_PAR, 0, "", &symPtr)) ReportGMDError();
			
			for(int i=0; i<size; i++) {
				Indx[0] = iarray[i];
				for(int j=0; j<size; j++) {
					Indx[1] = jarray[j];
					for(int k=0; k<size; k++) {
						Indx[2] = karray[k];
						if(!PGD.AddRecord(symPtr, Indx, &symRecPtr)) ReportGMDError();
						if(!PGD.SetLevel(symRecPtr, 11.0)) ReportGMDError();
					}
				}
			}
			break;
		case C:
			if(!gmdAddSymbol(gmdHandle, "Demand", 3, GMS_DT_PAR, 0, "", &symPtr)) ReportGMDError();
			
			for(int i=0; i<size; i++) {
				strcpy(IndxC[0],iarrayC[i]);
				for(int j=0; j<size; j++) {
					strcpy(IndxC[1],jarrayC[j]);
					for(int k=0; k<size; k++) {
						strcpy(IndxC[2],karrayC[k]);
						if(!gmdAddRecord(gmdHandle, symPtr, (const char **)IndxCPtrs, &symRecPtr)) ReportGMDError();
						if(!gmdSetLevel(gmdHandle, symRecPtr, 11.0)) ReportGMDError();
					}
				}
			}
			break;
	}

	PrintDiff("Before Done ("+ txt +"): ", clock());

	if (!PGD.WriteGDX(fileName, true)) ReportGMDError();

	PrintDiff("Running time ("+ txt +"): ", clock());
}

int main (int argc, char *argv[]) {
	string Sysdir;
	
	if (argc != 2) {
		cout << "**** DataWrite: incorrect number of parameters" << endl;
		exit(1);
	}
	
	Sysdir = argv[1];
	//Sysdir = "C:\\GAMS\\win32\\24.7";
	cout << "DataWrite using GAMS system directory: " << Sysdir << endl;
	
	worker w;
	
	cout << "Start using C++ API (aka deal with string to *char conversions)" << endl;
	w.initCPP(Sysdir);

	w.useStr("stringOrdered.gdx", "strOrdered", ordered);
	w.useStr("string1ooo.gdx",    "str1ooo",    oneOOO);
	w.useStr("stringRevOrd.gdx",  "strRevOrd",  reversed);

	w.useRaw("raw.gdx", "raw");

	w.useMap("mapOrdered.gdx", "mapOrdered", ordered);
	w.useMap("map1ooo.gdx",    "map1ooo",    oneOOO);
	w.useMap("mapRevOrd.gdx",  "mapRevOrd",  reversed);

	w.useGmd("gmd.gdx", "gmd");

	cout << "Start using C API (aka use *char all the time)" << endl;
	w.initC(Sysdir);

    w.useStr("stringOrderedC.gdx", "strOrdered", ordered);
	w.useStr("string1oooC.gdx",    "str1ooo",    oneOOO);
	w.useStr("stringRevOrdC.gdx",  "strRevOrd",  reversed);

	w.useGmd("gmd.gdx", "gmd");

	cout << "All data written" << endl;
	return 0;
} /* main */
