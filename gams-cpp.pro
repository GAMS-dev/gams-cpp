TEMPLATE=subdirs

!exists($$PWD/gamsinclude.pri) {
    isEmpty($$(GAMS_CORE_PATH)) {
        unix:GAMSINC = GAMS_DISTRIB=$$(HOME)/gams/gams24.9_linux_x64_64_sfx/apifiles/C/api
        win32:GAMSINC = GAMS_DISTRIB=C:\GAMS\win64\24.9\apifiles\C\api
        write_file($$PWD/gamsinclude.pri,GAMSINC)
    } else {
        unix:GAMSINC = GAMS_DISTRIB=$$(GAMS_CORE_PATH)
        win32:GAMSINC = GAMS_DISTRIB=$$(GAMS_CORE_PATH)
        write_file($$PWD/gamsinclude.pri,GAMSINC)
    }
}

SUBDIRS += gamscpp
SUBDIRS += examples
SUBDIRS += perf
SUBDIRS += tests

gamscpp.file = src/gamscpp.pro
examples.depends = gamscpp
tests.depends = gamscpp
perf.depends = gamscpp
