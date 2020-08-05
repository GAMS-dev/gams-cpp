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

#include <string>
#include <sstream>
#include <regex>
#include <vector>

#include "gamsversion.h"
#include "gamsoptions.h"
#include "gamsexception.h"
#include "gamspath.h"

using namespace std;
namespace gams {

const char* GAMSVersion::mApiVersion = API_VERSION;
const char* GAMSVersion::mApiBuildTime = GAMSOptions::gamsBuild();
const char* GAMSVersion::mApiDetail = string(API_VERSION).append(" (").append(GAMSVersion::mApiBuildTime)
                                                              .append(")").c_str();

const char* GAMSVersion::mGamsVersion = GAMSOptions::gamsVersion();

static const char* mApiName = "API version";
static const char* mGamsName = "required GAMS version";

static int part(const char* vString, const int index, const string typeName)
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

string GAMSVersion::systemVersion(string gamsSystemDir)
{
    string gams = "gams";
    gams.append(cExeSuffix);

    GAMSPath sys(gamsSystemDir);
    sys.append(gams);

    ostringstream ps;
    ps << sys.string() << " audit lo=3";
    string proc = ps.str();

    string result;
    FILE* out;
#ifdef _WIN32
    out = _popen(proc.c_str(), "r");
#else
    out = popen(proc.c_str(), "r");
#endif
    // if (!out) TODO(RG): error

    char buffer[255];
    while(fgets(buffer, 255, out)) {
        result += buffer;
    }
#ifdef _WIN32
    _pclose(out);
#else
    pclose(out);
#endif

    regex regex("[0-9]*\\.[][0-9]*\\.[0-9]*");
    smatch match;
    regex_match(result, match, regex);

    return match.str();
}

GAMSVersion::GAMSVersion()
{}

} // namespace gams
