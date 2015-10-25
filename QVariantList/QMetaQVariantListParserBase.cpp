#include "QMetaQVariantListParserBase.h"

QMetaQVariantListParserBase::QMetaQVariantListParserBase(int ruleId, const QVariant& inp)
    : m_indentLevel(0)
    , m_startRuleId(ruleId)
    , m_input(inp)
{
    initRuleMap();
}

void QMetaQVariantListParserBase::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    m_rule[STRING] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParserBase::string);
#pragma GCC diagnostic pop
}

bool QMetaQVariantListParserBase::applyRule(int ruleId, QVariant &inp, QVariant &result, ParseErrorPtr &pe)
{
    ENTRYV(ruleId, inp);

    EXPECT(m_rule.contains(ruleId));

    {
        MemoKey key = {ruleId, inp};

        if (m_memo.contains(key)) {
            MemoEntry me = m_memo.value(key);
            if (me.rest.canConvert<ParseFail>()) {
                //RETURN_FAILURE("Left recursion detected.");
                RETURN_FAILURE();
            }
            result = me.result;
            inp = me.rest;
            RETURN_SUCCESS();
        }

        QVariant res;
        QVariant fail;
        fail.setValue(ParseFail());

        RuleFuncPtr ruleFunc = m_rule[ruleId];
        m_memo.insert(key, {fail, res});

        EXPECT(ruleFunc(this, inp, res, cpe));

        m_memo.insert(key, {inp, res});
        result = res;
    }

    RETURN_SUCCESS();

    EXITV(result);
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

bool QMetaQVariantListParserBase::string(QVariant &inp, QVariant& output, ParseErrorPtr &pe)
{
    ENTRYV(inp, output);

    EXPECT(inp.type() == QVariant::String);

    {
        output = inp;
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
