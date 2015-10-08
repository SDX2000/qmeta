#include "qmeta_parser.h"
#include <iostream>

bool QMetaParser::parse(QString pos, QVariant& ast, ParseStatusPtr& ps)
{
    return QMetaParserBase::parse(pos, ast, ps);
}

QMetaParser::QMetaParser()
{
    initRuleMap();
}

bool QMetaParser::parse(int pos, QVariant &ast, ParseStatusPtr& ps)
{
    ENTER();
    return applyRule(GRAMMAR, pos, ast, ps);
}

bool QMetaParser::grammar(int& pos, QVariant &ast, ParseStatusPtr& ps)
{
    ENTER();
    QList<QVariant> l;

    QVariant _ast;
    while (applyRule(RULE, pos, _ast, ps)) {
        EXPECT(thisToken(pos, QSL(";"), ps));
        l.append(_ast);
    }

    ast = l;

    RETURN_SUCCESS(ps);
}

bool QMetaParser::rule(int &pos, QVariant &ast, ParseStatusPtr& ps)
{
    ENTER();
    QList<QVariant> l;
    l.append(QString(QSL("RULE")));
    QVariant id;
    EXPECT(applyRule(IDENTIFIER, pos, id, ps));
    l.append(id);

    EXPECT(thisToken(pos, QSL("="), ps));

    QVariant _ast;
    EXPECT(applyRule(CHOICES, pos, _ast, ps));
    l.append(_ast);

    ast = l;

    RETURN_SUCCESS(ps);
}

bool QMetaParser::choices(int &pos, QVariant &ast, ParseStatusPtr& ps)
{
    ENTER();
    CHECK_POINT(cp0, pos);
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("OR")));

        QVariant _ast;
        TRY(applyRule(CHOICE, pos, _ast, ps), choice1);
        l.append(_ast);

        TRY(thisToken(pos, QSL("|"), ps), choice1);
        TRY(applyRule(CHOICES, pos, _ast, ps), choice1);

        ast = l;
        RETURN_SUCCESS(ps);
    }

choice1:
    {
        pos = cp0;
        QVariant _ast;
        EXPECT(applyRule(CHOICE, pos, _ast, ps));
        ast = _ast;
    }

    RETURN_SUCCESS(ps);
}

bool QMetaParser::choice(int &pos, QVariant &ast, ParseStatusPtr& ps)
{
    ENTER();
    QList<QVariant> l;

    QVariant _ast;
    while (applyRule(TERM, pos, _ast, ps)) {
        l.append(_ast);
        if (thisToken(pos, QSL("->"), ps)) {
            l.append(QString(QSL("HOSTEXPR")));
            QVariant _hostExpr;
            EXPECT(applyRule(HOST_EXPR, pos, _hostExpr, ps));
            l.append(_hostExpr);
        }
    }

    ast = l;

    RETURN_SUCCESS(ps);
}

bool QMetaParser::hostExpr(int &pos, QVariant &ast, ParseStatusPtr &ps)
{
    ENTER();
    spaces(pos, ps);
    int count = 0;
    QChar c;
    QString hostexpr;
    while(someChar(pos, c, ps)) {
        if (c == QChar('{')) {
            count++;
        }
        if (c == QChar('}')) {
            count--;
        }
        hostexpr += c;
        if(count == 0) {
            break;
        }
    }

    if(count) {
        RETURN_FAILURE(ps, pos, QSL("Invalid host expression (unbalanced braces)."));
    }

    spaces(pos, ps);

    ast = hostexpr;

    RETURN_SUCCESS(ps);
}

bool QMetaParser::term(int &pos, QVariant &ast, ParseStatusPtr &ps)
{
    ENTER();
    CHECK_POINT(cp0, pos);
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("term")));

        QVariant _ast;
        TRY(applyRule(TERM1, pos, _ast, ps), choice1);
        l.append(_ast);

        TRY(thisToken(pos, QSL(":"), ps), choice1);

        QVariant id;
        TRY(applyRule(IDENTIFIER, pos, id, ps), choice1);
        l.append(id);

        ast = l;
        RETURN_SUCCESS(ps);
    }

choice1:
    {
        pos = cp0;
        QVariant _ast;
        EXPECT(applyRule(TERM1, pos, _ast, ps));
        ast = _ast;
    }

    RETURN_SUCCESS(ps);
}

bool QMetaParser::term1(int &pos, QVariant &ast, ParseStatusPtr &ps)
{
    ENTER();
    CHECK_POINT(cp0, pos);
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("NOT")));

        TRY(thisToken(pos, QSL("~"), ps), choice1);

        QVariant _ast;
        TRY(applyRule(TERM2, pos, _ast, ps), choice1);
        l.append(_ast);

        ast = l;
        RETURN_SUCCESS(ps);
    }

