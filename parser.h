#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "utils.h"
//#include <QTextStream>


#define QSL(str) QStringLiteral(str)

//QTextStream qStdOut();

//Note: There is no need to checkpoint EXPECT
//since all back tracking happens at TRY_CHOICE and TRY_CHOICE saves
//a check point.
#define EXPECT(X)                   \
    if (!(X)) {                     \
        return false;               \
    }                               \

#define TRY_CHOICE(X)                   \
    do                                  \
    {                                   \
        QStringRef _checkPoint = inp;   \
        if (X) {                        \
            return true;                \
        }                               \
        inp = _checkPoint;              \
    }                                   \
    while(0);

#define CHECK_POINT(CP, INP) \
    QStringRef CP = INP;


class Parser
{
    bool advance(QStringRef &inp, int length);

protected:
    //TERMINALS//
    bool thisStr(QStringRef &inp, QString thisStr);
    bool digit(QStringRef &inp, int& digit);
    bool strOf(QStringRef &inp, bool (QChar::*is_x)() const);
    bool strOf(QStringRef &inp, QStringRef &str, bool (QChar::*is_x)() const);
    bool someChar(QStringRef& inp, QChar& c);
    bool someCharOf(QStringRef &inp, bool (QChar::*is_x)() const);
    bool someCharOf(QStringRef &inp, QChar &c, bool (QChar::*is_x)() const);
    bool thisChar(QStringRef &inp, QChar c);
    bool oneOf(QStringRef& inp, QChar &opOut, QString operators);

    //NONTERMINALS//
    bool space(QStringRef &inp);
    bool space(QStringRef &inp, QStringRef& space);
    bool identifier(QStringRef &inp, QStringRef& ident);
    bool integer(QStringRef &inp, int& result);
};


#endif // PARSER_H
