#include "programinterpreter.h"


bool ProgramInterpreter::parse(QString inp, int &result)
{
    QStringRef  inpRef = inp.midRef(0);
    return parse(inpRef, result);
}

bool ProgramInterpreter::parse(QStringRef inp, int &result)
{
    return program(inp, result);
}

bool ProgramInterpreter::value(QStringRef &inp, int &result)
{
    return integer(inp, result);
}

bool ProgramInterpreter::assignment(QStringRef& inp, int &result)
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

bool ProgramInterpreter::program(QStringRef& inp, int &result)
{
    TRY_CHOICE(assignment(inp, result));

    //Else...

    EXPECT(expression(inp, result));

    return true;
}


bool ProgramInterpreter::factor(QStringRef& inp, int &result)
{
    TRY_CHOICE(integer(inp, result));

    //Else...

    EXPECT(thisStr(inp, QSL("(")));

    EXPECT(expression(inp, result));

    EXPECT(thisStr(inp, QSL(")")));

    return true;
}


bool ProgramInterpreter::term(QStringRef &inp, int &result)
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


bool ProgramInterpreter::expression(QStringRef &inp, int &result)
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
