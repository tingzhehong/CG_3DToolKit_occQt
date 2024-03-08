#-------------------------------------------------
#
# Project created by QtCreator 2013-08-18T15:52:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = occQt
TEMPLATE = app

SOURCES += main.cpp \
    dxfReader.cpp \
    occDimensionDlg.cpp \
    occQt.cpp       \
    occView.cpp

CONFIG += c++11

HEADERS  += \
    dxfReader.h \
    occDimensionDlg.h \
    occQt.h \
    occView.h

FORMS    += \
    occQt.ui

RESOURCES += \
    occqt.qrc

CASROOT = "D:/Program Files/OpenCASCADE-7.3.0-vc14-64/opencascade-7.3.0"
    
win32 {
    DEFINES +=  \
        WNT
    INCLUDEPATH +=  \
        $$quote($${CASROOT})/inc

    win32-msvc2010 {
        compiler=vc10
    }

    win32-msvc2012 {
        compiler=vc11
    }

    win32-msvc2013 {
        compiler=vc12
    }

    win32-msvc2015 {
        compiler=vc14
    }

    # Determine 32 / 64 bit and debug / release build
    !contains(QMAKE_TARGET.arch, x86_64) {
        CONFIG(debug, debug|release) {
            message("Debug 32 build")
            LIBS += -L$$quote($${CASROOT})/win32/$$compiler/libd
        }
        else {
            message("Release 32 build")
            LIBS += -L$$quote($${CASROOT})/win32/$$compiler/lib
        }
    }
    else {
        CONFIG(debug, debug|release) {
            message("Debug 64 build")
            LIBS += -L$$quote($${CASROOT})/win64/$$compiler/libd
        }
        else {
            message("Release 64 build")
            LIBS += -L$$quote($${CASROOT})/win64/$$compiler/lib
        }
    }
}

linux-g++ {
    INCLUDEPATH +=  \
        $$quote($${CASROOT})/include/opencascade

    LIBS +=         \
        -L$$quote($${CASROOT})/lib
}

LIBS +=         \
    -lTKernel   \
    -lTKMath    \
    -lTKG3d     \
    -lTKBRep    \
    -lTKIGES    \
    -lTKSTEP    \
    -lTKSTEPAttr\
    -lTKSTEP209 \
    -lTKSTEPBase\
    -lTKSTL     \
    -lTKVRML    \
    -lTKGeomBase\
    -lTKGeomAlgo\
    -lTKTopAlgo \
    -lTKMesh    \
    -lTKPrim    \
    -lTKBO      \
    -lTKXSBase  \
    -lTKBool    \
    -lTKOffset  \
    -lTKService \
    -lTKV3d     \
    -lTKOpenGl  \
    -lTKFillet

# dxflib
include($$PWD\dxflib\dxflib.pri)
