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
#include <vector>
#include "gams.h"
#include <iostream>

using namespace std;
using namespace gams;


string getModelText()
{
    return " Sets                                                                       \n"
           "      i   canning plants                                                    \n"
           "      j   markets                                                           \n"
           "                                                                            \n"
           " Parameters                                                                 \n"
           "      a(i)   capacity of plant i in cases                                   \n"
           "      b(j)   demand at market j in cases                                    \n"
           "      d(i,j) distance in thousands of miles                                 \n"
           " Scalar f  freight in dollars per case per thousand miles /90/;             \n"
           "                                                                            \n"
           "$if not set gdxincname $abort 'no include file name for data file provided' \n"
           "$gdxin %gdxincname%                                                         \n"
           "$load i j a b d                                                             \n"
           "$gdxin                                                                      \n"
           "                                                                            \n"
           " Parameter c(i,j)  transport cost in thousands of dollars per case ;        \n"
           "                                                                            \n"
           "           c(i,j) = f * d(i,j) / 1000 ;                                     \n"
           "                                                                            \n"
           " Variables                                                                  \n"
           "      x(i,j)  shipment quantities in cases                                  \n"
           "      z       total transportation costs in thousands of dollars ;          \n"
           "                                                                            \n"
           " Positive Variable x ;                                                      \n"
           "                                                                            \n"
           " Equations                                                                  \n"
           "      cost        define objective function                                 \n"
           "      supply(i)   observe supply limit at plant i                           \n"
           "      demand(j)   satisfy demand at market j ;                              \n"
           "                                                                            \n"
           " cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;                         \n"
           "                                                                            \n"
           " supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;                                 \n"
           "                                                                            \n"
           " demand(j) ..   sum(i, x(i,j))  =g=  b(j) ;                                 \n"
           "                                                                            \n"
           " Model transport /all/ ;                                                    \n"
           "                                                                            \n"
           " Solve transport using lp minimizing z ;                                    \n"
           "                                                                            \n"
           " Display x.l, x.m ;                                                         \n"
           "                                                                            \n";
}

void readSet(QSqlDatabase sqlDb, GAMSDatabase db, string strAccessSelect, string setName, int setDim, string setExp = "")
{
    QSqlQuery query(sqlDb);
    if (!query.exec(strAccessSelect.c_str()))
    {
        cout << "Error executing query on set '" << setName << "'" << endl;
        cout << query.lastError().text().toStdString() << endl;
        exit(1);
    }
    if (query.size() && (query.record().count() != setDim))
    {
        cout << "Number of fields in select statement does not match setDim" << endl;
        exit(1);
    }

    GAMSSet i = db.addSet(setName, setDim, setExp);
    vector<string> keys = vector<string>(setDim);

    while (query.next())
    {
        for (int idx = 0; idx < setDim; idx++)
            keys[idx] = query.value(idx).toString().toStdString();
        i.addRecord(keys);
    }
}

void readParameter(QSqlDatabase sqlDb, GAMSDatabase db, string strAccessSelect, string parName, int parDim, string parExp = "")
{
    QSqlQuery query(sqlDb);
    if (!query.exec(strAccessSelect.c_str()))
    {
        cout << "Error executing query on parameter '" << parName << "'" << endl;
        cout << query.lastError().text().toStdString() << endl;
        exit(1);
    }
    if (query.size() && (query.record().count() != parDim+1))
    {
        cout << "Number of fields in select statement does not match parDim" << endl;
        exit(1);
    }

    GAMSParameter a = db.addParameter(parName, parDim, parExp);
    vector<string> keys = vector<string>(parDim);

    while (query.next())
    {
        for (int idx = 0; idx < parDim; idx++)
            keys[idx] = query.value(idx).toString().toStdString();
        a.addRecord(keys).setValue(query.value(parDim).toDouble());
    }
}

