#ifndef PARSESTATUS_H
#define PARSESTATUS_H

#include <QString>
#include "utils.h"

class ParseError
{
public:
    ParseError(int pos, QString ruleName, QString msg);

    int getPos() const;

    QString toString() const;
    ~ParseError();

private:
    int             m_pos;
    QString         m_ruleName;
    QString         m_msg;
};

class ParseErrorTrail
{
public:
    void add(ParseError ps);
    void clear();
    QString toString() const;
    bool isEmpty() const;
private:
    QMultiHash<int, ParseError> m_childNodes;
};

#endif // PARSESTATUS_H
