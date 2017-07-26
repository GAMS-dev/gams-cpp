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
#include <vector>
#include "gams.h"
#include <iostream>
//#include <sqlext.h>

using namespace std;
using namespace gams;

// Deprecated:  <Atldbcli.h>  <Afxdao.h>  <Afxdb.h>

#ifdef WIN32
static string F_SEP = "\\";
#else
static string F_SEP = "/";
#endif

void TestQOdbc() {

}

//void TestOdbc() {
//    char szDSN[256] = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=D:\\home\\jmonki\\transport.accdb;";
//    /* Data Access Method used in this sample */
//    const char* DAM = "Direct ODBC";

//    HENV    hEnv;
//    HDBC    hDbc;

//    /* ODBC API return status */
//    SQLRETURN rc;

//    int     iConnStrLength2Ptr;
//    char    szConnStrOut[256];

//    SQLCHAR         chval1[128], chval2[128], colName[128];
//    int             ret1;
//    int             ret2;

//    /* Number of rows and columns in result set */
//    SQLINTEGER      rowCount = 0;
//    SQLSMALLINT     fieldCount = 0, currentField = 0;
//    HSTMT           hStmt;

//    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
//    rc = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
//    if (!SQL_SUCCEEDED(rc)) {
//        cout << "Error allocating SQL_HANDLE_DBC   SQLRETURN=" << rc << "  LastError=" << GetLastError() << endl;
//        exit(rc);
//    }
//    rc = SQLDriverConnect(hDbc, NULL, (SQLCHAR*)szDSN, SQL_NTS, (SQLCHAR*)szConnStrOut, 255, (SQLSMALLINT*)&iConnStrLength2Ptr, SQL_DRIVER_NOPROMPT);
//    if (!SQL_SUCCEEDED(rc)) {
//        cout << "fail: " << (SQLCHAR*)szDSN << "\nretCode: " << rc << "\nERROR: " << GetLastError();
//        exit(rc);
//    }
//    cout << endl;
//}

string getModelText()
{
    return " Sets                                                                       \n"
           "     i   canning plants                                                     \n"
           "     j   markets                                                            \n"
           "                                                                            \n"
           "Parameters                                                                  \n"
           "     a(i)   capacity of plant i in cases                                    \n"
           "     b(j)   demand at market j in cases                                     \n"
           "     d(i,j) distance in thousands of miles                                  \n"
           "Scalar f  freight in dollars per case per thousand miles /90/;              \n"
           "                                                                            \n"
           "$if not set gdxincname $abort 'no include file name for data file provided' \n"
           "$gdxin %gdxincname%                                                         \n"
           "$load i j a b d                                                             \n"
           "$gdxin                                                                      \n"
           "                                                                            \n"
           "Parameter c(i,j)  transport cost in thousands of dollars per case ;         \n"
           "                                                                            \n"
           "            c(i,j) = f * d(i,j) / 1000 ;                                    \n"
           "                                                                            \n"
           "Variables                                                                   \n"
           "     x(i,j)  shipment quantities in cases                                   \n"
           "     z       total transportation costs in thousands of dollars ;           \n"
           "                                                                            \n"
           "Positive Variable x ;                                                       \n"
           "                                                                            \n"
           "Equations                                                                   \n"
           "     cost        define objective function                                  \n"
           "     supply(i)   observe supply limit at plant i                            \n"
           "     demand(j)   satisfy demand at market j ;                               \n"
           "                                                                            \n"
           "cost ..        z  =e=  sum((i,j), c(i,j)*x(i,j)) ;                          \n"
           "                                                                            \n"
           "supply(i) ..   sum(j, x(i,j))  =l=  a(i) ;                                  \n"
           "                                                                            \n"
           "demand(j) ..   sum(i, x(i,j))  =g=  bmult*b(j) ;                            \n"
           "                                                                            \n"
           "Model transport /all/ ;                                                     \n"
           "                                                                            \n"
           "Solve transport using lp minimizing z ;                                     \n"
           "                                                                            \n"
           "Display x.l, x.m ;                                                          \n";
}

