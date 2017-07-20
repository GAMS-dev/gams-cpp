/**
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
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

#include <QFileInfo>

namespace gams {

/// Represents the GAMS path to a specific file.
class GAMSPath: public QFileInfo
{
public:
    /// Standard constructor.
    GAMSPath() : QFileInfo() {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param file Path or file.
    GAMSPath(const char *file) : QFileInfo(QString(file)) {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param file Path or file.
    GAMSPath(const std::string &file) : QFileInfo(QString(file.c_str())) {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param file Path or file.
    GAMSPath(const QString &file) : QFileInfo(file) {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param file File.
    GAMSPath(const QFile &file) : QFileInfo(file) {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param dir Path.
    GAMSPath(const QDir & dir, const QString &file) : QFileInfo(dir, file) {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param fileinfo QFileInfo.
    GAMSPath(const QFileInfo &fileinfo) : QFileInfo(fileinfo) {}

    /// Constructs a GAMSPath based on a path or file.
    /// \param gpath Another GAMSPath used as data source.
    GAMSPath(const GAMSPath &gpath) : QFileInfo(gpath.filePath()) {}

    /// Assigns a GAMSPath.
    /// \param other Another GAMSPath used as data source.
    /// \return Returns the assigned GAMSPath (*this).
    GAMSPath& operator=(const GAMSPath &other);

    /// Appends the string to the GAMSPath.
    /// \param append The string to append.
    /// \return Returns the GAMSPath with the string appended.
    GAMSPath &operator <<(const QString &append);

    /// Appends the string to the GAMSPath.
    /// \param append The string to append.
    /// \return Returns the GAMSPath with the string appended.
    GAMSPath &operator +=(const QString &append);

    /// Appends the string to the GAMSPath.
    /// \param append The string to append.
    /// \return Returns the GAMSPath with the string appended.
    GAMSPath operator +(const QString &append);

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
    GAMSPath operator /(const QString &append);

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

    /// Converts a <c>QString</c> to GAMSPath.
    operator QString();

    /// Get the path or file as <c>std::string</c>.
    std::string toStdString();

    /// Get the path or file as character string.
    const char* c_str();

    /// Create a GAMSPath to a temporary directory.
    /// \return Returns a GAMSPath to the temporary directory.
    GAMSPath tempDir(const QString &templatePath = QString());

    /// Create a GAMSPath to a temporary file.
    /// \return Returns a GAMSPath to the temporary file.
    GAMSPath tempFile(const QString &templateName = "XXXXXX.tmp");

    /// Create a GAMSPath to a temporary file.
    /// \return Returns a GAMSPath to the temporary file.
    GAMSPath tempFile(const std::string &templateName = "XXXXXX.tmp");

    /// Create a GAMSPath to a temporary file.
    /// \return Returns a GAMSPath to the temporary file.
    GAMSPath tempFile(const char* templateName = "XXXXXX.tmp");

    /// Add a suffix to the GAMSPath.
    /// \param suffix The suffix to add.
    void setSuffix(const char *suffix);

    /// Add a suffix to the GAMSPath.
    /// \param suffix The suffix to add.
    void setSuffix(const QString &suffix);

    /// Add a suffix to the GAMSPath.
    /// \param suffix The suffix to add.
    void setSuffix(const std::string &suffix);

    /// Add a suffix.
    /// \param suffix The suffix to add.
    /// \return Returns the GAMSPath with suffix.
    GAMSPath suffix(const QString &suffix) const;

    /// Add a suffix.
    /// \param suffix The suffix to add.
    /// \return Returns the GAMSPath with suffix.
    GAMSPath suffix(const std::string &suffix) const;

    /// Add a suffix.
    /// \param suffix The suffix to add.
    /// \return Returns the GAMSPath with suffix.
    GAMSPath suffix(const char *suffix) const;

    /// Get the GAMS path as optimized path.
    /// \return Returns a new GAMSPath object.
    GAMSPath up() const;

    /// Path suffix.
    QString suffix() const;

    /// Get the GAMSPath.
    GAMSPath path() const;

    /// Create the directory.
    bool mkDir();

    /// Removes the direcotry recursively.
    /// \return Returns <c>true</c> if the removal was successfull; otherwise <c>false</c>.
    bool rmDirRecurse();

    /// Optimize the path length, i.e. full path to relative path.
    void pack();

    /// Remmoves the file.
    /// \return Returns <c>true</c> if the removal was successfull; otherwise <c>false</c>.
    bool remove();

    /// Rename the already set file.
    /// \param newFileName The new file name.
    /// \return Returns <c>true</c> if the rename was successfull; otherwise <c>false</c>.
    bool rename(const QString &newFileName);

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
    static bool exists(const QString &file);

    /// Checks if the file exists.
    /// \param file The file name.
    /// \return Returns <c>true</c> if the file exists; otherwise <c>false</c>.
    static bool exists(const std::string &file);

    /// Checks if the file exists.
    /// \param file The file name.
    /// \return Returns <c>true</c> if the file exists; otherwise <c>false</c>.
    static bool exists(const char *file);

private:
    std::string mBuffer;
};

}

#endif // GPATH_H
