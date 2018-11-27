/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
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

#include <algorithm>
#include <iostream>

using namespace gams;
using namespace std;

/// Get model as string
string getModelText()
{
    return "  Sets                                                                     \n"
           "       i   canning plants   / seattle, san-diego /                         \n"
           "       j   markets          / new-york, chicago, topeka / ;                \n"
           "                                                                           \n"
           "  Parameters                                                               \n"
           "                                                                           \n"
           "       a(i)  capacity of plant i in cases                                  \n"
           "         /    seattle     350                                              \n"
           "              san-diego   600  /                                           \n"
           "                                                                           \n"
           "       b(j)  demand at market j in cases                                   \n"
           "         /    new-york    325                                              \n"
           "              chicago     300                                              \n"
           "              topeka      275  / ;                                         \n"
           "                                                                           \n"
           "  Table d(i,j)  distance in thousands of miles                             \n"
           "                    new-york       chicago      topeka                     \n"
           "      seattle          2.5           1.7          1.8                      \n"
           "      san-diego        2.5           1.8          1.4  ;                   \n"
           "                                                                           \n"
           "  Scalar f      freight in dollars per case per thousand miles  /90/ ;     \n"
           "  Scalar bmult  demand multiplier /1/;                                     \n"
           "                                                                           \n"
           "  Parameter c(i,j)  transport cost in thousands of dollars per case ;      \n"
           "                                                                           \n"
           "            c(i,j) = f * d(i,j) / 1000 ;                                   \n"
           "                                                                           \n"
           "  Variables                                                                \n"
           "       x(i,j)  shipment quantities in cases                                \n"
           "       z       total transportation costs in thousands of dollars ;        \n"
           "                                                                           \n"
           "  Positive Variable x ;                                                    \n"
           "                                                                           \n"
           "  Equations                                                                \n"
           "       cost        define objective function                               \n"
           "       supply(i)   observe supply limit at plant i                         \n"
           "       demand(j)   satisfy demand at market j ;                            \n"
           "                                                                           \n"
           "  cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;                       \n"
           "                                                                           \n"
           "  supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;                               \n"
           "                                                                           \n"
           "  demand(j) ..   sum(i, x(i,j))  =g=  bmult*b(j) ;                         \n"
           "                                                                           \n"
           "  Model transport /all/ ;                                                  \n";
}

/// Convert string to lower case
string toLower(const string &str)
{
    string lstr(str);
    transform(lstr.begin(), lstr.end(), lstr.begin(), ::tolower);
    return lstr;
}

