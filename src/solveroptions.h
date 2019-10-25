/*
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

#ifndef SOLVEROPTIONS_H
#define SOLVEROPTIONS_H
#include <string>

namespace gams{

/// Represents the solver options.
class SolverOptions
{
public:
    /// Standard constructor.
    SolverOptions();

    /// Destructor.
    ~SolverOptions();

    /// Get GAMS solver name.
    /// \return Returns the GAMS solver name.
    std::string getSolverName();

    /// Export GAMSOptions file.
    // \param fileName GAMSOptions file name.
    void exportOptFile(/*std::string fileName*/);

    /// Reset all GAMS options to default.
    void ResetToDefaults();

private:
    std::string SolverName = "";

};
} // namespace gams

#endif // GAMSWORKSPACE_H

