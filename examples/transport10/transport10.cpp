/*
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2019 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2019 GAMS Development Corp. <support@gams.com>
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
#include <vector>

using namespace gams;
using namespace std;

#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)

/// \file transport10.cpp
/// \brief This is the 10th model in a series of tutorial examples.
int main()
{
    cout << "---------- Transport 10 --------------" << endl;
    cout << "Transport 10 is a Microsoft Windows only example." << endl;
    return 0;
}

#else

#include <QAxObject>
#include <Windows.h> // includes "Ole2.h" that includes "objbase.h" to access CoInitialize() and CoUninitialize()

string getModelText()
{
    return  "  Sets                                                                     \n"
            "       i   canning plants                                                  \n"
            "       j   markets                                                         \n"
            "                                                                           \n"
            "  Parameters                                                               \n"
            "       a(i)   capacity of plant i in cases                                 \n"
            "       b(j)   demand at market j in cases                                  \n"
            "       d(i,j) distance in thousands of miles                               \n"
            "  Scalar f  freight in dollars per case per thousand miles /90/;           \n"
            "                                                                           \n"
            "$if not set gdxincname $abort 'no include file name for data file provided'\n"
            "$gdxin %gdxincname%                                                        \n"
            "$load i j a b d                                                            \n"
            "$gdxin                                                                     \n"
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
            "  demand(j) ..   sum(i, x(i,j))  =g=  b(j) ;                               \n"
            "                                                                           \n"
            "  Model transport /all/ ;                                                  \n"
            "                                                                           \n"
            "  Solve transport using lp minimizing z ;                                  \n"
            "                                                                           \n"
            "  Display x.l, x.m ;                                                       \n";
}

/// Creates an 1 dimensional GAMSParameter and transfers data from an Excel sheet (horizontal ordered)
///
/// \param sheets The sheets object of an open Excel workbook
/// \param sheetName The name of the sheet to be read
/// \param db The GAMSDatabase where the GAMSParameter is created in
/// \param paramName The name of the new GAMSParameter
/// \param paramText The explplanatory text of the new GAMSParameter
/// \param set The GAMSSet for GAMSParameter dimension
/// \return The new GAMSParameter in the GAMSDatabase
///
GAMSParameter sheetToParameter(QAxObject* sheets, string sheetName
                               , GAMSDatabase db, string paramName, string paramText, GAMSSet set)
{
    QAxObject* sheet = sheets->querySubObject( "Item( string )", sheetName.c_str() );
    GAMSParameter param = db.addParameter(paramName, paramText, set);

    QAxObject* usedrange = sheet->querySubObject( "UsedRange");
    QAxObject * columns = usedrange->querySubObject("Columns");
    int intCols = columns->property("Count").toInt();

    for (int i = 1; i <= intCols; i++) {
        std::string name = sheet->querySubObject("Cells( int, int )", 1, i)->dynamicCall("Value()").toString().toStdString();
        double value = sheet->querySubObject("Cells( int, int )", 2, i)->dynamicCall("Value()").toDouble();
        set.addRecord(name);
        GAMSParameterRecord rec = param.addRecord(name);
        rec.setValue(value);
    }
    return param;
}

/// Creates a 2 dimensional GAMSParameter and transfers data from an Excel sheet
///
/// \param sheets The sheets object of an open Excel workbook
/// \param sheetName The name of the sheet to be read
/// \param db The GAMSDatabase where the GAMSParameter is created in
/// \param paramName The name of the new GAMSParameter
/// \param paramText The explplanatory text of the new GAMSParameter
/// \param set1 The GAMSSet for first GAMSParameter dimension
/// \param set2 The GAMSSet for second GAMSParameter dimension
/// \return The new GAMSParameter in the GAMSDatabase
///
GAMSParameter sheetToParameter(QAxObject* sheets, string sheetName
                               , GAMSDatabase db, string paramName, string paramText, GAMSSet set1, GAMSSet set2)
{
    QAxObject* sheet = sheets->querySubObject( "Item( string )", sheetName.c_str() );
    vector<GAMSDomain> sets {set1, set2};
    GAMSParameter param = db.addParameter(paramName, paramText, sets);

    QAxObject* usedrange = sheet->querySubObject( "UsedRange");
    QAxObject * columns = usedrange->querySubObject("Columns");
    int intCols = columns->property("Count").toInt();
    QAxObject * rows = usedrange->querySubObject("Rows");
    int intRows = rows->property("Count").toInt();

    for (int j = 2; j <= intCols; j++) {
        string namej = sheet->querySubObject("Cells( int, int )", 1, j)->dynamicCall("Value()").toString().toStdString();
        for (int i = 2; i <= intRows; ++i) {
            string namei = sheet->querySubObject("Cells( int, int )", i, 1)->dynamicCall("Value()").toString().toStdString();
            GAMSParameterRecord rec = param.addRecord(namei, namej);
            double value = sheet->querySubObject("Cells( int, int )", i, j)->dynamicCall("Value()").toDouble();
            rec.setValue(value);
        }
    }
    return param;
}

/// \file transport10.cpp
/// \brief This is the 10th model in a series of tutorial examples.
///
/// Here we show:
///   - How to fill a GAMSDatabase by reading from MS Excel
int main(int argc, char* argv[])
{
    cout << "---------- Transport 10 --------------" << endl;
    try {
        ::CoInitialize(0); // initialize thread to use ActiveX (some systems may need CoInititializeEx)

        GAMSWorkspaceInfo wsInfo;
        if (argc > 1)
            wsInfo.setSystemDirectory(argv[1]);
        GAMSWorkspace ws(wsInfo);

        // Creating the GAMSDatabase and fill with the workbook data
        GAMSDatabase db = ws.addDatabase();
        QString fileName = QString::fromStdString(ws.systemDirectory())+ cPathSep + "apifiles" + cPathSep + "Data" + cPathSep + "transport.xls";

        QAxObject* excel = new QAxObject( "Excel.Application", 0 );
        QAxObject* workbooks = excel->querySubObject( "Workbooks" );
        QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", fileName );
        QAxObject* sheets = workbook->querySubObject( "Worksheets" );

        GAMSSet i = db.addSet("i", 1, "Plants");
        GAMSSet j = db.addSet("j", 1, "Markets");

        // read parameters
        sheetToParameter(sheets, "capacity", db, "a", "Capacity", i);
        sheetToParameter(sheets, "demand", db, "b", "Demand", j);
        sheetToParameter(sheets, "distance", db, "d", "Distance", i, j);

        // clean up and close up
        workbook->dynamicCall("Close()");
        excel->dynamicCall("Quit()");


        // Create and run the GAMSJob
        GAMSOptions opt = ws.addOptions();
        GAMSJob t10 = ws.addJobFromString(getModelText());
        opt.setDefine("gdxincname", db.name());
        opt.setAllModelTypes("xpress");
        t10.run(opt, db);
        for (GAMSVariableRecord record : t10.outDB().getVariable("x"))
            cout << "x(" << record.key(0) << "," << record.key(1) << "): level=" << record.level() <<
                    " marginal=" << record.marginal() << endl;

        ::CoUninitialize();

    } catch (GAMSException &ex) {
        cout << "GAMSException occured: " << ex.what() << endl;
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
#endif