/// Using GUSS to solve the model in different scenarios
void GUSSCall(GAMSSet dict, GAMSModelInstance mi, string solveStatement
              , gams::GAMSOptions* opt = nullptr
              , gams::GAMSModelInstanceOpt miOpt = gams::GAMSModelInstanceOpt()
              , std::ostream* output = nullptr)
{
    vector< tuple<GAMSModifier, GAMSParameter> > modifierList;

    if (dict.dim() != 3)
        throw GAMSException("Dict needs to be 3-dimensional");

    vector<string> strs { " ", "scenario", " " };
    string scenName = dict.firstRecord(strs).key(0);
    GAMSSet scenSymbol = dict.database().getSet(scenName);


    for (GAMSSetRecord rec : dict) {
        if (toLower(rec.key(1)) == "scenario")
            continue;
        if (toLower(rec.key(1)) == "param") {
            int modifierDim = dict.database().getParameter(rec.key(2)).dim() - scenSymbol.dim();
            if (modifierDim < 0)
                throw GAMSException("Dimension of " + rec.key(2) + " too small");
            auto tuple = make_tuple(GAMSModifier(mi.syncDb().addParameter(rec.key(0), modifierDim, "")),
                                                 dict.database().getParameter(rec.key(2)));
            modifierList.push_back(tuple);

        } else if ((rec.key(1) == "lower") || (toLower(rec.key(1)) == "upper") || (toLower(rec.key(1)) == "fixed")) {
            int modifierDim = dict.database().getParameter(rec.key(2)).dim() - scenSymbol.dim();
            if (modifierDim < 0)
                throw GAMSException("Dimension of " + rec.key(2) + " too small");

            GAMSVariable modifierVar;
            try {
                modifierVar = dict.database().getVariable(rec.key(0));
            } catch (...) {
                modifierVar = mi.syncDb().addVariable(rec.key(0),modifierDim, GAMSEnum::Free, "");
            }

            if (toLower(rec.key(1)) == "lower") {
                auto tuple = make_tuple(GAMSModifier(modifierVar, GAMSEnum::Lower,
                                                     mi.syncDb().addParameter(rec.key(2), modifierDim, "")),
                                        dict.database().getParameter(rec.key(2)));
                modifierList.push_back(tuple);

            } else if (toLower(rec.key(1)) == "upper") {
                auto tuple = make_tuple(GAMSModifier(modifierVar, GAMSEnum::Upper,
                                                     mi.syncDb().addParameter(rec.key(2), modifierDim, "")),
                                        dict.database().getParameter(rec.key(2)));
                modifierList.push_back(tuple);

            } else {  // fixed
                auto tuple = make_tuple(GAMSModifier(modifierVar, GAMSEnum::Fixed,
                                                     mi.syncDb().addParameter(rec.key(2), modifierDim, "")),
                                        dict.database().getParameter(rec.key(2)));
                modifierList.push_back(tuple);
            }
        } else if ((toLower(rec.key(1)) == "level") || (toLower(rec.key(1)) == "marginal")) {
            // Check that parameter exists in GAMSDatabase, will throw an exception if not
            dict.database().getParameter(rec.key(2));
        } else {
            throw GAMSException("Cannot handle UpdateAction " + rec.key(1));
        }
    }

    vector<GAMSModifier> mL;
    for (auto tuple : modifierList)
        mL.push_back(get<0>(tuple));
    if (opt)
        mi.instantiate(solveStatement, *opt, mL);
    else
        mi.instantiate(solveStatement, mL);

    vector<tuple<GAMSSymbol, GAMSParameter, string>> outList;

    for (GAMSSetRecord s : scenSymbol)
    {
        for (auto tup : modifierList)
        {
            GAMSParameter p;
            GAMSParameter pscen = get<1>(tup);

            if (!get<0>(tup).dataSymbol().isValid())
                p = get<0>(tup).gamsSymbol();
            else
                p = get<0>(tup).dataSymbol();

            // Implemented SymbolUpdateType=BaseCase
            p.clear();

            GAMSParameterRecord rec;
            vector<string> filter(pscen.dim());
            for (int i = 0; i < scenSymbol.dim(); i++)
                filter[i] = s.key(i);
            for (int i = scenSymbol.dim(); i < pscen.dim(); i++)
                filter[i] = " ";

            try {
                rec = pscen.firstRecord(filter);
            } catch (GAMSException&) {
                continue;
            }

            do {
                vector<string> myKeys(p.dim());
                for (int i = 0; i < p.dim(); i++)
                    myKeys[i] = rec.key(scenSymbol.dim()+i);
                p.addRecord(myKeys).setValue(rec.value());
            } while (rec.moveNext());
        }

        mi.solve(GAMSEnum::SymbolUpdateType::BaseCase, *output, miOpt);

        if (outList.size() == 0)
            for (GAMSSetRecord rec : dict)
                if ((toLower(rec.key(1)) == "level") || (toLower(rec.key(1)) == "marginal")) {
                    auto tuple = make_tuple(mi.syncDb().getSymbol(rec.key(0)),
                                            dict.database().getParameter(rec.key(2)),
                                            toLower(rec.key(1)));
                    outList.push_back(tuple);
                }

        for (tuple<GAMSSymbol, GAMSParameter, string> tup : outList) {
            GAMSSymbol mySym = get<0>(tup);
            vector<string> myKeys;
            for (int i = 0; i < scenSymbol.dim(); i++)
                myKeys.push_back(s.key(i));

            if ((get<2>(tup) == "level") && (mySym.type() == GAMSEnum::SymTypeVar)) {
                for (GAMSVariableRecord rec : GAMSVariable(mySym)) {
                    for (int i = 0; i < static_cast<int>(rec.keys().size()); i++)
                        myKeys[scenSymbol.dim() + i] = s.key(i);
                    get<1>(tup).addRecord(myKeys).setValue(rec.level());
                }
            } else if ((get<2>(tup) == "level") && (mySym.type() == GAMSEnum::SymTypeEqu)) {
                for (GAMSEquationRecord rec : GAMSEquation(mySym)) {
                    for (int i = 0; i < static_cast<int>(rec.keys().size()); i++)
                        myKeys[scenSymbol.dim() + i] = s.key(i);
                    get<1>(tup).addRecord(myKeys).setValue(rec.level());
                }
            } else if ((get<2>(tup) == "marginal") && (mySym.type() == GAMSEnum::SymTypeVar)) {
                for (GAMSVariableRecord rec : GAMSVariable(mySym)) {
                    for (int i = 0; i < static_cast<int>(rec.keys().size()); i++)
                        myKeys[scenSymbol.dim() + i] = s.key(i);
                    get<1>(tup).addRecord(myKeys).setValue(rec.marginal());
                }
            } else if ((get<2>(tup) == "marginal") && (mySym.type() == GAMSEnum::SymTypeEqu)) {
                for (GAMSEquationRecord rec : GAMSEquation(mySym)) {
                    for (int i = 0; i < static_cast<int>(rec.keys().size()); i++)
                        myKeys[scenSymbol.dim() + i] = s.key(i);
                    get<1>(tup).addRecord(myKeys).setValue(rec.marginal());
                }
            }
        }
    }
}

