#ifndef PARSESTATUS_H
#define PARSESTATUS_H

#include <QList>
#include <QString>
#include "utils.h"



class ParseError
{
public:
    ParseError(int pos, QString ruleName, QString fileName, int lineNumber);

    void addChild(ParseError * pe);

    int getPos() const;

    virtual ~ParseError();

    QTextStream& print(QTextStream& s, int indentLevel = 0) const;

private:
    int                 m_pos;
    QString             m_ruleName;
    QString             m_msg;
    QList<ParseError*>  m_children;
    int                 m_lineNumber;
};

typedef ParseError* ParseErrorPtr;

#endif // PARSESTATUS_H
