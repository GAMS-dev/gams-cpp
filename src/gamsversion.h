/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2021 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2021 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMSVERSION_H
#define GAMSVERSION_H

#include "gamsplatform.h"

namespace gams {

/// Represents the GAMS version information.
class LIBSPEC GAMSVersion // TODO(RG): what about this LIBSPEC?
{
public:
    /// Get the full API version information, including major and
    /// minor version number, build number as well as the build time stamp.
    /// \return Returns full API information.
    static const char* apiDetail();

    /// Get the API version string, including major and
    /// minor version number and build number.
    /// \return Returns the API version string.
    static const char* api();

    /// Get the API major version number.
    /// \return The API major version number.
    static int apiMajor();

    /// Get the API minor version number.
    /// \return The API minor version number.
    static int apiMinor();

    /// Get the API build number.
    /// \return The API build number.
    static int apiBuild();

    /// Get the API build time stamp.
    /// \return The API build time stamp.
    static const char* apiBuildTime();

    /// Get the required GAMS version number.
    /// \remark This number can be used to check the compatibilty
    /// of the API with the GAMS distribution.
    /// \return Returns the required GAMS version number.
    static const char* gamsVersion();

    /// Get the required GAMS major version number.
    /// \return Returns the required GAMS major version number.
    static int gamsMajor();

    /// Get the required GAMS minor version number.
    /// \return Returns the required GAMS minor version number.
    static int gamsMinor();

    /// Get the required GAMS build number.
    /// \return Returns the required GAMS build number.
    static int gamsBuild();

    /// Get the GAMS version number of an explicit installation path.
    /// \param gamsSystemDir The GAMS installation path.
    /// \return Returns the GAMS version if possible; otherwise an empty string.
    static std::string systemVersion(std::string gamsSystemDir);

private:
    GAMSVersion();

    static const char* mApiVersion;
    static const char* mApiBuildTime;
    static const char* mApiDetail;
    static const char* mGamsVersion;
};

} // namespace gams

#endif // GAMSVERSION_H
