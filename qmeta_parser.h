#ifndef PROGRAMINTERPRETER_H
#define PROGRAMINTERPRETER_H

#include <QString>
#include "qmeta_parser_base.h"

class QMetaParser : public QMetaParserBase
{
public:
    ParseStatusPtr parse(QStringRef inp, QVariant& ast);
    ParseStatusPtr parse(QString inp, QVariant &ast);
private:
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
