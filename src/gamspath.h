/**
 * GAMS C++ API
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

#ifndef GAMSPATH_H
#define GAMSPATH_H

#include <filesystem>

namespace gams {

/// Represents the GAMS path to a specific file.
class GAMSPath : public std::filesystem::path
{
public:
    /// Standard constructor.
    GAMSPath() : std::filesystem::path() {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param file Path or file.
    GAMSPath(const char *file) : std::filesystem::path(file) {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param file Path or file.
    GAMSPath(const std::string &file) : std::filesystem::path(file) {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param file File.
    GAMSPath(const std::filesystem::path &file) : std::filesystem::path(file) {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param dir Path.
    /// \param file File.
    GAMSPath(const std::filesystem::path & dir, const std::string &file) : std::filesystem::path(dir / file){ }

    /// Constructs a GAMSPath based on a path or file.
    /// \param gpath Another GAMSPath used as data source.
    GAMSPath(const GAMSPath &gpath) : std::filesystem::path(gpath.string()) {}

    /// Assigns a GAMSPath.
    /// \param other Another GAMSPath used as data source.
    /// \return Returns the assigned GAMSPath (*this).
    GAMSPath &operator=(const GAMSPath &other);

    /// Appends the string to the GAMSPath.
    /// \param append The string to append.
    /// \return Returns the GAMSPath with the string appended.
    GAMSPath &operator <<(const std::string &append);

    /// Appends the string to the GAMSPath.
    /// \param append The string to append.
    /// \return Returns the GAMSPath with the string appended.
    GAMSPath &operator +=(const std::string &append);

    /// Appends the string to the GAMSPath.
    /// \param append The string to append.
    /// \return Returns the GAMSPath with the string appended.
    GAMSPath operator +(const std::string &append);

    /// Appends the string to the GAMSPath.
    /// \param append The string to append.
    /// \return Returns the GAMSPath with the string appended.
    GAMSPath operator +(const char *append);

    /// Appends a path to the GAMSPath.
    /// \param append The path to append.
    /// \return Returns the GAMSPath with the path appended.
    GAMSPath operator /(const GAMSPath &append);

    /// Appends a path to the GAMSPath.
    /// \param append The path to append.
    /// \return Returns the GAMSPath with the path appended.
    GAMSPath operator /(const std::string &append);

    /// Appends a path to the GAMSPath.
    /// \param append The path to append.
    /// \return Returns the GAMSPath with the path appended.
    GAMSPath operator /(const char *append);

    /// Converts a <c>std::string</c> to GAMSPath.
    operator std::string();

    /// Get the path or file as <c>std::string</c>.
    std::string toStdString() const;

    /// Get the path or file as character string.
    /// Warning: This function may behave differently than path::c_str()
    const char* c_str() const;

    /// Create a GAMSPath to a temporary directory.
    /// \param tempPath Template temporary directory.
    /// \return Returns a GAMSPath to the temporary directory.
    GAMSPath tempDir(const std::string &tempPath = std::string());

    /// Create a GAMSPath to a temporary file.
    /// \param tempName Template for temporary file name.
    /// \return Returns a GAMSPath to the temporary file.
    GAMSPath tempFile(const std::string &tempName = "XXXXXX.tmp");

    /// Add a suffix to the GAMSPath.
    /// \param suffix The suffix to add.
    void setSuffix(const char *suffix);

    /// Add a suffix to the GAMSPath.
    /// \param suffix The suffix to add.
    void setSuffix(const std::string &suffix);

    /// Constructs a new file path using the current file's basename and adding the given suffix to it.
    /// \param suffix The suffix of the file to get.
    /// \return Returns a GAMSPath to a new file of the same name but with a replaced suffix.
    GAMSPath suffix(const std::string &suffix) const;

    /// Constructs a new file path using the current file's basename and adding the given suffix to it.
    /// \param suffix The suffix of the file to get.
    /// \return Returns a GAMSPath to a new file of the same name but with a replaced suffix.
    GAMSPath suffix(const char *suffix) const;

    /// Get the GAMS path as optimized path.
    /// \return Returns a new GAMSPath object.
    GAMSPath up() const;

    /// Path suffix.
    std::string suffix() const;

    /// Get the GAMSPath.
    GAMSPath path() const;

    /// Create the directory.
    bool mkDir() const;

    /// Removes the direcotry recursively.
    /// \return Returns <c>true</c> if the removal was successfull; otherwise <c>false</c>.
    bool rmDirRecurse();

    /// Normalizes the Path to make it comparable by converting it to it's shortest absolute representation.
    void pack();

    /// Removes the file.
    /// \return Returns <c>true</c> if the removal was successfull; otherwise <c>false</c>.
    bool remove();

    /// Rename the already set file.
    /// \param newFileName The new file name.
    /// \return Returns <c>true</c> if the rename was successfull; otherwise <c>false</c>.
    bool rename(const std::string &newFileName);

    /// Rename the already set file.
    /// \param newFileName The new file name.
    /// \return Returns <c>true</c> if the rename was successfull; otherwise <c>false</c>.
    bool rename(const char *newFileName);

    /// Checks if the path exists.
    /// \return Returns <c>true</c> if the path exists; otherwise <c>false</c>.
    bool exists() const;

    /// Checks if the file exists.
    /// \param file The file name.
    /// \return Returns <c>true</c> if the file exists; otherwise <c>false</c>.
    static bool exists(const std::string &file);

    /// Checks if the file exists.
    /// \param file The file name.
    /// \return Returns <c>true</c> if the file exists; otherwise <c>false</c>.
    static bool exists(const char *file);

};

#ifdef _WIN32
static thread_local bool seedGenerated;
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static thread_local bool seedGenerated;
#pragma GCC diagnostic pop
#endif

}

#endif // GPATH_H
