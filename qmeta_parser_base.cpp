#include "qmeta_parser_base.h"
#include <assert.h>
#include <algorithm>


//////////////////// PUBLIC FUNCTIONS ///////////////////

QMetaParserBase::QMetaParserBase()
    :m_indentLevel(0)
{
    initRuleMap();
}

bool QMetaParserBase::parse(int ruleId, QString str, QVariant& ast)
{
    m_input = str;
    m_memo.clear();
    m_errors.clear();
    m_indentLevel = 0;
    return parse(ruleId, 0, ast);
}

const ParseError *QMetaParserBase::getError() const
{
    return m_errors[0];
}


/////////////////////  TERMINALS  //////////////////////

bool QMetaParserBase::advance(int &pos, int count)
{
    ENTRYV(pos, count);

    EXPECT(pos + count <= m_input.length());

    pos += count;

    RETURN_SUCCESS();

    EXIT();
}

bool QMetaParserBase::digit(int &pos, int& digit)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        QChar c = m_input.at(pos);

        EXPECT(c.isDigit());

        digit = c.digitValue();

        EXPECT(advance(pos, 1));
    }

    EXITV(digit);
}


bool QMetaParserBase::someChar(int &pos, QChar& c)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    c = m_input.at(pos);

    EXPECT(advance(pos, 1));

    EXITV(c);
}

bool QMetaParserBase::anyChar(int &pos)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    EXPECT(advance(pos, 1));

    EXIT();
}


bool QMetaParserBase::someCharOf(int &pos, bool (QChar::*is_x)() const)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        QChar ch = m_input.at(pos);

        EXPECT((ch.*is_x)());

        EXPECT(advance(pos, 1));
    }

    EXIT();
}

bool QMetaParserBase::someCharOf(int &pos, QChar &c, bool (QChar::*is_x)() const)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        QChar ch = m_input.at(pos);

        EXPECT((ch.*is_x)());

        c = ch;

        EXPECT(advance(pos, 1));
    }

    EXITV(c);
}

bool QMetaParserBase::oneOf(int& pos, QString chars, QChar &outCh)
{
    ENTRYV(pos, chars);

    EXPECT(pos < m_input.length());

    {
        QChar c = m_input.at(pos);

        EXPECT(chars.contains(c));

        outCh = c;

        EXPECT(advance(pos, 1));
    }

    EXITV(outCh);
}


bool QMetaParserBase::thisChar(int &pos, QChar c)
{
    ENTRYV(pos, c);

    EXPECT(pos < m_input.length());

    EXPECT(m_input.at(pos) == c);

    EXPECT(advance(pos, 1));

    EXIT();
}

bool QMetaParserBase::thisStr(int &pos, QString str)
{
    ENTRYV(pos, str);

    EXPECT(m_input.midRef(pos).startsWith(str));

    EXPECT(advance(pos, str.length()));

    EXIT();
}

bool QMetaParserBase::strOf(int &pos, bool (QChar::*is_x)() const)
{
    ENTRYV(pos);

    EXPECT(pos < m_input.length());

    {
        int i = pos;

        for (; i < m_input.length() && (m_input.at(i).*is_x)(); i++) {
        }

        EXPECT(i - pos);

        EXPECT(advance(pos, i - pos));
    }

    EXIT();
}

/////////////////////  NONTERMINALS  //////////////////////

bool QMetaParserBase::strOf(int &pos, QVariant &str, bool (QChar::*is_x)() const)
{
    ENTRYV(pos);

    CHECK_POINT(cp0, pos);

    EXPECT(strOf(pos, is_x));

    {
        QString _str = mid(cp0, pos);
        str = _str;
    }

    RETURN_SUCCESS();

    EXITV(str);
}

/// For the time being anything is implemented interms of someChar
/// untill QMetaParserBase can operate on non-char streams.
bool QMetaParserBase::anything(int &pos, QVariant& val)
{
    ENTRYV(pos);

    QChar c;
    EXPECT(someChar(pos, c));
    val = c;

    EXITV(val);
}


bool QMetaParserBase::spaces(int &pos)
{
    ENTRYV(pos);

    EXPECT(strOf(pos, &QChar::isSpace));

    EXIT();
}

bool QMetaParserBase::spaces(int &pos, QVariant &space)
{
    ENTRYV(pos);

    EXPECT(strOf(pos, space, &QChar::isSpace));

    EXITV(space);
}

bool QMetaParserBase::identifier(int &pos, QVariant& ident)
{
    ENTRYV(pos);

    CHECK_POINT(cp0, pos);

    if(!thisChar(pos, QChar('_'))) {
        pos = cp0;
        EXPECT(someCharOf(pos, &QChar::isLetter));
    }

    //Optional
    strOf(pos, &QChar::isLetterOrNumber);

    {
        QString id =  mid(cp0, pos);
        ident = id;
    }

    RETURN_SUCCESS();

    EXITV(ident);
}

bool QMetaParserBase::integer(int &pos, QVariant& result)
{
    ENTRYV(pos);

    int sign = 1;

    if (thisChar(pos, QChar('-'))) {
        sign = -1;
    }

    QVariant variant;
    EXPECT(strOf(pos, variant, &QChar::isDigit));

    {
        int r = int(variant.value<QString>().toInt()) * sign;
        result = r;
    }

    RETURN_SUCCESS();

    EXITV(result);
}

bool QMetaParserBase::thisToken(int &pos, QString str)
{
    ENTRYV(pos, str);

    spaces(pos);
    EXPECT(thisStr(pos, str));
    spaces(pos);
    RETURN_SUCCESS();

    EXIT();
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

bool QMetaParserBase::applyRule(int ruleId, int &pos, QVariant &result)
{
    ENTRYV(ruleId, pos);

    EXPECT(m_rule.contains(ruleId));

    {
        MemoKey key = {ruleId, pos};

        if (m_memo.contains(key)) {
            MemoEntry me = m_memo.value(key);
            if (me.nextPos < 0) {
                RETURN_FAILURE("Left recursion detected.");
            }
            result = me.result;
            pos = me.nextPos;
            RETURN_SUCCESS();
        }

        QVariant res;

        RuleFuncPtr ruleFunc = m_rule[ruleId];
        m_memo.insert(key, {FAIL, res});

        EXPECT(ruleFunc(this, pos, res));

        m_memo.insert(key, {pos, res});
        result = res;
    }

    RETURN_SUCCESS();

    EXITV(result);
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

void QMetaParserBase::exitRule(int pos, QString ruleName, bool ok, QString msg)
{
    //TODO: Evaluate if this is the right thing to do
    if (ok) {
        m_errors.clear();
        return;
    }

    auto pe = new ParseError(pos, ruleName, msg);

    if(!m_errors.isEmpty()) {
        //The errors collected so far are from child nodes in the parse tree.
        //Clear the stack and create a new ParseError object on the stack with
        //the previos nodes as its children.

        foreach(ParseError* cpe, m_errors) {
            pe->addChild(cpe);
        }

        m_errors.clear();
    }

    m_errors.append(pe);
}

inline uint qHash(const QMetaParserBase::MemoKey &key, uint seed)
{
    return qHash(key.ruleId, seed) ^ qHash(key.position, seed);
}


