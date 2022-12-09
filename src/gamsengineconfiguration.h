#ifndef GAMSENGINECONFIGURATION_H
#define GAMSENGINECONFIGURATION_H

#include <string>

namespace gams {

class GAMSEngineConfiguration
{
public:
    GAMSEngineConfiguration();

    std::string host() const;
    void setHost(std::string newHost);

    std::string username() const;
    void setUsername(const std::string &newUsername);

    std::string password() const;
    void setPassword(const std::string &newPassword);

    std::string jwt() const;
    void setJwt(const std::string &newJwt);

    std::string space() const;
    void setSpace(const std::string &newSpace);

private:
//    AuthenticationHeaderValue authHeader();

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
