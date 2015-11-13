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
    QVariantList/ParseFail.cpp \
    QVariantList/QMetaQVariantToQStringTransformer.cpp \
    QVariantList/QMetaCodeGenerator.cpp \
    QString/QMetaQStringToQVariantTransformer.cpp \
    QString/QMetaParser.cpp

DEFINES += QT_RESTRICTED_CAST_FROM_ASCII \
           QT_NO_CAST_TO_ASCII

HEADERS += \
    ParseError.h \
    utils.h \
    QVariantList/ParseFail.h \
    QVariantList/macros.h \
    QString/macros.h \
    QVariantList/QMetaQVariantToQStringTransformer.h \
    QVariantList/QMetaCodeGenerator.h \
    QString/QMetaQStringToQVariantTransformer.h \
    QString/QMetaParser.h

DISTFILES += \
    TODO \
    NOTES \
    tests/Calc.qm \
    tests/QMetaParser.qm \
    AST/QMetaParser.ast \
    AST/Calc.ast \
    templates/grammar \
    tests/QMetaCodeGenerator.qm

RESOURCES += \
    resourcefiles.qrc
