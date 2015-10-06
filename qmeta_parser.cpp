#include "qmeta_parser.h"
#include <iostream>

ParseStatusPtr QMetaParser::parse(QString inp, QVariant& ast)
{
    return QMetaParserBase::parse(inp, ast);
}

QMetaParser::QMetaParser()
{
    initRuleMap();
}

ParseStatusPtr QMetaParser::parse(QStringRef inp, QVariant &ast)
{
    ENTER();
    return applyRule(GRAMMAR, inp, ast);
}

ParseStatusPtr QMetaParser::grammar(QStringRef& inp, QVariant &ast)
{
    ENTER();
    QList<QVariant> l;

    QVariant _ast;
    while (applyRule(RULE, inp, _ast)) {
        EXPECT(thisToken(inp, QSL(";")));
        l.append(_ast);
    }

    ast = l;

    return ParseStatus::success();
}

ParseStatusPtr QMetaParser::rule(QStringRef &inp, QVariant &ast)
{
    ENTER();
    QList<QVariant> l;
    l.append(QString(QSL("RULE")));
    QVariant id;
    EXPECT(applyRule(IDENTIFIER, inp, id));
    l.append(id);

    EXPECT(thisToken(inp, QSL("=")));

    QVariant _ast;
    EXPECT(applyRule(CHOICES, inp, _ast));
    l.append(_ast);

    ast = l;

    return ParseStatus::success();
}

ParseStatusPtr QMetaParser::choices(QStringRef &inp, QVariant &ast)
{
    ENTER();
    CHECK_POINT(cp0, inp);
    {
        inp = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("OR")));

        QVariant _ast;
        TRY(applyRule(CHOICE, inp, _ast), choice1);
        l.append(_ast);

        TRY(thisToken(inp, QSL("|")), choice1);
        TRY(applyRule(CHOICES, inp, _ast), choice1);

        ast = l;
        return ParseStatus::success();
    }

choice1:
    {
        inp = cp0;
        QVariant _ast;
        EXPECT(applyRule(CHOICE, inp, _ast));
        ast = _ast;
    }

    return ParseStatus::success();
}

ParseStatusPtr QMetaParser::choice(QStringRef &inp, QVariant &ast)
{
    ENTER();
    QList<QVariant> l;

    QVariant _ast;
    while (applyRule(TERM, inp, _ast)) {
        l.append(_ast);
        if (thisToken(inp, QSL("->"))) {
            l.append(QString(QSL("HOSTEXPR")));
            QVariant _hostExpr;
            EXPECT(applyRule(HOST_EXPR, inp, _hostExpr));
            l.append(_hostExpr);
        }
    }

    ast = l;

    return ParseStatus::success();
}

ParseStatusPtr QMetaParser::hostExpr(QStringRef &inp, QVariant &ast)
{
    ENTER();
    spaces(inp);
    int count = 0;
    QChar c;
    QString hexpr;
    while(someChar(inp, c)) {
        if (c == QChar('{')) {
            count++;
        }
        if (c == QChar('}')) {
            count--;
        }
        hexpr += c;
        if(count == 0) {
            break;
        }
    }

    if(count) {
        return ParseStatus::failure(inp, QSL("Invalid host expression (unbalanced braces)."));
    }

    spaces(inp);

    ast = hexpr;

    return ParseStatus::success();
}

ParseStatusPtr QMetaParser::term(QStringRef &inp, QVariant &ast)
{
    ENTER();
    CHECK_POINT(cp0, inp);
    {
        inp = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("term")));

        QVariant _ast;
        TRY(applyRule(TERM1, inp, _ast), choice1);
        l.append(_ast);

        TRY(thisToken(inp, QSL(":")), choice1);

        QVariant id;
        TRY(applyRule(IDENTIFIER, inp, id), choice1);
        l.append(id);

        ast = l;
        return ParseStatus::success();
    }

choice1:
    {
        inp = cp0;
        QVariant _ast;
        EXPECT(applyRule(TERM1, inp, _ast));
        ast = _ast;
    }

    return ParseStatus::success();
}

ParseStatusPtr QMetaParser::term1(QStringRef &inp, QVariant &ast)
{
    ENTER();
    CHECK_POINT(cp0, inp);
    {
        inp = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("NOT")));

        TRY(thisToken(inp, QSL("~")), choice1);

        QVariant _ast;
        TRY(applyRule(TERM2, inp, _ast), choice1);
        l.append(_ast);

        ast = l;
        return ParseStatus::success();
    }