/// \file transport12.cpp
/// \brief This is the 12th model in a series of tutorial examples.
///
/// Here we show:
///  - How to implement a GUSS approach using the GAMS API
int main(int argc, char* argv[])
{
    cout << "---------- Transport 12 --------------" << endl;

    try {
        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);
        GAMSCheckpoint cp = ws.addCheckpoint();

        // initialize a GAMSCheckpoint by running a GAMSJob
        GAMSJob t12 = ws.addJobFromString(getModelText());
        t12.run(cp);

        // create a GAMSModelInstance and solve it multiple times with different scalar bmult
        GAMSModelInstance mi = cp.addModelInstance();

        double bmultlist[] { 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3 };

        GAMSDatabase db = ws.addDatabase();

        GAMSSet scen = db.addSet("scen", 1, "");
        GAMSParameter bmult = db.addParameter("bmultlist", "", scen);
        GAMSParameter zscen = db.addParameter("zscen", "", scen);

        int i = 0;
        for (double b : bmultlist) {
            bmult.addRecord("s" + to_string(i)).setValue(b);
            scen.addRecord("s" + to_string(i++));
        }

        GAMSSet dict = db.addSet("dict",3,"");
        dict.addRecord(scen.name(), "scenario", "");
        dict.addRecord("bmult", "param", bmult.name());
        dict.addRecord("z", "level", zscen.name());


        GUSSCall(dict, mi, "transport use lp min z");

        for (GAMSParameterRecord rec : db.getParameter(zscen.name()))
            cout << rec.key(0) << " obj: " << rec.value() << endl;

        GAMSModelInstance mi2 = cp.addModelInstance();
        GAMSDatabase db2 = ws.addDatabase();

        GAMSSet scen2 = db2.addSet("scen", 1, "");
        GAMSParameter zscen2 = db2.addParameter("zscen", "", scen2);
        GAMSParameter xup = db2.addParameter("xup", 3);

        for (int j = 0; j < 4; j++) {
            for (GAMSSetRecord irec : t12.outDB().getSet("i"))
                for (GAMSSetRecord jrec : t12.outDB().getSet("j"))
                    xup.addRecord("s" + to_string(j), irec.key(0), jrec.key(0)).setValue(j+1);
            scen2.addRecord("s" + to_string(j));
        }

        GAMSSet dict2 = db2.addSet("dict", 3, "");
        dict2.addRecord(scen2.name(), "scenario", "");
        dict2.addRecord("x", "lower", xup.name());
        dict2.addRecord("z", "level", zscen2.name());

        GUSSCall(dict2, mi2, "transport use lp min z");

        for (GAMSParameterRecord rec : db2.getParameter(zscen2.name()))
            cout << rec.key(0) << " obj: " << rec.value() << endl;

    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
