/*
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

#include <QtSql>
#include "gams.h"
#include <iostream>
#include <vector>
#include <QAxObject>

#include <Windows.h> // includes "Ole2.h" that includes "objbase.h" to access CoInitialize() and CoUninitialize()

using namespace gams;
using namespace std;

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


GAMSDatabase readFromExcelActiveX(GAMSWorkspace ws)
{
    GAMSDatabase db = ws.addDatabase();
    QString fileName = QString::fromStdString(ws.systemDirectory()) + cPathSep + "apifiles" + cPathSep + "Data" + cPathSep + "transport.xls";

    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", fileName );
    QAxObject* sheets = workbook->querySubObject( "Worksheets" );
    int sheetCount = sheets->dynamicCall("Count()").toInt();        //worksheets count

    qDebug() << "Sheets: " << sheetCount;
    QAxObject* sheet = sheets->querySubObject( "Item( string )", "capacity" );

    // Find the cells that actually have content
    QAxObject* usedrange = sheet->querySubObject( "UsedRange");
    QAxObject * rows = usedrange->querySubObject("Rows");
    QAxObject * columns = usedrange->querySubObject("Columns");
    int intRowStart = usedrange->property("Row").toInt();
    int intColStart = usedrange->property("Column").toInt();
    int intCols = columns->property("Count").toInt();
    int intRows = rows->property("Count").toInt();

    qDebug() << "intRowStart: " << intRowStart << "intColStart: " << intColStart << "intCols: " << intCols << "intRows: " << intRows;

    // replicate the Excel content in the QTableWidget
//    this->setColumnCount(intColStart+intCols);
//    this->setRowCount(intRowStart+intRows);
//    for (int row=intRowStart ; row < intRowStart+intRows ; row++) {
//        for (int col=intColStart ; col < intColStart+intCols ; col++) {
//            QAxObject* cell = sheet->querySubObject( "Cells( int, int )", row, col );
//            QVariant value = cell->dynamicCall( "Value()" );
//            if (value.toString().isEmpty())
//                continue;

//            QTableWidgetItem * twi = new QTableWidgetItem(value.toString());
//            this->setItem(row-1, col-1, twi);
//        }
//    }

    // clean up and close up
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    return db;
}

GAMSDatabase readFromExcelOdbc(GAMSWorkspace ws)
{
    GAMSDatabase db = ws.addDatabase();

    QSqlDatabase sqlDb = QSqlDatabase::addDatabase("QODBC"/*, "readConnection"*/);

    QString strExcelConn = ("Driver={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + ws.systemDirectory()
                             + cPathSep + "apifiles" + cPathSep + "Data" + cPathSep + "transport.xls").c_str();
    sqlDb.setDatabaseName(strExcelConn);

    qDebug() << "Opening DB";
    if(sqlDb.open()) {
        qDebug() << "Accessing sheet:";
        QSqlQuery query("select * from [distance$]");
        while (query.next()) {
            QString res;
            for (int i = 0; i < 4; ++i) {
                if (query.value(i).isValid())
                    res += "  val" + QString::number(i) + ": " + query.value(i).toString();
                else
                    res += "  val" + QString::number(i) + ": <outOfRange>";
            }
            qDebug() << res;
        }


        sqlDb.close();



        // TODO(AF) add excel stuff
        // Reading input data from workbook
        //    var excelApp = new Excel.Application();
        //    Excel.Workbook wb = excelApp.Workbooks.Open(Path.Combine(ws.SystemDirectory, @"apifiles/Data/transport.xls"));

        //    Excel.Range range;

        //    Excel.Worksheet capacity = (Excel.Worksheet)wb.Worksheets.get_Item("capacity");
        //    range = capacity.UsedRange;
        //    Array capacityData = (Array)range.Cells.Value;
        //    int iCount = capacity.UsedRange.Columns.Count;

        //    Excel.Worksheet demand = (Excel.Worksheet)wb.Worksheets.get_Item("demand");
        //    range = demand.UsedRange;
        //    Array demandData = (Array)range.Cells.Value;
        //    int jCount = range.Columns.Count;

        //    Excel.Worksheet distance = (Excel.Worksheet)wb.Worksheets.get_Item("distance");
        //    range = distance.UsedRange;
        //    Array distanceData = (Array)range.Cells.Value;

        //    // number of markets/plants have to be the same in all spreadsheets
        //    Debug.Assert((range.Columns.Count - 1) == jCount && (range.Rows.Count - 1) == iCount,
        //                 "Size of the spreadsheets doesn't match");
        //    wb.Close();


    }
    else
    {
        qDebug() << "Could not open DB";
        cout << "Error: Failed to create a database connection. " << sqlDb.lastError().text().toStdString() << endl;
        exit(1);
    }
    return db;
}

