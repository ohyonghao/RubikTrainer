#-------------------------------------------------
#
# Project created by QtCreator 2010-09-22T04:14:13
#
#-------------------------------------------------

QT       += core gui sql
QXT += core

CONFIG += qxt

TARGET = RubikTrainer
TEMPLATE = app


SOURCES += main.cpp\
        RubikTrainer.cpp \
    Algorithm.cpp \
    ClockTimer.cpp \
    CustomWidgetListWidget.cpp

HEADERS  += RubikTrainer.h \
    Algorithm.h \
    ClockTimer.h \
    CustomWidgetListWidget.h

FORMS += Algorithm.ui
