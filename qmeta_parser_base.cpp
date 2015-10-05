#include "qmeta_parser_base.h"
#include <assert.h>
#include <algorithm>


/////////////////////  TERMINALS  //////////////////////

ParseStatusPtr QMetaParserBase::advance(QStringRef &inp, int count)
{
    EXPECT_B(inp.length() >= count);

    inp = inp.mid(count);

    return ParseStatus::success();
}

ParseStatusPtr QMetaParserBase::digit(QStringRef &inp, int& digit)
{
    EXPECT_B(!inp.isEmpty());

    QChar c = inp.at(0);

    EXPECT_B(c.isDigit());

    digit = c.digitValue();

    return advance(inp, 1);
}


ParseStatusPtr QMetaParserBase::someChar(QStringRef &inp, QChar& c)
{
    EXPECT_B(!inp.isEmpty());

    c = inp.at(0);

    return advance(inp, 1);
}

ParseStatusPtr QMetaParserBase::someCharOf(QStringRef &inp, bool (QChar::*is_x)() const)
{
    EXPECT_B(!inp.isEmpty());

    QChar ch = inp.at(0);

    EXPECT_B((ch.*is_x)());

    return advance(inp, 1);
}

ParseStatusPtr QMetaParserBase::someCharOf(QStringRef &inp, QChar &c, bool (QChar::*is_x)() const)
{
    EXPECT_B(!inp.isEmpty());

    QChar ch = inp.at(0);

    EXPECT_B((ch.*is_x)());

    c = ch;

    return advance(inp, 1);
}

ParseStatusPtr QMetaParserBase::oneOf(QStringRef& inp, QChar &opOut, QString chars)
{
    EXPECT_B(!inp.isEmpty());

    QChar c = inp.at(0);

    EXPECT_B(chars.contains(c));

    opOut = c;

    return advance(inp, 1);
}


ParseStatusPtr QMetaParserBase::thisChar(QStringRef &inp, QChar c)
{
    EXPECT_B(!inp.isEmpty());

    EXPECT_B(inp.at(0) == c);

    return advance(inp, 1);
}

ParseStatusPtr QMetaParserBase::thisStr(QStringRef &inp, QString str)
{
    EXPECT_B(inp.startsWith(str));

    return advance(inp, str.length());;
}

ParseStatusPtr QMetaParserBase::strOf(QStringRef &inp, bool (QChar::*is_x)() const)
{
    EXPECT_B(!inp.isEmpty());

    int count = 0;

    for (; count < inp.length() && (inp.at(count).*is_x)(); count++) {
    }

    EXPECT_B(count);

    return advance(inp, count);
}

/////////////////////  NONTERMINALS  //////////////////////

ParseStatusPtr QMetaParserBase::strOf(QStringRef &inp, QStringRef &str, bool (QChar::*is_x)() const)
{
    CHECK_POINT(cp0, inp);

    EXPECT(strOf(inp, is_x));

    str = mid(cp0, inp);

    return ParseStatus::success();
}

ParseStatusPtr QMetaParserBase::spaces(QStringRef &inp)
{
    return strOf(inp, &QChar::isSpace);
}

ParseStatusPtr QMetaParserBase::spaces(QStringRef &inp, QStringRef &space)
{
    return strOf(inp, space, &QChar::isSpace);
}

ParseStatusPtr QMetaParserBase::identifier(QStringRef &inp, QString& ident)
{
    CHECK_POINT(cp0, inp);

    if(!thisChar(inp, QChar('_'))) {
        inp = cp0;
        EXPECT(someCharOf(inp, &QChar::isLetter));
    }

    strOf(inp, &QChar::isLetterOrNumber);

    ident =  mid(cp0, inp).toString();

    return ParseStatus::success();
}

ParseStatusPtr QMetaParserBase::integer(QStringRef &inp, int &result)
{
    int sign = 1;

    if (thisChar(inp, QChar('-'))) {
        sign = -1;
    }

    QStringRef numbers;
    EXPECT(strOf(inp, numbers, &QChar::isDigit));

    result = int(numbers.toInt()) * sign;

    return ParseStatus::success();
}

ParseStatusPtr QMetaParserBase::thisToken(QStringRef &inp, QString str)
{
    spaces(inp);
    EXPECT(thisStr(inp, str));
    spaces(inp);
    return ParseStatus::success();
}

QChar QMetaParserBase::unescape(QChar c)
{
    if ('\'' == c)
        return '\'';
    if ('"' == c)
        return '"';
    if ('?' == c)
        return '\?';
    if ('\\' == c)
        return '\\';
    if ('a' == c)
        return '\a';
    if ('b' == c)
        return '\b';
    if ('f' == c)
        return '\f';
    if ('n' == c)
        return '\n';
    if ('r' == c)
        return '\r';
    if ('t' == c)
        return '\t';
    if ('v' == c)
        return '\v';

    return QChar(-1);
}
