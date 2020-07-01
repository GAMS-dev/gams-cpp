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
#endif

#include "gamsplatform.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
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
        if ((GAMSPath(s) / "gams").exists()) {
            gamsDir = s;
            break;
        }
    }
    if (!gamsDir.empty() && LoggerPool::instance().debug(logId) == GAMSEnum::DebugLevel::Off)
        return gamsDir;

    s.clear();
    ss.str(getenv(cLibEnv));
    while (getline(ss, s, cEnvSep)) {
        if ((GAMSPath(s) / "gams").exists()) {
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
    Q_UNUSED(dirName)
}

bool GAMSPlatform::interruptOnNonWindows(long pid)
{
    char* proc;
    sprintf(proc, "/bin/bash -c -kill -2 %ld", pid);

    system(proc);
    return true;
}

string GAMSPlatform::findGamsOnWindows(LogId logId)
{
//#ifdef NO_WINDOWS_REGISTRY
    Q_UNUSED(logId)
    string s;
    stringstream ss(getenv("PATH"));
    while (getline(ss, s, cEnvSep)) {
        if ((GAMSPath(s) / "gams").exists()) {
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
    string g = findGams();
    if (!g.empty()) {
        char* p;
        sprintf(p, "PATH=%s;%s", getenv("PATH"), dirName);
        // TODO(RG): check string here
        _putenv(p);
    }
}
bool GAMSPlatform::interruptOnWindows(long pid)
{
#ifdef _WIN32
    COPYDATASTRUCT cds;

    string stem = "___GAMSMSGWINDOW___";
    string pidStr = QString::number(pid).toUtf8().constData();
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
    Q_UNUSED(pid)
    throw GAMSException("interruptOnWindows only impemented on Windows");
#endif
}

} // namespace gams
