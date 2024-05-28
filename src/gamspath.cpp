/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2024 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2024 GAMS Development Corp. <support@gams.com>
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
#include <iostream>
#include <random>

#ifdef _WIN32
#include <wchar.h>
#endif

namespace gams {

GAMSPath& GAMSPath::operator=(const GAMSPath& other)
{
    assign(other.string());
    return *this;
}

GAMSPath& GAMSPath::operator<<(const std::string &append)
{
    this->append(append);
    return *this;
}

GAMSPath& GAMSPath::operator+=(const std::string &append)
{
    this->append(append);
    return *this;
}

GAMSPath GAMSPath::operator +(const std::string &append)
{
    GAMSPath res(*this);
    return res.append(append);
}

GAMSPath GAMSPath::operator +(const char *append)
{
    GAMSPath res(*this);
    return res.append(append);
}

GAMSPath GAMSPath::operator /(const GAMSPath &append)
{
    GAMSPath res(*this);
    return res.append(append.string());
}

GAMSPath GAMSPath::operator /(const std::string &append)
{
    GAMSPath res(*this);
    return res.append(append);
}

GAMSPath GAMSPath::operator /(const char *append)
{
    GAMSPath res(*this);
    return res.append(append);
}

GAMSPath::operator std::string()
{
    return string();
}

void GAMSPath::setSuffix(const char *suffix)
{
    setSuffix(std::string(suffix));
}

void GAMSPath::setSuffix(const std::string &suffix)
{
    std::string s = suffix;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    replace_extension(s);
}

GAMSPath GAMSPath::suffix(const std::string &suffix) const
{
    std::string s = suffix;
    GAMSPath p(*this);
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return p.replace_extension(s);
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
        return GAMSPath(string());
}

bool GAMSPath::mkDir() const
{
    return create_directory(*this);
}

bool GAMSPath::rmDirRecurse()
{
    bool result;
    std::error_code ec;
    if (std::filesystem::is_regular_file(*this))
        result = std::filesystem::exists(*this, ec);
    else
        result = std::filesystem::remove_all(*this, ec);
    if (ec) std::cerr << "GAMSPath::rmDirRecurse error: " << ec.message().c_str()
                      << "\n  For: " << c_str() << std::endl;
    return result;
}

void GAMSPath::pack()
{
    this->assign(std::filesystem::absolute(*this));
}

bool GAMSPath::remove()
{
    std::error_code ec;
    bool result = std::filesystem::remove_all(*this, ec);
    if (ec)
        std::cerr << "GAMSPath::remove error: " << ec.message().c_str()
                  << "\n  For: " << c_str() << std::endl;
    return result;
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

std::string GAMSPath::toStdString() const
{
    return string();
}

const char* GAMSPath::c_str() const
{
#ifdef _WIN32
    const wchar_t *w = native().c_str();
    size_t origsize = wcslen(w) + 1;
    char *nstring = new char[origsize * 2];
    wcstombs_s(0, nstring, origsize * 2, w, _TRUNCATE);

    return nstring;
#else
    return native().c_str();
#endif
}

// TODO(RG): these tempdir/tempfile functions create neither in the OS' tmp folder, nor is there any automatic cleanup. so why are they called temp?
GAMSPath GAMSPath::tempDir(const std::string &tempPath)
{
    if (!seedGenerated) {
        std::random_device rd;
        std::srand(rd());
        seedGenerated = true;
    }

    GAMSPath baseLocation = tempPath.empty() ? path().string() : tempPath;
    if (!baseLocation.exists()) baseLocation.mkDir();

    std::string folderName("gams-cpp");
    folderName += "-" + std::to_string(std::rand());
    GAMSPath tempDir(baseLocation + folderName);
    tempDir.mkDir();

    if (!std::filesystem::is_directory(tempDir))
        throw GAMSException("Could not create temporary directory in " + tempDir.string());
    return tempDir;
}

GAMSPath GAMSPath::tempFile(const std::string &tempName)
{
    if (!seedGenerated) {
        std::random_device rd;
        std::srand(rd());
        seedGenerated = true;
    }

    GAMSPath tmpFile(tempName);
    std::filesystem::path ext = tmpFile.extension();
    tmpFile.replace_filename(tmpFile.stem().string() + "_" + std::to_string(std::rand()));
    tmpFile.replace_extension(ext);

    std::ofstream of;
    of.open(tmpFile.string(), std::ofstream::out);
    if (!of.is_open())
        throw GAMSException("Could not create temporary file " + tmpFile.string());
    of.close();
    return tmpFile;
}

}
