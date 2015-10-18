#ifndef QMETAPARSERGENERATOR_H
#define QMETAPARSERGENERATOR_H

#include "QMetaQVariantListParserBase.h"

class QMetaParserGenerator : public QMetaQVariantListParserBase
{
public:
    QMetaParserGenerator();

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
    bool grammar(int& pos, QVariant& output, ParseErrorPtr& pe);
    bool rule(int& pos, QVariant& output, ParseErrorPtr& pe);
    bool _and(int& pos, QVariant& output, ParseErrorPtr& pe);
    bool _or(int& pos, QVariant& output, ParseErrorPtr& pe);
    bool _not(int& pos, QVariant& output, ParseErrorPtr& pe);
    bool apply(int& pos, QVariant& output, ParseErrorPtr& pe);
    bool optional(int& pos, QVariant& output, ParseErrorPtr& pe);
    bool zeroOrMore(int& pos, QVariant& output, ParseErrorPtr& pe);
    bool oneOrMore(int& pos, QVariant& output, ParseErrorPtr& pe);
    bool hostExpr(int& pos, QVariant& output, ParseErrorPtr& pe);
    bool anything(int& pos, QVariant& output, ParseErrorPtr& pe);

private:
    void initRuleMap();
};

#endif // QMETAPARSERGENERATOR_H
