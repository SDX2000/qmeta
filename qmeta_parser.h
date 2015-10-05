#ifndef PROGRAMINTERPRETER_H
#define PROGRAMINTERPRETER_H

#include <QString>
#include <QHash>
#include "qmeta_parser_base.h"

class QMetaParser : public QMetaParserBase
{
public:
    ParseStatusPtr parse(QStringRef inp, QVariant& ast) override;
    ParseStatusPtr parse(QString inp, QVariant& ast);
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
    ParseStatusPtr grammar(QStringRef& inp, QVariant& ast);
    ParseStatusPtr rule(QStringRef& inp, QVariant& ast);
    ParseStatusPtr choices(QStringRef& inp, QVariant& ast);
    ParseStatusPtr choice(QStringRef& inp, QVariant& ast);
    ParseStatusPtr hostExpr(QStringRef& inp, QVariant& ast);
    ParseStatusPtr term(QStringRef &inp, QVariant& ast);
    ParseStatusPtr term1(QStringRef &inp, QVariant& ast);
    ParseStatusPtr term2(QStringRef &inp, QVariant& ast);
    ParseStatusPtr someToken(QStringRef &inp, QVariant& ast);
    ParseStatusPtr escapedChar(QStringRef &inp, QChar& c);
};

#endif // PROGRAMINTERPRETER_H
