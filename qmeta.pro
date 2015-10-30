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
    QString/QMetaQStringParser.cpp \
    QString/QMetaQStringParserBase.cpp \
    QVariantList/ParseFail.cpp \
    QVariantList/QMetaQVariantTransformer.cpp \
    QVariantList/QMetaQVariantTransformerBase.cpp

DEFINES += QT_RESTRICTED_CAST_FROM_ASCII \
           QT_NO_CAST_TO_ASCII

HEADERS += \
    ParseError.h \
    utils.h \
    QString/QMetaQStringParser.h \
    QString/QMetaQStringParserBase.h \
    QVariantList/ParseFail.h \
    QVariantList/macros.h \
    QString/macros.h \
    QVariantList/QMetaQVariantTransformer.h \
    QVariantList/QMetaQVariantTransformerBase.h

DISTFILES += \
    TODO \
    NOTES \
    tests/Calc.qm \
    tests/QMetaParser.qm \
    AST/QMetaParser.ast \
    AST/Calc.ast \
    tests/QMetaQVariantTransformer.qm \
    templates/grammar

RESOURCES += \
    resourcefiles.qrc
