#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "utils.h"
#include "parse_status.h"


#define ENTRYV(...) \
    CHECK_POINT(pos0, pos) \
    QString msg;\
    bool ok = true; \
    QSTDOUT() << __FUNCTION__ << "("; \
    TRACEV(__VA_ARGS__); \
    qStdOut() << ")" << endl; \
    g_indentLevel++;

#define EXIT() \
    _exit: \
        if (!ok) { \
            ps.add(ParseError(pos0, __FUNCTION__, msg));\
        } \
        QSTDOUT() << "return " << __FUNCTION__ << "() "; \
        TRACEV(ok); \
        qStdOut() <<endl; \
        g_indentLevel--; \
        return ok;

#define EXITV(...) \
    _exit: \
        if (!ok) { \
            ps.add(ParseError(pos0, __FUNCTION__, msg));\
        } \
        QSTDOUT() << "return " << __FUNCTION__ << "() "; \
        TRACEV(ok, __VA_ARGS__); \
        qStdOut() <<endl; \
        g_indentLevel--; \
        return ok;

#define RETURN_SUCCESS() \
    ok = true; \
    goto _exit;

#define RETURN_FAILURE(MSG) \
    ok = false; \
    goto _exit;

//Note: There is no need to checkpoint EXPECT
//since all back tracking happens at TRY_CHOICE
//and TRY_CHOICE saves a check point.
#define EXPECT(X) \
    if (!(X)) { \
        ok = false; \
        goto _exit; \
    } \

#define TRY_CHOICE(X) \
    do \
    { \
        int _checkPoint = pos; \
        if (X) { \
            ok = true; \
            goto _exit; \
        } \
        pos = _checkPoint; \
    } \
    while(0);

#define TRY(X, NEXT) \
    if (!(X)) { \
        goto NEXT; \
    } \

#define TRY_INV(X, NEXT) \
    if (X) { \
        goto NEXT; \
    } \

#define CHECK_POINT(CP, pos) \
    int CP = pos;


class QMetaParserBase
{
public:
    QMetaParserBase();
    virtual bool parse(int ruleId, QString inp, QVariant& ast, ParseErrorTrail &ps);
    virtual bool parse(int ruleId, int pos, QVariant& ast, ParseErrorTrail &ps) = 0;
    virtual ~QMetaParserBase() {}

    const int FAIL = -1;

public:
    enum RuleEnum {
        //TERMINALS
        THIS_STR,       //Cannot memoize rule with arguments yet
        DIGIT,
        STR_OF,         //Cannot memoize rule with arguments yet
        SOME_CHAR,
        SOME_CHAR_OF,   //Cannot memoize rule with arguments yet
        THIS_CHAR,      //Cannot memoize rule with arguments yet
        ONE_OF,         //Cannot memoize rule with arguments yet
        ANYTHING,       //Cannot memoize rule with arguments yet

        //NONTERMINALS
        SPACES,
        IDENTIFIER,     //8
        INTEGER,
        THIS_TOKEN,     //Cannot memoize rule with arguments yet

        NEXT_RULE,
    };

    struct MemoKey
    {
        int      ruleId;
        int      position;

        bool operator == (const MemoKey& rhs) const {
            return ruleId == rhs.ruleId && position == rhs.position;
        }
    };

    struct MemoEntry
    {
        int        nextPos; //Takes RuleEnum values
        QVariant   result;
    };

protected:
    //TERMINALS//
    bool thisStr(int &pos, QString thisStr, ParseErrorTrail& ps);
    bool digit(int &pos, int& digit, ParseErrorTrail &ps);
    bool strOf(int &pos, bool (QChar::*is_x)() const, ParseErrorTrail &ps);
    bool strOf(int &pos, QVariant &str, bool (QChar::*is_x)() const, ParseErrorTrail &ps);
    bool anything(int& pos, QVariant &val, ParseErrorTrail &ps);
    bool anyChar(int &pos, ParseErrorTrail &ps);
    bool someChar(int& pos, QChar& c, ParseErrorTrail &ps);
    bool someCharOf(int &pos, bool (QChar::*is_x)() const, ParseErrorTrail &ps);
    bool someCharOf(int &pos, QChar &c, bool (QChar::*is_x)() const, ParseErrorTrail &ps);
    bool thisChar(int &pos, QChar c, ParseErrorTrail &ps);
    bool oneOf(int& pos, QChar &opOut, QString operators, ParseErrorTrail &ps);

    //NONTERMINALS//
    virtual bool spaces(int &pos, ParseErrorTrail &ps);
    virtual bool thisToken(int &pos, QString str, ParseErrorTrail &ps);

    //HELPER FUNCTIONS//
    virtual QChar unescape(QChar c);
    virtual bool applyRule(int ruleId, int &pos, QVariant& result, ParseErrorTrail &ps);
    QString mid(int pos0, int pos1);

protected:
    typedef bool (QMetaParserBase::*RuleMemberFuncPtr)(int &pos, QVariant &result, ParseErrorTrail& ps);
    typedef bool (*RuleFuncPtr)(QMetaParserBase* self, int &pos, QVariant &result, ParseErrorTrail& ps);
    QHash<MemoKey, MemoEntry> m_memo;
    QHash<int, RuleFuncPtr> m_rule;

private:
    bool advance(int &pos, int length, ParseErrorTrail &ps);

    //Accessible only through applyRule
    bool spaces(int &pos, QVariant& spaces, ParseErrorTrail &ps);
    bool identifier(int &pos, QVariant &ident, ParseErrorTrail &ps);
    bool integer(int &pos, QVariant &result, ParseErrorTrail &ps);
    void initRuleMap();

private:
    QString m_input;
};


#endif // PARSER_H
