#include "gamsengineconfiguration.h"
#include "gamsexception.h"
#include <regex>

using namespace std;

namespace gams {

GAMSEngineConfiguration::GAMSEngineConfiguration()
{ }

GAMSEngineConfiguration::GAMSEngineConfiguration(string host, string user, string pw, string space) :
    mHost(host), mUsername(user), mPassword(pw), mSpace(space)
{ }

std::string GAMSEngineConfiguration::host() const
{
    return mHost;
}

void GAMSEngineConfiguration::setHost(std::string newHost)
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

    // TODO(RG): test this!
    if (newHost.find_last_of('/') == newHost.size())
        newHost = newHost.substr(0, newHost.size()-1);

    // ends with postfix
    string postfix = "/api";
    if (newHost.compare(newHost.length() - postfix.length(), postfix.length(), postfix) != 0)
        newHost.append(postfix);

    mHost = newHost;
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
    // RG: isnt this a little unusual? (copied from another api)
    if (mSpace.empty())
        throw GAMSException("No GAMS Engine namespace set.");
    return mSpace;
}

void GAMSEngineConfiguration::setSpace(const std::string &newSpace)
{
    mSpace = newSpace;
}

// TODO(RG): finish this
//AuthenticationHeaderValue authHeader()
//{
//    if (mUsername.empty()) {
//        if (mJwt.empty())
//            throw GAMSException("Neither username/password nor JWT token provided for authentication with the GAMS Engine instance.");

//        return AuthenticationHeaderValue("Bearer", Convert.ToBase64String(Encoding.ASCII.GetBytes(Jwt)));
//    }
//    return new AuthenticationHeaderValue("Basic", Convert.ToBase64String(Encoding.ASCII.GetBytes(Username + " : " + Password)));
//}


}
