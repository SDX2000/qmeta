#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T22:26:25
#
#-------------------------------------------------

QT += core
QT -= gui

TARGET = qmeta
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    ParseError.cpp \
    QMetaParser.cpp \
    QMetaParserBase.cpp \
    utils.cpp

DEFINES += QT_RESTRICTED_CAST_FROM_ASCII \
           QT_NO_CAST_TO_ASCII

HEADERS += \
    ParseError.h \
    QMetaParser.h \
    QMetaParserBase.h \
    utils.h

DISTFILES += \
    tests/ometa.qm \
    TODO \
    NOTES \
    tests/calc.qm
