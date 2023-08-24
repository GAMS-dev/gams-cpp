/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2023 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2023 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMSENGINECONFIGURATION_H
#define GAMSENGINECONFIGURATION_H

#include "gamslib_global.h" // LIBSPEC
#include <string>

namespace gams {

class LIBSPEC GAMSEngineConfiguration
{
public:
    GAMSEngineConfiguration(const std::string &host, const std::string &user, const std::string &pw, const std::string &space);

    std::string host() const;
    void setHost(const std::string &newHost);

    std::string username() const;
    void setUsername(const std::string &newUsername);

    std::string password() const;
    void setPassword(const std::string &newPassword);

    std::string jwt() const;
    void setJwt(const std::string &newJwt);

    std::string space() const;
    void setSpace(const std::string &newSpace);

private:
    /// \brief mHost Base url
    std::string mHost;

    /// \brief mUsername Username for HTTP basic authentication
    std::string mUsername;

    /// \brief mPassword Password for HTTP basic authentication
    std::string mPassword;

    /// \brief mJwt JWT token to use for Bearer authentication. Will only be used if username is empty.
    std::string mJwt;

    /// \brief mSpace Namespace in which the job is to be executed
    std::string mSpace;
};

}

#endif // GAMSENGINECONFIGURATION_H
