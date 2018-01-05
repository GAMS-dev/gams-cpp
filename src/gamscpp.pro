TEMPLATE = lib
DESTDIR = ../bin

TARGET = gamscpp

include(../dependency.pri)
include(gamscpp.pri)

CONFIG += skip_target_version_ext console c++11 plugin
CONFIG -= app_bundle

DEFINES += GAMS_CPP_LIB MAKELIB

QT -= gui

win32-g++:QMAKE_CXXFLAGS += -DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS -D_CRT_SECURE_NO_WARNINGS -DHAVE_MUTEX
win32-msvc*:QMAKE_CXXFLAGS += -EHsc -GR -c -nologo -DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS -D_CRT_SECURE_NO_WARNINGS -DHAVE_MUTEX

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
    gamsoptions.cpp \
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
    gamsworkspaceinfoimpl.cpp \
    gamsoptionsimpl.cpp


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
    gamsoptions.h \
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
    gamsworkspaceinfoimpl.h \
    gamsoptionsimpl.h


OTHER_FILES += gamscpp.qmodel coreclasses.qmodel
