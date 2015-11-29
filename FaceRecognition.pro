#-------------------------------------------------
#
# Project created by QtCreator 2014-12-13T09:05:57
#
#-------------------------------------------------

QT       += core gui

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG += opencv

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH = /usr/local/include/opencv

TARGET = FaceRecognition
TEMPLATE = app


SOURCES += main.cpp\
        facerecognitionmain.cpp \
    entername.cpp \
    captureface.cpp \
    recognize.cpp \
    detectObject.cpp \
    ImageUtils_0.7.cpp \
    preprocessFace.cpp \
#    recognition.cpp \
    common_cv.cpp \
    getcamframe.cpp

HEADERS  += facerecognitionmain.h \
    entername.h \
    captureface.h \
    recognize.h \
    common_cv.h \
    detectObject.h \
    ImageUtils.h \
    preprocessFace.h \
#   recognition.h \
    getcamframe.h

FORMS    += facerecognitionmain.ui \
    entername.ui \
    captureface.ui \
    recognize.ui

target.path = /opt/$${TARGET}/bin
INSTALLS += target
