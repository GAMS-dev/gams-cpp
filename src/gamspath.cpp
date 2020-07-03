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
#include <fstream>
#include <algorithm>

namespace gams {

GAMSPath&GAMSPath::operator=(const GAMSPath& other)
{
    assign(other.string());
    return *this;
}

GAMSPath& GAMSPath::operator<<(const std::string &append)
{
    this->append(append);
    return *this;
}

GAMSPath &GAMSPath::operator+=(const std::string &append)
{
    this->append(append);
    return *this;
}

GAMSPath GAMSPath::operator +(const std::string &append)
{
    this->append(append);
    return *this;
}

GAMSPath GAMSPath::operator +(const char *append)
{
    this->append(append);
    return *this;
}

GAMSPath GAMSPath::operator /(const GAMSPath &append)
{
    return this->append(append.string());
}

GAMSPath GAMSPath::operator /(const std::string &append)
{
    return this->append(append);
}

GAMSPath GAMSPath::operator /(const char *append)
{
    return this->append(append);
}

// TODO(RG): i dont know what this does
//GAMSPath::operator QString()
//{
//    return filePath();
//}

// TODO(RG): maybe we dont need this function (and similar ones)
//void GAMSPath::setSuffix(const char *suffix)
//{
//    setSuffix(std::string(suffix));
//}

void GAMSPath::setSuffix(const std::string &suffix)
{
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
    replace_extension(suffix);
}

void GAMSPath::setSuffix(const std::string suffix)
{
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
    replace_extension(suffix);
}

GAMSPath GAMSPath::suffix(const std::string &suffix) const
{
    GAMSPath p(*this);
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
    return p.replace_extension(suffix);
}

GAMSPath GAMSPath::suffix(const char *suffix) const
{
    GAMSPath p(*this);
    return p.replace_extension(suffix);
}

GAMSPath GAMSPath::up() const
{
    GAMSPath p(*this);
    p.pack();
    return GAMSPath(path());
}

std::string GAMSPath::suffix() const
{
    return extension().string();
}

GAMSPath GAMSPath::path() const
{
    if (has_filename())
        return parent_path();
    else
        return path();
}

bool GAMSPath::mkDir()
{
    return create_directory(*this);
}

bool GAMSPath::rmDirRecurse()
{
    if (has_filename()) return std::filesystem::exists(*this);
    return std::filesystem::remove_all(*this);
}

void GAMSPath::pack()
{
    // TODO(RG): check with other APIs what is expected to do here
    if (exists())
        this->assign(std::filesystem::canonical(*this));
    else
        this->assign(std::filesystem::absolute(*this));

//    QString path = filePath();
//    QRegularExpression xp("/[^/]*/\\.\\.");
//    while (xp.match(path).hasMatch()) {
//        path = path.remove(xp);
//    }
//    setFile(path);
//    setFile(absoluteFilePath());
}

bool GAMSPath::remove()
{
    return std::filesystem::remove(*this);
}

bool GAMSPath::rename(const std::string &newFileName)
{
    std::error_code err;
    std::filesystem::rename(this->string(), newFileName, err);
    if (err.value() == 0)
        assign(newFileName);
    return err.value();
}

bool GAMSPath::rename(const char *newFileName)
{
    std::error_code err;
    std::filesystem::rename(this->string(), newFileName, err);
    if (err.value() == 0)
        assign(newFileName);
    return err.value();
}

bool GAMSPath::exists() const
{
    return std::filesystem::exists(*this);
}

bool GAMSPath::exists(const std::string &file)
{
    return std::filesystem::exists(file);
}

bool GAMSPath::exists(const char *file)
{
    return std::filesystem::exists(file);
}

std::string GAMSPath::toStdString()
{
    return string();
}

const char *GAMSPath::c_str()
{
    return toStdString().c_str();
}

GAMSPath GAMSPath::tempDir(const std::string tempPath)
{
    GAMSPath baseLocation = tempPath.empty() ? this->path().string() : tempPath;
    if (!baseLocation.exists()) baseLocation.mkDir();

    GAMSPath tempDir(baseLocation / "gams-cpp");

    if (!std::filesystem::is_directory(tempDir))
        throw GAMSException("Could not create temporary directory in " + tempDir.string());
    return tempDir.path();
}

GAMSPath GAMSPath::tempFile(const std::string &tempName)
{
    std::ofstream of;
    of.open(tempName, std::ofstream::out);
    if (!of.is_open())
        throw GAMSException("Could not create temporary file in " + tempName);
    of.close();
    return GAMSPath(tempName);
}

// TODO(RG): do we need this function?
GAMSPath GAMSPath::tempFile(const char* templateName)
{
    return tempFile(templateName);
}


}
