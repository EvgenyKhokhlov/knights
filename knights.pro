#-------------------------------------------------
#
# Project created by QtCreator 2023-07-25T18:26:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = knights
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    knight.cpp \
    knife.cpp

HEADERS  += mainwindow.h \
    knight.h \
    knife.h

FORMS    += mainwindow.ui
