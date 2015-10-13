#include "qmeta_parser.h"


bool QMetaParser::parse(int ruleId, QString inp, QVariant& ast)
{
    return QMetaParserBase::parse(ruleId, inp, ast);
}

QMetaParser::QMetaParser()
{
    initRuleMap();
}

bool QMetaParser::parse(int ruleId, int pos, QVariant &ast)
{
    ENTRYV(pos);

    EXPECT(applyRule(ruleId, pos, ast));

    EXITV(ast);
}

bool QMetaParser::rules(int& pos, QVariant &ast)
{
    ENTRYV(pos);

    QList<QVariant> l;

    QVariant _ast;

    EXPECT(applyRule(RULE, pos, _ast));

    do {
        EXPECT(thisToken(pos, QSL(";")));
        l.append(_ast);
    } while (applyRule(RULE, pos, _ast));

    ast = l;

    RETURN_SUCCESS();

    EXITV(ast);
}

bool QMetaParser::grammar(int &pos, QVariant &ast)
{
    ENTRYV(pos);

    QList<QVariant> l;
    l.append(QString(QSL("GRAMMAR")));

    EXPECT(thisToken(pos, "qmeta"));

    {
        QVariant id;
        EXPECT(applyRule(IDENTIFIER, pos, id));
        l.append(id);
    }

    EXPECT(thisToken(pos, "{"));

    {
        QVariant _ast;
        EXPECT(applyRule(RULES, pos, _ast));
        l.append(_ast);
    }

    EXPECT(thisToken(pos, "}"));

    ast = l;

    RETURN_SUCCESS();

    EXITV(ast);
}

bool QMetaParser::rule(int &pos, QVariant &ast)
{
    ENTRYV(pos);

    QList<QVariant> l;
    l.append(QString(QSL("RULE")));

    {
        QVariant id;
        EXPECT(applyRule(IDENTIFIER, pos, id));
        l.append(id);
    }

    {
        EXPECT(thisToken(pos, QSL("=")));
    }

    {
        QVariant _ast;
        EXPECT(applyRule(CHOICES, pos, _ast));
        l.append(_ast);
    }

    ast = l;

    RETURN_SUCCESS();

    EXITV(ast);
}

bool QMetaParser::choices(int &pos, QVariant &ast)
{
    ENTRYV(pos);

    CHECK_POINT(cp0, pos);
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("OR")));

        QVariant _ast;
        TRY(applyRule(CHOICE, pos, _ast), choice1);
        l.append(_ast);

        TRY(thisToken(pos, QSL("|")), choice1);
        TRY(applyRule(CHOICES, pos, _ast), choice1);
        l.append(_ast);

        ast = l;
        RETURN_SUCCESS();
    }

choice1:
    {
        pos = cp0;
        QVariant _ast;
        EXPECT(applyRule(CHOICE, pos, _ast));
        ast = _ast;
    }

    RETURN_SUCCESS();

    EXITV(ast);
}

bool QMetaParser::choice(int &pos, QVariant &ast)
{
    ENTRYV(pos);

    QList<QVariant> l;

    QVariant _ast;
    while (applyRule(TERM, pos, _ast)) {
        spaces(pos);
        l.append(_ast);
    }

    if(l.length() <= 0) {
        RETURN_FAILURE("choice failed");
    }

    if (thisToken(pos, QSL("->"))) {
        l.append(QString(QSL("HOSTEXPR")));
        QVariant _hostExpr;
        EXPECT(applyRule(HOST_EXPR, pos, _hostExpr));
        l.append(_hostExpr);
    }

    ast = l;

    RETURN_SUCCESS();

    EXITV(ast);
}

bool QMetaParser::hostExpr(int &pos, QVariant &ast)
{
    ENTRYV(pos);

    spaces(pos);
    int count = 0;
    QChar c;
    QString hostexpr;

    while(someChar(pos, c)) {
        if (c == QChar('{')) {
            count++;
            if(count == 1) {
                continue;
            }
        }
        if (c == QChar('}')) {
            count--;
            if(count == 0) {
                break;
            }
        }
        hostexpr += c;
    }

    if(count) {
        RETURN_FAILURE(QSL("Invalid host expression (unbalanced braces)."));
    }

    spaces(pos);

    ast = hostexpr;

    RETURN_SUCCESS();

    EXITV(ast);
}

bool QMetaParser::term(int &pos, QVariant &ast)
{
    ENTRYV(pos);

    CHECK_POINT(cp0, pos);
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("term")));

        QVariant _ast;
        TRY(applyRule(TERM1, pos, _ast), choice1);
        l.append(_ast);

        TRY(thisToken(pos, QSL(":")), choice1);

        QVariant id;
        TRY(applyRule(IDENTIFIER, pos, id), choice1);
        l.append(id);

        ast = l;
        RETURN_SUCCESS();
    }

