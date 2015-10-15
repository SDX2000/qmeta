#ifndef QMETAASTTRANSLATOR_H
#define QMETAASTTRANSLATOR_H

#include "QMetaParserBase.h"

#include <QVariant>
#include <QVariantList>

class QMetaQVariantListParserBase : public QMetaParserBase
{
public:
    QMetaQVariantListParserBase();
    virtual bool parse(int ruleId, const QVariantList &inp, QVariant& ast);
    virtual bool parse(int ruleId, int pos, QVariant& ast, ParseErrorPtr& pe) = 0;
    virtual ~QMetaQVariantListParserBase() {}
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
    bool anything(int& pos, QVariant &val, ParseErrorPtr& pe);
    bool anyChar(int &pos, ParseErrorPtr& pe);
    bool someChar(int& pos, QChar& c, ParseErrorPtr& pe);
    bool someCharOf(int &pos, bool (QChar::*is_x)() const, ParseErrorPtr& pe);
    bool someCharOf(int &pos, QChar &c, bool (QChar::*is_x)() const, ParseErrorPtr& pe);
    bool thisChar(int &pos, QChar c, ParseErrorPtr& pe);
    bool oneOf(int& pos, QString operators, QChar &opOut, ParseErrorPtr& pe);

    //HELPER FUNCTIONS//
    virtual QChar unescape(QChar c);

private:
    bool advance(int &pos, int length, ParseErrorPtr& pe);

    //Accessible only through applyRule
    bool integer(int &pos, QVariant &integer, ParseErrorPtr& pe);
    void initRuleMap();

private:
    QVariantList m_input;
};


#endif // QMETAASTTRANSLATOR_H
