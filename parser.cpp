#include "parser.h"

/*
expr    = term (( "+" | "-" ) term)*
term    = factor (( "*" | "/" ) factor)*
factor  = value | "(" expr ")"
*/

//QTextStream qStdOut()
//{
//    static QTextStream ts( stdout );
//    return ts;
//}


bool Interpreter::parse(QString inp, int &result)
{
    QStringRef  inpRef = inp.midRef(0);
    return expr(inpRef, result);
}

/////////////////////  TERMINALS  //////////////////////

bool Interpreter::advance(QStringRef &str, int count)
{
    EXPECT(str.length() >= count);

    str = str.mid(count);
    return true;
}

bool Interpreter::somechar(QStringRef &inp, QChar &c)
{
    EXPECT(!inp.isEmpty());

    c = inp.at(0);

    return advance(inp, 1);
}

bool Interpreter::thischar(QStringRef& inp, QChar &opOut, QString operators)
{
    EXPECT(!inp.isEmpty());

    QChar c = inp.at(0);
    if (operators.contains(c)) {
        opOut = c;
        advance(inp, 1);
        return true;
    }

    return false;
}

bool Interpreter::str(QStringRef &inp, QString str)
{
    EXPECT(inp.startsWith(str));
    advance(inp, str.length());
    return true;
}

bool Interpreter::value(QStringRef &inp, int &result)
{
    EXPECT(!inp.isEmpty());

    int count = 0;

    for(; count < inp.length() && inp.at(count).isDigit(); count++) {
    }

    if (count) {
        result = int(inp.left(count).toInt());
        advance(inp, count);
        return true;
    }

    return false;
}

/////////////////////  NONTERMINALS  //////////////////////

bool Interpreter::factor(QStringRef& inp, int &result)
{
    EXPECT(!inp.isEmpty());

    MAYBE(value(inp, result));

    //Else...

    EXPECT(!str(inp, QSL("(")))

    EXPECT(expr(inp, result));

    EXPECT(!str(inp, QSL(")")))

    return true;
}


bool Interpreter::term(QStringRef &inp, int &result)
{
    EXPECT(!inp.isEmpty());

    EXPECT(factor(inp, result));

    QChar lastOp;
    while(thischar(inp, lastOp, QSL("*/")))
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
    EXPECT(!inp.isEmpty());

    EXPECT(term(inp, result));

    QChar lastOp;
    while (thischar(inp, lastOp, QSL("+-")))
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
