/*
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

#ifndef GAMSDATABASE_H
#define GAMSDATABASE_H

#include "gamslib_global.h"
#include <string>
#include <memory>
#include <vector>
#include "gamsenum.h"
#include "gamsexception.h"
#include "gamsdomain.h"
#include "gamsdatabaseiter.h"
#include "gamsdatabasedomainviolation.h"

namespace gams {

class GAMSSymbol;
class GAMSWorkspace;
class GAMSEquation;
class GAMSSet;
class GAMSParameter;
class GAMSVariable;
class GAMSDatabaseImpl;
class GAMSSymbolImpl;


/// <p>An instance of GAMSDatabase communicates data between the C++ world and the
/// GAMS world. A GAMSDatabase consists of a collection of symbols, which can be
/// iterated by using the GAMSDatabaseIter. The symbol types available for a
/// GAMSDatabase correspond to the symbol types know from the GAMS language: Set,
/// Parameter, Variable, and Equation are represented in C++ by a derived class
/// (e.g. GAMSSet, GAMSParameter, etc). Besides the type, a GAMSSymbol has a name
/// (this has to match the name inside the GAMS model), a dimension (currently up to
/// 20) and some explanatory text.</p>
/// <p>Variables and equations also have a subtype: e.g. Binary, Positive, etc. for
/// variables (see type GAMSEnum.VarType) and e.g. E, G etc. for equations (see
/// type GAMSEnum.EquType).</p>
/// <p>GAMSDatabases can be created empty, or initialized from existing GDX files or
/// from another GAMSDatabase (copy). Symbols can be added at any time (e.g.
/// GAMSDatabase.addParameter), but once a symbol is part of a GAMSDatabase, it
/// cannot be removed. Only its associated data (GAMSSymbolRecord) can be purged
/// (see GAMSSymbol.clear()) or individually removed (GAMSSymbol.deleteRecord).
/// Individual data elements are accessed record by record. A record is identified
/// by the keys (a vector of strings). The record data varies by symbol type. For
/// example, a parameter record has a Value property, a variable has the properties
/// Level, Lower, Upper, Marginal, and Scale. Adding a record with keys that already
/// exist results in an exception. Similar, the unsuccessful search for a record
/// also results in an exception.</p>
/// <p>GAMSSymbolIter is used to conveniently iterate through
/// the records of a symbol. There are also sliced access methods to symbol records
/// that allow to iterate through all records with a fixed index at some positions.
/// GAMSDatabases can be exported as GDX files for permanent storage.</p>
/// <p>GAMSJob (OutDB) and GAMSModelInstance (SyncDB) provide instances of GAMSDatabase
/// to communicate results from a GAMS run or a solve. These databases should only
/// be used in the context of the base object (GAMSJob or GAMSModelInstance). If a
/// copy of such a database is required the GAMSDatabase constructor that
/// initializes a GAMSDatabase from another database should be used (e.g.
/// GAMSDatabase newdb = workspace.addDatabase(GAMSJob.outDB);).</p>
/// <p>GAMSDatabases often provide the input data for a GAMSJob. Such GAMSDatabases are
/// listed in the GAMSJob.run methods. Inside the GAMS model source the GAMSDatabase
/// is accessible through a GDX file. The GAMS model source requires a particular
/// file name to connect to the proper GDX file (e.g. $GDXIN filename). A
/// GAMSDatabase can be created with a given name which can be then used inside the
/// model (e.g. GAMSDatabase db = workspace.addDatabase(databaseName:"SupplyData");
/// and then inside the GAMS model source: $GDXIN SupplyData) or an automatically
/// generated name can be used. This name can be passed down to the GAMS model by
/// using the Defines list of a GAMSOptions instance:</p>
/// \code{.cpp}
///   GAMSDatabase db = workspace.addDatabase();
///   GAMSOptions opt = workspace.addOptions();
///   opt.setDefines("SupplyDataFileName", db.Name);
///   ...
///   gamsjob.run(opt, db);
/// \endcode
/// <p>Inside the GAMS model source the name is accessed as follows:</p>
/// \code{.gms}
///   $GDXIN %SupplyDataFileName%
/// \endcode
/// <p>One has to act with some caution when it comes to ordered sets which e.g.
/// allow lag and lead. By not enforcing the "domain checking" for the GAMSDatabase
/// we have aggravated the potential problems for ordered sets.
/// For GAMS, the labels of set elements are just strings, so the order of a set is
/// determined by the appearance of its elements. For example, if one has 'set k
/// / 2,3,4,1,5 /', the order of k is exactly given by this sequence. So the lag (k-1)
/// of k=4 is 3 and the lead (k+1) of k=4 is 1.</p>
/// <p>GAMS performs arithmetic with an extended number range. GAMS has special values
/// for infinity (+INF, -INF), epsilon (EPS), not available (NA), and undefined (UNDEF).
/// When GAMS evaluates expressions with these special values, the calculating engine
/// ensures the correctness of the result (e.g. 5*eps=eps or 5+eps=5). The GAMS model
/// CRAZY in the GAMS Model Library documents the results of the arithmetic operations
/// with respect to special values.</p>
/// <p>In the GAMS C++ API we map the IEEE standard values for +/-infinity (e.g.
/// double.PositiveInfinity) and NA (double.NaN) to the corresponding GAMS values. The
/// special value for UNDEF gets unfiltered through the GAMS C++ API. The internal
/// double value of UNDEF is 1.0E300 (or better use the constant sv_valund from gamsglobals).</p>
/// <p>Special attention needs to be given to the value of 0. Since GAMS is a sparse system
/// it does not store (parameter) records with a true 0. If a record with numerical value of
/// 0 is needed, EPS can help. For example:</p>
/// \code{.gms}
/// set j /1*10 /; parameter b(j); b(j) = 1; b('5') = 0;
/// scalar s,c; s = sum(j, b(j)); c = card(b); display s,c;
/// \endcode
/// <p>will result in</p>
/// \code{.gms}
/// ----      3 PARAMETER s                    =        9.000
///             PARAMETER c                    =        9.000
/// \endcode
/// <p>but</p>
/// \code{.gms}
/// b(j) = 1; b('5') = EPS;
/// \endcode
/// <p>will result in</p>
/// \code{.gms}
/// ----      3 PARAMETER s                    =        9.000
///             PARAMETER c                    =       10.000
/// \endcode
/// <p>What are the consequences for the GAMS C++ API? If we read parameter b in case of b('5')=0,
/// the GAMSDatabase will not have a record for b('5'). In case of b('5')=EPS, the GAMSDatabase will
/// have a record with EPS (numeric_limits<double>::min()). Unlike the IEEE values (e.g.
/// numeric_limits<double>::infinity()), arithmetic operations in C++ will modify EPS (e.g.
/// 5*numeric_limits\<double\>::min()==numeric_limits\<double\>::min()
/// but 5*numeric_limits\<double\>::min()!=numeric_limits<double>::min()). The same rules
/// apply for preparing input data for GAMS in a GAMSDatabase. If a value of EPS is written,
/// GAMS will see the special value EPS. The value used for EPS can be reset using GAMSWorkspace.setMyEPS().
/// All other small values (including 0) will be communicated unfiltered to GAMS. As mentioned before,
/// zeros will not be entered as data records in GAMS. The compiler control $on/offEPS can help to
/// automatically map zeros to EPS.</p>
/// <p>There is one oddity concerning values smaller than 1e-250 on GAMS input. Consider the following example:</p>
/// \code{.cpp}
/// GAMSParameter b = db.AddGAMSParameter("b",1,"");
/// for(int i=1; i &lt; 11; i++) b.AddRecord(i.ToString()).Value = 1;
/// b.FindRecord("5").Value = 1e-251;
/// job.Run(db);
/// \endcode
/// \code{.gms}
/// $load j b
/// scalar card_b; card_b = card(b); display card_b;
/// b(j) = 2*b(j); card_b = card(b); display card_b;
/// \endcode
/// <p>A record with values smaller than 1e-250 exists on input in GAMS, but as soon as the record gets
/// updated by GAMS and is still smaller than 1e-250, the record gets removed.</p>
/// <p>The ordering of a set in GAMS can be non-intuitive: Consider "set i /5/, j /1*5/;".
/// Elements '5' gets internal number 1, '1' get 2, '2' gets 3 and so on. The last element
/// of j '5' has already the internal number 1. The sequence of internal numbers in j is
/// not ascending and hence GAMS considers set j as not sorted, i.e. one can't use the
/// ord() function nor the lag or lead (-,--,+,++) operators. If 'j' would have been defined
/// before 'i' in this example, the "set not ordered" problem would have been avoided.</p>
/// <p>Please note that the GAMSDatabase actually does not implement a relational model for
/// database management. It should be seen as a data storage or data container.</p>
class LIBSPEC GAMSDatabase
{
public:
    /// Standard constructor.
    GAMSDatabase();

    /// Assigns a GAMSDatabase.
    /// \param other Another GAMSDatabase used as data source.
    /// \return Returns the assigned GAMSDatabase (*this).
    GAMSDatabase& operator=(const GAMSDatabase& other);

    ///Checks if a GAMSDatabase is valid.
    /// \return Returns <c>true</c> if the GAMSDatabase is valid; otherwise <c>false</c>.
    bool isValid() const;

    /// Interator to the begining.
    /// \return Iterator to the first GAMSEquation.
    GAMSDatabaseIter begin();

    /// Iterator to end.
    /// \return Iterator to the element following the last GAMSEquation.
    GAMSDatabaseIter end();

    /// Retrieve the number of symbols in the GAMSDatabase.
    int getNrSymbols();

    /// Set the value for controlling whether domain checking is called in GAMSDatabase export.
    /// \param value <c>true</c> to suppress automatically domain checking; otherwise <c>false</c>.
    void setSuppressAutoDomainChecking(bool value);

    /// Get current state of the automatically domain checking.
    /// \returns Returns <c>true</c> if it is activated; otherwise <c>false</c>.
    bool suppressAutoDomainChecking();

    /// Get GAMSDatabase name.
    std::string name();

    /// Get GAMSWorkspace containing GAMSDatabase.
    GAMSWorkspace workspace();

    /// Write database into a GDX file.
    /// \param filePath The path used to write the GDX file. A relative path is
    /// relative to the GAMS working directory. If not present, the file is
    /// written to the working directory using the name of the database.
    void doExport(const std::string& filePath = "");

    /// Check for domain violations.
    /// \returns Returns <c>true</c> if there is any domain violation; otherwise <c>false</c>.
    bool checkDomains();

    /// Get GAMSSymbol by name.
    /// \param name Name of the symbol to retrieve.
    /// \returns Returns the GAMSSymbol.
    /// \see getSymbol, getSet, getParameter, getVariable, getEquation
    GAMSSymbol getSymbol(const std::string& name);

    /// Get GAMSSet by name.
    /// \param name Name of the set to retrieve.
    /// \returns Returns the GAMSSet.
    /// \see getSymbol, getSet, getParameter, getVariable, getEquation
    GAMSSet getSet(const std::string& name);

    /// Get GAMSParameter by name.
    /// \param name Name of the parameter to retrieve.
    /// \returns Returns the GAMSParameter.
    /// \see getSymbol, getSet, getParameter, getVariable, getEquation
    GAMSParameter getParameter(const std::string& name);

    /// Get GAMSVariable by name.
    /// \param name Name of the variable to retrieve
    /// \returns Returns the GAMSVariable.
    /// \see getSymbol, getSet, getParameter, getVariable, getEquation
    GAMSVariable getVariable(const std::string& name);

    /// Get GAMSEquation by name.
    /// \param name Name of the equation to retrieve.
    /// \returns Returns the GAMSEquation.
    /// \see getSymbol, getSet, getParameter, getVariable, getEquation
    GAMSEquation getEquation(const std::string& name);

    /// Add set symbol to database.
    /// \param name Set name.
    /// \param dimension Set dimension.
    /// \param explanatoryText Explanatory text of set.
    /// \returns Returns the GAMSSet.
    /// \see addEquation, addParameter, addVariable
    GAMSSet addSet(const std::string& name, const int dimension, const std::string& explanatoryText = "");

    /// Add set symbol to database.
    /// \param name Set name.
    /// \param explanatoryText Explanatory text of set.
    /// \param domains Set domains.
    /// \returns Returns the GAMSSet.
    /// \see addEquation, addParameter, addVariable
    GAMSSet addSet(const std::string& name, const std::string& explanatoryText
                   , const std::vector<GAMSDomain>& domains = std::vector<GAMSDomain>());

    /// Add set symbol to database.
    /// \param name Set name.
    /// \param explanatoryText Explanatory text of set.
    /// \param domain1 Set domain for first index position.
    /// \param domain2 Set domain for second index position.
    /// \param domain3 Set domain for third index position.
    /// \returns Returns the GAMSSet.
    /// \see addEquation, addParameter, addVariable
    GAMSSet addSet(const std::string& name, const std::string& explanatoryText
                   , GAMSDomain domain1, GAMSDomain domain2 = GAMSDomain(), GAMSDomain domain3 = GAMSDomain());

    /// Add parameter symbol to database.
    /// \param name Parameter name.
    /// \param dimension Parameter dimension.
    /// \param explanatoryText Explanatory text of parameter.
    /// \returns Returns the GAMSParameter.
    /// \see addSet, addEquation, addVariable
    GAMSParameter addParameter(const std::string& name, const int dimension, const std::string& explanatoryText = "");

    /// Add parameter symbol to database.
    /// \param name Parameter name.
    /// \param explanatoryText Explanatory text of parameter.
    /// \param domains Set domains.
    /// \returns Returns the GAMSParameter.
    /// \see addSet, addEquation, addVariable
    GAMSParameter addParameter(const std::string& name, const std::string& explanatoryText = ""
                               , const std::vector<GAMSDomain>& domains = std::vector<GAMSDomain>());

    /// Add set symbol to database.
    /// \param name Parameter name.
    /// \param explanatoryText Explanatory text of parameter.
    /// \param domain1 Set domain for first index position.
    /// \param domain2 Set domain for second index position.
    /// \param domain3 Set domain for third index position.
    /// \returns Returns the GAMSParameter.
    /// \see addSet, addEquation, addVariable
    GAMSParameter addParameter(const std::string& name, const std::string& explanatoryText, GAMSDomain domain1
                               , GAMSDomain domain2 = GAMSDomain(), GAMSDomain domain3 = GAMSDomain());

    /// Add variable symbol to database.
    /// \param name Variable name.
    /// \param dimension Variable dimension
    /// \param varType Variable subtype (Binary, Integer, Positive, Negative, Free, SOS1, SOS2, SemiCont, SemiInt).
    /// \param explanatoryText Explanatory text of variable.
    /// \returns Returns the GAMSVariable.
    /// \see addSet, addEquation, addParameter
    GAMSVariable addVariable(const std::string& name, const int dimension, const GAMSEnum::VarType varType
                             , const std::string& explanatoryText = "");

    /// Add variable symbol to database.
    /// \param name Variable name.
    /// \param varType Variable subtype (Binary, Integer, Positive, Negative, Free, SOS1, SOS2, SemiCont, SemiInt).
    /// \param explanatoryText Explanatory text of variable.
    /// \param domains Set domains.
    /// \returns Returns the GAMSVariable.
    /// \see addSet, addEquation, addParameter
    GAMSVariable addVariable(const std::string& name, const GAMSEnum::VarType varType, const std::string& explanatoryText = ""
                             , const std::vector<GAMSDomain>& domains = std::vector<GAMSDomain>());

    /// Add variable symbol to database.
    /// \param name Variable name.
    /// \param varType Variable subtype (Binary, Integer, Positive, Negative, Free, SOS1, SOS2, SemiCont, SemiInt).
    /// \param explanatoryText Explanatory text of variable.
    /// \param domain1 Set domain for first index position.
    /// \param domain2 Set domain for second index position.
    /// \param domain3 Set domain for third index position.
    /// \returns Returns the GAMSVariable.
    /// \see addSet, addEquation, addParameter
    GAMSVariable addVariable(const std::string& name, const GAMSEnum::VarType varType, const std::string& explanatoryText
                             , GAMSDomain domain1, GAMSDomain domain2 = GAMSDomain(), GAMSDomain domain3 = GAMSDomain());

    /// Add equation symbol to database.
    /// \param name Equation name.
    /// \param dimension Equation dimension.
    /// \param equType Equation subtype (E: Equal, G: Greater, L: Less, N: No specification, X: External defined, C: Conic).
    /// \param explanatoryText Explanatory text of equation.
    /// \returns Returns the GAMSEquation.
    /// \see addSet, addVariable, GAMSParameter
    GAMSEquation addEquation(const std::string& name, const int dimension, const GAMSEnum::EquType equType
                             , const std::string& explanatoryText = "");

    /// Add equation symbol to database.
    /// \param name Equation name.
    /// \param equType Equation subtype (E: Equal, G: Greater, L: Less, N: No specification, X: External defined, C: Conic).
    /// \param explanatoryText Explanatory text of equation.
    /// \param domains Set domains.
    /// \returns Returns the GAMSEquation.
    /// \see addSet, addVariable, GAMSParameter
    GAMSEquation addEquation(const std::string& name, const GAMSEnum::EquType equType, const std::string& explanatoryText = ""
                             , const std::vector<GAMSDomain>& domains = std::vector<GAMSDomain>());

    /// Add equation symbol to database.
    /// \param name Equation name.
    /// \param equType Equation subtype (E: Equal, G: Greater, L: Less, N: No specification, X: External defined, C: Conic).
    /// \param explanatoryText Explanatory text of equation.
    /// \param domain1 Set domain for first index position.
    /// \param domain2 Set domain for second index position.
    /// \param domain3 Set domain for third index position.
    /// \returns Returns the GAMSEquation.
    /// \see addSet, addVariable, GAMSParameter
    GAMSEquation addEquation(const std::string& name, const GAMSEnum::EquType equType, const std::string& explanatoryText
                             , GAMSDomain domain1, GAMSDomain domain2 = GAMSDomain(), GAMSDomain domain3 = GAMSDomain());

    /// Get all <c>GAMSDatabaseDomainViolation</c>'s.
    /// \param maxViol The maximum number of domain violations which should be stored (0 for no limit).
    /// \param maxViolPerSym The maximum number of domain violations which should be stored per Symbol (0 for no limit).
    /// \return Returns a list of <c>GAMSDatabaseDomainViolations</c>.
    std::vector<GAMSDatabaseDomainViolation> getDatabaseDVs(int maxViol = 0, int maxViolPerSym = 0);

    /// Destructor.
    ~GAMSDatabase();

    /// Compares two GAMSDatabase objects.
    /// \param other Another GAMSDatabase to compare to.
    /// \return Returns <c>true</c> if the two GAMSDatabase are different; otherwise <c>false</c>.
    bool operator!=(const GAMSDatabase& other) const;

    /// Compares two GAMSDatabase objects.
    /// \param other Another GAMSDatabase to compare to.
    /// \return Returns <c>true</c> if the two GAMSDatabase are equal; otherwise <c>false</c>.
    bool operator==(const GAMSDatabase& other) const;

    /// Check if a GAMSDatabase is valid.
    /// \return Returns <c>true</c> if the GAMSDatabase is valid; otherwise <c>false</c>.
    LogId logID();

    /// Clears all <c>GAMSSymbol</c>'s of a <c>GAMSDatabase</c>.
    void clear();

private:
    friend class GAMSWorkspaceImpl;
    friend class GAMSModelInstanceImpl;
    friend class GAMSJobImpl;
    friend class GAMSSymbolImpl;
    friend class GAMSDatabaseIter;

    GAMSDatabase(const std::shared_ptr<GAMSDatabaseImpl>& impl);
    GAMSDatabase(void* gmdPtr, GAMSWorkspace workspace); ///> TODO(JM) experimental for addDatabaseFromGMD only
    GAMSDatabase(const std::string& gdxFileName, const GAMSWorkspace workspace, const double specValues[]
                 , const std::string& databaseName = "", const std::string& inModelName = "", bool forceName = false);
    GAMSDatabase(const GAMSWorkspace workspace, const double specValues[], const std::string& databaseName = ""
            , const std::string& inModelName = "", bool forceName = false);
    GAMSDatabase(const GAMSWorkspace workspace, const double specValues[], const GAMSDatabase& sourceDb
                 , const std::string& dbName = "", const std::string& inModelName = "");
    std::string inModelName();

private:
    std::shared_ptr<GAMSDatabaseImpl> mImpl;
};

} // namespace gams

#endif // GAMSDATABASE_H
