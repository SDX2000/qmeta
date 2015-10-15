#include "QMetaQStringParserGenerator.h"

QMetaQVariantListParserBase::QMetaQVariantListParserBase()
{
    initRuleMap();
}

bool QMetaQVariantListParserBase::parse(int ruleId, const QVariantList &inp, QVariant& ast)
{
    m_input = inp;
    m_memo.clear();
    m_indentLevel = 0;
    safeDelete(m_error);
    return parse(ruleId, 0, ast, m_error);
}

void QMetaQVariantListParserBase::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    m_rule[ANYTHING] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParserBase::anything);
    m_rule[INTEGER] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParserBase::integer);
#pragma GCC diagnostic pop
}

/////////////////////  TERMINALS  //////////////////////

bool QMetaQVariantListParserBase::advance(int &pos, int count, ParseErrorPtr &pe)
{
    ENTRYV(pos, count);

    EXPECT(pos + count <= m_input.length());

    pos += count;

    RETURN_SUCCESS();

    EXIT();
}

bool QMetaQVariantListParserBase::digit(int &pos, int& digit, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        QVariant vc = m_input.at(pos);

        EXPECT(vc.type() == QVariant::Char);

        QChar c = vc.value<QChar>();

        EXPECT(c.isDigit());

        digit = c.digitValue();

        EXPECT(advance(pos, 1, cpe));
    }

    EXITV(digit);
}


bool QMetaQVariantListParserBase::someChar(int &pos, QChar& c, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        QVariant vc = m_input.at(pos);

        EXPECT(vc.type() == QVariant::Char);

        c = vc.value<QChar>();

        EXPECT(advance(pos, 1, cpe));
    }

    EXITV(c);
}


bool QMetaQVariantListParserBase::anyChar(int &pos, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    EXPECT(advance(pos, 1, cpe));

    EXIT();
}


bool QMetaQVariantListParserBase::someCharOf(int &pos, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        QVariant vc = m_input.at(pos);

        EXPECT(vc.type() == QVariant::Char);

        QChar ch = vc.value<QChar>();

        EXPECT((ch.*is_x)());

        EXPECT(advance(pos, 1, cpe));
    }

    EXIT();
}


bool QMetaQVariantListParserBase::someCharOf(int &pos, QChar &c, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        QVariant vc = m_input.at(pos);

        EXPECT(vc.type() == QVariant::Char);

        QChar ch = vc.value<QChar>();

        EXPECT((ch.*is_x)());

        c = ch;

        EXPECT(advance(pos, 1, cpe));
    }

    EXITV(c);
}


bool QMetaQVariantListParserBase::oneOf(int& pos, QString chars, QChar &outCh, ParseErrorPtr &pe)
{
    ENTRYV(pos, chars);

    EXPECT(pos < m_input.length());

    {
        QVariant vc = m_input.at(pos);

        EXPECT(vc.type() == QVariant::Char);

        QChar c = vc.value<QChar>();

        EXPECT(chars.contains(c));

        outCh = c;

        EXPECT(advance(pos, 1, cpe));
    }

    EXITV(outCh);
}


bool QMetaQVariantListParserBase::thisChar(int &pos, QChar c, ParseErrorPtr &pe)
{
    ENTRYV(pos, c);

    EXPECT(pos < m_input.length());

    EXPECT(m_input.at(pos) == c);

    EXPECT(advance(pos, 1, cpe));

    EXIT();
}


bool QMetaQVariantListParserBase::thisStr(int &pos, QString str, ParseErrorPtr &pe)
{
    ENTRYV(pos, str);

    QVariant vc = m_input.at(pos);

    EXPECT(vc.type() == QVariant::String);

    {
        QString s = vc.value<QString>();

        EXPECT(s == str);

        EXPECT(advance(pos, str.length(), cpe));
    }

    EXIT();
}



/////////////////////  NONTERMINALS  //////////////////////


/// For the time being anything is implemented interms of someChar
/// untill QMetaQStringParserGeneratorBase can operate on non-char streams.
bool QMetaQVariantListParserBase::anything(int &pos, QVariant& val, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    QChar c;
    EXPECT(someChar(pos, c, cpe));
    val = c;

    EXITV(val);
}


bool QMetaQVariantListParserBase::integer(int &pos, QVariant& integer, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    QVariant v = m_input.at(pos);

    EXPECT(v.type() == QVariant::Int);

    integer = v;

    RETURN_SUCCESS();

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

