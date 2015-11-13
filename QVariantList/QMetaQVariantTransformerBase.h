#ifndef QMETAASTTRANSLATOR_H
#define QMETAASTTRANSLATOR_H

#include <QVariant>
#include <QVariantList>
#include <QHash>

#include "ParseError.h"
#include "ParseFail.h"
#include "utils.h"

class QMetaQVariantTransformerBase
{
public:
    QMetaQVariantTransformerBase(int ruleId, const QVariant &input);
    virtual bool parse(QString& output) = 0;
    virtual ~QMetaQVariantTransformerBase() {}

    const int FAIL = -1;
public:
    enum RuleEnum {
        //TERMINALS
        STRING,
        THIS_STR,       //Cannot memoize rule with arguments yet
        DIGIT,
        SOME_CHAR,
        SOME_CHAR_OF,   //Cannot memoize rule with arguments yet
        THIS_CHAR,      //Cannot memoize rule with arguments yet
        ONE_OF,         //Cannot memoize rule with arguments yet
        ANYTHING,       //Cannot memoize rule with arguments yet

        //NONTERMINALS
        INTEGER,
        NEXT_RULE,
    };

    struct MemoKey
    {
        int                 ruleId;
        QVariantConstPtr    input;

        bool operator == (const MemoKey& rhs) const {
            return ruleId == rhs.ruleId && input == rhs.input;
        }
    };

    struct MemoEntry
    {
        QVariantConstPtr rest;
        QString          result;
    };


protected:
    //TERMINALS//
    bool string(QVariantConstPtr input, QVariant& output, ParseErrorPtr &pe);
    bool thisStr(QVariantConstPtr input, const QString& thisStr, ParseErrorPtr& pe);
    bool digit(QVariantConstPtr input, int& digit, ParseErrorPtr& pe);
    bool anyChar(QVariantConstPtr input, ParseErrorPtr& pe);
    bool someChar(QVariantConstPtr input, QChar& c, ParseErrorPtr& pe);
    bool someCharOf(QVariantConstPtr input, bool (QChar::*is_x)() const, ParseErrorPtr& pe);
    bool someCharOf(QVariantConstPtr input, QChar &c, bool (QChar::*is_x)() const, ParseErrorPtr& pe);
    bool thisChar(QVariantConstPtr input, QChar c, ParseErrorPtr& pe);
    bool oneOf(QVariantConstPtr input, const QString& chars, QChar &outCh, ParseErrorPtr& pe);
    bool anything(QVariantConstPtr input, QVariant &val, ParseErrorPtr& pe);

    //HELPER FUNCTIONS//
    virtual QChar unescape(QChar c);
    void render(QString &out, QString templateName, const QVariantHash &vars);

private:
    //Accessible only through applyRule
    bool integer(QVariantConstPtr input, QVariant &integer, ParseErrorPtr& pe);
    void initRuleMap();

protected:
    virtual bool applyRule(int ruleId, QVariantConstPtr input, QString& result, ParseErrorPtr& pe);
    typedef bool (QMetaQVariantTransformerBase::*RuleMemberFuncPtr)(QVariantConstPtr input, QString &result, ParseErrorPtr& pe);
    typedef bool (*RuleFuncPtr)(QMetaQVariantTransformerBase* self, QVariantConstPtr input, QString &result, ParseErrorPtr& pe);
    QHash<int, RuleFuncPtr> m_rule;

    QHash<MemoKey, MemoEntry>   m_memo;
    ParseError*                 m_error;
    int                         m_indentLevel;
    int                         m_startRuleId;
    //TODO: Make this a const variable
    QVariant                    m_input;
    QVariant                    m_fail;
};


#endif // QMETAASTTRANSLATOR_H
