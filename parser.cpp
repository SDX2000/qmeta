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



/////////////////////  TERMINALS  //////////////////////

bool Parser::advance(QStringRef &inp, int count)
{
    EXPECT(inp.length() >= count);

    inp = inp.mid(count);

    return true;
}

bool Parser::digit(QStringRef &inp, int &digit)
{
    EXPECT(!inp.isEmpty());

    QChar c = inp.at(0);

    EXPECT(c.isDigit());

    digit = c.digitValue();

    return advance(inp, 1);
}


bool Parser::someChar(QStringRef &inp, QChar &c)
{
    EXPECT(!inp.isEmpty());

    c = inp.at(0);

    return advance(inp, 1);
}

bool Parser::oneOf(QStringRef& inp, QChar &opOut, QString chars)
{
    EXPECT(!inp.isEmpty());

    QChar c = inp.at(0);

    EXPECT(chars.contains(c));

    opOut = c;

    return advance(inp, 1);
}

bool Parser::thisChar(QStringRef &inp, QChar c)
{
    EXPECT(!inp.isEmpty());

    EXPECT(inp.at(0) == c);

    return advance(inp, 1);
}

bool Parser::thisStr(QStringRef &inp, QString str)
{
    EXPECT(inp.startsWith(str));

    return advance(inp, str.length());;
}

bool Parser::strOf(QStringRef &inp, bool (QChar::*is_x)() const)
{
    EXPECT(!inp.isEmpty());

    int count = 0;

    for (; count < inp.length() && (inp.at(count).*is_x)(); count++) {
    }

    EXPECT(count);

    return advance(inp, count);
}

/////////////////////  NONTERMINALS  //////////////////////

bool Parser::strOf(QStringRef &inp, QStringRef &str, bool (QChar::*is_x)() const)
{
    CHECK_POINT(cp0, inp);

    EXPECT(strOf(inp, is_x));

    str = mid(cp0, inp);

    return true;
}

bool Parser::space(QStringRef &inp)
{
    return strOf(inp, &QChar::isSpace);
}

bool Parser::space(QStringRef &inp, QStringRef &space)
{
    return strOf(inp, space, &QChar::isSpace);
}

bool Parser::identifier(QStringRef &inp, QStringRef& ident)
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

bool Parser::integer(QStringRef &inp, int &result)
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


