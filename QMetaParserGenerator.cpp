#include "QMetaParserGenerator.h"

QMetaParserGenerator::QMetaParserGenerator(int ruleId, const QVariant &input)
    : QMetaQVariantListParserBase(ruleId, input)
{
    initRuleMap();
}

bool QMetaParserGenerator::grammar(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

bool QMetaParserGenerator::rule(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

bool QMetaParserGenerator::_and(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

bool QMetaParserGenerator::_or(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

bool QMetaParserGenerator::_not(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

bool QMetaParserGenerator::apply(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

bool QMetaParserGenerator::optional(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

bool QMetaParserGenerator::zeroOrMore(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

bool QMetaParserGenerator::oneOrMore(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

bool QMetaParserGenerator::hostExpr(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

bool QMetaParserGenerator::anything(QVariant &inp, QVariant &output, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    RETURN_SUCCESS();

    EXITV(output);
}

void QMetaParserGenerator::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
//    m_rule[GRAMMAR] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::grammar);
//    m_rule[RULE] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::rule);
//    m_rule[AND] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::_and);
//    m_rule[OR] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::_or);
//    m_rule[NOT] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::_not);
//    m_rule[APPLY] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::apply);
//    m_rule[OPTIONAL] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::optional);
//    m_rule[ZERO_OR_MORE] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::zeroOrMore);
//    m_rule[ONE_OR_MORE] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::oneOrMore);
//    m_rule[HOST_EXPR] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::hostExpr);
//    m_rule[ANYTHING] = reinterpret_cast<RuleFuncPtr>(&QMetaParserGenerator::anything);
#pragma GCC diagnostic pop
}
