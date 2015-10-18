#ifndef QMETAQSTRINGPARSERBASE_H
#define QMETAQSTRINGPARSERBASE_H

#include "QMetaParserBase.h"

class QMetaQStringParserBase : public QMetaParserBase
{
public:
    QMetaQStringParserBase(int ruleId, const QString &inp);
    virtual bool parse(QVariant& ast);
    virtual bool parse(int ruleId, int pos, QVariant& ast, ParseErrorPtr& pe) = 0;
    virtual ~QMetaQStringParserBase();
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

private:
    QString m_input;
    int     m_ruleId;
};

#endif // QMETAQSTRINGPARSERBASE_H
