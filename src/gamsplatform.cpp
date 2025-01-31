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

#include <cstring>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <wchar.h>
#pragma comment(lib, "advapi32")
#endif

#include "gamsplatform.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <array>
#include <thread>

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
#elif __APPLE__
    return findGamsOnApple(logId);
#else
    return findGamsOnUnix(logId);
#endif
}

void GAMSPlatform::ensureEnvPathContains(const char *dirName)
{
#ifdef _WIN32
    ensureEnvPathSetOnWindows(dirName);
#elif __APPLE__
    ensureEnvPathSetOnApple(dirName);
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

string GAMSPlatform::findGamsOnApple(LogId logId)
{
    string path = findGamsOnUnix(logId);
    if (!path.empty())
        return path;

    string defaultPath = "/Library/Frameworks/GAMS.framework/Versions/Current/Resources";
    if (std::filesystem::is_symlink(defaultPath))
        return std::filesystem::read_symlink(defaultPath).string();
    else
        return defaultPath;
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

    const char* ldPathContents = getenv(cLibEnv);
    if (ldPathContents) { // not empty
        ss.str(ldPathContents);
        while (getline(ss, s, cEnvSep)) {
            GAMSPath g(s + "/gams");
            if (g.exists() && filesystem::is_regular_file(g)) {
                gamsDirLD = s;
                break;
            }
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

void GAMSPlatform::ensureEnvPathSetOnApple(const char *dirName)
{
    // TODO?
}

bool GAMSPlatform::interruptOnNonWindows(long pid)
{
    string result;
    runProcess("", "/bin/bash", "-c -kill -2 " + to_string(pid), result);
    return true;
}

string GAMSPlatform::findGamsOnWindows(LogId logId)
{
#ifdef _WIN32
#ifdef NO_WINDOWS_REGISTRY
    string s;
    stringstream ss(getenv("PATH"));
    while (getline(ss, s, cEnvSep)) {
        if (GAMSPath(s + "/gams.exe").exists()) {
            return s;
        }
    }
    return "";
#else
    const DWORD SIZE = 1024;
    string firstFound;

    vector<HKEY> locations = {HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE};
    for (HKEY hkey : locations) {

        // TODO(RG): these should be wchars when we decided how to handle paths with gams
        char inputVal[SIZE];
        DWORD bufferSize = SIZE;
        long retCode;

        retCode = RegOpenKeyA(hkey, "Software\\Classes", &hkey);
        if (retCode != ERROR_SUCCESS) continue;

        retCode = RegGetValueA(hkey, "gams.location", 0, RRF_RT_REG_SZ, nullptr, (LPBYTE) inputVal, &bufferSize);
        RegCloseKey(hkey);
        if (retCode != ERROR_SUCCESS) continue;

        // second run, already found something
        if (!firstFound.empty()) {
            DEB_S(logId) << "--- Warning: Found GAMS system directory " << firstFound.c_str()
                         << "at HKEY_CURRENT_USER and a different one\n ---          in "
                         << "HKEY_LOCAL_MACHINE (" << inputVal << "). The latter is ignored.";
        } else {
            firstFound = inputVal;
        }
    }
    return firstFound;
#endif
#else
    throw GAMSException("interruptOnWindows only impemented on Windows");
#endif
}

void GAMSPlatform::ensureEnvPathSetOnWindows(const char *dirName)
{
#ifdef _WIN32
    string g = findGams();
    if (!g.empty()) {
        ostringstream ps;
        ps << "PATH=" << getenv("PATH") << ";" << dirName;
        _putenv(ps.str().c_str());
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

///
/// \brief GAMSPlatform::runProcess Non-thread safe helper function to quickly get the output of a executable
/// \param where working directory
/// \param what path to executable
/// \param args arguments
/// \param output output
/// \return exit code of called executable
///
int GAMSPlatform::runProcess(const string where, const string what, const string args, string& output)
{
    ostringstream ssp;
    string result;
    FILE* out;

#ifdef _WIN32
    filesystem::path p = filesystem::current_path();

    ssp << "\"" << what << "\"" << " " << args ;
    _chdir(where.c_str()); // for some reason we need to do this on windows
    out = _popen(ssp.str().c_str(), "r");

    _chdir(p.string().c_str()); // change back to old working dir
#else
    ssp << "cd \"" << where << "\" && \"" << what << "\" " << args;
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

thread GAMSPlatform::runProcessParallel(const string executable, const string args) {
    string call = executable;
    if (!args.empty()) call += " " + args;

    thread t(system, call.c_str());
    t.detach();
    return t;
}

} // namespace gams
