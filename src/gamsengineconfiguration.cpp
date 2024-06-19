/*
 * GAMS - General Algebraic Modeling System C++ API
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

#include "gamsengineconfiguration.h"
#include "gamsexception.h"
#include <iostream>
#include <regex>

namespace gams {

using namespace std;

GAMSEngineConfiguration::GAMSEngineConfiguration(const string &host, const string &user,
                                                 const string &pw, const string &space)
    : mHost(host)
    , mUsername(user)
    , mPassword(pw)
    , mSpace(space)
{
    if (mHost.empty())
        cerr << "host must not be null" << endl;
    if (mUsername.empty())
        cerr << "user must not be null" << endl;
    // TODO(RG): those two also not null?
    if (mPassword.empty())
        cerr << "password must not be null" << endl;
    if (mSpace.empty())
        cerr << "space must not be null" << endl;
}

GAMSEngineConfiguration::~GAMSEngineConfiguration()
{

}

std::string GAMSEngineConfiguration::host() const
{
    return mHost;
}

void GAMSEngineConfiguration::setHost(const std::string &newHost)
{
    if (! (newHost.rfind("http", 0) == 0 || newHost.rfind("https", 0) == 0))
        throw GAMSException("Invalid GAMS Engine host. Only HTTP and HTTPS protocols supported.");

    // validate host in new url, either by name or ip. source: https://stackoverflow.com/a/106223
    const regex validIpAddressRegex("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}"
                                    "([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");
    const regex validHostnameRegex("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*"
                                   "([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$");

    smatch matchesIp;
    smatch matchesHostname;

    std::regex_match(newHost, matchesIp, validIpAddressRegex);
    std::regex_match(newHost, matchesHostname, validHostnameRegex);

    if (matchesIp.empty() && matchesHostname.empty())
        throw GAMSException("Invalid GAMS Engine host. Make sure you provide a valid URL.");

    mHost = newHost;
    if (newHost.find_last_of('/') == newHost.size())
        mHost = newHost.substr(0, newHost.size()-1);

    // ends with postfix
    string postfix = "/api";
    if (newHost.compare(newHost.length() - postfix.length(), postfix.length(), postfix) != 0)
        mHost.append(postfix);
}

std::string GAMSEngineConfiguration::username() const
{
    return mUsername;
}

void GAMSEngineConfiguration::setUsername(const std::string &newUsername)
{
    mUsername = newUsername;
}

std::string GAMSEngineConfiguration::password() const
{
    return mPassword;
}

void GAMSEngineConfiguration::setPassword(const std::string &newPassword)
{
    mPassword = newPassword;
}

std::string GAMSEngineConfiguration::jwt() const
{
    return mJwt;
}

void GAMSEngineConfiguration::setJwt(const std::string &newJwt)
{
    mJwt = newJwt;
}

std::string GAMSEngineConfiguration::space() const
{
    // TODO(RG): isnt this a little unusual? (copied from another api)
    if (mSpace.empty())
        throw GAMSException("No GAMS Engine namespace set.");
    return mSpace;
}

void GAMSEngineConfiguration::setSpace(const std::string &newSpace)
{
    mSpace = newSpace;
}

}
