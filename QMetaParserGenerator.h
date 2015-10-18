#ifndef QMETAPARSERGENERATOR_H
#define QMETAPARSERGENERATOR_H

#include "QMetaQVariantListParserBase.h"

class QMetaParserGenerator : public QMetaQVariantListParserBase
{
public:
    QMetaParserGenerator(int ruleId, const QVariant &input);

    enum RuleEnum {
        GRAMMAR = NEXT_RULE,
        RULE,
        AND,
        OR,
        NOT,
        APPLY,
        OPTIONAL,
        ZERO_OR_MORE,
        ONE_OR_MORE,
        HOST_EXPR,
        ANYTHING,
    };

private:
    //Use applyRule() to access these functions
    bool grammar(QVariant &inp, QVariant& output, ParseErrorPtr& pe);
    bool rule(QVariant &inp, QVariant& output, ParseErrorPtr& pe);
    bool _and(QVariant &inp, QVariant& output, ParseErrorPtr& pe);
    bool _or(QVariant &inp, QVariant& output, ParseErrorPtr& pe);
    bool _not(QVariant &inp, QVariant& output, ParseErrorPtr& pe);
    bool apply(QVariant &inp, QVariant& output, ParseErrorPtr& pe);
    bool optional(QVariant &inp, QVariant& output, ParseErrorPtr& pe);
    bool zeroOrMore(QVariant &inp, QVariant& output, ParseErrorPtr& pe);
    bool oneOrMore(QVariant &inp, QVariant& output, ParseErrorPtr& pe);
    bool hostExpr(QVariant &inp, QVariant& output, ParseErrorPtr& pe);
    bool anything(QVariant &inp, QVariant& output, ParseErrorPtr& pe);

private:
    void initRuleMap();
};

#endif // QMETAPARSERGENERATOR_H