choice1:
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("PRED")));

        TRY(thisToken(pos, QSL("?"), ps), choice2);

        QVariant _ast;
        TRY(applyRule(HOST_EXPR, pos, _ast, ps), choice2);
        l.append(_ast);

        ast = l;
        RETURN_SUCCESS(ps);
    }

choice2:
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("MANY")));

        QVariant _ast;
        TRY(applyRule(TERM2, pos, _ast, ps), choice3);
        l.append(_ast);

        TRY(thisToken(pos, QSL("*"), ps), choice3);

        ast = l;
        RETURN_SUCCESS(ps);
    }

choice3:
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("MANY1")));

        QVariant _ast;
        TRY(applyRule(TERM2, pos, _ast, ps), choice4);
        l.append(_ast);

        TRY(thisToken(pos, QSL("+"), ps), choice4);

        ast = l;
        RETURN_SUCCESS(ps);
    }

choice4:
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("OPTIONAL")));

        QVariant _ast;
        TRY(applyRule(TERM2, pos, _ast, ps), choice5);
        l.append(_ast);

        TRY(thisToken(pos, QSL("?"), ps), choice5);

        ast = l;
        RETURN_SUCCESS(ps);
    }

choice5:
    {
        pos = cp0;
        QVariant _ast;
        EXPECT(applyRule(TERM2, pos, _ast, ps));
        ast = _ast;
    }

    RETURN_SUCCESS(ps);
}

bool QMetaParser::term2(int &pos, QVariant &ast, ParseStatusPtr &ps)
{
    ENTER();
    CHECK_POINT(cp0, pos);
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("CHAR")));

        TRY(thisChar(pos, QChar('\''), ps), choice1);

        CHECK_POINT(cp1, pos);
        TRY(thisChar(pos, QChar('\''), ps), choice1);
        pos = cp1;

        QChar c;
        TRY_INV(escapedChar(pos, c, ps), choice1);
        l.append(c);

        TRY(thisChar(pos, QChar('\''), ps), choice1);

        ast = l;
        RETURN_SUCCESS(ps);
    }

choice1:
    {
        pos = cp0;
        QVariant _ast;
        TRY(applyRule(SOME_TOKEN, pos, _ast, ps), choice2);
        ast = _ast;
        RETURN_SUCCESS(ps);
    }

choice2:
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("APPLY")));

        QVariant ruleName;
        TRY(applyRule(IDENTIFIER, pos, ruleName, ps), choice3);
        l.append(ruleName);

        ast = l;
        RETURN_SUCCESS(ps);
    }

choice3:
    {
        pos = cp0;
        EXPECT(thisToken(pos, QSL("("), ps));

        QVariant _ast;
        EXPECT(applyRule(CHOICE, pos, _ast, ps));

        EXPECT(thisToken(pos, QSL(")"), ps));

        ast = _ast;
    }

    RETURN_SUCCESS(ps);
}

bool QMetaParser::someToken(int &pos, QVariant& ast, ParseStatusPtr &ps)
{
    ENTER();
    spaces(pos, ps);

    EXPECT(thisChar(pos, QChar('"'), ps));

    QList<QVariant> l;
    l.append(QString(QSL("TOKEN")));


    QString token;

    while (true) {
        CHECK_POINT(cp0, pos);
        if(thisChar(pos, QChar('"'), ps)) {
            break;
        }
        pos = cp0;
        QChar c;
        EXPECT(someChar(pos, c, ps));
        token += c;
    }

    EXPECT(thisChar(pos, QChar('"'), ps));

    spaces(pos, ps);

    l.append(token);

    ast = l;

    RETURN_SUCCESS(ps);
}

bool QMetaParser::escapedChar(int &pos, QChar &c, ParseStatusPtr &ps)
{
    ENTER();
    CHECK_POINT(cp0, pos);
    {
        pos = cp0;
        TRY(thisChar(pos, QChar('\\'), ps), choice1);
        TRY(thisChar(pos, QChar('\\'), ps), choice1);
        QChar _c;
        TRY(someChar(pos, _c, ps), choice1);
        c = unescape(_c);
        RETURN_SUCCESS(ps);
    }

choice1:
    {
        pos = cp0;
        QChar _c;
        EXPECT(someChar(pos, _c, ps));
        c = _c;
    }
    RETURN_SUCCESS(ps);
}

void QMetaParser::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    m_rule[GRAMMAR] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::grammar);
    m_rule[RULE] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::rule);
    m_rule[CHOICES] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::choices);
    m_rule[CHOICE] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::choice);
    m_rule[HOST_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::hostExpr);
    m_rule[TERM] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::term);
    m_rule[TERM1] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::term1);
    m_rule[TERM2] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::term2);
    m_rule[SOME_TOKEN] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::someToken);
#pragma GCC diagnostic pop
}
