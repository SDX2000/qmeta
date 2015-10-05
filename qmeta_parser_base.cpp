#include "qmeta_parser_base.h"
#include <assert.h>
#include <algorithm>

//////////////////// PUBLIC FUNCTIONS ///////////////////

ParseStatusPtr QMetaParserBase::parse(QString inp, QVariant& ast)
{
    QStringRef  inpRef = inp.midRef(0);
    return parse(inpRef, ast);
}



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

ParseStatusPtr QMetaParserBase::strOf(QStringRef &inp, QVariant &str, bool (QChar::*is_x)() const)
{
    CHECK_POINT(cp0, inp);

    EXPECT(strOf(inp, is_x));

    QString _str = mid(cp0, inp).toString();
    str = _str;
    return ParseStatus::success();
}

ParseStatusPtr QMetaParserBase::spaces(QStringRef &inp)
{
    return strOf(inp, &QChar::isSpace);
}

ParseStatusPtr QMetaParserBase::spaces(QStringRef &inp, QVariant &space)
{
    return strOf(inp, space, &QChar::isSpace);
}

ParseStatusPtr QMetaParserBase::identifier(QStringRef &inp, QVariant& ident)
{
    CHECK_POINT(cp0, inp);

    if(!thisChar(inp, QChar('_'))) {
        inp = cp0;
        EXPECT(someCharOf(inp, &QChar::isLetter));
    }

    strOf(inp, &QChar::isLetterOrNumber);

    QString id =  mid(cp0, inp).toString();
    ident = id;
    return ParseStatus::success();
}

ParseStatusPtr QMetaParserBase::integer(QStringRef &inp, QVariant& result)
{
    int sign = 1;

    if (thisChar(inp, QChar('-'))) {
        sign = -1;
    }

    QVariant variant;
    EXPECT(strOf(inp, variant, &QChar::isDigit));

    int r = int(variant.value<QString>().toInt()) * sign;

    result = r;

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

ParseStatusPtr QMetaParserBase::applyRule(int ruleId, QStringRef &inp, QVariant &result)
{
    MemoKey key = {ruleId, inp.position()};

    if (m_memo.contains(key)) {
        MemoEntry me = m_memo.value(key);
        result = me.result;
        inp = inp.string()->midRef(me.nextPos);
        return ParseStatus::success();
    }

    QVariant res;
    ParseStatusPtr pstatus;

    switch (ruleId){
        case SPACES:
            pstatus = spaces(inp, res);
            break;
        case IDENTIFIER:
            pstatus = identifier(inp, res);
            break;
        case INTEGER:
            pstatus = integer(inp, res);
            break;
        default:
            assert(false);
            return ParseStatus::failure(inp, "Invalid ruleId given to apply_rule.");
    }

    m_memo.insert(key, {inp.position(), res});
    result = res;
    return pstatus;
}


inline uint qHash(const QMetaParserBase::MemoKey &key, uint seed)
{
    return qHash(key.ruleId, seed) ^ qHash(key.position, seed);
}