int main(int argc, char* argv[])
{
    cout << "---------- Transport 9 --------------" << endl;
    cout << "Transport9: not implementet yet." << endl;
    // TestOdbc();

    GAMSWorkspaceInfo wsInfo;
    if (argc > 1)
        wsInfo.setSystemDirectory(argv[1]);
    GAMSWorkspace ws(wsInfo);

    //// fill GAMSDatabase by reading from Access
    //GAMSDatabase db = ReadFromAccess(ws);

    //// run job
    //GAMSOptions opt = ws.AddOptions();
    //GAMSJob t9 = ws.AddJobFromString(getModelText());
    //opt.Defines["gdxincname"] = db.getName();
    //opt.setAllModelTypes("xpress");
    //t9.run(opt, db);

    //for (GAMSVariableRecord rec : t9.getOutDB().GetVariable("x")) {
    //    cout << "x(" << rec.getKey(0) << ", " << rec.getKey(1) << "): level=" << rec.getLevel() << " marginal = " << rec.getMarginal();
    //}
    //// write results into Access file
    //WriteToAccess(ws, t9.getOutDB());

    return 0;
}

//void Transport9::ReadSet(CSession session, GAMSDatabase db, string strAccessSelect, string setName, int setDim, string setExp)
//{
//    USES_CONVERSION;
//    HRESULT hr;

//    // open the command
//    CCommand<CDynamicStringAccessor> cmd;
//    hr = cmd.Open(session, T2OLE(strAccessSelect.c_str()));
//    if (FAILED(hr)) {
//        cout << "ERROR: Failed to open command.";
//        exit(1);
//    }

//    // check if data exists
//    DBORDINAL colCount = cmd.GetColumnCount();
//    if (colCount == 0) {
//        cout << "ERROR: No records found.";
//        exit(1);
//    }

//    // ensure the correct number of columns
//    DBORDINAL cColumns;
//    DBCOLUMNINFO* rgInfo = NULL;
//    OLECHAR* pStringsBuffer = NULL;
//    cmd.GetColumnInfo(&cColumns, &rgInfo, &pStringsBuffer);
//    if (cColumns != setDim) {
//        cout << "Number of fields in select statement does not match setDim";
//        exit(1);
//    }

//    // prepare and read the data
//    CRowset<CDynamicStringAccessor>* pRS = (CRowset<CDynamicStringAccessor>*)&cmd;
//    GAMSSet gSet = db.AddSet(setName, setDim, setExp);
//    vector<string> keys;
//    while (pRS->MoveNext() == S_OK) {
//        for (int idx = 0; idx < setDim; idx++) {
//            keys.push_back(cmd.GetString(idx));
//        }
//        gSet.AddRecord(keys);
//    }
//    cmd.Close();
//}

//void Transport9::ReadParameter(CSession session, GAMSDatabase db, string strAccessSelect, string parName, int parDim, string parExp)
//{
//    USES_CONVERSION;
//    HRESULT hr;

//    // open the command
//    CCommand<CDynamicStringAccessor> cmd;
//    hr = cmd.Open(session, T2OLE(strAccessSelect.c_str()));
//    if (FAILED(hr)) {
//        cout << "ERROR: Failed to open command.";
//        exit(1);
//    }

//    // check if data exists
//    DBORDINAL colCount = cmd.GetColumnCount();
//    if (colCount == 0) {
//        cout << "ERROR: No records found.";
//        exit(1);
//    }

//    // ensure the correct number of columns
//    DBORDINAL cColumns;
//    DBCOLUMNINFO* rgInfo = NULL;
//    OLECHAR* pStringsBuffer = NULL;
//    cmd.GetColumnInfo(&cColumns, &rgInfo, &pStringsBuffer);
//    if (cColumns != parDim+1) {
//        cout << "Number of fields in select statement does not match parDim+1";
//        exit(1);
//    }

//    // prepare and read the data
//    CRowset<CDynamicStringAccessor>* pRS = (CRowset<CDynamicStringAccessor>*)&cmd;
//    GAMSParameter gPar = db.AddParameter(parName, parDim, parExp);
//    vector<string> keys;
//    while (pRS->MoveNext() == S_OK) {
//        for (int idx = 0; idx < parDim; idx++) {
//            keys.push_back(cmd.GetString(idx));
//        }
//        gPar.AddRecord(keys).setValue(*(double*)(cmd.GetValue(parDim)));
//    }
//    cmd.Close();
//}

