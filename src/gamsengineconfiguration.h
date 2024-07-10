/**
 * GAMS C++ API
 *
 * Copyright (c) 2017-2024 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2024 GAMS Development Corp. <support@gams.com>
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

class GAMSEngineConfiguration
{
public:
    LIBSPEC GAMSEngineConfiguration(const std::string &host,
                                    const std::string &user,
                                    const std::string &pw,
                                    const std::string &space);

    LIBSPEC ~GAMSEngineConfiguration();

    /// \brief host Base url
    LIBSPEC std::string host() const;

    /// \brief setHost sets engine url
    LIBSPEC void setHost(const std::string &newHost);

    /// \brief username Username for HTTP basic authentication
    LIBSPEC std::string username() const;

    /// \brief setUsername sets engine username
    LIBSPEC void setUsername(const std::string &newUsername);

    /// \brief password Password for HTTP basic authentication
    LIBSPEC std::string password() const;

    /// \brief setPassword sets engine password
    LIBSPEC void setPassword(const std::string &newPassword);

    /// \brief jwt JWT token to use for Bearer authentication. Will only be used if username is empty.
    LIBSPEC std::string jwt() const;

    /// \brief setJwt sets JWT token
    LIBSPEC void setJwt(const std::string &newJwt);

    /// \brief space Namespace in which the job is to be executed
    LIBSPEC std::string space() const;

    /// \brief setSpace sets engine namespace in which the job is executed
    LIBSPEC void setSpace(const std::string &newSpace);

private:
    std::string mHost;
    std::string mUsername;
    std::string mPassword;
    std::string mJwt;
    std::string mSpace;
};

}

#endif // GAMSENGINECONFIGURATION_H
