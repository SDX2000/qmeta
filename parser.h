#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "utils.h"
#include "parsestatus.h"
//#include <QTextStream>


#define QSL(str) QStringLiteral(str)

//QTextStream qStdOut();

//Note: There is no need to checkpoint EXPECT
//since all back tracking happens at TRY_CHOICE and TRY_CHOICE saves
//a check point.
#define EXPECT(X)                           \
    if (!(X)) {                             \
        return ParseStatus::failure(inp);   \
    }                                       \

#define TRY_CHOICE(X)                       \
    do                                      \
    {                                       \
        QStringRef _checkPoint = inp;       \
        if (X) {                            \
            return ParseStatus::success();  \
        }                                   \
        inp = _checkPoint;                  \
    }                                       \
    while(0);

#define CHECK_POINT(CP, INP) \
    QStringRef CP = INP;


class Parser
{
    ParseStatus advance(QStringRef &inp, int length);

protected:
    //TERMINALS//
    ParseStatus thisStr(QStringRef &inp, QString thisStr);
    ParseStatus digit(QStringRef &inp, int& digit);
    ParseStatus strOf(QStringRef &inp, bool (QChar::*is_x)() const);
    ParseStatus strOf(QStringRef &inp, QStringRef &str, bool (QChar::*is_x)() const);
    ParseStatus someChar(QStringRef& inp, QChar& c);
    ParseStatus someCharOf(QStringRef &inp, bool (QChar::*is_x)() const);
    ParseStatus someCharOf(QStringRef &inp, QChar &c, bool (QChar::*is_x)() const);
    ParseStatus thisChar(QStringRef &inp, QChar c);
    ParseStatus oneOf(QStringRef& inp, QChar &opOut, QString operators);

    //NONTERMINALS//
    ParseStatus space(QStringRef &inp);
    ParseStatus space(QStringRef &inp, QStringRef& space);
    ParseStatus identifier(QStringRef &inp, QStringRef& ident);
    ParseStatus integer(QStringRef &inp, int& result);
};


#endif // PARSER_H
