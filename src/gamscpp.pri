unix:LIBS += -ldl
win32:LIBS += -luser32

DEFINES += API_VERSION=\\\"$$VERSION\\\"
DEFINES += GAMS_MIN_VERSION=\\\"$$GAMS_MIN_VER\\\"
DEFINES += CLIB_MIN_VERSION=\\\"$$CLIB_MIN_VER\\\"

DEFINES -= UNICODE
