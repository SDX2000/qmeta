#include "QMetaParserBase.h"
#include <assert.h>
#include <algorithm>


//////////////////// PUBLIC FUNCTIONS ///////////////////

QMetaParserBase::QMetaParserBase()
    : m_error(nullptr)
    , m_indentLevel(0)
{
    //initRuleMap();
}

const ParseError *QMetaParserBase::getError() const
{
    //return m_errors[0];
    return m_error;
}

bool QMetaParserBase::applyRule(int ruleId, int &pos, QVariant &result, ParseErrorPtr &pe)
{
    ENTRYV(ruleId, pos);

    EXPECT(m_rule.contains(ruleId));

    {
        MemoKey key = {ruleId, pos};

        if (m_memo.contains(key)) {
            MemoEntry me = m_memo.value(key);
            if (me.nextPos < 0) {
                //RETURN_FAILURE("Left recursion detected.");
                RETURN_FAILURE();
            }
            result = me.result;
            pos = me.nextPos;
            RETURN_SUCCESS();
        }

        QVariant res;

        RuleFuncPtr ruleFunc = m_rule[ruleId];
        m_memo.insert(key, {FAIL, res});

        EXPECT(ruleFunc(this, pos, res, cpe));

        m_memo.insert(key, {pos, res});
        result = res;
    }

    RETURN_SUCCESS();

    EXITV(result);
}

inline uint qHash(const QMetaParserBase::MemoKey &key, uint seed)
{
    return qHash(key.ruleId, seed) ^ qHash(key.position, seed);
}


