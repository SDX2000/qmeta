#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "utils.h"
#include "parse_status.h"


//Note: There is no need to checkpoint EXPECT
//since all back tracking happens at TRY_CHOICE
//and TRY_CHOICE saves a check point.
#define EXPECT(X) \
    do \
    { \
        ParseStatusPtr __ps = (X);\
        if (!*__ps) { \
            return ParseStatus::failure(inp, "Expected: " #X, __ps); \
        } \
    } while(0); \

#define EXPECT_B(X) \
    if (!(X)) { \
        return ParseStatus::failure(inp, #X); \
    }


#define TRY_CHOICE(X) \
    do \
    { \
        QStringRef _checkPoint = inp; \
        if (*(X)) { \
            return ParseStatus::success(); \
        } \
        inp = _checkPoint; \
    } \
    while(0);

#define TRY(X, NEXT) \
    if (!*(X)) { \
        goto NEXT; \
    } \

#define TRY_INV(X, NEXT) \
    if (*(X)) { \
        goto NEXT; \
    } \

#define CHECK_POINT(CP, INP) \
    QStringRef CP = INP;


class QMetaParserBase
{
    ParseStatusPtr advance(QStringRef &inp, int length);

protected:
    //TERMINALS//
    ParseStatusPtr thisStr(QStringRef &inp, QString thisStr);
    ParseStatusPtr digit(QStringRef &inp, int& digit);
    ParseStatusPtr strOf(QStringRef &inp, bool (QChar::*is_x)() const);
    ParseStatusPtr strOf(QStringRef &inp, QStringRef &str, bool (QChar::*is_x)() const);
    ParseStatusPtr someChar(QStringRef& inp, QChar& c);
    ParseStatusPtr someCharOf(QStringRef &inp, bool (QChar::*is_x)() const);
    ParseStatusPtr someCharOf(QStringRef &inp, QChar &c, bool (QChar::*is_x)() const);
    ParseStatusPtr thisChar(QStringRef &inp, QChar c);
    ParseStatusPtr oneOf(QStringRef& inp, QChar &opOut, QString operators);

    //NONTERMINALS//
    ParseStatusPtr spaces(QStringRef &inp);
    ParseStatusPtr spaces(QStringRef &inp, QStringRef& spaces);
    ParseStatusPtr identifier(QStringRef &inp, QString &ident);
    ParseStatusPtr integer(QStringRef &inp, int& result);
    ParseStatusPtr thisToken(QStringRef &inp, QString str);

    //HELPER FUNCTIONS//
    QChar unescape(QChar c);
};


#endif // PARSER_H
