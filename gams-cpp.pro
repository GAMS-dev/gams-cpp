TEMPLATE=subdirs

GAMSROOT=$$PWD
DEFINES += GAMSTEST

SUBDIRS += gamscpp
SUBDIRS += examples
SUBDIRS += perf
SUBDIRS += tests

gamscpp.file = src/gamscpp.pro
examples.depends = gamscpp
tests.depends = gamscpp
perf.depends = gamscpp
