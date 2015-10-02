#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QTextStream>

#define QSL(str) QStringLiteral(str)

//QTextStream qStdOut();

class ParseError
{
public:
    ParseError(QString msg)
        : m_msg(msg)
    {
    }

    const QString& getMessage() const
    {
        return m_msg;
    }

private:
    QString m_msg;
};

class ArgumentException
{
public:
    ArgumentException(QString msg)
        : m_msg(msg)
    {
    }

    const QString& getMessage() const
    {
        return m_msg;
    }

private:
    QString m_msg;
};

class Interpreter
{
public:
    void parse(QString inp, int& result);

private:
    QStringRef expect(QStringRef inp,  QString str);
    QStringRef value(QStringRef inp,   int& result);
    QStringRef factor(QStringRef inp,  int& result);
    QStringRef term(QStringRef inp,    int& result);
    QStringRef expr(QStringRef inp,    int& result);

private:
    QString     m_inputStr;
};



#endif // PARSER_H
