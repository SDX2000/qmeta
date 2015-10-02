#include "parser.h"
#include <assert.h>
#include <algorithm>

/*
program     = assignment | expression
assignment  = identifier "=" expression
expression  = term (( "+" | "-" ) term)*
term        = factor (( "*" | "/" ) factor)*
factor      = value | "(" expression ")"
value       = integer
*/

//QTextStream qStdOut()
//{
//    static QTextStream ts( stdout );
//    return ts;
//}


bool Interpreter::parse(QString inp, int &result)
{
    QStringRef  inpRef = inp.midRef(0);
    return parse(inpRef, result);
}

bool Interpreter::parse(QStringRef inp, int &result)
{
    return program(inp, result);
}

/////////////////////  TERMINALS  //////////////////////

bool Interpreter::advance(QStringRef &str, int count)
{
    EXPECT(str.length() >= count);

    str = str.mid(count);

    return true;
}

bool Interpreter::digit(QStringRef &inp, int &digit)
{
    EXPECT(!inp.isEmpty());

    QChar c = inp.at(0);

    EXPECT(c.isDigit());

    digit = c.digitValue();

    return advance(inp, 1);
}


bool Interpreter::someChar(QStringRef &inp, QChar &c)
{
    EXPECT(!inp.isEmpty());

    c = inp.at(0);

    return advance(inp, 1);
}

bool Interpreter::oneOf(QStringRef& inp, QChar &opOut, QString chars)
{
    EXPECT(!inp.isEmpty());

    QChar c = inp.at(0);

    EXPECT(chars.contains(c));

    opOut = c;

    return advance(inp, 1);
}

bool Interpreter::thisChar(QStringRef &inp, QChar c)
{
    EXPECT(!inp.isEmpty());

    EXPECT(inp.at(0) == c);

    return advance(inp, 1);
}

bool Interpreter::thisStr(QStringRef &inp, QString str)
{
    EXPECT(inp.startsWith(str));

    return advance(inp, str.length());;
}

bool Interpreter::strOf(QStringRef &inp, bool (QChar::*is_x)() const)
{
    EXPECT(!inp.isEmpty());

    int count = 0;

    for (; count < inp.length() && (inp.at(count).*is_x)(); count++) {
    }

    EXPECT(count);

    return advance(inp, count);
}

/////////////////////  NONTERMINALS  //////////////////////

bool Interpreter::strOf(QStringRef &inp, QStringRef &str, bool (QChar::*is_x)() const)
{
    CHECK_POINT(cp0, inp);

    EXPECT(strOf(inp, is_x));

    str = mid(cp0, inp);

    return true;
}

bool Interpreter::space(QStringRef &inp)
{
    return strOf(inp, &QChar::isSpace);
}

bool Interpreter::space(QStringRef &inp, QStringRef &space)
{
    return strOf(inp, space, &QChar::isSpace);
}

bool Interpreter::identifier(QStringRef &inp, QStringRef& ident)
{
    CHECK_POINT(cp0, inp);

    bool ok = false;

    ok = ok || thisChar(inp, QChar('_'));

    ok = ok || strOf(inp, &QChar::isLetterOrNumber);

    if(ok) {
        ident =  mid(cp0, inp);
    }

    return ok;
}

bool Interpreter::integer(QStringRef &inp, int &result)
{
    int sign = 1;

    if (thisChar(inp, QChar('-'))) {
        sign = -1;
    }

    QStringRef numbers;
    EXPECT(strOf(inp, numbers, &QChar::isDigit));

    result = int(numbers.toInt()) * sign;

    return true;
}

bool Interpreter::value(QStringRef &inp, int &result)
{
    return integer(inp, result);
}

bool Interpreter::assignment(QStringRef& inp, int &result)
{
    //assignment  = identifier "=" expression

    QStringRef ident;

    EXPECT(identifier(inp, ident));

    space(inp);

    EXPECT(thisChar(inp, QChar('=')));

    space(inp);

    EXPECT(expression(inp, result));

    return true;
}

bool Interpreter::program(QStringRef& inp, int &result)
{
    TRY_CHOICE(assignment(inp, result));

    //Else...

    EXPECT(expression(inp, result));

    return true;
}


bool Interpreter::factor(QStringRef& inp, int &result)
{
    TRY_CHOICE(integer(inp, result));

    //Else...

    EXPECT(thisStr(inp, QSL("(")));

    EXPECT(expression(inp, result));

    EXPECT(thisStr(inp, QSL(")")));

    return true;
}


bool Interpreter::term(QStringRef &inp, int &result)
{
    EXPECT(factor(inp, result));

    QChar lastOp;
    while (oneOf(inp, lastOp, QSL("*/")))
    {
        int f;

        EXPECT(factor(inp, f));

        if (lastOp == QChar('*')) {
            result *= f;
        } else if (lastOp == QChar('/')) {
            result /= f;
        }
    }

    return true;
}


bool Interpreter::expression(QStringRef &inp, int &result)
{
    EXPECT(term(inp, result));

    QChar lastOp;
    while (oneOf(inp, lastOp, QSL("+-")))
    {
        int t;
        EXPECT(term(inp, t));

        if (lastOp == QChar('+')) {
            result += t;
        } else if (lastOp == QChar('-')) {
            result -= t;
        }
    }

    return true;
}

QStringRef mid(QStringRef lhs, QStringRef rhs)
{
    return lhs.string()->midRef(lhs.position(), rhs.position() - lhs.position());
}


