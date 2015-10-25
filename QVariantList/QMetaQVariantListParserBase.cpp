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
    m_rule[STRING] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParserBase::string);
#pragma GCC diagnostic pop
}

bool QMetaQVariantListParserBase::applyRule(int ruleId, QVariant &input, QVariant &result, ParseErrorPtr &pe)
{
    ENTRYV(ruleId, input);

    EXPECT(m_rule.contains(ruleId));

    {
        MemoKey key = {ruleId, input};

        if (m_memo.contains(key)) {
            MemoEntry me = m_memo.value(key);
            if (me.rest.canConvert<ParseFail>()) {
                //RETURN_FAILURE("Left recursion detected.");
                RETURN_FAILURE();
            }
            result = me.result;
            input = me.rest;
            RETURN_SUCCESS();
        }

        QVariant res;
        QVariant fail;
        fail.setValue(ParseFail());

        RuleFuncPtr ruleFunc = m_rule[ruleId];
        m_memo.insert(key, {fail, res});

        EXPECT(ruleFunc(this, input, res, cpe));

        m_memo.insert(key, {input, res});
        result = res;
    }

    RETURN_SUCCESS();

    EXITV(result);
}

/////////////////////  TERMINALS  //////////////////////



bool QMetaQVariantListParserBase::digit(QVariant &input, int& digit, ParseErrorPtr &pe)
{
    ENTRYV(input);

    {
        EXPECT(input.type() == QVariant::Char);

        QChar c = input.value<QChar>();

        EXPECT(c.isDigit());

        digit = c.digitValue();
    }

    EXITV(digit);
}


bool QMetaQVariantListParserBase::someChar(QVariant &input, QChar& c, ParseErrorPtr &pe)
{
    ENTRYV(input);

    {
        EXPECT(input.type() == QVariant::Char);

        c = input.value<QChar>();
    }

    EXITV(c);
}


bool QMetaQVariantListParserBase::anyChar(QVariant &input, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(input.type() == QVariant::Char);

    EXIT();
}


bool QMetaQVariantListParserBase::someCharOf(QVariant &input, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(input);

    {
        EXPECT(input.type() == QVariant::Char);

        QChar ch = input.value<QChar>();

        EXPECT((ch.*is_x)());
    }

    EXIT();
}


bool QMetaQVariantListParserBase::someCharOf(QVariant &input, QChar &c, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(input);

    {
        EXPECT(input.type() == QVariant::Char);

        QChar ch = input.value<QChar>();

        EXPECT((ch.*is_x)());

        c = ch;
    }

    EXITV(c);
}


bool QMetaQVariantListParserBase::oneOf(QVariant &input, const QString& chars, QChar &outCh, ParseErrorPtr &pe)
{
    ENTRYV(input, chars);

    {
        EXPECT(input.type() == QVariant::Char);

        QChar c = input.value<QChar>();

        EXPECT(chars.contains(c));

        outCh = c;

    }

    EXITV(outCh);
}


bool QMetaQVariantListParserBase::thisChar(QVariant &input, QChar c, ParseErrorPtr &pe)
{
    ENTRYV(input, c);

    EXPECT(input.type() == QVariant::Char);

    EXPECT(c == input.value<QChar>());

    EXIT();
}


bool QMetaQVariantListParserBase::thisStr(QVariant &input, const QString& str, ParseErrorPtr &pe)
{
    ENTRYV(input, str);

    EXPECT(input.type() == QVariant::String);

    {
        QString s = input.value<QString>();

        EXPECT(s == str);
    }

    EXIT();
}

bool QMetaQVariantListParserBase::string(QVariant &input, QVariant& output, ParseErrorPtr &pe)
{
    ENTRYV(input, output);

    EXPECT(input.type() == QVariant::String);

    {
        output = input;
    }

    EXIT();
}



/////////////////////  NONTERMINALS  //////////////////////


/// For the time being anything is implemented interms of someChar
/// untill QMetaQStringParserGeneratorBase can operate on non-char streams.
bool QMetaQVariantListParserBase::anything(QVariant &input, QVariant& val, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(!input.isNull());

    val = input;

    EXITV(val);
}

bool QMetaQVariantListParserBase::integer(QVariant &input, QVariant& integer, ParseErrorPtr &pe)
{
    ENTRYV(input);

    EXPECT(input.type() == QVariant::Int);

    integer = input;

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
