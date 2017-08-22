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
    milestone.cpp \
    teeamproject.cpp \
    taskgroup.cpp \
    ganttcontroller.cpp \
    addprojectdialog.cpp \
    freedaysdialog.cpp \
    freedaysmodel.cpp \
    addtaskgroupdialog.cpp \
    addtaskdialog.cpp \
    addmilestonedialog.cpp \
    addpersondialog.cpp \
    edittaskgroupdialog.cpp \
    edittaskdialog.cpp \
    editmilestonedialog.cpp

HEADERS += \
        mainwindow.h \
    abstractview.h \
    ganttentity.h \
    abstractmodel.h \
    generictask.h \
    task.h \
    milestone.h \
    teeamproject.h \
    taskgroup.h \
    ganttcontroller.h \
    addprojectdialog.h \
    freedaysdialog.h \
    freedaysmodel.h \
    addtaskgroupdialog.h \
    addtaskdialog.h \
    addmilestonedialog.h \
    addpersondialog.h \
    edittaskgroupdialog.h \
    edittaskdialog.h \
    editmilestonedialog.h \
    errors.h

FORMS += \
        mainwindow.ui \
    addprojectdialog.ui \
    freedaysdialog.ui \
    addtaskgroupdialog.ui \
    addtaskdialog.ui \
    addmilestonedialog.ui \
    addpersondialog.ui \
    edittaskgroupdialog.ui \
    edittaskdialog.ui \
    editmilestonedialog.ui

INCLUDEPATH += "C:\KDAB\KDChart-2.6.0\include\KDGantt"
INCLUDEPATH += "C:\KDAB\KDChart-2.6.0\include\KDChart"

CONFIG(Debug, Debug|Release){
LIBS += "C:\KDAB\KDChart-2.6.0\lib\kdchartd2.dll"
}

CONFIG(Release, Debug|Release){
LIBS += "C:\KDAB\KDChart-2.6.0\lib\kdchart2.dll"
}

RESOURCES += \
    teeamres.qrc

DEFINES += APP_NAME=\\\"Teeam\\\"

VERSION = 0.3

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32: RC_ICONS = barchart.ico
