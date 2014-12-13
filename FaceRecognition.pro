#-------------------------------------------------
#
# Project created by QtCreator 2014-12-13T09:05:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceRecognition
TEMPLATE = app


SOURCES += main.cpp\
        facerecognitionmain.cpp \
    entername.cpp \
    captureface.cpp \
    recognize.cpp

HEADERS  += facerecognitionmain.h \
    entername.h \
    captureface.h \
    recognize.h

FORMS    += facerecognitionmain.ui \
    entername.ui \
    captureface.ui \
    recognize.ui
