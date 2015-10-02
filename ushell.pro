#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T22:26:25
#
#-------------------------------------------------

QT += core
QT -= gui

TARGET = ushell
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    parser.cpp

DEFINES += QT_RESTRICTED_CAST_FROM_ASCII \
           QT_NO_CAST_TO_ASCII

HEADERS += \
    parser.h
