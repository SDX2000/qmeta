#ifndef QMETAPARSERGENERATOR_H
#define QMETAPARSERGENERATOR_H

#include "QMetaQVariantListParserBase.h"

class QMetaQVariantListParser : public QMetaQVariantListParserBase
{
public:
    QMetaQVariantListParser(int ruleId, const QVariant &input);
    bool parse(QVariant& output) override;
    const ParseError *getError() const;

    enum RuleEnum {
        GRAMMAR = NEXT_RULE,
        RULE,
        EXPR,
        BOOL_EXPR,
        LOOP_EXPR,
        OPTIONAL_EXPR,
        VAR_DEF,
        HOST_EXPR,
        RULE_APP,
    };

private:
    //Use applyRule() to access these functions
    bool grammar(QVariant &input, QVariant& output, ParseErrorPtr& pe);
    bool rule(QVariant &input, QVariant& output, ParseErrorPtr& pe);
    bool expr(QVariant &input, QVariant& output, ParseErrorPtr& pe);
    bool boolExpr(QVariant &input, QVariant& output, ParseErrorPtr& pe);
    bool loopExpr(QVariant &input, QVariant& output, ParseErrorPtr& pe);
    bool optionalExpr(QVariant &input, QVariant& output, ParseErrorPtr& pe);
    bool varDef(QVariant &input, QVariant& output, ParseErrorPtr& pe);
    bool hostExpr(QVariant &input, QVariant& output, ParseErrorPtr& pe);
    bool ruleApp(QVariant &input, QVariant& output, ParseErrorPtr& pe);

private:
    void initRuleMap();
};

#endif // QMETAPARSERGENERATOR_H
