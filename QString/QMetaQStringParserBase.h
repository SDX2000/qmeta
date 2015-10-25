#ifndef QMETAQSTRINGPARSERBASE_H
#define QMETAQSTRINGPARSERBASE_H

#include <assert.h>
#include <QString>
#include "utils.h"
#include "ParseError.h"

class QMetaQStringParserBase
{
public:
    QMetaQStringParserBase(int ruleId, const QString &inp);
    virtual ~QMetaQStringParserBase();

    virtual bool parse(QVariant& ast);
    virtual bool parse(int ruleId, int pos, QVariant& ast, ParseErrorPtr& pe) = 0;
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
    QString mid(int pos0, int pos1);

private:
    bool advance(int &pos, int length, ParseErrorPtr& pe);

    //Accessible only through applyRule
    bool spaces(int &pos, QVariant& spaces, ParseErrorPtr& pe);
    bool identifier(int &pos, QVariant &ident, ParseErrorPtr& pe);
    bool integer(int &pos, QVariant &result, ParseErrorPtr& pe);
    void initRuleMap();

protected:
    virtual bool applyRule(int ruleId, int &pos, QVariant& result, ParseErrorPtr& pe);
    typedef bool (QMetaQStringParserBase::*RuleMemberFuncPtr)(int &pos, QVariant &result, ParseErrorPtr& pe);
    typedef bool (*RuleFuncPtr)(QMetaQStringParserBase* self, int &pos, QVariant &result, ParseErrorPtr& pe);
    QHash<int, RuleFuncPtr> m_rule;

    QHash<MemoKey, MemoEntry>   m_memo;
    ParseError*                 m_error;
    int                         m_indentLevel;

private:
    const QString   m_input;
    int             m_ruleId;
};

#endif // QMETAQSTRINGPARSERBASE_H
