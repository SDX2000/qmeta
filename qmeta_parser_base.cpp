#include "qmeta_parser_base.h"
#include <assert.h>
#include <algorithm>
#include <iostream>

using namespace std;

//////////////////// PUBLIC FUNCTIONS ///////////////////

QMetaParserBase::QMetaParserBase()
{
    initRuleMap();
}

bool QMetaParserBase::parse(QString str, QVariant& ast, ParseStatusPtr &ps)
{
    m_input = str;
    return parse(0, ast, ps);
}


/////////////////////  TERMINALS  //////////////////////

bool QMetaParserBase::advance(int &pos, int count, ParseStatusPtr &ps)
{
    EXPECT_B(pos + count < m_input.length());

    pos += count;

    RETURN_SUCCESS(ps);
}

bool QMetaParserBase::digit(int &pos, int& digit, ParseStatusPtr &ps)
{
    EXPECT_B(pos < m_input.length());

    QChar c = m_input.at(pos);

    EXPECT_B(c.isDigit());

    digit = c.digitValue();

    return advance(pos, 1, ps);
}


bool QMetaParserBase::someChar(int &pos, QChar& c, ParseStatusPtr &ps)
{
    EXPECT_B(pos < m_input.length());

    c = m_input.at(pos);

    return advance(pos, 1, ps);
}


bool QMetaParserBase::someCharOf(int &pos, bool (QChar::*is_x)() const, ParseStatusPtr &ps)
{
    EXPECT_B(pos < m_input.length());

    QChar ch = m_input.at(pos);

    EXPECT_B((ch.*is_x)());

    return advance(pos, 1, ps);
}

bool QMetaParserBase::someCharOf(int &pos, QChar &c, bool (QChar::*is_x)() const, ParseStatusPtr &ps)
{
    EXPECT_B(pos < m_input.length());

    QChar ch = m_input.at(pos);

    EXPECT_B((ch.*is_x)());

    c = ch;

    return advance(pos, 1, ps);
}

bool QMetaParserBase::oneOf(int& pos, QChar &opOut, QString chars, ParseStatusPtr &ps)
{
    EXPECT_B(pos < m_input.length());

    QChar c = m_input.at(pos);

    EXPECT_B(chars.contains(c));

    opOut = c;

    return advance(pos, 1, ps);
}


bool QMetaParserBase::thisChar(int &pos, QChar c, ParseStatusPtr &ps)
{
    EXPECT_B(pos < m_input.length());

    EXPECT_B(m_input.at(pos) == c);

    return advance(pos, 1, ps);
}

bool QMetaParserBase::thisStr(int &pos, QString str, ParseStatusPtr &ps)
{
    EXPECT_B(m_input.midRef(pos).startsWith(str));

    return advance(pos, str.length(), ps);;
}

bool QMetaParserBase::strOf(int &pos, bool (QChar::*is_x)() const, ParseStatusPtr &ps)
{
    EXPECT_B(pos < m_input.length());

    int i = pos;

    for (; i < m_input.length() && (m_input.at(i).*is_x)(); i++) {
    }

    EXPECT_B(i - pos);

    return advance(pos, i - pos, ps);
}

/////////////////////  NONTERMINALS  //////////////////////

bool QMetaParserBase::strOf(int &pos, QVariant &str, bool (QChar::*is_x)() const, ParseStatusPtr &ps)
{
    LOG();
    CHECK_POINT(cp0, pos);

    EXPECT(strOf(pos, is_x, ps));

    QString _str = mid(cp0, pos);
    str = _str;

    RETURN_SUCCESS(ps);
}

bool QMetaParserBase::spaces(int &pos, ParseStatusPtr &ps)
{
    LOG();
    return strOf(pos, &QChar::isSpace, ps);
}

bool QMetaParserBase::spaces(int &pos, QVariant &space, ParseStatusPtr &ps)
{
    LOG();
    return strOf(pos, space, &QChar::isSpace, ps);
}

bool QMetaParserBase::identifier(int &pos, QVariant& ident, ParseStatusPtr &ps)
{
    LOG();
    CHECK_POINT(cp0, pos);

    if(!thisChar(pos, QChar('_'), ps)) {
        pos = cp0;
        EXPECT(someCharOf(pos, &QChar::isLetter, ps));
    }

    strOf(pos, &QChar::isLetterOrNumber, ps);

    QString id =  mid(cp0, pos);
    ident = id;
    RETURN_SUCCESS(ps);
}

bool QMetaParserBase::integer(int &pos, QVariant& result, ParseStatusPtr &ps)
{
    LOG();
    int sign = 1;

    if (thisChar(pos, QChar('-'), ps)) {
        sign = -1;
    }

    QVariant variant;
    EXPECT(strOf(pos, variant, &QChar::isDigit, ps));

    int r = int(variant.value<QString>().toInt()) * sign;

    result = r;

    RETURN_SUCCESS(ps);
}

bool QMetaParserBase::thisToken(int &pos, QString str, ParseStatusPtr &ps)
{
    LOG();
    printIndent();
    cout<<"Token: "<<str.toStdString().c_str()<<endl;
    spaces(pos, ps);
    EXPECT(thisStr(pos, str, ps));
    spaces(pos, ps);
    RETURN_SUCCESS(ps);
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
    if (!m_rule.contains(ruleId)) {
        assert(false);
        RETURN_FAILURE(ps, pos, "Invalid ruleId given to apply_rule.");
    }

    MemoKey key = {ruleId, pos};

    if (m_memo.contains(key)) {
        MemoEntry me = m_memo.value(key);
        if (me.nextPos < 0) {
            RETURN_FAILURE(ps, pos, "Left recursion detected.");
        }
        result = me.result;
        pos = me.nextPos;
        RETURN_SUCCESS(ps);
    }

    QVariant res;
    //ParseStatusPtr rps;
    RuleFuncPtr ruleFunc = m_rule[ruleId];
    m_memo.insert(key, {FAIL, res});
    bool ok = ruleFunc(this, pos, res, ps);
    if (ok) {
        printIndent(g_indentLevel);
        cout<<"***Rule "<<ruleId<<" succeeded @"<<key.position<<". Result..."<<endl;
        print(res);
        m_memo.insert(key, {pos, res});
        result = res;
    }
    return ok;
}

void QMetaParserBase::initRuleMap()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    m_rule[SPACES] = reinterpret_cast<RuleFuncPtr>(static_cast<RuleMemberFuncPtr>(&QMetaParserBase::spaces));
    m_rule[IDENTIFIER] = reinterpret_cast<RuleFuncPtr>(&QMetaParserBase::identifier);
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


