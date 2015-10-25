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
    utils.cpp \
    QVariantList/QMetaQVariantListParserBase.cpp \
    QString/QMetaQStringParser.cpp \
    QString/QMetaQStringParserBase.cpp \
    QVariantList/QMetaQVariantListParser.cpp \
    QVariantList/ParseFail.cpp

DEFINES += QT_RESTRICTED_CAST_FROM_ASCII \
           QT_NO_CAST_TO_ASCII

HEADERS += \
    ParseError.h \
    utils.h \
    QVariantList/QMetaQVariantListParserBase.h \
    QString/QMetaQStringParser.h \
    QString/QMetaQStringParserBase.h \
    QVariantList/QMetaQVariantListParser.h \
    QVariantList/ParseFail.h

DISTFILES += \
    TODO \
    NOTES \
    tests/Calc.qm \
    tests/QMetaParser.qm \
    AST/QMetaParser.ast \
    tests/QMetaQStringParserGenerator.qm \
    tests/QMetaQVariantListParser.qm \
    AST/Calc.ast
