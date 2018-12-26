QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS


#v8
QMAKE_CXXFLAGS = -pthread -std=c++0x -I/home/boris/Sketchbook/c++/v8-GN/v8/include
LIBS += -lv8_monolith -L/home/boris/Sketchbook/c++/v8-GN/v8/out.gn/x64.release.sample/obj/ \
        -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer


SOURCES += \
        main.cpp \
    engine.cpp \
    console.cpp \
    fs.cpp \
    json.cpp \
    overhype.cpp \
    overhypecore.cpp \
    spm.cpp \
    path.cpp \
    timecontrol.cpp \
    object.cpp \
    nodeapi.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    engine.h \
    console.h \
    fs.h \
    json.h \
    overhype.h \
    overhypecore.h \
    spm.h \
    path.h \
    timecontrol.h \
    object.h \
    nodeapi.h
