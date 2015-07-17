#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>

#include "parser.h"

int main(/*int argc, char *argv[]*/)
{
    //QCoreApplication a(argc, argv);

    QString inp(QStringLiteral("2+3*4-1"));

    Parser parser;

    try
    {
        int result;
        parser.parse(inp, result);
        qDebug()<<result<<endl;
    }
    catch(ParseError &ex)
    {
        qDebug()<<ex.getMessage()<<endl;
    }

    //return a.exec();

    return 0;
}



