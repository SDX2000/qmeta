#include "QMetaQStringToQVariantTransformer.h"

#include <assert.h>
#include <algorithm>

#include "QString/macros.h"


//////////////////// PUBLIC FUNCTIONS ///////////////////

QMetaQStringToQVariantTransformer::QMetaQStringToQVariantTransformer(int ruleId, const QString &inp)
    : m_error(nullptr)
    , m_indentLevel(0)
    , m_input(inp)
    , m_ruleId(ruleId)
{
    initRuleMap();
}

QMetaQStringToQVariantTransformer::~QMetaQStringToQVariantTransformer()
{
    safeDelete(m_error);
}

bool QMetaQStringToQVariantTransformer::parse(QVariant& ast)
{
    return parse(m_ruleId, 0, ast, m_error);
}

const ParseError *QMetaQStringToQVariantTransformer::getError() const
{
    return m_error;
}

/////////////////////  TERMINALS  //////////////////////

bool QMetaQStringToQVariantTransformer::advance(int &pos, int count, ParseErrorPtr &pe)
{
    ENTRYV(pos, count);

    EXPECT(pos + count <= m_input.length());

    pos += count;

    RETURN_SUCCESS();

    EXIT();
}

bool QMetaQStringToQVariantTransformer::digit(int &pos, int& digit, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        QChar c = m_input.at(pos);

        EXPECT(c.isDigit());

        digit = c.digitValue();

        EXPECT(advance(pos, 1, cpe));
    }

    EXITV(digit);
}


bool QMetaQStringToQVariantTransformer::someChar(int &pos, QChar& c, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    c = m_input.at(pos);

    EXPECT(advance(pos, 1, cpe));

    EXITV(c);
}

bool QMetaQStringToQVariantTransformer::anyChar(int &pos, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    EXPECT(advance(pos, 1, cpe));

    EXIT();
}


bool QMetaQStringToQVariantTransformer::someCharOf(int &pos, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        QChar ch = m_input.at(pos);

        EXPECT((ch.*is_x)());

        EXPECT(advance(pos, 1, cpe));
    }

    EXIT();
}

bool QMetaQStringToQVariantTransformer::someCharOf(int &pos, QChar &c, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        QChar ch = m_input.at(pos);

        EXPECT((ch.*is_x)());

        c = ch;

        EXPECT(advance(pos, 1, cpe));
    }

    EXITV(c);
}

bool QMetaQStringToQVariantTransformer::oneOf(int& pos, QString chars, QChar &outCh, ParseErrorPtr &pe)
{
    ENTRYV(pos, chars);

    EXPECT(pos < m_input.length());

    {
        QChar c = m_input.at(pos);

        EXPECT(chars.contains(c));

        outCh = c;

        EXPECT(advance(pos, 1, cpe));
    }

    EXITV(outCh);
}

bool QMetaQStringToQVariantTransformer::eof(int &pos, ParseErrorPtr &pe)
{
    ENTRYV(pos);
    EXPECT(pos >= m_input.length());
    EXIT();
}

bool QMetaQStringToQVariantTransformer::eol(int &pos, ParseErrorPtr &pe)
{
    ENTRYV(pos);
    EXPECT(pos < m_input.length());
    {
        QChar c = m_input.at(pos);
        if (c == '\n') {
            RETURN_SUCCESS();
        }

        if (c == '\r') {
            pos++;
            EXPECT(pos < m_input.length());
            c = m_input.at(pos);
            EXPECT(m_input.at(pos) == '\n');
        }
    }

    EXIT();
}


bool QMetaQStringToQVariantTransformer::thisChar(int &pos, QChar c, ParseErrorPtr &pe)
{
    ENTRYV(pos, c);

    EXPECT(pos < m_input.length());

    EXPECT(m_input.at(pos) == c);

    EXPECT(advance(pos, 1, cpe));

    EXIT();
}

bool QMetaQStringToQVariantTransformer::thisStr(int &pos, QString str, ParseErrorPtr &pe)
{
    ENTRYV(pos, str);

    EXPECT(m_input.midRef(pos).startsWith(str));

    EXPECT(advance(pos, str.length(), cpe));

    EXIT();
}

bool QMetaQStringToQVariantTransformer::strOf(int &pos, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        int i = pos;

        for (; i < m_input.length() && (m_input.at(i).*is_x)(); i++) {
        }

        EXPECT(i - pos);

        EXPECT(advance(pos, i - pos, cpe));
    }

    EXIT();
}

/////////////////////  NONTERMINALS  //////////////////////

