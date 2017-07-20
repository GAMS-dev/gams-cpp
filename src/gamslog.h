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

#ifndef GAMSLOG_H
#define GAMSLOG_H

#include <sstream>
#include <QTextStream>
#include <QHash>
#include <QSet>
#include "gamsenum.h"
#include "gamslib_global.h"

namespace gams {

/// The class to manage all loggers used in an API instance.
class LoggerPool
{
    struct TargetSet {
        TargetSet() : mDebug(GAMSEnum::DebugLevel::Verbose) {}
        TargetSet(const GAMSEnum::DebugLevel debug, FILE *target = 0) : mDebug(debug) {
            mTargets << (target ? target : stdout);
        }
        GAMSEnum::DebugLevel mDebug;
        QSet<FILE*> mTargets;
    };

public:

    /// Get the one and only LoggerPool instance.
    /// \return Returns the LoggerPool instance.
    static LoggerPool &instance();

    /// Register a logger.
    /// \param logId A GAMS logger ID.
    /// \param debug A GAMS debug level.
    /// \param target The logger ouput file.
    void registerLogger(const LogId logId, const GAMSEnum::DebugLevel debug, FILE *target = stdout);

    /// Unregister a logger.
    /// \param logId A logger ID used for unregistration.
    void unregisterLogger(const LogId logId);

    /// Get all logging targets for a specifc logger.
    /// \param logId A GAMS logger ID.
    /// \param debug A GAMS debug level.
    /// \return Returns all file targets.
    QSet<FILE*> targets(const LogId logId, const GAMSEnum::DebugLevel debug) const {
        if (!mBinds.contains(logId))
            return QSet<FILE*>();
        TargetSet lc = mBinds.value(logId);
        if (lc.mDebug < debug)
            return QSet<FILE*>();
        return lc.mTargets;
    }

    /// Get the GAMS debug level for a specific logger.
    /// \param logId A GAMS logger ID.
    /// \return Returns the GAMS debug level of a certain logger.
    GAMSEnum::DebugLevel debug(const LogId logId) const {
        return mBinds.value(logId, TargetSet()).mDebug;
    }

private:
    static LoggerPool *mInstance;
    QHash<LogId, TargetSet> mBinds;
    LoggerPool() {}
    LoggerPool(LoggerPool const&) {}
    void operator=(LoggerPool const&) {}
};

/// Logging class used to get feedback about the API actions.
class Logger
{
public:

    /// Constructor.
    /// \param logID A GAMS log ID.
    /// \param debug A GAMS debug level.
    /// \param where Location of the log file.
    Logger(const LogId logID, const GAMSEnum::DebugLevel debug, const char* where)
        : mBufferStream(&mBuffer), mWhere(where), mTargets(LoggerPool::instance().targets(logID, debug))
    {}

    /// Destructor.
    ~Logger();

    /// Inserts data to log.
    /// \return Returns the logger reference (*this).
    Logger& operator <<(std::ostream& (*os)(std::ostream&)) {
        std::stringstream strStream;
        strStream << os;
        mBufferStream << strStream.str().c_str();
        return *this;
    }

    /// Inserts data to log.
    /// \return Returns the logger reference (*this).
    Logger& operator <<(const std::string& value) {
        mBufferStream << value.c_str();
        return *this;
    }

    /// Inserts data to log.
    /// \return Returns the logger reference (*this).
    template<typename T>
    Logger& operator <<(const T& value) {
        mBufferStream << value;
        return *this;
    }

private:
    QString mBuffer;
    QTextStream mBufferStream;
    QString mWhere;
    QSet<FILE*> mTargets;
};

}

#define DEB_S(logID) ::gams::Logger(logID, ::gams::GAMSEnum::DebugLevel::Verbose, __FUNCTION__)
#define DEB ::gams::Logger(logID(), ::gams::GAMSEnum::DebugLevel::Verbose, __FUNCTION__)
#define MSG ::gams::Logger(logID(), ::gams::GAMSEnum::DebugLevel::ShowLog, __FUNCTION__)
//#define MSG(ws) ::gams::Logger(ws, ::gams::GAMSEnum::DebugLevel::ShowLog, __FUNCTION__)

#endif // GAMSLOG_H
