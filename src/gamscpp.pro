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
TEMPLATE = lib
DESTDIR = ../bin

TARGET = gamscpp

USE_GAMS_DISTRIB_CPP_API_SRC=false
include(../gamsdependency.pri)

# Do not search GAMS via the Widnows resgistry. This causes
# trouble if multible GAMS versions are used in parallel.
NO_WINDOWS_REGISTRY = $$(NO_WINDOWS_REGISTRY)
equals(NO_WINDOWS_REGISTRY, "1") {
    DEFINES += NO_WINDOWS_REGISTRY
}

CONFIG += skip_target_version_ext cmdline c++1z plugin
CONFIG -= app_bundle

DEFINES += GAMS_CPP_LIB MAKELIB

DEFINES -= UNICODE

#!win32:LIBS += -lstdc++fs
win32:LIBS += -luser32
unix:LIBS += -ldl

unix:!macx{
    # set minimum gcc version
    GCCMAJORVERSION=$$system("gcc -dumpversion")
    message("----- ROGO -----")
    message($$GCCMAJORVERSION)
    lessThan(GCCMAJORVERSION, 8): {
        message("using gcc8")
        QMAKE_CC=gcc-8
        QMAKE_CXX=g++-8
    }
    message("================")
}

# TODO (RG) restructure platform sections
macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}
unix:!macx {
    LIBS += -lstdc++fs
}

win32-g++:QMAKE_CXXFLAGS += -DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS -DHAVE_MUTEX
win32-msvc*:QMAKE_CXXFLAGS += -EHsc -GR -c -nologo -DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS -DHAVE_MUTEX
unix:!macx:QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\',-rpath,\'\$$ORIGIN/../../..\'"


# TODO(RG): we need to find a solution for how to handle generated files... #HEADERS += generated/gamsoptions.h      \
#           generated/gamsoptionsimpl.h
# we dont need headers included, if they are located in one of the includepath folders

SOURCES += generated/gamsoptions.cpp      \
           generated/gamsoptionsimpl.cpp
# ------------------------------------------------------------------------

SOURCES += \
    gamscheckpoint.cpp \
    gamscheckpointimpl.cpp \
    gamsdatabase.cpp \
    gamsdatabaseimpl.cpp \
    gamsenum.cpp \
    gamsequation.cpp \
    gamsequationrecord.cpp \
    gamsexception.cpp \
    gamsjob.cpp \
    gamsjobimpl.cpp \
    gamslib_global.cpp \
    gamslog.cpp \
    gamsmodelinstance.cpp \
    gamsmodelinstanceimpl.cpp \
    gamsmodelinstanceimpl_p.cpp \
    gamsmodelinstanceopt.cpp \
    gamsmodelinstanceoptimpl.cpp \
    gamsmodifier.cpp \
    gamsmodifierimpl.cpp \
    gamsparameter.cpp \
    gamsparameterrecord.cpp \
    gamsplatform.cpp \
    gamsset.cpp \
    gamssetrecord.cpp \
    gamssymbol.cpp \
    gamssymbolimpl.cpp \
    gamssymbolimpl_p.cpp \
    gamssymboliter.cpp \
    gamssymbolrecord.cpp \
    gamsvariable.cpp \
    gamsvariablerecord.cpp \
    gamsversion.cpp \
    gamsworkspace.cpp \
    gamsworkspaceimpl.cpp \
    gamsworkspaceinfo.cpp \
    gamsworkspacepool.cpp \
    solveroptions.cpp \
    gamssymbolrecordimpl.cpp \
    gamssymboldomainviolation.cpp \
    gamssymboldomainviolationimpl.cpp \
    gamsdatabasedomainviolation.cpp \
    gamsdatabasedomainviolationimpl.cpp \
    gamsdomain.cpp \
    gamspath.cpp \
    gamsdatabaseiter.cpp \
    gamsexceptionexecution.cpp \
    gamsdomainimpl.cpp \
    gamsworkspaceinfoimpl.cpp

HEADERS += \
    gams.h \
    gamscheckpoint.h \
    gamscheckpointimpl.h \
    gamsdatabase.h \
    gamsdatabaseimpl.h \
    gamsenum.h \
    gamsequation.h \
    gamsequationrecord.h \
    gamsexception.h \
    gamsjob.h \
    gamsjobimpl.h \
    gamslib_global.h \
    gamslog.h \
    gamsmodelinstance.h \
    gamsmodelinstanceimpl.h \
    gamsmodelinstanceopt.h \
    gamsmodelinstanceoptimpl.h \
    gamsmodifier.h \
    gamsmodifierimpl.h \
    gamsparameter.h \
    gamsparameterrecord.h \
    gamsplatform.h \
    gamsset.h \
    gamssetrecord.h \
    gamssymbol.h \
    gamssymboliter.h \
    gamssymbolimpl.h \
    gamssymbolrecord.h \
    gamsvariable.h \
    gamsvariablerecord.h \
    gamsversion.h \
    gamsworkspace.h \
    gamsworkspaceimpl.h \
    gamsworkspaceinfo.h \
    gamsworkspacepool.h \
    solveroptions.h \
    gamssymbolrecordimpl.h \
    gamssymboldomainviolation.h \
    gamssymboldomainviolationimpl.h \
    gamsdatabasedomainviolation.h \
    gamsdatabasedomainviolationimpl.h \
    gamsdomain.h \
    gamspath.h \
    gamsdatabaseiter.h \
    gamsexceptionexecution.h \
    gamsdomainimpl.h \
    gamsworkspaceinfoimpl.h

OTHER_FILES +=          \
    gamscpp.qmodel      \
    coreclasses.qmodel  \
    ../CHANGELOG        \
    ../jenkinsfile      \
    ../version
