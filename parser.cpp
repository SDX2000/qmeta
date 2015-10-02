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

bool Interpreter::str(QStringRef &inp, QString str)
{
    if (!inp.startsWith(str)) {
        return false;
    }

    advance(inp, str.length());
    return true;
}

bool Interpreter::value(QStringRef &inp, int &result)
{
    if(inp.isEmpty()) {
        return false;
    }

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

bool Interpreter::factor(QStringRef& inp, int &result)
{
    if(inp.isEmpty()) {
        return false;
    }

    if(value(inp, result)) {
        return true;
    }

    if (!str(inp, QSL("("))) {
        return false;
    }

    expr(inp, result);

    if (!str(inp, QSL(")"))) {
        return false;
    }

    return true;
}

bool Interpreter::advance(QStringRef &str, int count)
{
    if(str.length() < count) {
        return false;
    }

    str = str.mid(count);
    return true;
}

bool Interpreter::somechar(QStringRef &inp, QChar &c)
{
    if(inp.isEmpty()) {
        return false;
    }

    c = inp.at(0);

    return advance(inp, 1);
}

bool Interpreter::thischar(QStringRef& inp, QChar &opOut, QString operators)
{
    if(inp.isEmpty()) {
        return false;
    }

    QChar c = inp.at(0);
    if (operators.contains(c)) {
        opOut = c;
        advance(inp, 1);
        return true;
    }

    return false;
}

bool Interpreter::term(QStringRef &inp, int &result)
{
    if(inp.isEmpty()) {
        return false;
    }

    if(!factor(inp, result)) {
       return false;
    }

    QChar lastOp;
    while(thischar(inp, lastOp, QSL("*/")))
    {
        int f;
        if(!factor(inp, f)) {
            return false;
        }

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
    if(inp.isEmpty()) {
        return false;
    }

    if(!term(inp, result)) {
        return false;
    }

    QChar lastOp;
    while (thischar(inp, lastOp, QSL("+-")))
    {
        int t;
        if(!term(inp, t)) {
            return false;
        }

        if (lastOp == QChar('+')) {
            result += t;
        } else if (lastOp == QChar('-')) {
            result -= t;
        }
    }

    return true;
}
