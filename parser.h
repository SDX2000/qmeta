#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QTextStream>

#define QSL(str) QStringLiteral(str)

//QTextStream qStdOut();

#define EXPECT(X)       \
    if (!(X)) {         \
        return false;   \
    }

#define TRY_CHOICE(X)     \
    if (X) {         \
        return true; \
    }


class Interpreter
{
public:
    bool parse(QStringRef inp, int& result);
    bool parse(QString inp, int& result);

private:
    //TERMINALS//
    bool thisStr(QStringRef &inp, QString thisStr);
    bool digit(QStringRef &inp, int& digit);
    bool strOf(QStringRef &inp, QStringRef &str, bool (QChar::*is_x)() const);
    bool someChar(QStringRef& inp, QChar& c);
    bool thisChar(QStringRef &inp, QChar c);
    bool oneOf(QStringRef& inp, QChar &opOut, QString operators);
    bool advance(QStringRef &thisStr, int length);

    //NONTERMINALS//
    bool value(QStringRef &inp, int& result);
    bool integer(QStringRef &inp, int& result);
    bool space(QStringRef &inp);
    bool space(QStringRef &inp, QStringRef& space);
    bool factor(QStringRef& inp, int &result);
    bool term(QStringRef &inp, int& result);
    bool expr(QStringRef &inp, int& result);
};
#endif // PARSER_H
