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

#include "gamspath.h"
#include "gamsexception.h"

using namespace std;
namespace gams {

GAMSPath&GAMSPath::operator=(const GAMSPath& other)
{
    mBuffer = other.mBuffer;
    return *this;
}

GAMSPath& GAMSPath::operator<<(const string &append)
{
    setFile(filePath() + append);
    return *this;
}

GAMSPath &GAMSPath::operator+=(const string &append)
{
    setFile(filePath() + append);
    return *this;
}

GAMSPath GAMSPath::operator +(const std::string &append)
{
    return GAMSPath(this->filePath() + QString::fromStdString(append));
}

GAMSPath GAMSPath::operator +(const char *append)
{
    return GAMSPath(this->filePath() + append);
}

GAMSPath GAMSPath::operator /(const GAMSPath &append)
{
    return GAMSPath(this->filePath() + "/" + append.filePath());
}

GAMSPath GAMSPath::operator /(const QString &append)
{
    return GAMSPath(this->filePath() + "/" + append);
}

GAMSPath GAMSPath::operator /(const std::string &append)
{
    return GAMSPath(this->filePath() + "/" + QString::fromStdString(append));
}

GAMSPath GAMSPath::operator /(const char *append)
{
    return GAMSPath(this->filePath() + "/" + QString(append));
}

GAMSPath::operator std::string()
{
    return QDir::toNativeSeparators(filePath()).toStdString();
}

GAMSPath::operator QString()
{
    return filePath();
}

void GAMSPath::setSuffix(const char *suffix)
{
    setSuffix(QString(suffix));
}

void GAMSPath::setSuffix(const QString &suffix)
{
    int j = filePath().lastIndexOf("/");
    int i = filePath().lastIndexOf(".");
    if (i <= j) i = filePath().length();
    setFile(filePath().left(i) + suffix);
}

void GAMSPath::setSuffix(const std::string &suffix)
{
    setSuffix(QString::fromStdString(suffix));
}

GAMSPath GAMSPath::suffix(const QString &suffix) const
{
    GAMSPath p(*this);
    p.setSuffix(suffix);
    return p;
}

GAMSPath GAMSPath::suffix(const std::string &suffix) const
{
    GAMSPath p(*this);
    p.setSuffix(suffix);
    return p;
}

GAMSPath GAMSPath::suffix(const char *suffix) const
{
    GAMSPath p(*this);
    p.setSuffix(suffix);
    return p;
}

GAMSPath GAMSPath::up() const
{
    GAMSPath p(*this);
    p.pack();
    return GAMSPath(path());
}

QString GAMSPath::suffix() const
{
    QString name = fileName();
    int i = name.lastIndexOf(".");
    if (i < 0) return "";
    return name.right(name.length() - i);
}

GAMSPath GAMSPath::path() const
{
    return GAMSPath(QFileInfo::path());
}

bool GAMSPath::mkDir()
{
    if (!isDir()) {
        QDir dir(*this);
        dir.mkpath(*this);
    }
    return isDir();
}

bool GAMSPath::rmDirRecurse()
{
    if (!isDir()) return !QFileInfo::exists();
    QDir dir(*this);
    return dir.removeRecursively();
}

void GAMSPath::pack()
{
    QString path = filePath();
    QRegularExpression xp("/[^/]*/\\.\\.");
    while (xp.match(path).hasMatch()) {
        path = path.remove(xp);
    }
    setFile(path);
    setFile(absoluteFilePath());
}

bool GAMSPath::remove()
{
    QFile f(filePath());
    return f.remove();
}

bool GAMSPath::rename(const QString &newFileName)
{
    QFile f(filePath());
    bool ok = f.rename(newFileName);
    if (ok) setFile(newFileName);
    return ok;
}

bool GAMSPath::rename(const std::string &newFileName)
{
    return rename(QString::fromStdString(newFileName));
}

bool GAMSPath::rename(const char *newFileName)
{
    return rename(QString(newFileName));
}

bool GAMSPath::exists() const
{
    return (!filePath().isEmpty() && QFileInfo::exists());
}

bool GAMSPath::exists(const QString &file)
{
    return (!file.isEmpty() && QFileInfo::exists(file));
}

bool GAMSPath::exists(const std::string &file)
{
    return exists(QString::fromStdString(file));
}

bool GAMSPath::exists(const char *file)
{
    return exists(QString::fromLatin1(file));
}

std::string GAMSPath::toStdString()
{
    return QDir::toNativeSeparators(this->filePath()).toStdString();
}

const char *GAMSPath::c_str()
{
    mBuffer = *this;
    return mBuffer.c_str();
}

static void initSeed()
{
    // generate seed with timestamp and random number
    qsrand(static_cast<uint>(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() + qrand()));
}

GAMSPath GAMSPath::tempDir(const QString &templatePath)
{
    initSeed();
    QString mask = templatePath.isEmpty() ? absoluteFilePath() : templatePath;
    if (!QDir(mask).exists()) QDir(mask).mkpath(mask);
    QTemporaryDir temp(mask + "/gams-cpp");
    temp.setAutoRemove(false);
    if (!temp.isValid())
        throw GAMSException("Could not create temporary directory in " + templatePath.toStdString());
    return GAMSPath(temp.path());
}

GAMSPath GAMSPath::tempFile(const QString &templateName)
{
    initSeed();
    QTemporaryFile temp(*this / templateName);
    temp.setAutoRemove(false);
    if (!temp.open())
        throw GAMSException("Could not create temporary file in " + templateName.toStdString());
    temp.close();
    return GAMSPath(temp.fileName());
}

GAMSPath GAMSPath::tempFile(const std::string &templateName)
{
    return tempFile(QString::fromStdString(templateName));
}

GAMSPath GAMSPath::tempFile(const char* templateName)
{
    return tempFile(QString::fromLatin1(templateName));
}


}
