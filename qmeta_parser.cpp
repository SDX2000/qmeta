#include "qmeta_parser.h"


ParseStatusPtr QMetaParser::parse(QString inp, QVariant& ast)
{
    QStringRef  inpRef = inp.midRef(0);
    return parse(inpRef, ast);
}

ParseStatusPtr QMetaParser::parse(QStringRef inp, QVariant &ast)
{
    return grammar(inp, ast);
}

ParseStatusPtr QMetaParser::grammar(QStringRef& inp, QVariant &ast)
{
    return ParseStatus::success();
}

