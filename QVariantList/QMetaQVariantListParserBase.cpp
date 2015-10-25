#include "QMetaQVariantListParserBase.h"

QMetaQVariantListParserBase::QMetaQVariantListParserBase(int ruleId, const QVariant& input)
    : m_indentLevel(0)
    , m_startRuleId(ruleId)
    , m_input(input)
{
    initRuleMap();
}

void QMetaQVariantListParserBase::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
//    m_rule[ANYTHING] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParserBase::anything);
//    m_rule[INTEGER] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParserBase::integer);
#pragma GCC diagnostic pop
}

/////////////////////  TERMINALS  //////////////////////



bool QMetaQVariantListParserBase::digit(QVariant &inp, int& digit, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    {
        EXPECT(inp.type() == QVariant::Char);

        QChar c = inp.value<QChar>();

        EXPECT(c.isDigit());

        digit = c.digitValue();
    }

    EXITV(digit);
}


bool QMetaQVariantListParserBase::someChar(QVariant &inp, QChar& c, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    {
        EXPECT(inp.type() == QVariant::Char);

        c = inp.value<QChar>();
    }

    EXITV(c);
}


bool QMetaQVariantListParserBase::anyChar(QVariant &inp, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    EXPECT(inp.type() == QVariant::Char);

    EXIT();
}


bool QMetaQVariantListParserBase::someCharOf(QVariant &inp, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    {
        EXPECT(inp.type() == QVariant::Char);

        QChar ch = inp.value<QChar>();

        EXPECT((ch.*is_x)());
    }

    EXIT();
}


bool QMetaQVariantListParserBase::someCharOf(QVariant &inp, QChar &c, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    {
        EXPECT(inp.type() == QVariant::Char);

        QChar ch = inp.value<QChar>();

        EXPECT((ch.*is_x)());

        c = ch;
    }

    EXITV(c);
}


bool QMetaQVariantListParserBase::oneOf(QVariant &inp, const QString& chars, QChar &outCh, ParseErrorPtr &pe)
{
    ENTRYV(inp, chars);

    {
        EXPECT(inp.type() == QVariant::Char);

        QChar c = inp.value<QChar>();

        EXPECT(chars.contains(c));

        outCh = c;

    }

    EXITV(outCh);
}


bool QMetaQVariantListParserBase::thisChar(QVariant &inp, QChar c, ParseErrorPtr &pe)
{
    ENTRYV(inp, c);

    EXPECT(inp.type() == QVariant::Char);

    EXPECT(c == inp.value<QChar>());

    EXIT();
}


bool QMetaQVariantListParserBase::thisStr(QVariant &inp, const QString& str, ParseErrorPtr &pe)
{
    ENTRYV(inp, str);

    EXPECT(inp.type() == QVariant::String);

    {
        QString s = inp.value<QString>();

        EXPECT(s == str);
    }

    EXIT();
}



/////////////////////  NONTERMINALS  //////////////////////


/// For the time being anything is implemented interms of someChar
/// untill QMetaQStringParserGeneratorBase can operate on non-char streams.
bool QMetaQVariantListParserBase::anything(QVariant &inp, QVariant& val, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    EXPECT(!inp.isNull());

    val = inp;

    EXITV(val);
}


bool QMetaQVariantListParserBase::integer(QVariant &inp, QVariant& integer, ParseErrorPtr &pe)
{
    ENTRYV(inp);

    EXPECT(inp.type() == QVariant::Int);

    integer = inp;

    EXITV(integer);
}

QChar QMetaQVariantListParserBase::unescape(QChar c)
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

