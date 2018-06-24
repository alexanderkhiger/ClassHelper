#-------------------------------------------------
#
# Project created by QtCreator 2017-11-18T14:21:56
#
#-------------------------------------------------

QT       += core gui sql printsupport webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

lessThan(QT_MAJOR_VERSION,5): QT += webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 5): QT += webenginewidgets


TARGET = diplom
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    droparea.cpp \
    universityview.cpp \
    queryrunner.cpp \
    authview.cpp \
    loadnewfileview.cpp \
    universitymodel.cpp \
    loadnewfilemodel.cpp \
    tableeditorview.cpp \
    tableeditormodel.cpp \
    customlineedit.cpp \
    customtableview.cpp \
    mainwindowview.cpp \
    dataselectorview.cpp \
    customdoublevalidator.cpp \
    outputtofileview.cpp \
    customquerymodel.cpp \
    distributionview.cpp

HEADERS += \
    droparea.h \
    universityview.h \
    queryrunner.h \
    authview.h \
    loadnewfileview.h \
    universitymodel.h \
    loadnewfilemodel.h \
    tableeditorview.h \
    tableeditormodel.h \
    customlineedit.h \
    customtableview.h \
    mainwindowview.h \
    dataselectorview.h \
    customdoublevalidator.h \
    outputtofileview.h \
    customquerymodel.h \
    distributionview.h
