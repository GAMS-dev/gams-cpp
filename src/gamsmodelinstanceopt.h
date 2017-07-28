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

#ifndef GAMSMODELINSTANCEOPT_H
#define GAMSMODELINSTANCEOPT_H

#include "gamslib_global.h"
#include <string>
#include <memory>

namespace gams {

class GAMSModelInstanceOptImpl;

/// The GAMSModelInstanceOpt can be used to customize the GAMSModelInstance.solve() routine.
class LIBSPEC GAMSModelInstanceOpt
{
public:
    /// Constructor
    /// \param solver GAMS Solver
    /// \param optfile GAMS Optionfile number
    /// \param noMatchLimit Controls the maximum number of accepted unmatched scenario records before terminating the solve
    /// \param debug Debug Flag
    GAMSModelInstanceOpt(std::string solver = "", int optfile = -1, int noMatchLimit = 0, bool debug = false);

    /// Destructor
    ~GAMSModelInstanceOpt();

    /// Assigns a GAMSModelInstanceOpt.
    /// \param other Another GAMSModelInstanceOpt used as data source.
    /// \return Returns the assigned GAMSModelInstanceOpt (*this).
    GAMSModelInstanceOpt& operator=(const GAMSModelInstanceOpt& other);

    /// Compares two GAMSModelInstanceOpt objects.
    /// \param other Another GAMSModelInstanceOpt to compare to.
    /// \return Returns <c>true</c> if the two GAMSModelInstanceOpt are different; otherwise <c>false</c>.
    bool operator!=(const GAMSModelInstanceOpt& other) const;

    /// Compares two GAMSModelInstanceOpt objects.
    /// \param other Another GAMSModelInstanceOpt to compare to.
    /// \return Returns <c>true</c> if the two GAMSModelInstanceOpt are equal; otherwise <c>false</c>.
    bool operator==(const GAMSModelInstanceOpt& other) const;

    /// Retrieve the specified solver
    std::string solver();

    /// Set a solver for use
    void setSolver(const std::string& solver);

    /// Get the GAMS Optionfile number
    int optFile();

    /// Set the GAMS Optionfile number
    void setOptFile(const int optfile);

    /// Get the no match limit: This number controls the maximum number of accepted unmatched scenario records before terminating the solve
    int noMatchLimit();

    /// Set the no match limit: This number controls the maximum number of accepted unmatched scenario records before terminating the solve
    void setNoMatchLimit(const int noMatchLimit);

    /// Get the debug flag
    bool debug();

    /// Set the debug flag
    void setDebug(const bool debug);

private:
    std::shared_ptr<GAMSModelInstanceOptImpl> mImpl;
};

} // namespace gams

#endif // GAMSMODELINSTANCEOPT_H