choice1:
    {
        inp = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("PRED")));

        TRY(thisToken(inp, QSL("?")), choice2);

        QVariant _ast;
        TRY(applyRule(HOST_EXPR, inp, _ast), choice2);
        l.append(_ast);

        ast = l;
        return ParseStatus::success();
    }

choice2:
    {
        inp = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("MANY")));

        QVariant _ast;
        TRY(applyRule(TERM2, inp, _ast), choice3);
        l.append(_ast);

        TRY(thisToken(inp, QSL("*")), choice3);

        ast = l;
        return ParseStatus::success();
    }

choice3:
    {
        inp = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("MANY1")));

        QVariant _ast;
        TRY(applyRule(TERM2, inp, _ast), choice4);
        l.append(_ast);

        TRY(thisToken(inp, QSL("+")), choice4);

        ast = l;
        return ParseStatus::success();
    }

choice4:
    {
        inp = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("OPTIONAL")));

        QVariant _ast;
        TRY(applyRule(TERM2, inp, _ast), choice5);
        l.append(_ast);

        TRY(thisToken(inp, QSL("?")), choice5);

        ast = l;
        return ParseStatus::success();
    }

choice5:
    {
        inp = cp0;
        QVariant _ast;
        EXPECT(applyRule(TERM2, inp, _ast));
        ast = _ast;
    }

    return ParseStatus::success();
}

ParseStatusPtr QMetaParser::term2(QStringRef &inp, QVariant &ast)
{
    ENTER();
    CHECK_POINT(cp0, inp);
    {
        inp = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("CHAR")));

        TRY(thisChar(inp, QChar('\'')), choice1);

        CHECK_POINT(cp1, inp);
        TRY(thisChar(inp, QChar('\'')), choice1);
        inp = cp1;

        QChar c;
        TRY_INV(escapedChar(inp, c), choice1);
        l.append(c);

        TRY(thisChar(inp, QChar('\'')), choice1);

        ast = l;
        return ParseStatus::success();
    }

choice1:
    {
        inp = cp0;
        QVariant _ast;
        TRY(applyRule(SOME_TOKEN, inp, _ast), choice2);
        ast = _ast;
        return ParseStatus::success();
    }

choice2:
    {
        inp = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("APPLY")));

        QVariant ruleName;
        TRY(applyRule(IDENTIFIER, inp, ruleName), choice3);
        l.append(ruleName);

        ast = l;
        return ParseStatus::success();
    }

choice3:
    {
        inp = cp0;
        EXPECT(thisToken(inp, QSL("(")));

        QVariant _ast;
        EXPECT(applyRule(CHOICE, inp, _ast));

        EXPECT(thisToken(inp, QSL(")")));

        ast = _ast;
    }

    return ParseStatus::success();
}

ParseStatusPtr QMetaParser::someToken(QStringRef &inp, QVariant& ast)
{
    ENTER();
    spaces(inp);

    EXPECT(thisChar(inp, QChar('"')));

    QList<QVariant> l;
    l.append(QString(QSL("TOKEN")));


    QString token;

    while (true) {
        CHECK_POINT(cp0, inp);
        if(thisChar(inp, QChar('"'))) {
            break;
        }
        inp = cp0;
        QChar c;
        EXPECT(someChar(inp, c));
        token += c;
    }

    EXPECT(thisChar(inp, QChar('"')));

    spaces(inp);

    l.append(token);

    ast = l;

    return ParseStatus::success();
}

ParseStatusPtr QMetaParser::escapedChar(QStringRef &inp, QChar &c)
{
    ENTER();
    CHECK_POINT(cp0, inp);
    {
        inp = cp0;
        TRY(thisChar(inp, QChar('\\')), choice1);
        TRY(thisChar(inp, QChar('\\')), choice1);
        QChar _c;
        TRY(someChar(inp, _c), choice1);
        c = unescape(_c);
        return ParseStatus::success();
    }

choice1:
    {
        inp = cp0;
        QChar _c;
        EXPECT(someChar(inp, _c));
        c = _c;
    }
    return ParseStatus::success();
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
