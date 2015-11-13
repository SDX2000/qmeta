#ifndef QMETAPARSERGENERATOR_H
#define QMETAPARSERGENERATOR_H

#include "QMetaQVariantTransformerBase.h"

class QMetaQVariantTransformer : public QMetaQVariantTransformerBase
{
public:
    QMetaQVariantTransformer(int ruleId, const QVariant& input);
    bool parse(QString& output) override;
    const ParseError *getError() const;

    enum RuleEnum {
        GRAMMAR = NEXT_RULE,
        RULE,
        EXPR,
        COMPOUND_EXPR,
        NOT_EXPR,
        LOOP_EXPR,
        OPTIONAL_EXPR,
        VAR_DEF,
        HOST_EXPR,
        RULE_APP,
    };

private:
    //Use applyRule() to access these functions
    bool grammar(QVariantConstPtr input, QString& output, ParseErrorPtr& pe);
    bool rule(QVariantConstPtr input, QString& output, ParseErrorPtr& pe);
    bool expr(QVariantConstPtr input, QString& output, ParseErrorPtr& pe);
    bool compoundExpr(QVariantConstPtr input, QString& output, ParseErrorPtr& pe);
    bool notExpr(QVariantConstPtr input, QString& output, ParseErrorPtr& pe);
    bool loopExpr(QVariantConstPtr input, QString& output, ParseErrorPtr& pe);
    bool optionalExpr(QVariantConstPtr input, QString& output, ParseErrorPtr& pe);
    bool varDef(QVariantConstPtr input, QString& output, ParseErrorPtr& pe);
    bool hostExpr(QVariantConstPtr input, QString& output, ParseErrorPtr& pe);
    bool ruleApp(QVariantConstPtr input, QString& output, ParseErrorPtr& pe);

private:
    void initRuleMap();
};

#endif // QMETAPARSERGENERATOR_H
