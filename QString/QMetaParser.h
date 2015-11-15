#ifndef QMETAPARSER_H
#define QMETAPARSER_H

#include <QString>
#include <QHash>
#include "QMetaQStringToQVariantTransformer.h"

class QMetaParser : public QMetaQStringToQVariantTransformer
{
public:
    QMetaParser(int ruleId, const QString &inp);
    bool parse(QVariant& ast);
    bool parse(int ruleId, int pos, QVariant& ast, ParseErrorPtr &pe) override;

    enum RuleEnum {
        GRAMMAR = NEXT_RULE,
        RULES,
        RULE,
        CHOICES,
        CHOICE,
        HOST_EXPR,
        TERM,
        TERM1,
        TERM2,
        SOME_TOKEN,
        ESCAPED_CHAR,
    };

protected:
    bool escapedChar(int &pos, QChar& c, ParseErrorPtr& pe);

    //Private rules.
private:
    //Use applyRule() to access these functions
    bool grammar(int& pos, QVariant& ast, ParseErrorPtr& pe);
    bool rules(int& pos, QVariant& ast, ParseErrorPtr& pe);
    bool rule(int& pos, QVariant& ast, ParseErrorPtr& pe);
    bool choices(int& pos, QVariant& ast, ParseErrorPtr& pe);
    bool choice(int& pos, QVariant& ast, ParseErrorPtr& pe);
    bool hostExpr(int& pos, QVariant& ast, ParseErrorPtr& pe);
    bool term(int &pos, QVariant& ast, ParseErrorPtr& pe);
    bool term1(int &pos, QVariant& ast, ParseErrorPtr& pe);
    bool term2(int &pos, QVariant& ast, ParseErrorPtr& pe);
    bool someToken(int &pos, QVariant& ast, ParseErrorPtr& pe);

    //Private helpers
private:
    void initRuleMap();
};

#endif // QMETAPARSER_H