//GAMSDatabase Transport9::ReadFromAccess(GAMSWorkspace ws)
//{
//    USES_CONVERSION;
//    GAMSDatabase db = ws.AddDatabase();
//    HRESULT hr;

//    // connect to database
//    string strAccessConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + ws.getSystemDirectory() + F_SEP + "apifiles" + F_SEP + "Data" + F_SEP + "transport.accdb";
//    CDataSource dbDataSource;
//    hr = dbDataSource.OpenFromInitializationString(T2OLE(strAccessConn.c_str()));
//    if FAILED(hr) {
//        cout << "ERROR " << hr << " connecting " << strAccessConn << endl;
//        exit(1);
//    }

//    // open the session
//    CSession session;
//    hr = session.Open(dbDataSource);
//    if FAILED(hr) {
//        cout << "ERROR opening data source." << endl;
//        exit(1);
//    }

//    // read GAMS sets
//    ReadSet(session, db, "SELECT Plant FROM Plant", "i", 1, "canning plants");
//    ReadSet(session, db, "SELECT Market FROM Market", "j", 1, "markets");

//    // read GAMS parameters
//    ReadParameter(session, db, "SELECT Plant,Capacity FROM Plant", "a", 1, "capacity of plant i in cases");
//    ReadParameter(session, db, "SELECT Market,Demand FROM Market", "b", 1, "demand at market j in cases");
//    ReadParameter(session, db, "SELECT Plant,Market,Distance FROM Distance", "d", 2, "distance in thousands of miles");

//    session.Close();
//    dbDataSource.Close();
//    return db;
//}

//void Transport9::WriteVariable(CSession session, GAMSDatabase db, string varName, vector<string> Domains)
//{
//    USES_CONVERSION;
//    HRESULT hr;

//    GAMSVariable var = db.GetVariable(varName);
//    if (Domains.size() != var.getDim()) {
//        cout << "Number of column names does not match the dimension of the variable.";
//        exit(1);
//    }

//    session.StartTransaction();

//    // drop the table
//    CCommand<CDynamicStringAccessor> cmd;
//    string query = "drop table " + varName;
//    hr = cmd.Open(session, T2OLE(query.c_str()));
//    if FAILED(hr) {
//        session.Abort();
//        cout << "ERROR dropping table " << varName << endl;
//        exit(1);
//    }
//    cmd.Close();

//    // create the table
//    query = "create table " + varName + "(";
//    for (string& dom: Domains)
//        query += dom + " varchar(64), ";
//    query += "lvl double)";
//    hr = cmd.Open(session, T2OLE(query.c_str()));
//    if FAILED(hr) {
//        session.Abort();
//        cout << "ERROR dropping table " << varName << endl;
//        exit(1);
//    }
//    cmd.Close();

//    // paste the content into the table
//    for (GAMSVariableRecord rec: var) {
//        query = "insert into " + varName + "(";
//        for (string dom : Domains)
//            query += dom + ", ";
//        query += "lvl) values (";
//        for (string key: rec.getKeys())
//            query += "'" + key + "', ";
//        query += std::to_string(rec.getLevel()) + ")"; // TODO convert double
//        hr = cmd.Open(session, T2OLE(query.c_str()));
//        if FAILED(hr) {
//            session.Abort();
//            cout << "ERROR dropping table " << varName << endl;
//            exit(1);
//        }
//        cmd.Close();
//    }
//    session.Commit();
//}

//void Transport9::WriteToAccess(GAMSWorkspace ws, GAMSDatabase db)
//{
//    USES_CONVERSION;
//    HRESULT hr;

//    // connect to database
//    string strAccessConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + ws.getSystemDirectory() + F_SEP + "apifiles" + F_SEP + "Data" + F_SEP + "transport.accdb";
//    CDataSource dbDataSource;
//    hr = dbDataSource.OpenFromInitializationString(T2OLE(strAccessConn.c_str()));
//    if FAILED(hr) {
//        cout << "ERROR connecting " << strAccessConn << endl;
//        exit(1);
//    }

//    // open the session
//    CSession session;
//    hr = session.Open(dbDataSource);
//    if FAILED(hr) {
//        cout << "ERROR opening data source." << endl;
//        exit(1);
//    }

//    // write levels of variable x
//    vector<string> domains;
//    domains.push_back("i");
//    domains.push_back("j");
//    WriteVariable(session, db, "x", domains);

//}
