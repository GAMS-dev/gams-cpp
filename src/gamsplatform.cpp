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

#ifdef _WIN32
#include "Windows.h"
#include <direct.h>
#endif

#include "gamsplatform.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <array>

#include "gamspath.h"
#include "gamsenum.h"
#include "gamslog.h"
#include "gamsexception.h"
namespace gams {

using namespace std;

#ifdef __linux__ // Linux
const char cEnvSep = ':';
const char *cPathSep = "/";
const char *cCmpFileName = "gmscmpun.txt";
const int cExitCodeDiv = 256;
const char *cExeSuffix = "";
const char *cLibPrefix = "lib";
const char *cLibSuffix = ".so";
const char *cLibEnv = "LD_LIBRARY_PATH";
#elif __APPLE__ // Apple iOS
const char cEnvSep = ':';
const char *cPathSep = "/";
const char *cCmpFileName = "gmscmpun.txt";
const int cExitCodeDiv = 256;
const char *cExeSuffix = "";
const char *cLibPrefix = "lib";
const char *cLibSuffix = ".dylib";
const char *cLibEnv = "DYLD_LIBRARY_PATH";
#elif __unix__ // other *nix
const char cEnvSep = ':';
const char *cPathSep = "/";
const char *cCmpFileName = "gmscmpun.txt";
const int cExitCodeDiv = 256;
const char *cExeSuffix = "";
const char *cLibPrefix = "lib";
const char *cLibSuffix = ".so";
const char *cLibEnv = "LD_LIBRARY_PATH";
#else // Windows
const char cEnvSep = ';';
const char *cLibEnv = "";
const char *cPathSep = "\\";
const char *cCmpFileName = "gmscmpnt.txt";
const int cExitCodeDiv = 1;
const char *cExeSuffix = ".exe";
const char *cLibPrefix = "";
const char *cLibSuffix = ".dll";
#endif

string GAMSPlatform::findGams(LogId logId)
{
#ifdef _WIN32
    return findGamsOnWindows(logId);
#else
    return findGamsOnUnix(logId);
#endif
}

void GAMSPlatform::ensureEnvPathContains(const char *dirName)
{
#ifdef _WIN32
    ensureEnvPathSetOnWindows(dirName);
#else
    ensureEnvPathSetOnUnix(dirName);
#endif
}

bool GAMSPlatform::interrupt(long pid)
{
#ifdef _WIN32
    return interruptOnWindows(pid);
#else
    return interruptOnNonWindows(pid);
#endif
}

string GAMSPlatform::findGamsOnUnix(LogId logId)
{
    string gamsDir;
    string gamsDirLD;

    string s;
    stringstream ss(getenv("PATH"));
    while (getline(ss, s, cEnvSep)) {
        GAMSPath g(s + "/gams");
        if (g.exists() && filesystem::is_regular_file(g)) {
            gamsDir = s;
            break;
        }
    }
    if (!gamsDir.empty() && LoggerPool::instance().debug(logId) == GAMSEnum::DebugLevel::Off)
        return gamsDir;

    s.clear();
    ss.str(getenv(cLibEnv));
    while (getline(ss, s, cEnvSep)) {
        GAMSPath g(s + "/gams");
        if (g.exists() && filesystem::is_regular_file(g)) {
            gamsDirLD = s;
            break;
        }
    }
    if (gamsDir.empty())
        return gamsDirLD;
    else if (!gamsDirLD.empty() && gamsDir != gamsDirLD) {
        DEB_S(logId) << "--- Warning: Found GAMS system directory " << gamsDir << " in PATH and a different one" << endl
                     << "---          in (DY)LD_LIBRARY_PATH (" << gamsDirLD << "). The latter is ignored.";
    }
    return gamsDir;
}

void GAMSPlatform::ensureEnvPathSetOnUnix(const char *dirName)
{
    // TODO?
}

bool GAMSPlatform::interruptOnNonWindows(long pid)
{
    ostringstream s;
    s << "/bin/bash -c -kill -2 " << pid;
    system(s.str().c_str());
    return true;
}

string GAMSPlatform::findGamsOnWindows(LogId logId)
{
//#ifdef NO_WINDOWS_REGISTRY
    string s;
    stringstream ss(getenv("PATH"));
    while (getline(ss, s, cEnvSep)) {
        if (GAMSPath(s + "/gams.exe").exists()) {
            return s;
        }
    }
    return "";
// TODO(RG):
//#else
//    QString firstFound("");
//    QStringList locations;
//    locations << "HKEY_CURRENT_USER\\Software\\Classes\\" << "HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\";
//    for (QString loc: locations) {
//        QString gamsPath = QSettings(loc + "gams.location", QSettings::NativeFormat).value(".").toString();
//        if (!gamsPath.isEmpty()) {

//            if (LoggerPool::instance().debug(logId) == GAMSEnum::DebugLevel::Off) {
//                return gamsPath.toStdString();
//            } else if (firstFound.isNull()) {
//                firstFound = gamsPath;
//            } else {
//                // compare second key
//                if (firstFound.isEmpty()) {
//                    return gamsPath.toStdString();
//                } else if (!gamsPath.isEmpty() && (gamsPath != firstFound)) {
//                    DEB_S(logId) << "--- Warning: Found GAMS system directory " << firstFound.toStdString() << " at "
//                                 << locations.first() << " and a different one\n"
//                                 << "---          in "
//                                 << locations.last() << " (" << gamsPath.toStdString() << "). The latter is ignored.";
//                }
//                return firstFound.toStdString();
//            }
//        }
//    }
//    return firstFound.toStdString();

//#endif
// TODOend(RG)
}

void GAMSPlatform::ensureEnvPathSetOnWindows(const char *dirName)
{
#ifdef _WIN32
    string g = findGams();
    if (!g.empty()) {
        ostringstream ps;
        ps << "PATH=" << getenv("PATH") << ";" << dirName;
        const char* p = ps.str().c_str();

        _putenv(p);
    }
#else
    throw GAMSException("ensureEnvPathSetOnWindows only impemented on Windows");
#endif
}

bool GAMSPlatform::interruptOnWindows(long pid)
{
#ifdef _WIN32
    COPYDATASTRUCT cds;

    string stem = "___GAMSMSGWINDOW___";
    string pidStr = std::to_string(pid);
    string windowName = stem + pidStr;

    HWND receiver = FindWindow(nullptr, windowName.c_str());
    if (!receiver)
        return false;

    char lpData[] = "GAMS Message Interrupt";
    cds.dwData = 1;
    cds.lpData = lpData;
    cds.cbData = sizeof(lpData);

    SendMessageA(receiver, WM_COPYDATA, 0, (LPARAM)(LPVOID)&cds);

    return true;
#else
    throw GAMSException("interruptOnWindows only impemented on Windows");
#endif
}

int GAMSPlatform::runProcess(const string where, const string what, const string args, string& output)
{
    ostringstream ssp;
    string result;
    FILE* out;
#ifdef _WIN32
    filesystem::path p = filesystem::current_path();
    ssp << what << " " << args;
    _chdir(where.c_str()); // for some reason we need to do this on windows
    out = _popen(ssp.str().c_str(), "r");
    _chdir(p.string().c_str()); // change back to old working dir
#else
    ssp << "cd " << where << " && " << what << " " << args;
    out = popen(ssp.str().c_str(), "r");
#endif
    if (!out) {
        std::cerr << "Couldn't start command: " << ssp.str() << std::endl;
        return -1;
    }
    std::array<char, 128> buffer;
    while (fgets(buffer.data(), 128, out))
        result += buffer.data();

    output = result;

    int exitCode;
#ifdef _WIN32
    exitCode = _pclose(out);
#else
    exitCode = pclose(out);
#endif

    return exitCode;
}

} // namespace gams