GAMSDatabase readFromAccess(GAMSWorkspace ws)
{
    GAMSDatabase db = ws.addDatabase();

    QSqlDatabase sqlDb = QSqlDatabase::addDatabase("QODBC", "readConnection");

    QString strAccessConn = ("Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=" + ws.systemDirectory() \
                             + cPathSep + "apifiles" + cPathSep + "Data" + cPathSep + "transport.accdb").c_str();
    sqlDb.setDatabaseName(strAccessConn);

    if(sqlDb.open())
    {
        // read GAMS sets
        readSet(sqlDb, db, "SELECT Plant FROM Plant", "i", 1, "canning plants");
        readSet(sqlDb, db, "SELECT Market FROM Market", "j", 1, "markets");

        // read GAMS parameters
        readParameter(sqlDb, db, "SELECT Plant,Capacity FROM Plant", "a", 1, "capacity of plant i in cases");
        readParameter(sqlDb, db, "SELECT Market,Demand FROM Market", "b", 1, "demand at market j in cases");
        readParameter(sqlDb, db, "SELECT Plant,Market,Distance FROM Distance", "d", 2, "distance in thousands of miles");
        sqlDb.close();
    }
    else
    {
        cout << "Error: Failed to create a database connection. " << sqlDb.lastError().text().toStdString() << endl;
        exit(1);
    }
    return db;
}

void writeVariable(QSqlDatabase sqlDb, GAMSDatabase db, string varName, vector<string> domains)
{
    GAMSVariable var = db.getVariable(varName);
    if(domains.size() != var.dim())
    {
        cout << "Number of column names does not match the dimension of the variable." << endl;
        exit(1);
    }

    // delete table varName if it exists already
    QSqlQuery query(sqlDb);
    query.exec(("drop table " + varName).c_str());

    string queryStr = "create table " + varName + "(";
    for (string dom : domains)
        queryStr += dom + " varchar(64), ";
    queryStr += "lvl double)";

    query.exec(queryStr.c_str());

    for (GAMSVariableRecord rec : var)
    {
        queryStr = "insert into " + varName + "(";
        for (string dom : domains)
            queryStr += dom + ", ";
        queryStr += "lvl) values (";
        for (string key : rec.keys())
            queryStr += "'" + key + "', ";
        queryStr += std::to_string(rec.level()) + ")";
        if(!query.exec(queryStr.c_str()))
        {
            cout << "Error: Failed to write variable to the database" << endl;
            cout << sqlDb.lastError().text().toStdString() << endl;
            exit(1);
        }
    }
}

void writeToAccess(GAMSWorkspace ws, GAMSDatabase db)
{
    // connect to database
    QSqlDatabase sqlDb = QSqlDatabase::addDatabase("QODBC", "writeConnection");

    QString strAccessConn = ("Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=" + ws.systemDirectory() \
                             + cPathSep + "apifiles" + cPathSep + "Data" + cPathSep + "transport.accdb").c_str();
    sqlDb.setDatabaseName(strAccessConn);

    if(sqlDb.open())
    {
        // write levels of variable x
        vector<string> domains{"i", "j"};
        writeVariable(sqlDb, db, "x", domains);
        sqlDb.close();
    }
    else
    {
        cout << "Error: Failed to create a database connection. " << sqlDb.lastError().text().toStdString() << endl;
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    cout << "---------- Transport 9 --------------" << endl;

    GAMSWorkspaceInfo wsInfo;
    if (argc > 1)
        wsInfo.setSystemDirectory(argv[1]);
    GAMSWorkspace ws(wsInfo);

    // fill GAMSDatabase by reading from Access
    GAMSDatabase db = readFromAccess(ws);

    // run job
    GAMSOptions opt = ws.addOptions();
    GAMSJob t9 = ws.addJobFromString(getModelText());
    opt.setDefine("gdxincname", db.name());
    opt.setAllModelTypes("xpress");
    t9.run(opt, db);
    for (GAMSVariableRecord rec : t9.outDB().getVariable("x"))
        cout << "x(" << rec.key(0) << "," << rec.key(1) << "):" << " level=" << rec.level() << " marginal=" << rec.marginal() << endl;
    // write results into Access file
    writeToAccess(ws, t9.outDB());

    return 0;
}


