#-------------------------------------------------
#
# Project created by QtCreator 2015-11-18T22:32:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphs
TEMPLATE = app


SOURCES += main.cpp\
        graphs.cpp \
    qcustomplot.cpp \
    testweb.cpp

HEADERS  += graphs.h \
    codeeditor.h \
    entry.h \
    qcustomplot.h \
    testweb.h

FORMS    += graphs.ui \
    testweb.ui
