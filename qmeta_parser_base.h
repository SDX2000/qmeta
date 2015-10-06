#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "utils.h"
#include "parse_status.h"


//Note: There is no need to checkpoint EXPECT
//since all back tracking happens at TRY_CHOICE
//and TRY_CHOICE saves a check point.
#define EXPECT(X) \
    do \
    { \
        ParseStatusPtr __ps = (X);\
        if (!*__ps) { \
            return ParseStatus::failure(inp, "Expected: " #X, __ps); \
        } \
    } while(0); \

#define EXPECT_B(X) \
    if (!(X)) { \
        return ParseStatus::failure(inp, #X); \
    }


#define TRY_CHOICE(X) \
    do \
    { \
        QStringRef _checkPoint = inp; \
        if (*(X)) { \
            return ParseStatus::success(); \
        } \
        inp = _checkPoint; \
    } \
    while(0);

#define TRY(X, NEXT) \
    if (!*(X)) { \
        goto NEXT; \
    } \

#define TRY_INV(X, NEXT) \
    if (*(X)) { \
        goto NEXT; \
    } \

#define CHECK_POINT(CP, INP) \
    QStringRef CP = INP;


class QMetaParserBase
{
public:
    QMetaParserBase();
    virtual ParseStatusPtr parse(QString inp, QVariant& ast);
    virtual ParseStatusPtr parse(QStringRef inp, QVariant &ast) = 0;
    virtual ~QMetaParserBase() {}

private:
    ParseStatusPtr advance(QStringRef &inp, int length);

    //Accessible only through applyRule
    ParseStatusPtr spaces(QStringRef &inp, QVariant& spaces);
    ParseStatusPtr identifier(QStringRef &inp, QVariant &ident);
    ParseStatusPtr integer(QStringRef &inp, QVariant &result);


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

        //NONTERMINALS
        SPACES,
        IDENTIFIER,
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
    ParseStatusPtr thisStr(QStringRef &inp, QString thisStr);
    ParseStatusPtr digit(QStringRef &inp, int& digit);
    ParseStatusPtr strOf(QStringRef &inp, bool (QChar::*is_x)() const);
    ParseStatusPtr strOf(QStringRef &inp, QVariant &str, bool (QChar::*is_x)() const);
    ParseStatusPtr someChar(QStringRef& inp, QChar& c);
    ParseStatusPtr someCharOf(QStringRef &inp, bool (QChar::*is_x)() const);
    ParseStatusPtr someCharOf(QStringRef &inp, QChar &c, bool (QChar::*is_x)() const);
    ParseStatusPtr thisChar(QStringRef &inp, QChar c);
    ParseStatusPtr oneOf(QStringRef& inp, QChar &opOut, QString operators);

    //NONTERMINALS//
    virtual ParseStatusPtr spaces(QStringRef &inp);
    virtual ParseStatusPtr thisToken(QStringRef &inp, QString str);

    //HELPER FUNCTIONS//
    virtual QChar unescape(QChar c);
    virtual ParseStatusPtr applyRule(int ruleId, QStringRef &inp, QVariant& result);
    virtual void initRuleMap();

protected:
    typedef ParseStatusPtr (QMetaParserBase::*RuleFuncPtr)(QStringRef &inp, QVariant &result);
    QHash<MemoKey, MemoEntry> m_memo;
    QHash<int, RuleFuncPtr> m_rule;
};


#endif // PARSER_H
