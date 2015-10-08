#ifndef PROGRAMINTERPRETER_H
#define PROGRAMINTERPRETER_H

#include <QString>
#include <QHash>
#include "qmeta_parser_base.h"

class QMetaParser : public QMetaParserBase
{
public:
    QMetaParser();
    bool parse(QString inp, QVariant& ast, ParseStatusPtr &ps);
    bool parse(int pos, QVariant& ast, ParseStatusPtr &ps) override;
protected:
    enum RuleEnum {
        GRAMMAR = NEXT_RULE,
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
    bool escapedChar(int &pos, QChar& c, ParseStatusPtr &ps);

private:
    //Use applyRule() to access these functions
    bool grammar(int& pos, QVariant& ast, ParseStatusPtr &ps);
    bool rule(int& pos, QVariant& ast, ParseStatusPtr &ps);
    bool choices(int& pos, QVariant& ast, ParseStatusPtr &ps);
    bool choice(int& pos, QVariant& ast, ParseStatusPtr &ps);
    bool hostExpr(int& pos, QVariant& ast, ParseStatusPtr &ps);
    bool term(int &pos, QVariant& ast, ParseStatusPtr &ps);
    bool term1(int &pos, QVariant& ast, ParseStatusPtr &ps);
    bool term2(int &pos, QVariant& ast, ParseStatusPtr &ps);
    bool someToken(int &pos, QVariant& ast, ParseStatusPtr &ps);

private:
    void initRuleMap();
};

#endif // PROGRAMINTERPRETER_H
