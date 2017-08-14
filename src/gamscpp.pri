unix:LIBS += -ldl
win32:LIBS += -luser32

VERSION = 0.1.1

DEFINES += API_VERSION=\\\"$$VERSION\\\"

DEFINES -= UNICODE
