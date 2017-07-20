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
  cl xp_example1.cpp api/gdxco.cpp ../C/api/gdxcc.c -Iapi -I../C/api
*/

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "gdxco.hpp"

using namespace std;
using namespace GAMS;

static std::string Indx[GMS_MAX_INDEX_DIM];
static gdxValues_t Values;

void ReportGDXError(GDX &PGX) {
  std::string S;

  cout << "**** Fatal GDX Error" << endl;
  PGX.ErrorStr(PGX.GetLastError(), S);
  cout << "**** " << S << endl;
  exit(1);
}

void ReportIOError(int N, const std::string &msg) {
  cout << "**** Fatal I/O Error = " << N << " when calling " << msg << endl;
  exit(1);
}

void WriteData(GDX &PGX, const std::string &s, const double V) {
  Indx[0] = s;
  Values[GMS_VAL_LEVEL] = V;
  PGX.DataWriteStr(Indx,Values);
}

int main (int argc, char *argv[]) {

  std::string Msg, FileName, Producer, Sysdir, VarName;
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

  Sysdir = argv[1];
  cout << "xp_Example1 using GAMS system directory: " << Sysdir << endl;

  GDX PGX(Sysdir, Msg); 

  if (Msg != "") {
    cout << "**** Could not load GDX library" << endl << "**** " << Msg << endl;
    exit(1);
  }

  PGX.GetDLLVersion(Msg);
  cout << "Using GDX DLL version: " << Msg << endl;

  if (2 == argc) {
    /* Write demand data */
    PGX.OpenWrite("demanddata.gdx", "xp_example1", ErrNr);
    if (ErrNr) ReportIOError(ErrNr,"gdxOpenWrite");
    if (!PGX.DataWriteStrStart("Demand","Demand data",1,GMS_DT_PAR ,0))
      ReportGDXError(PGX);
    WriteData(PGX,"New-York",324.0);
    WriteData(PGX,"Chicago" ,299.0);
    WriteData(PGX,"Topeka"  ,274.0);
    if (!PGX.DataWriteDone()) ReportGDXError(PGX);
    cout << "Demand data written by example1" << endl;
  } else {
    FileName = argv[2];
    PGX.OpenRead(FileName, ErrNr);
    if (ErrNr) ReportIOError(ErrNr,"gdxOpenRead");
    PGX.FileVersion(Msg,Producer);
    cout << "GDX file written using version: " << Msg << endl;
    cout << "GDX file written by: " <<  Producer << endl;

    if (!PGX.FindSymbol("x",VarNr)) {
      cout << "**** Could not find variable X" << endl;
      exit(1);
    }

    PGX.SymbolInfo(VarNr,VarName,Dim,VarTyp);
    if (Dim != 2 || GMS_DT_VAR != VarTyp) {
      cout << "**** X is not a two dimensional variable: " 
           << Dim << ":" << VarTyp << endl;
      exit(1);
    }

    if (!PGX.DataReadStrStart(VarNr,NrRecs)) ReportGDXError(PGX);
    cout << "Variable X has " << NrRecs << " records" << endl;
    while (PGX.DataReadStr(Indx,Values,N)) {
      if (0 == Values[GMS_VAL_LEVEL]) continue; /* skip level 0.0 is default */
      for (D=0; D<Dim; D++) cout << (D? '.':' ') <<  Indx[D];
      cout << " = " << Values[GMS_VAL_LEVEL] << endl;
    }
    cout << "All solution values shown" << endl;
    PGX.DataReadDone();
  }

  if (ErrNr = PGX.Close()) ReportIOError(ErrNr,"gdxClose");

  return 0;
} /* main */
