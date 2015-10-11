#include "qmeta_parser_base.h"
#include <assert.h>
#include <algorithm>


//////////////////// PUBLIC FUNCTIONS ///////////////////

QMetaParserBase::QMetaParserBase()
{
    initRuleMap();
}

bool QMetaParserBase::parse(int ruleId, QString str, QVariant& ast, ParseStatusPtr &ps)
{
    m_input = str;
    m_memo.clear();
    ps = nullptr;
    return parse(ruleId, 0, ast, ps);
}


/////////////////////  TERMINALS  //////////////////////

bool QMetaParserBase::advance(int &pos, int count, ParseStatusPtr &ps)
{
    bool ok = true;
    //QSTDOUT()<< __FUNCTION__ << "(" << ")" << endl;
    g_indentLevel++;

    EXPECT(pos + count <= m_input.length());

    pos += count;

    RETURN_SUCCESS();

_exit:
    //QSTDOUT()<< "return " << __FUNCTION__ << "() = " << ok << endl;
    g_indentLevel--;
    return ok;
}

bool QMetaParserBase::digit(int &pos, int& digit, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;

    EXPECT(pos < m_input.length());

    {
        QChar c = m_input.at(pos);

        EXPECT(c.isDigit());

        digit = c.digitValue();

        EXPECT(advance(pos, 1, ps));
    }

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = " << digit << ", " << ok << endl;
    g_indentLevel--;
    return ok;
}


bool QMetaParserBase::someChar(int &pos, QChar& c, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;

    EXPECT(pos < m_input.length());

    c = m_input.at(pos);

    EXPECT(advance(pos, 1, ps));

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = '" << c << "', " << ok << endl;
    g_indentLevel--;
    return ok;
}

bool QMetaParserBase::anyChar(int &pos, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;

    EXPECT(pos < m_input.length());

    EXPECT(advance(pos, 1, ps));

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = " << ok << endl;
    g_indentLevel--;
    return ok;
}


bool QMetaParserBase::someCharOf(int &pos, bool (QChar::*is_x)() const, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;

    EXPECT(pos < m_input.length());

    {
        QChar ch = m_input.at(pos);

        EXPECT((ch.*is_x)());

        EXPECT(advance(pos, 1, ps));
    }

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = " << ok << endl;
    g_indentLevel--;
    return ok;
}

bool QMetaParserBase::someCharOf(int &pos, QChar &c, bool (QChar::*is_x)() const, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;

    EXPECT(pos < m_input.length());

    {
        QChar ch = m_input.at(pos);

        EXPECT((ch.*is_x)());

        c = ch;

        EXPECT(advance(pos, 1, ps));
    }

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = '" << c << "', " << ok << endl;
    g_indentLevel--;
    return ok;
}

bool QMetaParserBase::oneOf(int& pos, QChar &outCh, QString chars, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos <<", \"" << chars << "\")" << endl;
    g_indentLevel++;

    EXPECT(pos < m_input.length());

    {
        QChar c = m_input.at(pos);

        EXPECT(chars.contains(c));

        outCh = c;

        EXPECT(advance(pos, 1, ps));
    }

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = '" << outCh << "'', " << ok << endl;
    g_indentLevel--;
    return ok;
}


bool QMetaParserBase::thisChar(int &pos, QChar c, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ", '" << c << "')" << endl;
    g_indentLevel++;

    EXPECT(pos < m_input.length());

    EXPECT(m_input.at(pos) == c);

    EXPECT(advance(pos, 1, ps));

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = " << ok << endl;
    g_indentLevel--;
    return ok;
}

bool QMetaParserBase::thisStr(int &pos, QString str, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ", \"" << str << "\")" << endl;
    g_indentLevel++;

    EXPECT(m_input.midRef(pos).startsWith(str));

    EXPECT(advance(pos, str.length(), ps));

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = " << ok << endl;
    g_indentLevel--;
    return ok;
}

bool QMetaParserBase::strOf(int &pos, bool (QChar::*is_x)() const, ParseStatusPtr &ps)
{
    bool ok = true;
    //QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    //g_indentLevel++;

    EXPECT(pos < m_input.length());

    {
        int i = pos;

        for (; i < m_input.length() && (m_input.at(i).*is_x)(); i++) {
        }

        EXPECT(i - pos);

        EXPECT(advance(pos, i - pos, ps));
    }

_exit:
    //QSTDOUT()<< "return " << __FUNCTION__ << "() = " << ok << endl;
    //g_indentLevel--;
    return ok;
}

/////////////////////  NONTERMINALS  //////////////////////

