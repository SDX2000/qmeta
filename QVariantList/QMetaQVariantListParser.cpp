#include "QMetaQVariantListParser.h"
#include "QVariantList/macros.h"

QMetaQVariantListParser::QMetaQVariantListParser(int ruleId, const QVariant &input)
    : QMetaQVariantListParserBase(ruleId, input)
{
    initRuleMap();
}

bool QMetaQVariantListParser::parse(QVariant &output)
{
    return applyRule(m_startRuleId, m_input, output, m_error);
}

const ParseError *QMetaQVariantListParser::getError() const
{
    return m_error;
}

bool QMetaQVariantListParser::grammar(QVariant &input, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(input.type() == QVariant::List);

    {
        auto l = input.value<QVariantList>();

        EXPECT(l.count() == 3);

        EXPECT(thisStr(l[0], "GRAMMAR", cpe));

        {
            QVariant str;
            EXPECT(string(l[1], str, cpe));

            auto inp2 = l[2].value<QVariantList>();

            QVariantList rules;

            foreach(QVariant inp, inp2) {
                QVariant rule;
                EXPECT(applyRule(RULE, inp, rule, cpe));
                rules.append(rule);
            }
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantListParser::rule(QVariant &input, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(input.type() == QVariant::List);

    {
        auto l = input.value<QVariantList>();

        EXPECT(l.count() == 3);

        EXPECT(thisStr(l[0], "RULE", cpe));

        {
            QVariant str;
            EXPECT(string(l[1], str, cpe));

            auto inp2 = l[2].value<QVariantList>();

            QVariantList exprs;

            foreach(QVariant inp, inp2) {
                QVariant expr;
                EXPECT(applyRule(EXPR, inp, expr, cpe));
                exprs.append(expr);
            }
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantListParser::expr(QVariant &input, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(input);

    TRY_CHOICE(applyRule(RULE_APP, input, output, pe));
    TRY_CHOICE(applyRule(BOOL_EXPR, input, output, pe));
    TRY_CHOICE(applyRule(LOOP_EXPR, input, output, pe));
    TRY_CHOICE(applyRule(OPTIONAL_EXPR, input, output, pe));
    TRY_CHOICE(applyRule(VAR_DEF, input, output, pe));
    TRY_CHOICE(applyRule(HOST_EXPR, input, output, pe));

    RETURN_FAILURE();

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantListParser::boolExpr(QVariant &input, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(input.type() == QVariant::List);

    {
        auto l = input.value<QVariantList>();

        EXPECT(l.count() == 2);

        TRY(thisStr(l[0], "OR", cpe), choice1);
choice1:
        TRY(thisStr(l[0], "AND", cpe), choice2);
choice2:
        EXPECT(thisStr(l[0], "NOT", cpe));

        {
            auto inp1 = l[1].value<QVariantList>();

            QVariantList exprs;

            foreach(QVariant inp, inp1) {
                QVariant expr;
                EXPECT(applyRule(EXPR, inp, expr, cpe));
                exprs.append(expr);
            }
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantListParser::loopExpr(QVariant &input, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(input.type() == QVariant::List);

    {
        auto l = input.value<QVariantList>();

        EXPECT(l.count() == 2);

        TRY(thisStr(l[0], "ZERO_OR_MORE", cpe), choice1);
choice1:
        EXPECT(thisStr(l[0], "ONE_OR_MORE", cpe));

        {
            auto inp1 = l[1].value<QVariantList>();

            QVariantList exprs;

            foreach(QVariant inp, inp1) {
                QVariant expr;
                EXPECT(applyRule(EXPR, inp, expr, cpe));
                exprs.append(expr);
            }
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantListParser::optionalExpr(QVariant &input, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(input.type() == QVariant::List);

    {
        auto l = input.value<QVariantList>();

        EXPECT(l.count() == 2);

        EXPECT(thisStr(l[0], "OPTIONAL", cpe));

        {
            auto inp1 = l[1].value<QVariantList>();

            QVariantList exprs;

            foreach(QVariant inp, inp1) {
                QVariant expr;
                EXPECT(applyRule(EXPR, inp, expr, cpe));
                exprs.append(expr);
            }
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantListParser::varDef(QVariant &input, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(input.type() == QVariant::List);

    {
        auto l = input.value<QVariantList>();

        EXPECT(l.count() == 3);

        EXPECT(thisStr(l[0], "DEFINE", cpe));

        {
            QVariant name;
            EXPECT(string(l[1], name, cpe));

            {
                auto inp2 = l[2].value<QVariantList>();

                QVariantList exprs;

                foreach(QVariant inp, inp2) {
                    QVariant expr;
                    EXPECT(applyRule(EXPR, inp, expr, cpe));
                    exprs.append(expr);
                }
            }
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantListParser::hostExpr(QVariant &input, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(input.type() == QVariant::List);

    {
        auto l = input.value<QVariantList>();

        EXPECT(l.count() == 3);

        EXPECT(thisStr(l[0], "HOST_EXPR", cpe));

        {
            {
                auto inp1 = l[1].value<QVariantList>();

                QVariantList exprs;

                foreach(QVariant inp, inp1) {
                    QVariant expr;
                    EXPECT(applyRule(EXPR, inp, expr, cpe));
                    exprs.append(expr);
                }
            }

            QVariant hexpr;
            EXPECT(string(l[2], hexpr, cpe));
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantListParser::ruleApp(QVariant &input, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(input.type() == QVariant::List);

    {
        auto l = input.value<QVariantList>();

        EXPECT(l.count() == 2);

        EXPECT(thisStr(l[0], "APPLY", cpe));

        {
            QVariant ruleName;
            EXPECT(string(l[1], ruleName, cpe));
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

void QMetaQVariantListParser::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    m_rule[GRAMMAR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParser::grammar);
    m_rule[RULE] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParser::rule);
    m_rule[EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParser::expr);
    m_rule[BOOL_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParser::boolExpr);
    m_rule[LOOP_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParser::loopExpr);
    m_rule[OPTIONAL_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParser::optionalExpr);
    m_rule[VAR_DEF] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParser::varDef);
    m_rule[HOST_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParser::hostExpr);
    m_rule[RULE_APP] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParser::ruleApp);
#pragma GCC diagnostic pop
}
