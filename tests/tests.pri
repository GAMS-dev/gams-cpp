#
# GAMS - General Algebraic Modeling System C++ API
#
# Copyright (c) 2017-2020 GAMS Software GmbH <support@gams.com>
# Copyright (c) 2017-2020 GAMS Development Corp. <support@gams.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += test
DESTDIR = ../bin

QT += testlib
QT -= gui

include(../gamsdependency.pri)
win32:{
    LIBS += -luser32
    LIBS += -L../../bin/ -lgamscpp
    SOURCES += $$PWD/../src/gamspath.cpp # TODO(RG): this needs better handling
}

unix:{
    LIBS += -ldl -L../../bin -lgamscpp
    !macx{
        LIBS += -lstdc++fs
        # set minimum gcc version
        GCCMAJORVERSION=$$system("gcc -dumpversion")
        lessThan(GCCMAJORVERSION, 9): {
            QMAKE_CC=gcc-9
            QMAKE_CXX=g++-9
        }
    }
}
macx:QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

TESTROOT = $$_PRO_FILE_PWD_/..

INCLUDEPATH += $$TESTROOT $$TESTROOT/../src/

# GoogleTest:
GOOGLETEST_DIR = $$_PRO_FILE_PWD_/../../googletest

isEmpty(GOOGLETEST_DIR):GOOGLETEST_DIR=$$(GOOGLETEST_DIR)
isEmpty(GOOGLETEST_DIR) {
    GOOGLETEST_DIR =
    !isEmpty(GOOGLETEST_DIR) {
        warning("Using googletest src dir specified at Qt Creator wizard")
        message("set GOOGLETEST_DIR as environment variable or qmake variable to get rid of this message")
    }
}

!isEmpty(GOOGLETEST_DIR): {
    GTEST_SRCDIR = $$GOOGLETEST_DIR/googletest
    GMOCK_SRCDIR = $$GOOGLETEST_DIR/googlemock
} else: unix {
    exists(/usr/src/gtest):GTEST_SRCDIR=/usr/src/gtest
    exists(/usr/src/gmock):GMOCK_SRCDIR=/usr/src/gmock
    !isEmpty(GTEST_SRCDIR): message("Using gtest from system")
}

requires(exists($$GTEST_SRCDIR):exists($$GMOCK_SRCDIR))

!isEmpty(GTEST_SRCDIR) {
    INCLUDEPATH *= \
        $$GTEST_SRCDIR \
        $$GTEST_SRCDIR/include

    SOURCES += \
        $$GTEST_SRCDIR/src/gtest-all.cc
}

# Mocks not used for now
#!isEmpty(GMOCK_SRCDIR) {
#    INCLUDEPATH *= \
#        $$GMOCK_SRCDIR \
#        $$GMOCK_SRCDIR/include

#    SOURCES += \
#        $$GMOCK_SRCDIR/src/gmock-all.cc
#}
