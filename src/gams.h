/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMS_H
#define GAMS_H

#include "gamsenum.h"
#include "gamsplatform.h"
#include "gamsexception.h"
#include "gamsexceptionexecution.h"
#include "gamsoptions.h"
#include "gamsworkspaceinfo.h"
#include "gamsworkspace.h"
#include "gamsdatabase.h"
#include "gamsjob.h"
#include "gamscheckpoint.h"
#include "gamsmodelinstance.h"
#include "gamsdomain.h"
#include "gamsset.h"
#include "gamsvariable.h"
#include "gamsparameter.h"
#include "gamsequation.h"
#include "gamssetrecord.h"
#include "gamsparameterrecord.h"
#include "gamsvariablerecord.h"
#include "gamsequationrecord.h"
#include "gamsmodifier.h"

/// <p>The GAMS namespace provides objects to interact with the General Algebraic Modeling
/// System (GAMS). Objects in this namespace allow convenient exchange of input data
/// and model results (GAMSDatabase), help to create and run GAMS models (GAMSJob),
/// that can be customized by GAMS options (GAMSOptions). Furthermore, it
/// introduces a way to solve a sequence of closely related model instances in the
/// most efficient way (GAMSModelInstance).</p>
/// <p>The underlying GAMS engine relies to some extent on file based communication
/// (e.g. the listing file) and other low-level resources. A GAMS program can include
/// other source files (e.g. $include), load data from
/// GDX files (e.g. $GDXIN or execute_load), and create PUT files. All these files
/// can be specified with a (relative) path and therefore an anchor into the file
/// system is required. The base class GAMSWorkspace manages the anchor to the file
/// system. If external file communication is not an issue in a particular application,
/// temporary directories and files will be managed by objects within the library.</p>
/// <p>With the exception of GAMSWorkspace the objects in the GAMS namespace cannot be
/// accessed across different threads unless the instance is locked. The classes themself
/// are thread safe and multiple objects of the class can be used from different threads
/// (see below for restrictions on solvers that are not thread safe within the
/// GAMSModelInstance class).</p>
/// <p>Note: If you use multiple instances of the GAMSWorkspace in parallel, you should
/// avoid using the same WorkingDirectory. Otherwise you may end up with conflicting
/// file names.</p>
/// <p>This version of the GAMS namespace lacks support for the following GAMS
/// components:</p>
/// <p>Acronyms, support for GAMS compilation/execution errors
/// (GAMSJob.Run just throws an exception), structured access to listing file, and
/// proper support for solver options.</p>
/// <p>Currently only Cplex, Gurobi, ODHCplex, and SoPlex fully utilize the power of solving
/// GAMSModelInstances, i.e. only communicate the changes from the previous
/// model instance to the solver (model instance update versus complete new 
/// setup inside the solver) and perform a hot start. For quadratic model
/// instances and instances with "exotic" features like indicator constraints, the instance
/// update and hot start are not available. The solver will indicate 
/// this with the log message `*** Hotstart failed! Falling back to cold start...`.</p> 
/// <p>Some solvers will not work in a multi-threaded application using GAMSModelInstances. 
/// For some solvers this is unavoidable because the solver library is not thread safe 
/// (e.g.  MINOS), other solvers are in principle thread safe but the GAMS link is not 
/// (e.g. LINDO). A table in the chapter "The Grid and Multi-Threading Solve Facility"
/// provides an overview which solvers work in a multi-threaded application.</p>
/// <p>There are examples in C++ using this API in [GAMS system]\\apifiles.</p>
namespace gams {

}

#endif // GAMS_H
