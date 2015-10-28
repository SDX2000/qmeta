#include "QMetaQVariantListParserBase.h"
#include "QVariantList/macros.h"

QMetaQVariantListParserBase::QMetaQVariantListParserBase(int ruleId, const QVariant& input)
    : m_error(nullptr)
    , m_indentLevel(0)
    , m_startRuleId(ruleId)
    , m_input(input)
{
    m_fail.setValue(ParseFail());
    initRuleMap();
}

void QMetaQVariantListParserBase::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    m_rule[STRING] = reinterpret_cast<RuleFuncPtr>(&QMetaQVariantListParserBase::string);
#pragma GCC diagnostic pop
}

bool QMetaQVariantListParserBase::applyRule(int ruleId, QVariantConstPtr input, QVariant &result, ParseErrorPtr &pe)
{
    ENTRYV(ruleId, *input);

    EXPECT(m_rule.contains(ruleId));

    {
        MemoKey key = {ruleId, input};

        if (m_memo.contains(key)) {
            MemoEntry me = m_memo.value(key);
            if (me.rest->canConvert<ParseFail>()) {
                //RETURN_FAILURE("Left recursion detected.");
                RETURN_FAILURE();
            }
            result = me.result;
            input = me.rest;
            RETURN_SUCCESS();
        }

        QVariant res;

        RuleFuncPtr ruleFunc = m_rule[ruleId];
        m_memo.insert(key, {&m_fail, res});

        EXPECT(ruleFunc(this, input, res, cpe));

        m_memo.insert(key, {input, res});
        result = res;
    }

    RETURN_SUCCESS();
    EXITV(result);
}

/////////////////////  TERMINALS  //////////////////////



bool QMetaQVariantListParserBase::digit(QVariantConstPtr input, int& digit, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    {
        EXPECT(input->type() == QVariant::Char);

        QChar c = input->value<QChar>();

        EXPECT(c.isDigit());

        digit = c.digitValue();
    }

    RETURN_SUCCESS();
    EXITV(digit);
}


bool QMetaQVariantListParserBase::someChar(QVariantConstPtr input, QChar& c, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    {
        EXPECT(input->type() == QVariant::Char);

        c = input->value<QChar>();
    }

    RETURN_SUCCESS();
    EXITV(c);
}


bool QMetaQVariantListParserBase::anyChar(QVariantConstPtr input, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::Char);

    RETURN_SUCCESS();
    EXIT();
}


bool QMetaQVariantListParserBase::someCharOf(QVariantConstPtr input, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    {
        EXPECT(input->type() == QVariant::Char);

        QChar ch = input->value<QChar>();

        EXPECT((ch.*is_x)());
    }

    RETURN_SUCCESS();
    EXIT();
}


bool QMetaQVariantListParserBase::someCharOf(QVariantConstPtr input, QChar &c, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    {
        EXPECT(input->type() == QVariant::Char);

        QChar ch = input->value<QChar>();

        EXPECT((ch.*is_x)());

        c = ch;
    }

    RETURN_SUCCESS();
    EXITV(c);
}


bool QMetaQVariantListParserBase::oneOf(QVariantConstPtr input, const QString& chars, QChar &outCh, ParseErrorPtr &pe)
{
    ENTRYV(*input, chars);

    {
        EXPECT(input->type() == QVariant::Char);

        QChar c = input->value<QChar>();

        EXPECT(chars.contains(c));

        outCh = c;

    }

    RETURN_SUCCESS();
    EXITV(outCh);
}


bool QMetaQVariantListParserBase::thisChar(QVariantConstPtr input, QChar c, ParseErrorPtr &pe)
{
    ENTRYV(*input, c);

    EXPECT(input->type() == QVariant::Char);

    EXPECT(c == input->value<QChar>());

    RETURN_SUCCESS();
    EXIT();
}

bool QMetaQVariantListParserBase::thisStr(QVariantConstPtr input, const QString& str, ParseErrorPtr &pe)
{
    ENTRYV(*input, str);

    EXPECT(input->type() == QVariant::String);

    {
        QString s = input->value<QString>();

        EXPECT(s == str);
    }

    RETURN_SUCCESS();
    EXIT();
}

bool QMetaQVariantListParserBase::string(QVariantConstPtr input, QVariant& output, ParseErrorPtr &pe)
{
    ENTRYV(*input, output);

    EXPECT(input->type() == QVariant::String);

    {
        output = input;
    }

    RETURN_SUCCESS();
    EXIT();
}



/////////////////////  NONTERMINALS  //////////////////////


/// For the time being anything is implemented interms of someChar
/// untill QMetaQStringParserGeneratorBase can operate on non-char streams.
bool QMetaQVariantListParserBase::anything(QVariantConstPtr input, QVariant& val, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(!input->isNull());

    val = input;


    RETURN_SUCCESS();
    EXITV(val);
}

bool QMetaQVariantListParserBase::integer(QVariantConstPtr input, QVariant& integer, ParseErrorPtr &pe)
{
    ENTRYV(*input);

    EXPECT(input->type() == QVariant::Int);

    integer = input;

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

inline uint qHash(const QMetaQVariantListParserBase::MemoKey &key, uint seed)
{
    return qHash(key.ruleId, seed) ^ qHash(key.input, seed);
}
