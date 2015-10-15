#ifndef PARSER_H
#define PARSER_H


#include <assert.h>
#include <QString>
#include "utils.h"
#include "ParseError.h"

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
    virtual ~QMetaParserBase() {}
    const ParseError* getError() const;

    const int FAIL = -1;

public:
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
    virtual bool applyRule(int ruleId, int &pos, QVariant& result, ParseErrorPtr& pe);

protected:
    typedef bool (QMetaParserBase::*RuleMemberFuncPtr)(int &pos, QVariant &result, ParseErrorPtr& pe);
    typedef bool (*RuleFuncPtr)(QMetaParserBase* self, int &pos, QVariant &result, ParseErrorPtr& pe);
    QHash<int, RuleFuncPtr> m_rule;

    QHash<MemoKey, MemoEntry>   m_memo;
    ParseError*                 m_error;
    int                         m_indentLevel;
};


#endif // PARSER_H