choice1:
    {
        pos = cp0;
        QVariant _ast;
        EXPECT(applyRule(TERM1, pos, _ast));
        ast = _ast;
    }

    RETURN_SUCCESS();

    EXITV(ast);
}

bool QMetaParser::term1(int &pos, QVariant &ast)
{
    ENTRYV(pos);

    CHECK_POINT(cp0, pos);
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("NOT")));

        TRY(thisToken(pos, QSL("~")), choice1);

        QVariant _ast;
        TRY(applyRule(TERM2, pos, _ast), choice1);
        l.append(_ast);

        ast = l;
        RETURN_SUCCESS();
    }

choice1:
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("REPEAT{0,}")));

        QVariant _ast;
        TRY(applyRule(TERM2, pos, _ast), choice2);
        l.append(_ast);

        TRY(thisToken(pos, QSL("*")), choice2);

        ast = l;
        RETURN_SUCCESS();
    }

choice2:
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("REPEAT{1,}")));

        QVariant _ast;
        TRY(applyRule(TERM2, pos, _ast), choice3);
        l.append(_ast);

        TRY(thisToken(pos, QSL("+")), choice3);

        ast = l;
        RETURN_SUCCESS();
    }

choice3:
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("OPTIONAL")));

        QVariant _ast;
        TRY(applyRule(TERM2, pos, _ast), choice4);
        l.append(_ast);

        TRY(thisToken(pos, QSL("?")), choice4);

        ast = l;
        RETURN_SUCCESS();
    }

choice4:
    {
        pos = cp0;
        QVariant _ast;
        EXPECT(applyRule(TERM2, pos, _ast));
        ast = _ast;
    }

    RETURN_SUCCESS();

    EXITV(ast);
}

bool QMetaParser::term2(int &pos, QVariant &ast)
{
    ENTRYV(pos);

    CHECK_POINT(cp0, pos);
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("CHAR")));

        TRY(thisChar(pos, QChar('\'')), choice1);

        CHECK_POINT(cp1, pos);
        TRY_INV(thisChar(pos, QChar('\'')), choice1);
        pos = cp1;

        QChar c;
        TRY(escapedChar(pos, c), choice1);
        l.append(c);

        TRY(thisChar(pos, QChar('\'')), choice1);

        ast = l;
        RETURN_SUCCESS();
    }

choice1:
    {
        pos = cp0;
        QVariant _ast;
        TRY(applyRule(SOME_TOKEN, pos, _ast), choice2);
        ast = _ast;
        RETURN_SUCCESS();
    }

choice2:
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("APPLY")));

        QVariant ruleName;
        TRY(applyRule(IDENTIFIER, pos, ruleName), choice3);
        l.append(ruleName);

        ast = l;
        RETURN_SUCCESS();
    }

choice3:
    {
        pos = cp0;
        QList<QVariant> l;
        l.append(QString(QSL("ANYTHING")));

        QVariant val;
        TRY(thisChar(pos, QChar('.')), choice4);
        l.append(val);

        ast = l;
        RETURN_SUCCESS();
    }

choice4:
    {
        pos = cp0;
        EXPECT(thisToken(pos, QSL("(")));

        QVariant _ast;
        EXPECT(applyRule(CHOICE, pos, _ast));

        EXPECT(thisToken(pos, QSL(")")));

        ast = _ast;
    }

    RETURN_SUCCESS();

    EXITV(ast);
}

bool QMetaParser::someToken(int &pos, QVariant& ast)
{
    ENTRYV(pos);

    spaces(pos);

    QList<QVariant> l;
    l.append(QString(QSL("TOKEN")));

    EXPECT(thisChar(pos, QChar('"')));

    {
        QString token;

        while (true) {
            CHECK_POINT(cp0, pos);
            if(thisChar(pos, QChar('"'))) {
                pos = cp0;
                break;
            }
            pos = cp0;
            QChar c;
            EXPECT(someChar(pos, c));
            token += c;
        }

        EXPECT(thisChar(pos, QChar('"')));
        l.append(token);
    }

    spaces(pos);

    ast = l;

    RETURN_SUCCESS();

    EXITV(ast);
}

bool QMetaParser::escapedChar(int &pos, QChar &c)
{
    ENTRYV(pos);

    CHECK_POINT(cp0, pos);

    QChar _c;

    {
        pos = cp0;
        TRY(thisChar(pos, QChar('\\')), choice1);
        TRY(someChar(pos, _c), choice1);
        c = unescape(_c);
        RETURN_SUCCESS();
    }

choice1:
    {
        pos = cp0;
        QChar _c;
        EXPECT(someChar(pos, _c));
        c = _c;
    }
    RETURN_SUCCESS();

    EXITV(c);
}

void QMetaParser::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    m_rule[GRAMMAR] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::grammar);
    m_rule[RULES] = reinterpret_cast<RuleFuncPtr>(&QMetaParser::rules);
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
