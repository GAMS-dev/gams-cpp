/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2020 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2020 GAMS Development Corp. <support@gams.com>
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
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace gams;
using namespace std;

std::string getDataText()
{
    return "Sets                                             \n"
           "i   canning plants   / seattle, san-diego /;     \n"
           "                                                 \n"
           "Alias (i,ii);                                    \n";
}

/// \file domainchecking.cpp
/// \brief This example enforces referential integrity also known in the GAMS lingo
/// as domain checking.
///
/// The GAMS C++ API does a delayed domain checking of symbols. So you can add records
/// to a domain controlled parameter (e.g. p(i)) even though the GAMSSet i does not
/// contain the label (yet). The user can trigger an explicit check of the referential
/// integrity by calling the GAMSDatabase.CheckDomains (or GAMSSymbol.CheckDomains)
/// method. The Object- oriented API provides methods to access the records that
/// violate the referential integrity (see GAMSDatabaseDomainViolation for details).
/// Domain checking is implicitly done when the GAMSDatabase is exported to a GDX file
/// via the GAMSDatabase.export method or for databases provided in the GAMSJob.run method.
/// The implicit domain check can be suppressed (and left to GAMS when importing data)
/// via the GAMSDatabase.SuppressAutoDomainChecking property. This example demonstrates
/// how to trigger domain checking and how to access the records that violate
/// the referential integrity.
int main(int argc, char* argv[])
{
    cout << "---------- Domain Checking --------------" << endl;

    // define some data by using C++ data structures
    vector<string> plants { "Seattle", "San-Diego" };
    vector<string> markets { "New-York", "Chicago", "Topeka" };
    map<string, double> capacity { { "Seattle", 350.0 }, { "San-Diego", 600.0 } };
    map<string, double> demand { { "New-York", 325.0 }, { "Chicago", 300.0 }, { "Topeka", 275.0 } };
    map<tuple<string, string>, double> distance {
        { make_tuple<string,string> ("Seattle",   "New-York"), 2.5 },
        { make_tuple<string,string> ("Seattle",   "Chicago"),  1.7 },
        { make_tuple<string,string> ("Seattle",   "Topeka"),   1.8 },
        { make_tuple<string,string> ("San-Diego", "New-York"), 2.5 },
        { make_tuple<string,string> ("San-Diego", "Chicago"),  1.8 },
        { make_tuple<string,string> ("San-Diego", "Topeka"),   1.4 }
    };


    try {
        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);
        // prepare a GAMSDatabase with data from the C++ data structures
        GAMSDatabase db = ws.addDatabase();

        // add two sets to the GAMSDatabase
        GAMSSet i = db.addSet("i", "");
        for (auto plant: plants)
            i.addRecord(plant);
        GAMSSet j = db.addSet("j", "");
        for (auto market: markets)
            j.addRecord(market);

        // add a parameter with domain information
        GAMSParameter a = db.addParameter("a", "capacity at plant", i);
        for (string plant: plants)
            a.addRecord(plant).setValue(capacity[plant]);

        // if we see a domain violation something went wrong
        if (!a.checkDomains()) {
            cout << "*** Unexpected domain violation in a" << endl;
            return 1;
        }

        // add a parameter with relaxed domain information
        GAMSParameter b = db.addParameter("b", "demand at market j in cases", "j");
        for (string market: markets)
            b.addRecord(market).setValue(demand[market]);

        // if we see a domain violation something went wrong
        if (!b.checkDomains()) {
            cout << "*** Unexpected domain violation in b" << endl;
            return 1;
        }

        // add a 2-dim parameter with domain information
        GAMSParameter d = db.addParameter("d", "distance in thousands of miles", i, j);
        for (auto t : distance) {
            auto tuple = t.first;
            auto t1 = get<0>(tuple);
            auto t2 = get<1>(tuple);
            d.addRecord(t1, t2).setValue(distance[tuple]);
        }

        // if we see a domain violation something went wrong
        if (!d.checkDomains()) {
            cout << "*** Unexpected domain violation in d" << endl;
            return 1;
        }

        // if we see a domain violation in the database something went wrong
        if (!db.checkDomains()) {
            cout << "*** Unexpected domain violation in db" << endl;
            return 1;
        }

        // create some "wrong" entries
        d.addRecord("Seattle", "aa").setValue(1);
        d.addRecord("bb", "Seattle").setValue(1);
        a.addRecord("aa").setValue(1);
        a.addRecord("bb").setValue(1);
        b.addRecord("aa").setValue(1);
        b.addRecord("bb").setValue(1);

        // now the GAMSdatabase as well as the symbols a and d should have domain violations
        if (db.checkDomains()) {
            cout << "*** Domain violation for db not recognized" << endl;
            return 1;
        }
        if (a.checkDomains()) {
            cout << "*** Domain violation for a not recognized" << endl;
            return 1;
        }
        if (d.checkDomains()) {
            cout << "*** Domain violation for d not recognized" << endl;
            return 1;
        }

        // b in contrast was defined with realxed domain info only, therefore we should never see a domain violation
        if (!b.checkDomains()) {
            cout << "*** Unexpected domain violation in b" << endl;
            return 1;
        }

        // for a we should see 2 domain violations ("aa" and "bb")
        int dvCnt = 0;
        cout << "Domain Violations of a:" << endl;
        for (GAMSSymbolDomainViolation sdv: a.getSymbolDVs()) {
            cout << " > ";
            for (bool vi: sdv.violInd())
                cout << (vi ? "true " : "false ");
            cout << "<> ";
            for (string k: sdv.violRec().keys())
                cout << k << " ";
            cout << "<<" << endl;
            dvCnt++;
        }
        if (dvCnt != 2) {
            cout << "*** Number of domain violations for a should be 2 but saw " << dvCnt << endl;
            return 1;
        }

        // for d we should see 3 domain violations ("Seattle", *"aa"*; *"bb"*, *"Seattle"*)
        dvCnt = 0;
        cout << "Domain Violations of d:" << endl;
        for (GAMSSymbolDomainViolation sdv: d.getSymbolDVs()) {
            cout << " > ";
            for (bool vi: sdv.violInd()) {
                cout << (vi ? "true " : "false ");
                if (vi)
                    dvCnt++;
            }
            cout << "<> ";
            for (string k: sdv.violRec().keys())
                cout << k << " ";
            cout << "<<" << endl;
        }
        if (dvCnt != 3) {
            cout << "*** Number of domain violations for a should be 3 but saw " << dvCnt << endl;
            return 1;
        }

        // for db we should see 5 domain violations (all the ones from a and d)
        dvCnt = 0;
        cout << "Domain Violations of db:" << endl;
        for (GAMSDatabaseDomainViolation DDV: db.getDatabaseDVs()) {
            cout << " > " + DDV.violSym().name() + ": " << endl;
            for (GAMSSymbolDomainViolation sdv: DDV.violRecs()) {
                cout << "   ";
                for (bool vi: sdv.violInd()) {
                    cout << vi << " ";
                    if (vi)
                        dvCnt++;
                }
                cout << "<> ";
                for (string k: sdv.violRec().keys())
                    cout << k << " ";
                cout << "<<" << endl;
            }
        }
        if (dvCnt != 5) {
            cout << "*** Number of domain violations for db should be 5 but saw " << dvCnt << endl;
            return 1;
        }

        // now we limit the amount of violated records reported to a total of 3
        dvCnt = 0;
        cout << "Domain Violations of db:" << endl;
        for (GAMSDatabaseDomainViolation DDV: db.getDatabaseDVs(3)) {
            cout << " > " + DDV.violSym().name() + ": " << endl;
            for (GAMSSymbolDomainViolation sdv: DDV.violRecs()) {
                cout << "   ";
                for (bool vi: sdv.violInd())
                    cout << vi << " ";
                cout << "<> ";
                for (string k: sdv.violRec().keys())
                    cout << k << " ";
                cout << "<<" << endl;
                dvCnt++;
            }
        }
        if (dvCnt != 3) {
            cout << "*** Number of domain violations for db should be 3 but saw " << dvCnt << endl;
            return 1;
        }
        // now we limit the amount of violated records reported to 1 per symbol
        dvCnt = 0;
        cout << "Domain Violations of db:" << endl;
        for (GAMSDatabaseDomainViolation DDV: db.getDatabaseDVs(0, 1)) {
            cout << " > " + DDV.violSym().name() + ": " << endl;
            for (GAMSSymbolDomainViolation sdv: DDV.violRecs()) {
                cout << "   ";
                for (bool vi: sdv.violInd())
                    cout << vi << " ";
                cout << "<> ";
                for (string k: sdv.violRec().keys())
                    cout << k << " ";
                cout << "<<" << endl;
                dvCnt++;
            }
        }
        if (dvCnt != 2) {
            cout << "*** Number of domain violations for db should be 2 but saw " << dvCnt << endl;
            return 1;
        }

        // by default we should get an exception when exporting a GAMSDatabase with domain violations
        bool sawException = false;
        try {
            db.doExport("test.gdx");
        } catch (...) {
            sawException = true;
            db.setSuppressAutoDomainChecking(true);
            db.doExport("test.gdx");
        }
        if (!sawException) {
            cout << "*** It should not be possible to export a GAMSDatabase containing domain violations by default" << endl;
            return 1;
        }

        // read a parameter with domain info from gdx
        GAMSDatabase db2 = ws.addDatabaseFromGDX("test.gdx");
        GAMSParameter d2 = db2.getParameter("d");

        // the domain of the parameter should be GAMSSet i and GAMSSet j
        for (GAMSDomain item: d2.domains()) {
            if (!item.isRelaxed()) {
                GAMSSet domSet = item.getSet();
                if (domSet.name() == "i") {
                    for (GAMSSetRecord uel: domSet)
                        if (find(plants.begin(), plants.end(), uel.key(0)) == plants.end()) {
                            cout << "*** Unexpected uel " + uel.key(0) + " found in domain i" << endl;
                            return 1;
                        }
                } else if (domSet.name() == "j") {
                    for (GAMSSetRecord uel: domSet)
                        if (find(markets.begin(), markets.end(), uel.key(0)) == markets.end()) {
                            cout << "*** Unexpected uel " + uel.key(0) + " found in domain j" << endl;
                            return 1;
                        }
                } else {
                    cout << "*** Expected GAMSSet i and j but found " + domSet.name() << endl;
                    return 1;
                }
            }
            else
            {
                cout << "*** Expected GAMSSet as domain but found relaxed domain " + item.name() << endl;
                return 1;
            }
        }

        /* *************************************************************** *
         * This next section is acutally not about domain checking, but we *
         * make sure that certain things are working as expected.          *
         * *************************************************************** */

        // Try reading an Alias as Set
        GAMSJob jAlias = ws.addJobFromString(getDataText());
        jAlias.run();
        GAMSSet ii = jAlias.outDB().getSet("ii");
        cout << "Elements of aliased Set:" << endl;
        for (GAMSSetRecord item: ii)
            cout << " > " + item.key(0) << endl;

        GAMSDatabase testDB = ws.addDatabase();
        GAMSSet testSet = testDB.addSet("test", 1);

        // Try adding empty UEL
        testSet.addRecord("");
        cout << "Elements of test Set after adding empty UEL:" << endl;
        cout << " > " << testSet.numberRecords() << endl;

        // GAMS strips pending blanks while leading blanks are relevant
        testSet.addRecord(" a ").setText("a");
        cout << "Record ' a ' should be the same as ' a':" << endl;
        cout << " > " << testSet.findRecord(" a").text() << endl;

        // GAMS cannot handle UELs with more than 63 characters
        // This should be OK ...
        testSet.addRecord("123456789012345678901234567890123456789012345678901234567890123 ").setText("OK");
        // ... but not this
        sawException = false;
        try {
            testSet.addRecord("1234567890123456789012345678901234567890123456789012345678901234").setText("not OK");
        } catch (...) {
            sawException = true;
        }
        if (!sawException) {
            cout << "*** It should not be possible to add a record with more than 63 characters" << endl;
            return 1;
        }

        // GAMS cannot handle explanatory texts with more than 255 characters
        testDB.addSet("textOK", "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345");
        sawException = false;
        try {
            testDB.addSet("textNotOK", "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456");
        } catch (...) {
            sawException = true;
        }
        if (!sawException) {
            cout << "*** It should not be possible to add an explanatory text with more than 255 characters" << endl;
            return 1;
        }

        testSet.addRecord("OK").setText("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345");
        sawException = false;
        try {
            testSet.addRecord("notOK").setText("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456");
        } catch (...) {
            sawException = true;
        }
        if (!sawException) {
            cout << "*** It should not be possible to add an explanatory text with more than 255 characters" << endl;
            return 1;
        }

        // GAMS can handle UELs containing single and double quotes but not at the same time
        testSet.addRecord("quote'");
        testSet.addRecord("quote\"");
        sawException = false;
        try {
            testSet.addRecord("quote'\"");
        } catch (...) {
            sawException = true;
        }
        if (!sawException) {
            cout << "*** It should not be possible to add a record single AND double quote" << endl;
            return 1;
        }

        testDB.doExport("test.gdx");
    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
