#include "parse_status.h"
#include "utils.h"

int ParseError::getPos() const {
    return m_pos;
}

QString ParseError::toString() const {
    return QString(QSL("ERROR:%1 in %2() at %3"))
                   .arg(m_msg).arg(m_ruleName).arg(m_pos);
}

ParseError::~ParseError()
{
}

ParseError::ParseError(int pos, QString ruleName, QString msg)
    : m_pos(pos)
    , m_ruleName(ruleName)
    , m_msg(msg)
{
}

void ParseErrorTrail::add(ParseError ps)
{
    m_childNodes.insert(ps.getPos(), ps);
}

void ParseErrorTrail::clear()
{
    m_childNodes.clear();
}

QString ParseErrorTrail::toString() const
{
    QString str;
    auto values = m_childNodes.values();

    int i = values.length();

    foreach(ParseError ps, values) {
        str += ps.toString();
        i--;

        if(i) {
            str += ", ";
        }
    }

    return str;
}

bool ParseErrorTrail::isEmpty() const
{
    return m_childNodes.isEmpty();
}
