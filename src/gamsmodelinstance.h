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

#ifndef GAMSMODELINSTANCE_H
#define GAMSMODELINSTANCE_H

#include <string>
#include <memory>
#include <vector>
#include "gamsenum.h"
#include "gamsmodifier.h"
#include "gamsmodelinstanceopt.h"

namespace gams {

class GAMSCheckpoint;
class GAMSDatabase;
class GAMSOptions;
class GAMSModelInstanceImpl;

/// <p>The GAMSJob class is the standard  way of dealing with a GAMS model and the
/// corresponding solution provided by a solver. The GAMS language provides
/// programming flow that allows to solve models in a loop and do other
/// sophisticated tasks, like building decomposition algorithms.</p>
/// <p>In rare cases, the GAMS model generation time dominates the solver solution time
/// and GAMS itself becomes the bottleneck in an optimization application. For a
/// model instance which is a single mathematical model generated by a GAMS solve
/// statement, the GAMSModelInstance class provides a controlled way of modifying a
/// model instance and solving the resulting problem in the most efficient way, by
/// communicating only the changes of the model to the solver and doing a hot start
/// (in case of a continuous model like LP) without the use of disk IO.</p>
/// <p>The GAMSModelInstance requires a GAMSCheckpoint that contains the model
/// definition. Significant parts of the GAMS solve need to be provided for the
/// instantiation of the GAMSModelInstance. The modification of the model instance is
/// done through data in SyncDB (a property of GAMSModelInstance of type GAMSDatabase).
/// One needs to create GAMSModifiers which contain the information on how to modify
/// the GAMSModelInstance. Such a GAMSModifier consists either of a GAMSParameter or
/// of a triple with the GAMSVariable or GAMSEquation to be updated, the modification
/// action (e.g. Upper, Lower or Fixed for updating bounds of a variable, or Primal/Dual
/// for updating the level/marginal of a variable or equation mainly used for starting
/// non-linear models from different starting points), and a GAMSParameter that holds
/// the data for modification. GAMSSymbols of a GAMSModifier must belong to SyncDB.
/// The list of GAMSModifiers needs to be supplied on the Instantiate call. The use of
/// GAMSParameters that are GAMSModifiers is restricted in the GAMS model source. For
/// example, the parameter cannot be used inside $(). Such parameters become endogenous
/// to the model and will be treated by the GAMS compiler as such. Moreover, the rim of
/// the model instance is fixed: No addition of variables and equations is possible.</p>
/// <p>The Instantiate call will only query the symbol information of the GAMSModifiers,
/// not the data of SyncDB, e.g. to retrieve the dimension of the modifiers. That's why
/// the modifier symbols have to exist (but don't have to have data) in SyncDB when
/// Instantiate is called. The GAMSParameters that contain the update data in SyncDB can
/// be filled at any time before executing the Solve method. The Solve method uses this
/// data to update the model instance. The Solve method will iterate through all records
/// of modifier symbols in the model instance and try to find update data in SyncDB. If
/// a record in SyncDB is found, this data record will be copied into the model instance.
/// If no corresponding record is found in SyncDB there are different choices: 1) the
/// original data record is restored (UpdateType=BaseCase) which is the default, 2) the
/// default record of a GAMSParameter (which is 0)  is used (UpdateType=Zero, and 3) no
/// copy takes place and we use the previously copied record value (UpdateType=Accumulate).
/// After the model instance has been updated, the model is passed to the selected solver.</p>
/// <p>After the completion of the Solve method, the SyncDB will contain the primal and
/// dual solution of the model just solved. Moreover, the GAMSParameters that are
/// GAMSModifiers are also accessible in SyncDB as GAMSVariables with the name of the
/// GAMSParameter plus "_var". The Marginal of this GAMSVariable can provide sensitivity
/// information about the parameter setting. The status of the solve is accessible through
/// the ModelStatus and SolveStatus properties.</p>
/// <p>In general, file operations in GAMS C++ take place in the WorkingDirectory
/// defined in the GAMSWorkspace. Execptions to this rule are files read or created
/// due to solver specific options in the solve routine of GAMSModelInstance. These files
/// are written to (or read from) the current directory, meaning the directory where
/// the application gets executed.</p>
///
/// <p>Example on how to create a GAMSModelInstance from a GAMSCheckpoint that was
/// generated by the method GAMSJob.run().</p>
///
/// \code{.cpp}
/// GAMSWorkspace ws;
/// GAMSCheckpoint cp;
///
/// ws.gamsLib("trnsport");
///
/// GAMSJob job = ws.addJobFromFile("trnsport.gms");
/// job.run(cp);
///
/// GAMSModelInstance mi = cp.addModelInstance();
/// GAMSParameter b = mi.syncDB.addParameter("b", 1, "demand");
///
/// mi.instantiate("transport use lp min z", GAMSModifier(b));
///
/// vector<double> bmult = { 0.7, 0.9, 1.1, 1.3 };
/// for (double bm : bmult)
/// {
///     b.clear();
///     for (GAMSParameterRecord rec : job.outDB.getParameter("b"))
///         b.addRecord(rec.keys()).setValue(rec.value() * bm);
///     mi.solve();
///     cout << "Scenario bmult=" << bm << ":" << endl;
///     cout << "  Modelstatus: " << mi.modelStatus << endl;
///     cout << "  Solvestatus: " << mi.SolveStatus <<endl;
///     cout << "  Obj: " << mi.syncDB.getVariable("z").findRecord().level() << endl;
/// }
/// \endcode
class LIBSPEC GAMSModelInstance
{
public:
    /// Standard constructor.
    GAMSModelInstance();

