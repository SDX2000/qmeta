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
    utils.cpp \
    parse_status.cpp \
    qmeta_parser.cpp \
    qmeta_parser_base.cpp

DEFINES += QT_RESTRICTED_CAST_FROM_ASCII \
           QT_NO_CAST_TO_ASCII

HEADERS += \
    utils.h \
    parse_status.h \
    qmeta_parser.h \
    qmeta_parser_base.h

DISTFILES += \
    tests/ometa.qm
