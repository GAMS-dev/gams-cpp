/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
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
#include <QStringList>
#include <QSettings>
#include <QProcess>
#include <string>
#include <iostream>
#include <sstream>
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
const char *cLibEnv = "";                   // JM: unused until now
const char *cPathSep = "\\";
const char *cCmpFileName = "gmscmpnt.txt";
const int cExitCodeDiv = 1;
const char *cExeSuffix = ".exe";
const char *cLibPrefix = "";
const char *cLibSuffix = ".dll";
#endif

std::string GAMSPlatform::findGams(LogId logId)
{
#ifdef __linux__
    return findGamsOnLinux(logId);
#elif __APPLE__
    return findGamsOnApple(logId);
#elif __unix__
    return findGamsOnLinux(logId);
#else
    return findGamsOnWindows(logId);
#endif
}

void GAMSPlatform::ensureEnvPathContains(const char *dirName)
{
#ifdef __linux__
    ensureEnvPathSetOnLinux(dirName);
#elif __APPLE__
    ensureEnvPathSetOnApple(dirName);
#elif __unix__
    ensureEnvPathSetOnLinux(dirName);
#else
    ensureEnvPathSetOnWindows(dirName);
#endif
}

bool GAMSPlatform::interrupt(long pid)
{
#ifdef __linux__
    return interruptOnNonWindows(pid);
#elif __APPLE__
    return interruptOnNonWindows(pid);
#elif __unix__
    return interruptOnNonWindows(pid);
#else
    return interruptOnWindows(pid);
#endif
}

std::string GAMSPlatform::findGamsOnLinux(LogId logId)
{
    QString gamsDir;
    QString gamsDirLD;

    QStringList envSplit = QString(qgetenv("PATH")).split(cEnvSep);
    for (QString envStr: envSplit) {
        if ((GAMSPath(envStr) / "gams").exists()) {
            gamsDir = envStr;
            break;
        }
    }
    if (!gamsDir.isEmpty() && LoggerPool::instance().debug(logId) == GAMSEnum::DebugLevel::Off)
        return gamsDir.toStdString();

    envSplit = QString(qgetenv(cLibEnv)).split(cEnvSep);
    for (QString envStr: envSplit) {
        if ((GAMSPath(envStr) / "gams").exists()) {
            gamsDirLD = envStr;
            break;
        }
    }
    if (gamsDir.isEmpty())
        return gamsDirLD.toStdString();
    else if (!gamsDirLD.isEmpty() && gamsDir != gamsDirLD) {
        DEB_S(logId) << "--- Warning: Found GAMS system directory " << gamsDir << " in PATH and a different one" << endl
                     << "---          in (DY)LD_LIBRARY_PATH (" << gamsDirLD << "). The latter is ignored.";
    }

    return gamsDir.toStdString();

}

void GAMSPlatform::ensureEnvPathSetOnLinux(const char *dirName)
{
    Q_UNUSED(dirName)
}

bool GAMSPlatform::interruptOnNonWindows(long pid)
{
    QProcess proc;
    proc.setProgram("/bin/bash");

    //start "kill" with List of children PID
    proc.setProgram("/bin/bash");
    QStringList s2 { "-c", "kill -2 " + QString::number(pid)};
    proc.setArguments(s2);
    proc.start();

    proc.waitForFinished(-1);
    return true;
}

std::string GAMSPlatform::findGamsOnApple(LogId logId)
{
    QString gamsDir;
    QString gamsDirLD;

    QStringList envSplit = QString(qgetenv("PATH")).split(cEnvSep);
    for (QString envStr: envSplit) {
        if ((GAMSPath(envStr) / "gams").exists()) {
            gamsDir = envStr;
            break;
        }
    }
    if (!gamsDir.isEmpty() && LoggerPool::instance().debug(logId) == GAMSEnum::DebugLevel::Off)
        return gamsDir.toStdString();

    envSplit = QString(qgetenv(cLibEnv)).split(cEnvSep);
    for (QString envStr: envSplit) {
        if ((GAMSPath(envStr) / "gams").exists()) {
            gamsDirLD = envStr;
            break;
        }
    }
    if (gamsDir.isEmpty())
        return gamsDirLD.toStdString();
    else if (!gamsDirLD.isEmpty() && gamsDir != gamsDirLD) {
        DEB_S(logId) << "--- Warning: Found GAMS system directory " << gamsDir << " in PATH and a different one" << endl
                     << "---          in (DY)LD_LIBRARY_PATH (" << gamsDirLD << "). The latter is ignored.";
    }

    return gamsDir.toStdString();

}

void GAMSPlatform::ensureEnvPathSetOnApple(const char *dirName)
{
    Q_UNUSED(dirName)
}

std::string GAMSPlatform::findGamsOnWindows(LogId logId)
{
    QString firstFound("");
    QStringList locations;
    locations << "HKEY_CURRENT_USER\\" << "HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\";
    for (QString loc: locations) {
        QString gamsPath = QSettings(loc + "gams.location", QSettings::NativeFormat).value(".").toString();
        if (!gamsPath.isEmpty()) {

            if (LoggerPool::instance().debug(logId) == GAMSEnum::DebugLevel::Off) {
                return gamsPath.toStdString();
            } else if (firstFound.isNull()) {
                firstFound = gamsPath;
            } else {
                // compare second key
                if (firstFound.isEmpty()) {
                    return gamsPath.toStdString();
                } else if (!gamsPath.isEmpty() && (gamsPath != firstFound)) {
                    DEB_S(logId) << "--- Warning: Found GAMS system directory " << firstFound.toStdString() << " at HKEY_CURRENT_USER and a different one\n"
                                 << "---          in HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\ (" << gamsPath.toStdString() << "). The latter is ignored.";
                }
                return firstFound.toStdString();
            }
        }
    }
    return firstFound.toStdString();
}

void GAMSPlatform::ensureEnvPathSetOnWindows(const char *dirName)
{
    QByteArray envPath = qgetenv("PATH");
    if (!envPath.contains(dirName)) {
        qputenv("PATH", envPath + ";" + dirName);
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
