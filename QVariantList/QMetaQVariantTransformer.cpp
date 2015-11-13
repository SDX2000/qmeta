#include "QMetaQVariantTransformer.h"
#include "QVariantList/macros.h"

QMetaQVariantTransformer::QMetaQVariantTransformer(int ruleId, const QVariant& input)
    : QMetaQVariantToQStringTransformer(ruleId, input)
{
    initRuleMap();
}

bool QMetaQVariantTransformer::parse(QString& output)
{
    return applyRule(m_startRuleId, &m_input, output, m_error);
}

const ParseError *QMetaQVariantTransformer::getError() const
{
    return m_error;
}

bool QMetaQVariantTransformer::grammar(QVariantConstPtr input, QString& output, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::List);

    {
        const QVariantList& l = input->value<QVariantList>();

        EXPECT(l.count() == 3);

        EXPECT(thisStr(&l[0], "GRAMMAR", cpe));

        {
            QVariant str;
            EXPECT(string(&l[1], str, cpe));

            const QVariantList& inp2 = l[2].value<QVariantList>();

            QVariantList rules;

            foreach(const QVariant& inp, inp2) {
                QString rule;
                EXPECT(applyRule(RULE, &inp, rule, cpe));
                rules.append(rule);
            }
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantTransformer::rule(QVariantConstPtr input, QString& output, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::List);

    {
        const QVariantList& l = input->value<QVariantList>();

        EXPECT(l.count() == 3);

        EXPECT(thisStr(&l[0], "RULE", cpe));

        {
            QVariant str;
            EXPECT(string(&l[1], str, cpe));

            QString expr;
            EXPECT(applyRule(EXPR, &l[2], expr, cpe));
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantTransformer::expr(QVariantConstPtr input, QString& output, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    TRY_CHOICE(applyRule(COMPOUND_EXPR, input, output, pe));
    TRY_CHOICE(applyRule(RULE_APP, input, output, pe));
    TRY_CHOICE(applyRule(NOT_EXPR, input, output, pe));
    TRY_CHOICE(applyRule(LOOP_EXPR, input, output, pe));
    TRY_CHOICE(applyRule(OPTIONAL_EXPR, input, output, pe));
    TRY_CHOICE(applyRule(VAR_DEF, input, output, pe));
    TRY_CHOICE(applyRule(HOST_EXPR, input, output, pe));

    RETURN_FAILURE();

    EXITV(output);
}

bool QMetaQVariantTransformer::compoundExpr(QVariantConstPtr input, QString& output, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::List);

    {
        const QVariantList& l = input->value<QVariantList>();

        EXPECT(l.count() >= 2);

        EXPECT(thisStr(&l[0], "AND", cpe) || thisStr(&l[0], "OR", cpe));

        {
            QVariantList exprs;

            for(int i = 1; i < l.length(); i++) {
                QString expr;
                EXPECT(applyRule(EXPR, &l[i], expr, cpe));
                exprs.append(expr);
            }
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantTransformer::notExpr(QVariantConstPtr input, QString& output, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::List);

    {
        const QVariantList& l = input->value<QVariantList>();

        EXPECT(l.count() == 2);

        EXPECT(thisStr(&l[0], "NOT", cpe));

        {
            QString expr;
            EXPECT(applyRule(EXPR, &l[1], expr, cpe));
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantTransformer::loopExpr(QVariantConstPtr input, QString& output, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::List);

    {
        const QVariantList& l = input->value<QVariantList>();

        EXPECT(l.count() == 2);

        EXPECT(thisStr(&l[0], "ZERO_OR_MORE", cpe) || thisStr(&l[0], "ONE_OR_MORE", cpe));

        {
            QString expr;
            EXPECT(applyRule(EXPR, &l[1], expr, cpe));
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantTransformer::optionalExpr(QVariantConstPtr input, QString& output, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::List);

    {
        const QVariantList& l = input->value<QVariantList>();

        EXPECT(l.count() == 2);

        EXPECT(thisStr(&l[0], "OPTIONAL", cpe));

        {
            QString expr;
            EXPECT(applyRule(EXPR, &l[1], expr, cpe));
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantTransformer::varDef(QVariantConstPtr input, QString& output, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::List);

    {
        const QVariantList& l = input->value<QVariantList>();

        EXPECT(l.count() == 3);

        EXPECT(thisStr(&l[0], "DEFINE", cpe));

        {
            QVariant name;
            EXPECT(string(&l[1], name, cpe));

            {
                QString expr;
                EXPECT(applyRule(EXPR, &l[2], expr, cpe));
            }
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantTransformer::hostExpr(QVariantConstPtr input, QString& output, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::List);

    {
        const QVariantList& l = input->value<QVariantList>();

        EXPECT(l.count() == 3);

        EXPECT(thisStr(&l[0], "HOST_EXPR", cpe));

        {
            {
                QString expr;
                EXPECT(applyRule(EXPR, &l[1], expr, cpe));
            }

            QVariant hexpr;
            EXPECT(string(&l[2], hexpr, cpe));
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

bool QMetaQVariantTransformer::ruleApp(QVariantConstPtr input, QString& output, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::List);

    {
        const QVariantList& l = input->value<QVariantList>();

        EXPECT(l.count() >= 2);

        EXPECT(thisStr(&l[0], "APPLY", cpe));

        {
            QVariant ruleName;
            EXPECT(string(&l[1], ruleName, cpe));

            QVariant ruleArgs = l.mid(2);
        }
    }

    RETURN_SUCCESS();
    EXITV(output);
}

void QMetaQVariantTransformer::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    m_rule[GRAMMAR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantTransformer::grammar);
    m_rule[RULE] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantTransformer::rule);
    m_rule[EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantTransformer::expr);
    m_rule[COMPOUND_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantTransformer::compoundExpr);
    m_rule[NOT_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantTransformer::notExpr);
    m_rule[LOOP_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantTransformer::loopExpr);
    m_rule[OPTIONAL_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantTransformer::optionalExpr);
    m_rule[VAR_DEF] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantTransformer::varDef);
    m_rule[HOST_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantTransformer::hostExpr);
    m_rule[RULE_APP] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantTransformer::ruleApp);
#pragma GCC diagnostic pop
}
