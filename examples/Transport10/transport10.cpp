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

#include "transport10.h"
#include "gams.h"
#include "gamssymbolrecord.h"

#include <iostream>
#include <vector>

using namespace gams;
using namespace std;

/// This is the 10th model in a series of tutorial examples. Here we show:
///   - How to fill a GAMSDatabase by reading from MS Excel
int Transport10::becomes_main(int argc, char* argv[])
{
    GAMSWorkspaceInfo wsInfo;
    wsInfo.setDebug(GAMSEnum::DebugLevel::ShowLog);
    if (argc > 1)
        wsInfo.setSystemDirectory(argv[1]);
    GAMSWorkspace ws(wsInfo);

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

    // Creating the GAMSDatabase and fill with the workbook data
    GAMSDatabase db = ws.addDatabase();

    GAMSSet i = db.addSet("i", 1, "Plants");
    GAMSSet j = db.addSet("j", 1, "Markets");
    GAMSParameter capacityParam = db.addParameter("a", "Capacity", i);
    GAMSParameter demandParam = db.addParameter("b", "Demand", j);
    vector<GAMSDomain> sets {i, j};
    GAMSParameter distanceParam = db.addParameter("d", "Distance", sets);

    cout << "Transport10: not implementet yet." << endl;

    //    for (int ic = 1; ic <= iCount; ic++)
    //    {
    //        i.AddRecord((string)capacityData.GetValue(1, ic));
    //        capacityParam.AddRecord((string)capacityData.GetValue(1, ic)).Value = (double)capacityData.GetValue(2, ic);
    //    }
    //    for (int jc = 1; jc <= jCount; jc++)
    //    {
    //        j.AddRecord((string)demandData.GetValue(1, jc));
    //        demandParam.AddRecord((string)demandData.GetValue(1, jc)).Value = (double)demandData.GetValue(2, jc);
    //        for (int ic = 1; ic <= iCount; ic++)
    //        {
    //            distanceParam.AddRecord((string)distanceData.GetValue(ic + 1, 1), (string)distanceData.GetValue(1, jc + 1)).Value = (double)distanceData.GetValue(ic + 1, jc + 1);
    //        }
    //    }

    // Create and run the GAMSJob
    //    GAMSOptions opt = ws.addOptions();
    //    GAMSJob t10 = ws.addJobFromString(getModelText());
    //    opt.mDefines["gdxincname"] = db.name();
    //    opt.setAllModelTypes("xpress");
    //    t10.run(opt, db);
    //    for (GAMSVariableRecord record : t10.outDB().getVariable("x"))
    //        cout << "x(" << record.key(0) << "," << record.key(1) << "): level=" << record.level() <<
    //                " marginal=" << record.marginal() << endl;

    return 0;
}


string Transport10::getModelText()
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
