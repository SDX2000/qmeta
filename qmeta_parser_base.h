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
        if (!__ps->isOk()) { \
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
        if ((X)->isOk()) { \
            return ParseStatus::success(); \
        } \
        inp = _checkPoint; \
    } \
    while(0);

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
    ParseStatusPtr space(QStringRef &inp);
    ParseStatusPtr space(QStringRef &inp, QStringRef& space);
    ParseStatusPtr identifier(QStringRef &inp, QStringRef& ident);
    ParseStatusPtr integer(QStringRef &inp, int& result);
};


#endif // PARSER_H
