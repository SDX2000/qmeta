#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "utils.h"
#include "parse_status.h"

#define QSTDOUT() printIndent(m_indentLevel); qStdOut()

#define ENTRYV(...) \
    CHECK_POINT(_pos0, pos); \
    QString msg;\
    bool ok = true; \
    QSTDOUT() << __FUNCTION__ << "("; \
    TRACEV(__VA_ARGS__); \
    qStdOut() << ")" << endl; \
    m_indentLevel++;

#define EXIT() \
    _exit: \
        exitRule(_pos0, __FUNCTION__, ok, msg); \
        QSTDOUT() << "return " << __FUNCTION__ << "() "; \
        TRACEV(ok); \
        qStdOut() <<endl; \
        m_indentLevel--; \
        return ok;

#define EXITV(...) \
    _exit: \
        exitRule(_pos0, __FUNCTION__, ok, msg); \
        QSTDOUT() << "return " << __FUNCTION__ << "() "; \
        TRACEV(ok, __VA_ARGS__); \
        qStdOut() <<endl; \
        m_indentLevel--; \
        return ok;

#define RETURN_SUCCESS() \
    ok = true; \
    goto _exit;

#define RETURN_FAILURE(MSG) \
    ok = false; \
    msg = MSG; \
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
    virtual bool parse(int ruleId, QString inp, QVariant& ast);
    virtual bool parse(int ruleId, int pos, QVariant& ast) = 0;
    virtual ~QMetaParserBase() {}
    const ParseError* getError() const;

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
    bool thisStr(int &pos, QString thisStr);
    bool digit(int &pos, int& digit);
    bool strOf(int &pos, bool (QChar::*is_x)() const);
    bool strOf(int &pos, QVariant &str, bool (QChar::*is_x)() const);
    bool anything(int& pos, QVariant &val);
    bool anyChar(int &pos);
    bool someChar(int& pos, QChar& c);
    bool someCharOf(int &pos, bool (QChar::*is_x)() const);
    bool someCharOf(int &pos, QChar &c, bool (QChar::*is_x)() const);
    bool thisChar(int &pos, QChar c);
    bool oneOf(int& pos, QString operators, QChar &opOut);

    //NONTERMINALS//
    virtual bool spaces(int &pos);
    virtual bool thisToken(int &pos, QString str);

    //HELPER FUNCTIONS//
    virtual QChar unescape(QChar c);
    virtual bool applyRule(int ruleId, int &pos, QVariant& result);
    QString mid(int pos0, int pos1);

    void exitRule(int pos, QString ruleName, bool ok, QString msg);

protected:
    typedef bool (QMetaParserBase::*RuleMemberFuncPtr)(int &pos, QVariant &result);
    typedef bool (*RuleFuncPtr)(QMetaParserBase* self, int &pos, QVariant &result);
    QHash<int, RuleFuncPtr> m_rule;

    QHash<MemoKey, MemoEntry>   m_memo;
    QList<ParseError*>          m_errors;
    int                         m_indentLevel;

private:
    bool advance(int &pos, int length);

    //Accessible only through applyRule
    bool spaces(int &pos, QVariant& spaces);
    bool identifier(int &pos, QVariant &ident);
    bool integer(int &pos, QVariant &result);
    void initRuleMap();

private:
    QString m_input;
};


#endif // PARSER_H
