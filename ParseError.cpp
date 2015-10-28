#include "ParseError.h"
#include "utils.h"

int ParseError::getPos() const {
    return m_pos;
}

QTextStream& ParseError::print(QTextStream& s, int indentLevel) const {
    printIndent(s, indentLevel);
    s << "FAIL: " << m_msg << ":" << m_lineNumber << ": "
      << m_ruleName << " at pos " << m_pos << endl;
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

ParseError::ParseError(int pos, QString ruleName, QString fileName, int lineNumber)
    : m_pos(pos)
    , m_inp(nullptr)
    , m_ruleName(ruleName)
    , m_msg(fileName)
    , m_lineNumber(lineNumber)
{
}

ParseError::ParseError(const QVariantConstPtr inp, QString ruleName, QString fileName, int lineNumber)
    : m_inp(inp)
    , m_ruleName(ruleName)
    , m_msg(fileName)
    , m_lineNumber(lineNumber)
{
}

void ParseError::addChild(ParseError *pe)
{
    m_children.append(pe);
}

