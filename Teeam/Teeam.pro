#-------------------------------------------------
#
# Project created by QtCreator 2017-06-26T23:05:14
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Teeam
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    task.cpp \
    teeamganttview.cpp \
    milestone.cpp \
    teeamproject.cpp \
    taskgroup.cpp \
    ganttcontroller.cpp

HEADERS += \
        mainwindow.h \
    abstractview.h \
    ganttentity.h \
    abstractmodel.h \
    abstractcontroller.h \
    generictask.h \
    task.h \
    teeamganttview.h \
    milestone.h \
    teeamproject.h \
    taskgroup.h \
    ganttcontroller.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += "C:\KDAB\KDChart-2.6.0\include\KDGantt"
INCLUDEPATH += "C:\KDAB\KDChart-2.6.0\include\KDChart"

LIBS += "C:\KDAB\KDChart-2.6.0\lib\kdchartd2.dll"

RESOURCES += \
    teeamres.qrc
