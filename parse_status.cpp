#include "parse_status.h"
#include "utils.h"

int ParseError::getPos() const {
    return m_pos;
}

QTextStream& ParseError::print(QTextStream& s, int indentLevel) const {
    printIndent(s, indentLevel);
    s << "ERROR: " << m_msg << " in " << m_ruleName << " at " << m_pos <<endl;
    foreach(ParseError* cpe, m_children) {
        cpe->print(s, indentLevel + 1);
    }
    return s;
}

ParseError::~ParseError()
{
    foreach(ParseError* cpe, m_children) {
        safeDelete(cpe);
    }
}

ParseError::ParseError(int pos, QString ruleName, QString msg)
    : m_pos(pos)
    , m_ruleName(ruleName)
    , m_msg(msg)
{
}

void ParseError::addChild(ParseError *pe)
{
    m_children.append(pe);
}

