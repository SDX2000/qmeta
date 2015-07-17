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

//class Number
//{
//public:
//    Number()
//        : m_value(0)
//    {
//    }

//    Number(int value)
//        : m_value(value)
//    {
//    }

//    int getValue() const {
//        return m_value;
//    }

//private:
//    int m_value;
//};


//class BinOp
//{
//public:
//    BinOp(QString op, const Number& arg1, const Number& arg2)
//        : m_op(op)
//        , m_arg1(arg1)
//        , m_arg2(arg2)
//    {
//    }

//    virtual Number exec() const = 0;
//    virtual ~BinOp() {}

//    QString getOp() const {
//        return m_op;
//    }

//protected:
//    QString m_op;
//    Number  m_arg1;
//    Number  m_arg2;
//};

/*
#define BINOP(Name, op) \
    class Name: public BinOp \
    { \
    public: \
        Name(const Number& arg1, const Number& arg2) \
            : BinOp(QStringLiteral(#op), arg1, arg2) \
        { \
        } \
        Number exec() const { \
            return Number(m_arg1.getValue() op m_arg2.getValue()); \
        } \
        virtual ~Name() {} \
    };
*/

//BINOP(AddOp, +)
//BINOP(SubOp, -)
//BINOP(MulOp, *)
//BINOP(DivOp, /)


//struct ParseResult
//{
//    const Number value;
//    const QStringRef nextInput;

//    ParseResult(Number val, QStringRef nextInp)
//        : value(val)
//        , nextInput(nextInp)
//    {
//    }
//};

class Parser
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
