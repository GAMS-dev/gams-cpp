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

#ifndef GAMSCHECKPOINT_H
#define GAMSCHECKPOINT_H

#include "gamslib_global.h"
#include "gamsenum.h"
#include <string>
#include <memory>


namespace gams {

class GAMSModelInstance;
class GAMSCheckpointImpl;
class GAMSWorkspace;

/// A GAMSCheckpoint class captures the state of a GAMSJob after the GAMSJob.run
/// method has been carried out. Another GAMSJob can continue (or restart) from a
/// GAMSCheckpoint. A GAMSCheckpoint constructed with a file name will create a file
/// (extension .g00) for permanent storage when supplied as parameter on the
/// GAMSJob.run method. Moreover, a GAMSModelInstance is also initialized from a
/// checkpoint that contains the model definition of the model instance.
class LIBSPEC GAMSCheckpoint
{
public:
    /// Standard constructor.
    GAMSCheckpoint();

    /// Copy constructor.
    /// \param other GAMSCheckpoint used to initialize the new GAMSCheckpoint.
    GAMSCheckpoint(const GAMSCheckpoint &other);

    /// Constructor.
    /// \param ws GAMSWorkspace containing GAMSCheckpoint.
    /// \param checkpointName Identifier of GAMSCheckpoint (determined automatically if omitted)
    GAMSCheckpoint(const GAMSWorkspace& ws, const std::string& checkpointName = "");

    /// Assigns a GAMSCheckpoint.
    /// \param other Another GAMSCheckpoint used as data source.
    /// \return Returns the assigned GAMSCheckpoint (*this).
    GAMSCheckpoint& operator=(const GAMSCheckpoint& other);

    /// Compares two GAMSCheckpoint objects.
    /// \param other Another GAMSCheckpoint to compare to.
    /// \return Returns <c>true</c> if the two GAMSCheckpoint are different; otherwise <c>false</c>.
    bool operator!=(const GAMSCheckpoint& other) const;

    /// Compares two GAMSCheckpoint objects.
    /// \param other Another GAMSCheckpoint to compare to.
    /// \return Returns <c>true</c> if the two GAMSCheckpoint are equal; otherwise <c>false</c>.
    bool operator==(const GAMSCheckpoint& other) const;

    /// Create model instance.
    /// \param modelInstanceName Identifier of GAMSModelInstance (determined automatically if omitted).
    /// \returns Returns the GAMSModelInstance.
    GAMSModelInstance addModelInstance(const std::string& modelInstanceName = "");

    /// Get the checkpoint name.
    /// \return Returns the checkpoint name.
    std::string name();

    /// Get the GAMSWorkspace.
    /// \return Returns the GAMSWorkspace.
    GAMSWorkspace workspace();

    /// Get the GAMS log ID.
    /// \return Returns the GAMS log ID.
    gams::LogId logID();

    ///Checks if a GAMSCheckpoint is valid.
    /// \return Returns <c>true</c> if the GAMSCheckpoint is valid; otherwise <c>false</c>.
    bool isValid() const;

    /// Get the checkpoint file name.
    /// \return Returns the checkpoint file name.
    std::string fileName() const;

private:
    std::shared_ptr<GAMSCheckpointImpl> mImpl;
};

} // namespace gams

#endif // GAMSCHECKPOINT_H
