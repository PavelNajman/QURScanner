TEMPLATE = app
TARGET = camera

QT += multimedia multimediawidgets

HEADERS = \
    multiparturstatus.h \
    resultspage.h \
    scanpage.h \
    urscanner.h \
    urutils.h \
    videosurface.h

SOURCES = \
    main.cpp \
    multiparturstatus.cpp \
    resultspage.cpp \
    scanpage.cpp \
    urscanner.cpp \
    urutils.cpp \
    videosurface.cpp

QT+=widgets

QMAKE_CXXFLAGS += -std=c++17

# OpenCV
INCLUDEPATH += /usr/local/include/opencv4
LIBS += -L/usr/local/lib -lopencv_world

# bc-ur
INCLUDEPATH += /usr/local/include/bc-ur
LIBS += -L/usr/local/lib -lbc-ur

# lifehash
INCLUDEPATH += /usr/local/include/lifehash
LIBS += -L/usr/local/lib -llifehash
