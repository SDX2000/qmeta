#ifndef PROGRAMINTERPRETER_H
#define PROGRAMINTERPRETER_H

#include <QString>
#include <QHash>
#include "qmeta_parser_base.h"

class QMetaParser : public QMetaParserBase
{
public:
    QMetaParser();
    bool parse(int ruleId, QString inp, QVariant& ast);
    bool parse(int ruleId, int pos, QVariant& ast) override;

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
    bool escapedChar(int &pos, QChar& c);

private:
    //Use applyRule() to access these functions
    bool grammar(int& pos, QVariant& ast);
    bool rules(int& pos, QVariant& ast);
    bool rule(int& pos, QVariant& ast);
    bool choices(int& pos, QVariant& ast);
    bool choice(int& pos, QVariant& ast);
    bool hostExpr(int& pos, QVariant& ast);
    bool term(int &pos, QVariant& ast);
    bool term1(int &pos, QVariant& ast);
    bool term2(int &pos, QVariant& ast);
    bool someToken(int &pos, QVariant& ast);

private:
    void initRuleMap();
};

#endif // PROGRAMINTERPRETER_H
