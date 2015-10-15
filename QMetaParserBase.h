#ifndef PARSER_H
#define PARSER_H


#include <assert.h>
#include <QString>
#include "utils.h"
#include "ParseError.h"

#define QSTDOUT() printIndent(m_indentLevel); qStdOut()

#define ENTRYV(...) \
    CHECK_POINT(_pos0); \
    QString msg;\
    safeDelete(pe); \
    ParseErrorPtr cpe = nullptr; \
    QSTDOUT() << __FUNCTION__ << "("; \
    TRACEV(__VA_ARGS__); \
    qStdOut() << ")" << endl; \
    m_indentLevel++;

#define EXIT() \
    _exit: \
        QSTDOUT() << "return " << __FUNCTION__ << "() "; \
        bool ok = (pe == nullptr); \
        TRACEV(ok); \
        qStdOut() <<endl; \
        m_indentLevel--; \
        return ok;

#define EXITV(...) \
    _exit: \
        QSTDOUT() << "return " << __FUNCTION__ << "() "; \
        bool ok = (pe == nullptr); \
        TRACEV(ok, __VA_ARGS__); \
        qStdOut() <<endl; \
        m_indentLevel--; \
        return ok;

#define RETURN_SUCCESS() \
    safeDelete(pe); \
    goto _exit;

#define RETURN_FAILURE() \
    _RETURN_FAILURE(_exit)

#define _RETURN_FAILURE(TARGET) \
    if (!pe) { \
        pe = new ParseError(_pos0, __FUNCTION__, __FILE__, __LINE__); \
    } \
    if (cpe) { \
        pe->addChild(cpe); \
        cpe = nullptr; \
    } \
    goto TARGET;

//Note: There is no need to checkpoint EXPECT
//since all back tracking happens at TRY_CHOICE
//and TRY_CHOICE saves a check point.
#define EXPECT(X) \
    if (!(X)) { \
        RETURN_FAILURE() \
    } \

#define TRY_CHOICE(X) \
    do \
    { \
        int _checkPoint = pos; \
        if (X) { \
            RETURN_SUCCESS() \
        } \
        pos = _checkPoint; \
    } \
    while(0);

#define TRY(X, NEXT) \
    if (!(X)) { \
        _RETURN_FAILURE(NEXT) \
    } \

#define TRY_INV(X, NEXT) \
    if (X) { \
        _RETURN_FAILURE(NEXT) \
    } \

#define CHECK_POINT(CP) \
    int CP = pos;


class QMetaParserBase
{
public:
    QMetaParserBase();
    virtual bool parse(int ruleId, const QString& inp, QVariant& ast);
    virtual bool parse(int ruleId, int pos, QVariant& ast, ParseErrorPtr& pe) = 0;
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
    bool thisStr(int &pos, QString thisStr, ParseErrorPtr& pe);
    bool digit(int &pos, int& digit, ParseErrorPtr& pe);
    bool strOf(int &pos, bool (QChar::*is_x)() const, ParseErrorPtr& pe);
    bool strOf(int &pos, QVariant &str, bool (QChar::*is_x)() const, ParseErrorPtr& pe);
    bool anything(int& pos, QVariant &val, ParseErrorPtr& pe);
    bool anyChar(int &pos, ParseErrorPtr& pe);
    bool someChar(int& pos, QChar& c, ParseErrorPtr& pe);
    bool someCharOf(int &pos, bool (QChar::*is_x)() const, ParseErrorPtr& pe);
    bool someCharOf(int &pos, QChar &c, bool (QChar::*is_x)() const, ParseErrorPtr& pe);
    bool thisChar(int &pos, QChar c, ParseErrorPtr& pe);
    bool oneOf(int& pos, QString operators, QChar &opOut, ParseErrorPtr& pe);

    //NONTERMINALS//
    virtual bool spaces(int &pos, ParseErrorPtr& pe);
    virtual bool thisToken(int &pos, QString str, ParseErrorPtr& pe);

    //HELPER FUNCTIONS//
    virtual QChar unescape(QChar c);
    virtual bool applyRule(int ruleId, int &pos, QVariant& result, ParseErrorPtr& pe);
    QString mid(int pos0, int pos1);

protected:
    typedef bool (QMetaParserBase::*RuleMemberFuncPtr)(int &pos, QVariant &result, ParseErrorPtr& pe);
    typedef bool (*RuleFuncPtr)(QMetaParserBase* self, int &pos, QVariant &result, ParseErrorPtr& pe);
    QHash<int, RuleFuncPtr> m_rule;

    QHash<MemoKey, MemoEntry>   m_memo;
    ParseError*                 m_error;
    int                         m_indentLevel;

private:
    bool advance(int &pos, int length, ParseErrorPtr& pe);

    //Accessible only through applyRule
    bool spaces(int &pos, QVariant& spaces, ParseErrorPtr& pe);
    bool identifier(int &pos, QVariant &ident, ParseErrorPtr& pe);
    bool integer(int &pos, QVariant &result, ParseErrorPtr& pe);
    void initRuleMap();

private:
    QString m_input;
};


#endif // PARSER_H
