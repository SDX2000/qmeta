#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "utils.h"
#include "parse_status.h"

#define RETURN_SUCCESS() \
    ok = true; \
    ps = ParseStatus::success(); \
    goto _exit;

#define RETURN_FAILURE(...) \
    ok = false; \
    if(ps) { \
        ps->chain(ParseStatus::failure(__VA_ARGS__));\
    } else { \
        ps = ParseStatus::failure(__VA_ARGS__); \
    } \
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
    virtual bool parse(int ruleId, QString inp, QVariant& ast, ParseStatusPtr &ps);
    virtual bool parse(int ruleId, int pos, QVariant& ast, ParseStatusPtr &ps) = 0;
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
    bool thisStr(int &pos, QString thisStr, ParseStatusPtr& ps);
    bool digit(int &pos, int& digit, ParseStatusPtr &ps);
    bool strOf(int &pos, bool (QChar::*is_x)() const, ParseStatusPtr &ps);
    bool strOf(int &pos, QVariant &str, bool (QChar::*is_x)() const, ParseStatusPtr &ps);
    bool anything(int& pos, QVariant &val, ParseStatusPtr &ps);
    bool anyChar(int &pos, ParseStatusPtr &ps);
    bool someChar(int& pos, QChar& c, ParseStatusPtr &ps);
    bool someCharOf(int &pos, bool (QChar::*is_x)() const, ParseStatusPtr &ps);
    bool someCharOf(int &pos, QChar &c, bool (QChar::*is_x)() const, ParseStatusPtr &ps);
    bool thisChar(int &pos, QChar c, ParseStatusPtr &ps);
    bool oneOf(int& pos, QChar &opOut, QString operators, ParseStatusPtr &ps);

    //NONTERMINALS//
    virtual bool spaces(int &pos, ParseStatusPtr &ps);
    virtual bool thisToken(int &pos, QString str, ParseStatusPtr &ps);

    //HELPER FUNCTIONS//
    virtual QChar unescape(QChar c);
    virtual bool applyRule(int ruleId, int &pos, QVariant& result, ParseStatusPtr &ps);
    QString mid(int pos0, int pos1);

protected:
    typedef bool (QMetaParserBase::*RuleMemberFuncPtr)(int &pos, QVariant &result, ParseStatusPtr& ps);
    typedef bool (*RuleFuncPtr)(QMetaParserBase* self, int &pos, QVariant &result, ParseStatusPtr& ps);
    QHash<MemoKey, MemoEntry> m_memo;
    QHash<int, RuleFuncPtr> m_rule;

private:
    bool advance(int &pos, int length, ParseStatusPtr &ps);

    //Accessible only through applyRule
    bool spaces(int &pos, QVariant& spaces, ParseStatusPtr &ps);
    bool identifier(int &pos, QVariant &ident, ParseStatusPtr &ps);
    bool integer(int &pos, QVariant &result, ParseStatusPtr &ps);
    void initRuleMap();

private:
    QString m_input;
};


#endif // PARSER_H
