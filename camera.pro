TEMPLATE = app
TARGET = camera

QT += multimedia multimediawidgets

HEADERS = \
    camera.h \
    multiparturstatus.h \
    videosurface.h

SOURCES = \
    main.cpp \
    camera.cpp \
    multiparturstatus.cpp \
    videosurface.cpp

FORMS += \
    camera.ui

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/camera
INSTALLS += target

QT+=widgets

QMAKE_CXXFLAGS += -std=c++17

# OpenCV
INCLUDEPATH += /usr/local/include/opencv4
LIBS += -L/usr/local/lib -lopencv_world

# bc-ur
INCLUDEPATH += /usr/local/include/bc-ur
LIBS += -L/usr/local/lib -lbc-ur
