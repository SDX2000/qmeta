#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QTextStream>

#define QSL(str) QStringLiteral(str)

//QTextStream qStdOut();

#define EXPECT(X)       \
    if (!(X)) {         \
        return false;   \
    }

#define MAYBE(X)       \
    if (X) {         \
        return true; \
    }

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
    bool parse(QString inp, int& result);

private:
    bool str(QStringRef &inp, QString str);
    bool value(QStringRef &inp, int& result);
    bool factor(QStringRef& inp, int &result);
    bool term(QStringRef &inp, int& result);
    bool expr(QStringRef &inp, int& result);
    bool somechar(QStringRef& inp, QChar &c);
    bool thischar(QStringRef& inp, QChar &opOut, QString operators);
    bool advance(QStringRef &str, int length);
};



#endif // PARSER_H
