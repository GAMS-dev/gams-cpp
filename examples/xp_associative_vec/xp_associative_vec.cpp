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
  cl xp_associative_vec.cpp ../C/api/gdxcc.c -I../C/api
*/

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include "gdxcc.h"

using namespace std;

int main(int argc, char* argv[]){
  gdxHandle_t PGX = NULL;
  int dim = 0;
  int errNr = 0;
  int varNr = 0;
  int varType = 0;
  int nrRecs = 0;
  int n = 0;
  char   msg[GMS_SSSIZE], sysDir[GMS_SSSIZE], varName[GMS_SSSIZE];
  static gdxStrIndexPtrs_t idx;
  static gdxStrIndex_t     idxXXX;
  gdxValues_t values;

  if (argc > 1) {
	  strcpy(sysDir, argv[1]);
  }
  else {
	  strcpy(sysDir, "c:\\gams\\win32\\24.9");
  }

  if (!gdxCreateD(&PGX, sysDir, msg, sizeof(msg))) {
	  cout << "**** Could not load GDX library" << endl << "**** " << msg << endl;
	  exit(1);
  }

  map<vector<string>, double> xlevel;

  gdxOpenRead(PGX, "..\\..\\GAMS\\trnsport.gdx", &errNr);
  if (errNr != 0) {
	  gdxErrorStr(PGX, errNr, msg);
	  cout << msg << endl;
	  cout << "*** Error opening trnsport.gdx" << endl;
	  exit(1);
  }
  if (gdxFindSymbol(PGX, "x", &varNr) == 0) {
	  cout << "*** Cannot find symbol 'x'" << endl;
	  exit(1);
  }
  gdxSymbolInfo(PGX, varNr, varName, &dim, &varType);
  if (dim != 2 || GMS_DT_VAR != varType) {
	  cout << "**** x is not a two dimensional variable.  dim:" << dim << "  type:" << varType << endl;
	  exit(1);
  }
  if (gdxDataReadStrStart(PGX, varNr, &nrRecs) == 0) {
	  cout << "*** Cannot read symbol 'x'" << endl;
	  exit(1);
  }

  GDXSTRINDEXPTRS_INIT(idxXXX, idx);

  vector<string> key(2);
  for (int i=0; i<nrRecs; i++){
    gdxDataReadStr(PGX, idx, values, &n);
    key[0] = idx[0];
	key[1] = idx[1];
    xlevel[key] = values[GMS_VAL_LEVEL];
  }
  gdxClose(PGX);

  
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
