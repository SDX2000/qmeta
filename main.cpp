#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>

#include "parser.h"

int main(/*int argc, char *argv[]*/)
{
    //QCoreApplication a(argc, argv);

    //QString inp(QStringLiteral("2+3*4-1"));

    QTextStream cin(stdin);
    QTextStream cout(stdout);
    Interpreter interp;


    while(true)
    {
        cout << QSL("> ");
        cout.flush();
        QString inp = cin.readLine();

        if (inp.toLower() == QSL("q")) {
            break;
        }

        try
        {
            int result;
            interp.parse(inp, result);
            cout<<result<<endl;
        }
        catch(ParseError &ex)
        {
            qDebug()<<ex.getMessage()<<endl;
        }
    }

    //return a.exec();

    return 0;
}