bool QMetaQStringToQVariantTransformer::strOf(int &pos, QVariant &str, bool (QChar::*is_x)() const, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    CHECK_POINT(cp0);

    EXPECT(strOf(pos, is_x, cpe));

    {
        QString _str = mid(cp0, pos);
        str = _str;
    }

    RETURN_SUCCESS();

    EXITV(str);
}

/// For the time being anything is implemented interms of someChar
/// untill QMetaQStringParserBase can operate on non-char streams.
bool QMetaQStringToQVariantTransformer::anything(int &pos, QVariant& val, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    QChar c;
    EXPECT(someChar(pos, c, cpe));
    val = c;

    EXITV(val);
}


bool QMetaQStringToQVariantTransformer::spaces(int &pos, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(strOf(pos, &QChar::isSpace, cpe));

    EXIT();
}

bool QMetaQStringToQVariantTransformer::spaces(int &pos, QVariant &space, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    EXPECT(strOf(pos, space, &QChar::isSpace, cpe));

    EXITV(space);
}

bool QMetaQStringToQVariantTransformer::identifier(int &pos, QVariant& ident, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    CHECK_POINT(cp0);

    if(!thisChar(pos, QChar('_'), cpe)) {
        pos = cp0;
        EXPECT(someCharOf(pos, &QChar::isLetter, cpe));
    }

    //Optional
    strOf(pos, &QChar::isLetterOrNumber, cpe);

    {
        QString id =  mid(cp0, pos);
        ident = id;
    }

    RETURN_SUCCESS();

    EXITV(ident);
}

bool QMetaQStringToQVariantTransformer::integer(int &pos, QVariant& result, ParseErrorPtr &pe)
{
    ENTRYV(pos);

    int sign = 1;

    if (thisChar(pos, QChar('-'), cpe)) {
        sign = -1;
    }

    QVariant variant;
    EXPECT(strOf(pos, variant, &QChar::isDigit, cpe));

    {
        int r = int(variant.value<QString>().toInt()) * sign;
        result = r;
    }

    RETURN_SUCCESS();

    EXITV(result);
}

bool QMetaQStringToQVariantTransformer::thisToken(int &pos, QString str, ParseErrorPtr &pe)
{
    ENTRYV(pos, str);

    spaces(pos, cpe);
    EXPECT(thisStr(pos, str, cpe));
    spaces(pos, cpe);
    RETURN_SUCCESS();

    EXIT();
}

QChar QMetaQStringToQVariantTransformer::unescape(QChar c)
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

bool QMetaQStringToQVariantTransformer::applyRule(int ruleId, int &pos, QVariant &result, ParseErrorPtr &pe)
{
    ENTRYV(ruleId, pos);

    EXPECT(m_rule.contains(ruleId));

    {
        MemoKey key = {ruleId, pos};

        if (m_memo.contains(key)) {
            MemoEntry me = m_memo.value(key);
            if (me.nextPos < 0) {
                //RETURN_FAILURE("Left recursion detected.");
                RETURN_FAILURE();
            }
            result = me.result;
            pos = me.nextPos;
            RETURN_SUCCESS();
        }

        QVariant res;

        RuleFuncPtr ruleFunc = m_rule[ruleId];
        m_memo.insert(key, {FAIL, res});

        EXPECT(ruleFunc(this, pos, res, cpe));

        m_memo.insert(key, {pos, res});
        result = res;
    }

    RETURN_SUCCESS();

    EXITV(result);
}

void QMetaQStringToQVariantTransformer::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    m_rule[SPACES] = reinterpret_cast<RuleFuncPtr>(static_cast<RuleMemberFuncPtr>(&QMetaQStringToQVariantTransformer::spaces));
    m_rule[IDENTIFIER] = reinterpret_cast<RuleFuncPtr>(&QMetaQStringToQVariantTransformer::identifier);
    m_rule[ANYTHING] = reinterpret_cast<RuleFuncPtr>(&QMetaQStringToQVariantTransformer::anything);
    m_rule[INTEGER] = reinterpret_cast<RuleFuncPtr>(&QMetaQStringToQVariantTransformer::integer);
#pragma GCC diagnostic pop
}


QString QMetaQStringToQVariantTransformer::mid(int pos0, int pos1)
{
    return m_input.mid(pos0, pos1 - pos0);
}

inline uint qHash(const QMetaQStringToQVariantTransformer::MemoKey &key, uint seed)
{
    return qHash(key.ruleId, seed) ^ qHash(key.position, seed);
}
