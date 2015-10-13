#ifndef PROGRAMINTERPRETER_H
#define PROGRAMINTERPRETER_H

#include <QString>
#include <QHash>
#include "qmeta_parser_base.h"

class QMetaParser : public QMetaParserBase
{
public:
    QMetaParser();
    bool parse(int ruleId, QString inp, QVariant& ast, ParseErrorTrail &ps);
    bool parse(int ruleId, int pos, QVariant& ast, ParseErrorTrail &ps) override;

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
    bool escapedChar(int &pos, QChar& c, ParseErrorTrail &ps);

private:
    //Use applyRule() to access these functions
    bool grammar(int& pos, QVariant& ast, ParseErrorTrail &ps);
    bool rules(int& pos, QVariant& ast, ParseErrorTrail &ps);
    bool rule(int& pos, QVariant& ast, ParseErrorTrail &ps);
    bool choices(int& pos, QVariant& ast, ParseErrorTrail &ps);
    bool choice(int& pos, QVariant& ast, ParseErrorTrail &ps);
    bool hostExpr(int& pos, QVariant& ast, ParseErrorTrail &ps);
    bool term(int &pos, QVariant& ast, ParseErrorTrail &ps);
    bool term1(int &pos, QVariant& ast, ParseErrorTrail &ps);
    bool term2(int &pos, QVariant& ast, ParseErrorTrail &ps);
    bool someToken(int &pos, QVariant& ast, ParseErrorTrail &ps);

private:
    void initRuleMap();
};

#endif // PROGRAMINTERPRETER_H
