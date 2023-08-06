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
    knife.cpp \
    knightsdata.cpp \
    GraphicScene/knightanimation.cpp \
    GraphicScene/animationpool.cpp \
    GraphicScene/scene.cpp

HEADERS  += mainwindow.h \
    knight.h \
    knife.h \
    knightsdata.h \
    GraphicScene/knightanimation.h \
    GraphicScene/animationpool.h \
    GraphicScene/scene.h

FORMS    += mainwindow.ui

RESOURCES += \
    spritesheets.qrc