GAMSParameter sheetToParameter(QAxObject* sheets, string sheetName
                               , GAMSDatabase db, string paramName, string paramText, GAMSSet set)
{
    QAxObject* sheet = sheets->querySubObject( "Item( string )", sheetName.c_str() );
    GAMSParameter param = db.addParameter(paramName, paramText, set);

    // Find the cells that actually have content
    QAxObject* usedrange = sheet->querySubObject( "UsedRange");

    QAxObject * columns = usedrange->querySubObject("Columns");
//    int intColStart = usedrange->property("Column").toInt();
    int intCols = columns->property("Count").toInt();

//    QAxObject * rows = usedrange->querySubObject("Rows");
//    int intRowStart = usedrange->property("Row").toInt();
//    int intRows = rows->property("Count").toInt();

    for (int i = 1; i <= intCols; i++) {
        std::string name = sheet->querySubObject("Cells( int, int )", 1, i)->dynamicCall("Value()").toString().toStdString();
        double value = sheet->querySubObject("Cells( int, int )", 2, i)->dynamicCall("Value()").toDouble();
        set.addRecord(name);
        GAMSParameterRecord rec = param.addRecord(name);
        rec.setValue(value);
    }
    return param;
}

GAMSParameter sheetToParameter(QAxObject* sheets, string sheetName
                               , GAMSDatabase db, string paramName, string paramText, GAMSSet set1, GAMSSet set2)
{
    QAxObject* sheet = sheets->querySubObject( "Item( string )", sheetName.c_str() );
    vector<GAMSDomain> sets {set1, set2};
    GAMSParameter param = db.addParameter(paramName, paramText, sets);

    // Find the cells that actually have content
    QAxObject* usedrange = sheet->querySubObject( "UsedRange");

    QAxObject * columns = usedrange->querySubObject("Columns");
//    int intColStart = usedrange->property("Column").toInt();
    int intCols = columns->property("Count").toInt();

    QAxObject * rows = usedrange->querySubObject("Rows");
//    int intRowStart = usedrange->property("Row").toInt();
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

/// This is the 10th model in a series of tutorial examples. Here we show:
///   - How to fill a GAMSDatabase by reading from MS Excel
int main(int argc, char* argv[])
{
    cout << "---------- Transport 10 --------------" << endl;
    cout << "Transport10: not implementet yet." << endl;

    ::CoInitialize(0); // from objbase.h included via Windows.h

    GAMSWorkspaceInfo wsInfo;
    if (argc > 1)
        wsInfo.setSystemDirectory(argv[1]);
    GAMSWorkspace ws(wsInfo);


    // Creating the GAMSDatabase and fill with the workbook data
    GAMSDatabase db = ws.addDatabase();
    QString fileName = QString::fromStdString(ws.systemDirectory()) + cPathSep + "apifiles" + cPathSep + "Data" + cPathSep + "transport.xls";

    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", fileName );
    QAxObject* sheets = workbook->querySubObject( "Worksheets" );

//    int sheetCount = sheets->dynamicCall("Count()").toInt();        //worksheets count
//    qDebug() << "Sheets: " << sheetCount;

    GAMSSet i = db.addSet("i", 1, "Plants");
    GAMSSet j = db.addSet("j", 1, "Markets");

    // read parameters
    GAMSParameter capacityParam = sheetToParameter(sheets, "capacity", db, "a", "Capacity", i);
    GAMSParameter demandParam = sheetToParameter(sheets, "demand", db, "b", "Demand", j);
    GAMSParameter distanceParam = sheetToParameter(sheets, "distance", db, "d", "Distance", i, j);


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
    return 0;
}
