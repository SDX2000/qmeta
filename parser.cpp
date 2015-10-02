#include "parser.h"

/*
expr    = term (( "+" | "-" ) term)*
term    = factor (( "*" | "/" ) factor)*
factor  = value | "(" expr ")"
value   = integer
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
    return expr(inp, result);
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



bool Interpreter::strOf(QStringRef &inp, QStringRef &str, bool (QChar::*is_x)() const)
{
    EXPECT(!inp.isEmpty());

    int count = 0;

    for (; count < inp.length() && (inp.at(count).*is_x)(); count++) {
    }

    EXPECT(count);

    str = inp.left(count);

    return advance(inp, count);
}


/////////////////////  NONTERMINALS  //////////////////////

bool Interpreter::space(QStringRef &inp, QStringRef &space)
{
    return strOf(inp, space, &QChar::isSpace);
}

bool Interpreter::space(QStringRef &inp)
{
    QStringRef temp;
    return strOf(inp, temp, &QChar::isSpace);
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

bool Interpreter::factor(QStringRef& inp, int &result)
{
    TRY_CHOICE(integer(inp, result));

    //Else...

    EXPECT(thisStr(inp, QSL("(")));

    EXPECT(expr(inp, result));

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


bool Interpreter::expr(QStringRef &inp, int &result)
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
