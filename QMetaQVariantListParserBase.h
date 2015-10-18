#ifndef QMETAASTTRANSLATOR_H
#define QMETAASTTRANSLATOR_H

#include <QVariant>
#include <QVariantList>

#include "ParseError.h"

#define ENTRYV(...) \
    CHECK_POINT(_inp0); \
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
        pe = new ParseError(_inp0, __FUNCTION__, __FILE__, __LINE__); \
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
        QVariant _checkPoint = pos; \
        if (X) { \
            RETURN_SUCCESS() \
        } \
        inp = _checkPoint; \
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
    QVariant CP = inp;

class QMetaQVariantListParserBase
{
public:
    QMetaQVariantListParserBase(int ruleId, const QVariant &input);
    virtual bool parse(QVariant& output) = 0;
    virtual ~QMetaQVariantListParserBase() {}
public:
    enum RuleEnum {
        //TERMINALS
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

protected:
    //TERMINALS//
    bool thisStr(QVariant &inp, const QString& thisStr, ParseErrorPtr& pe);
    bool digit(QVariant &inp, int& digit, ParseErrorPtr& pe);
    bool anyChar(QVariant &inp, ParseErrorPtr& pe);
    bool someChar(QVariant &inp, QChar& c, ParseErrorPtr& pe);
    bool someCharOf(QVariant &inp, bool (QChar::*is_x)() const, ParseErrorPtr& pe);
    bool someCharOf(QVariant &inp, QChar &c, bool (QChar::*is_x)() const, ParseErrorPtr& pe);
    bool thisChar(QVariant &inp, QChar c, ParseErrorPtr& pe);
    bool oneOf(QVariant &inp, const QString& chars, QChar &outCh, ParseErrorPtr& pe);
    bool anything(QVariant &inp, QVariant &val, ParseErrorPtr& pe);

    //HELPER FUNCTIONS//
    virtual QChar unescape(QChar c);

private:
    //Accessible only through applyRule
    bool integer(QVariant &inp, QVariant &integer, ParseErrorPtr& pe);
    void initRuleMap();

protected:
    int             m_indentLevel;

private:
    int             m_startRuleId;
    const QVariant  m_input;

};


#endif // QMETAASTTRANSLATOR_H