bool QMetaParserBase::strOf(int &pos, QVariant &str, bool (QChar::*is_x)() const, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;

    CHECK_POINT(cp0, pos);

    EXPECT(strOf(pos, is_x, ps));

    {
        QString _str = mid(cp0, pos);
        str = _str;
    }

    RETURN_SUCCESS();

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = \"" << str << "\", " << ok << endl;
    g_indentLevel--;
    return ok;
}

/// For the time being anything is implemented interms of someChar
/// untill QMetaParserBase can operate on non-char streams.
bool QMetaParserBase::anything(int &pos, QVariant& val, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;

    QChar c;
    EXPECT(someChar(pos, c, ps));
    val = c;

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = \"" << val << "\", " << ok << endl;
    g_indentLevel--;
    return ok;
}


bool QMetaParserBase::spaces(int &pos, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;

    EXPECT(strOf(pos, &QChar::isSpace, ps));

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = " << ok << endl;
    g_indentLevel--;
    return ok;
}

bool QMetaParserBase::spaces(int &pos, QVariant &space, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;

    EXPECT(strOf(pos, space, &QChar::isSpace, ps));

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = \"" << space << "\", " << ok << endl;
    g_indentLevel--;
    return ok;
}

bool QMetaParserBase::identifier(int &pos, QVariant& ident, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;

    CHECK_POINT(cp0, pos);

    if(!thisChar(pos, QChar('_'), ps)) {
        pos = cp0;
        EXPECT(someCharOf(pos, &QChar::isLetter, ps));
    }

    //Optional
    strOf(pos, &QChar::isLetterOrNumber, ps);

    {
        QString id =  mid(cp0, pos);
        ident = id;
    }

    RETURN_SUCCESS();

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = " << ident << ", " << ok << endl;
    g_indentLevel--;
    return ok;
}

bool QMetaParserBase::integer(int &pos, QVariant& result, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    g_indentLevel++;
    int sign = 1;

    if (thisChar(pos, QChar('-'), ps)) {
        sign = -1;
    }

    QVariant variant;
    EXPECT(strOf(pos, variant, &QChar::isDigit, ps));

    {
        int r = int(variant.value<QString>().toInt()) * sign;
        result = r;
    }

    RETURN_SUCCESS();

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = "<< result << ", " << ok << endl;
    g_indentLevel--;
    return ok;
}

bool QMetaParserBase::thisToken(int &pos, QString str, ParseStatusPtr &ps)
{
    bool ok = true;
    QSTDOUT()<< __FUNCTION__ << "(" << pos << ", \"" << str << "\")" << endl;
    g_indentLevel++;

    spaces(pos, ps);
    EXPECT(thisStr(pos, str, ps));
    spaces(pos, ps);
    RETURN_SUCCESS();

_exit:
    QSTDOUT()<< "return " << __FUNCTION__ << "() = " << ok << endl;
    g_indentLevel--;
    return ok;
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

bool QMetaParserBase::applyRule(int ruleId, int &pos, QVariant &result, ParseStatusPtr &ps)
{
    bool ok = true;
    //QSTDOUT()<< __FUNCTION__ << "(" << pos << ")" << endl;
    //g_indentLevel++;

    EXPECT(m_rule.contains(ruleId));

    {
        MemoKey key = {ruleId, pos};

        if (m_memo.contains(key)) {
            MemoEntry me = m_memo.value(key);
            if (me.nextPos < 0) {
                RETURN_FAILURE(pos, "Left recursion detected.");
            }
            result = me.result;
            pos = me.nextPos;
            RETURN_SUCCESS();
        }

        QVariant res;

        RuleFuncPtr ruleFunc = m_rule[ruleId];
        m_memo.insert(key, {FAIL, res});

        EXPECT(ruleFunc(this, pos, res, ps));

        m_memo.insert(key, {pos, res});
        result = res;
    }

    RETURN_SUCCESS();

_exit:
    //QSTDOUT()<< "return " << __FUNCTION__ << " = " << ok << endl;
    //g_indentLevel--;
    return ok;
}

void QMetaParserBase::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    m_rule[SPACES] = reinterpret_cast<RuleFuncPtr>(static_cast<RuleMemberFuncPtr>(&QMetaParserBase::spaces));
    m_rule[IDENTIFIER] = reinterpret_cast<RuleFuncPtr>(&QMetaParserBase::identifier);
    m_rule[ANYTHING] = reinterpret_cast<RuleFuncPtr>(&QMetaParserBase::anything);
    m_rule[INTEGER] = reinterpret_cast<RuleFuncPtr>(&QMetaParserBase::integer);
#pragma GCC diagnostic pop
}


QString QMetaParserBase::mid(int pos0, int pos1)
{
    return m_input.mid(pos0, pos1 - pos0);
}

inline uint qHash(const QMetaParserBase::MemoKey &key, uint seed)
{
    return qHash(key.ruleId, seed) ^ qHash(key.position, seed);
}