    /// Assigns a GAMSModelInstance.
    /// \param other Another GAMSModelInstance used as data source.
    /// \return Returns the assigned GAMSModelInstance (*this).
    GAMSModelInstance& operator=(const GAMSModelInstance& other);

    /// Compares two GAMSModelInstance objects.
    /// \param other Another GAMSModelInstance to compare to.
    /// \return Returns <c>true</c> if the two GAMSModelInstance are different; otherwise <c>false</c>.
    bool operator!=(const GAMSModelInstance& other) const;

    /// Compares two GAMSModelInstance objects.
    /// \param other Another GAMSModelInstance to compare to.
    /// \return Returns <c>true</c> if the two GAMSModelInstance are equal; otherwise <c>false</c>.
    bool operator==(const GAMSModelInstance& other) const;

    ///Checks if a GAMSModelInstance is valid.
    /// \return Returns <c>true</c> if the GAMSModelInstance is valid; otherwise <c>false</c>.
    bool isValid() const;

    /// Retrieve GAMSDatabase used to synchronize modifiable data.
    GAMSDatabase syncDb();

    /// Instantiate the GAMSModelInstance.
    /// \param modelDefinition Model definition.
    /// \param options GAMSOptions.
    /// \param modifiers Vector of GAMSModifiers.
    void instantiate(const std::string& modelDefinition, const gams::GAMSOptions& options
                     , const std::vector<gams::GAMSModifier>& modifiers = { });

    /// Instantiate the GAMSModelInstance.
    /// \param modelDefinition Model definition.
    /// \param options GAMSOptions.
    /// \param modifier1 GAMSModifiers one.
    /// \param modifier2 GAMSModifiers two.
    /// \param modifier3 GAMSModifiers three.
    void instantiate(const std::string& modelDefinition, const gams::GAMSOptions& options
                     , const GAMSModifier &modifier1 = GAMSModifier(), const GAMSModifier &modifier2 = GAMSModifier()
                     , const GAMSModifier &modifier3 = GAMSModifier());

    /// Instantiate the GAMSModelInstance.
    /// \param modelDefinition Model definition.
    /// \param modifiers Vector of GAMSModifiers.
    void instantiate(const std::string& modelDefinition, const std::vector<gams::GAMSModifier>& modifiers = { });

    /// Instantiate the GAMSModelInstance.
    /// \param modelDefinition Model definition.
    /// \param modifier1 GAMSModifiers one.
    /// \param modifier2 GAMSModifiers two.
    /// \param modifier3 GAMSModifiers three.
    void instantiate(const std::string& modelDefinition, const GAMSModifier &modifier1
                     , const GAMSModifier &modifier2 = GAMSModifier(), const GAMSModifier &modifier3 = GAMSModifier());

    /// Solve model instance.
    /// \param updateType Update type.
    /// \param output Stream to capture GAMS log.
    /// \param miOpt GAMSModelInstance options.
    void solve(GAMSEnum::SymbolUpdateType updateType, std::ostream& output, GAMSModelInstanceOpt miOpt);

    /// Solve model instance.
    /// \param miOpt GAMSModelInstance options.
    void solve(GAMSModelInstanceOpt miOpt);

    /// Solve model instance.
    /// \param output Stream to capture GAMS log.
    void solve(std::ostream& output);

    /// Solve model instance.
    /// \param updateType Update type.
    void solve(GAMSEnum::SymbolUpdateType updateType = GAMSEnum::SymbolUpdateType::BaseCase);

    /// Get model state.
    GAMSEnum::ModelStat modelStatus();

    /// Get model state as string.
    std::string modelStatusAsString();

    /// Get solve state.
    GAMSEnum::SolveStat solveStatus();

    /// Get solve state as string.
    std::string solveStatusAsString();

    /// Copies this ModelInstance to a new ModelInstance.
    /// \param modelInstanceName Identifier of GAMSModelInstance (determined automatically if omitted).
    /// \return Returns the new ModelInstance.
    GAMSModelInstance copyModelInstance(std::string modelInstanceName = "");

    /// Send interrupt signal to running GAMSModelInstance.
    void interrupt();

    /// Retrieve GAMSCheckpoint.
    /// \return Returns the GAMSCheckpoint.
    GAMSCheckpoint checkpoint();

    /// Retrieve name of GAMSModelInstance
    /// \return Returns the GAMSModelInstance name.
    std::string name();

    /// Get the GAMS log ID.
    /// \return Returns the GAMS log ID.
    LogId logID();

private:
    friend class GAMSCheckpointImpl;
    GAMSModelInstance(const std::shared_ptr<GAMSModelInstanceImpl>& impl);
    GAMSModelInstance(GAMSCheckpoint& checkpoint, const std::string& modelInstanceName = "");

private:
    std::shared_ptr<GAMSModelInstanceImpl> mImpl;
};

} // namespace gams

#endif // GAMSMODELINSTANCE_H
