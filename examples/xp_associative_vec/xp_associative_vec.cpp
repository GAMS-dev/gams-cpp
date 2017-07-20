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
  cl xp_associative_vec.cpp api/gdxco.cpp ../C/api/gdxcc.c -Iapi -I../C/api
*/

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include "gdxco.hpp"

using namespace std;
using namespace GAMS;

int main(int argc, char* argv[]){
  string sysDir = "c:\\gams\\win32\\23.9";
  if (argc > 1) {
    sysDir = argv[1];
    if (sysDir.length() >= GMS_SSSIZE){
      cout << "*** Your system directory (argument 1) cannot exceed 255 characters" << endl;
      exit(1);
    }
  }
  int dim = 0;
  int errNr = 0;
  int varNr = 0;
  int varType = 0;
  int nrRecs = 0;
  int n = 0;
  string msg, varName;
  string idx[GMS_MAX_INDEX_DIM];
  gdxValues_t values;
  
  GDX PGX(sysDir, msg);
  if (msg != "") {
    cout << "**** Could not load GDX library" << endl;
    cout << "**** " + msg << endl;
    exit(1);
  }

  map<vector<string>, double> xlevel;

  PGX.OpenRead("..\\GAMS\\trnsport.gdx", errNr);
  if (errNr != 0) {
    PGX.ErrorStr(errNr, msg);
    cout << msg << endl;
    cout << "*** Error opening trnsport.gdx" << endl;
    exit(1);
  }
  if (PGX.FindSymbol("x", varNr) == 0) {
    cout << "*** Cannot find symbol 'x'" << endl;
    exit(1);
  }
  PGX.SymbolInfo(varNr, varName, dim, varType);
  if (dim != 2 || GMS_DT_VAR != varType) {
    cout << "**** x is not a two dimensional variable.  dim:" << dim << "  type:" << varType << endl;
    exit(1);
  }
  if (PGX.DataReadStrStart(varNr, nrRecs) == 0) {
    cout << "*** Cannot read symbol 'x'" << endl;
    exit(1);
  }

  vector<string> key(2);
  for (int i=0; i<nrRecs; i++){
    PGX.DataReadStr(idx, values, n);
    key[0] = idx[0];
	key[1] = idx[1];
    xlevel[key] = values[GMS_VAL_LEVEL];
  }
  PGX.Close();

  
  double val = 0.;
  cout << "-----" << endl << "random access of a key that does not exist:" << endl;
  key[0] = "seattle";
  key[1] = "something else";
  if (xlevel.find(key) != xlevel.end()) {
	cout << key[0] << "." << key[1] << " : " << xlevel[key] << endl;
  }
  else cout << "key does not exist" << endl;
  
  cout << "-----" << endl << "random access of a key that does exist:" << endl;
  key[0] = "seattle";
  key[1] = "chicago";
  if (xlevel.find(key) != xlevel.end()) {
    cout << key[0] << "." << key[1] << " : " << xlevel[key] << endl;
  }
  else cout << "key does not exist" << endl;

  cout << "-----" << endl << "iterate through keys and perform random access to get the values:" << endl;
  
  map<vector<string>, double>::iterator it;
  for(it=xlevel.begin(); it != xlevel.end(); it++) {
    cout << it->first[0] << "." << it->first[1] << " : " << it->second << endl;
  }

     
  cout << "-----" << endl << "change the value of a key and access it once again:" << endl;
  key[0] = "seattle";
  key[1] = "chicago";
  xlevel[key] = 111.1;
  cout << key[0] << "." << key[1] << " : " << xlevel[key] << endl;

  cout << endl << "xp_associative_vec DONE" << endl;

}
