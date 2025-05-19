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
#include "gams.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>

using namespace gams;
using namespace std;


/// \file transportGDX.cpp
/// \brief This example shows the use of the GAMSDatabase class for reading and writing GDX files.
///
/// Here we show:
///   - How to fill a GAMSDatabase from C++ data structures and export it to a GDX file
///   - How to import a GDX file as a GAMSDatabase
int main(int argc, char* argv[])
{
    cout << "---------- Transport GDX --------------" << endl;

    try {
        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);

        // define some data by using C++ data structures
        vector<string> plants = {
            "Seattle", "San-Diego"
        };
        vector<string> markets = {
            "New-York", "Chicago", "Topeka"
        };
        map<string, double> capacity = {
            { "Seattle", 350.0 }, { "San-Diego", 600.0 }
        };
        map<string, double> demand = {
            { "New-York", 325.0 }, { "Chicago", 300.0 }, { "Topeka", 275.0 }
        };
        map<tuple<string, string>, double> distance = {
            { make_tuple("Seattle", "New-York"), 2.5 },
            { make_tuple("Seattle", "Chicago"), 1.7 },
            { make_tuple("Seattle", "Topeka"), 1.8 },
            { make_tuple("San-Diego", "New-York"), 2.5 },
            { make_tuple("San-Diego", "Chicago"), 1.8 },
            { make_tuple("San-Diego", "Topeka"), 1.4 }
        };

        // create new GAMSDatabase instance
        GAMSDatabase db = ws.addDatabase();

        // add 1-dimensional set 'i' with explanatory text 'canning plants' to the GAMSDatabase
        GAMSSet i = db.addSet("i", 1, "canning plants");
        for (string p: plants)
            i.addRecord(p);

        // add 1-dimensional set 'j' with explanatory text 'markets' to the GAMSDatabase
        GAMSSet j = db.addSet("j", 1, "markets");
        for (string m: markets)
            j.addRecord(m);

        // add parameter 'a' with domain 'i'
        GAMSParameter a = db.addParameter("a", "capacity of plant i in cases", i);
        for (string p: plants)
            a.addRecord(p).setValue(capacity[p]);

        // add parameter 'b' with domain 'j'
        GAMSParameter b = db.addParameter("b", "demand at market j in cases", j);
        for (string m: markets)
            b.addRecord(m).setValue(demand[m]);

        // add parameter 'd' with domains 'i' and 'j'
        GAMSParameter d = db.addParameter("d", "distance in thousands of miles", i, j);
        for (auto t : distance)
            d.addRecord(get<0>(t.first), get<1>(t.first)).setValue(t.second);

        // add scalar 'f'
        GAMSParameter f = db.addParameter("f", "freight in dollars per case per thousand miles");
        f.addRecord().setValue(90);

        // export the GAMSDatabase to a GDX file with name 'data.gdx' located in the 'workingDirectory' of the GAMSWorkspace
        db.doExport("data.gdx");

        cout << "Content of GDX file 'data.gdx':";
        string command = "gdxdump " + ws.workingDirectory() + cPathSep + "data.gdx";
        int ret = system(command.c_str());
        if (ret)
            cerr << "system(" << command.c_str() << ") returned " << ret << endl;

        // add a new GAMSDatabase and initialize it from the GDX file just created
        GAMSDatabase db2 = ws.addDatabaseFromGDX("data.gdx");

        // read data from symbols into C++ data structures
        vector<string> iNew;
        for(GAMSSetRecord rec : db2.getSet("i"))
            iNew.push_back(rec.key(0));

        vector<string> jNew;
        for(GAMSSetRecord rec :  db2.getSet("j"))
            jNew.push_back(rec.key(0));

        map<string, double> aNew;
        for(GAMSParameterRecord rec : db2.getParameter("a"))
            aNew[rec.key(0)] = rec.value();

        map<string, double> bNew;
        for(GAMSParameterRecord rec : db2.getParameter("b"))
            bNew[rec.key(0)] = rec.value();

        map<tuple<string, string>, double> dNew;
        for(GAMSParameterRecord rec : db2.getParameter("d"))
            dNew[make_tuple(rec.key(0), rec.key(1))] = rec.value();

        double fNew = db2.getParameter("f").firstRecord().value();

        cout << "i:" << endl;
        for(string s : iNew)
            cout << "  " << s << endl;
        cout  << "j:" << endl;
        for(string s : jNew)
            cout << "  " << s << endl;
        cout << "a:" << endl;
        for (auto rec : aNew)
            cout << "  " << rec.first << " : " << rec.second << endl;
        cout << "b:" << endl;
        for (auto rec : bNew)
            cout << "  " << rec.first << " : " << rec.second << endl;
        cout << "d:" << endl;
        for (auto rec : dNew)
            cout << "  " << get<0>(rec.first) << ", " << get<1>(rec.first) << " : " << rec.second << endl;
        cout << "f:" << endl;
        cout << "  " << fNew;

    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
