#-------------------------------------------------
#
# Project created by QtCreator 2012-10-25T00:51:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gMentoHust
TEMPLATE = app
DEPENDPATH += . src
INCLUDEPATH += . src

# Input
HEADERS += src/configwindow.h src/mainwindow.h src/authmsgwindow.h
SOURCES += src/configwindow.cpp src/main.cpp src/mainwindow.cpp src/authmsgwindow.cpp

OTHER_FILES += \
    images/gmentohust.png

RESOURCES += \
    images/images.qrc

