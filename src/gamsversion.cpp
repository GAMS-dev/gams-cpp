/**
 * GAMS C++ API
 *
 * Copyright (c) 2017-2025 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2025 GAMS Development Corp. <support@gams.com>
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

#include <string>
#include <sstream>
#include <fstream>
#include <regex>
#include <vector>

#include "gamsversion.h"
#include "gamsoptions.h"
#include "gamsexception.h"

using namespace std;
namespace gams {

const char* GAMSVersion::mApiVersion = API_VERSION;
const char* GAMSVersion::mApiBuildTime = GAMSOptions::gamsBuild();
const char* GAMSVersion::mApiDetail = string(API_VERSION).append(" (").append(GAMSVersion::mApiBuildTime)
                                                              .append(")").c_str();

const char* GAMSVersion::mGamsVersion = GAMSOptions::gamsVersion();

static const char* mApiName = "API version";
static const char* mGamsName = "required GAMS version";

static int part(const char* vString, const int index, const string &typeName)
{
    vector<string> parts;
    string s;
    istringstream iss(vString);
    while (getline(iss, s, '.'))
        parts.push_back(s);

    if (parts.size() <= (unsigned)index)
        throw GAMSException("GAMSVersion: invalid version definition for " + typeName + " " + vString);
    return stoi(parts.at(index));
}

const char* GAMSVersion::apiDetail()
{
    return mApiDetail;
}

const char* GAMSVersion::api()
{
    return mApiVersion;
}

int GAMSVersion::apiMajor()
{
    return part(api(), 0, mApiName);
}

int GAMSVersion::apiMinor()
{
    return part(api(), 1, mApiName);
}

int GAMSVersion::apiBuild()
{
    return part(api(), 2, mApiName);
}

const char* GAMSVersion::apiBuildTime()
{
    return mApiBuildTime;
}

const char* GAMSVersion::gamsVersion()
{
    return mGamsVersion;
}

int GAMSVersion::gamsMajor()
{
    return part(gamsVersion(), 0, mGamsName);
}

int GAMSVersion::gamsMinor()
{
    return part(gamsVersion(), 1, mGamsName);
}

int GAMSVersion::gamsBuild()
{
    return part(gamsVersion(), 2, mGamsName);
}

string GAMSVersion::systemVersion(const string &gamsSystemDir)
{
    std::string stmpStr;
    std::ifstream stmpFile(gamsSystemDir + "/gamsstmp.txt", std::ifstream::in);

    stmpFile.seekg(0, std::ios::end);
    stmpStr.reserve(stmpFile.tellg());
    stmpFile.seekg(0, std::ios::beg);
    stmpStr.assign(std::istreambuf_iterator<char>(stmpFile), std::istreambuf_iterator<char>());
    stmpStr.erase(std::remove(stmpStr.begin(), stmpStr.end(), '\n'), stmpStr.cend());

    std::smatch stmp_match;
    const std::regex stmp_regex(R"(\w+\s+(\d+\.\d+\.\d+).*)");
    if (std::regex_match(stmpStr, stmp_match, stmp_regex) && stmp_match.size() == 2) {
        std::ssub_match stmp_sub_match = stmp_match[1];
        return stmp_sub_match.str();
    }
    return string();
}

GAMSVersion::GAMSVersion()
{}

} // namespace gams
