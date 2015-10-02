#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>

#include "programinterpreter.h"

int main(/*int argc, char *argv[]*/)
{
    //QCoreApplication a(argc, argv);

    //QString inp(QStringLiteral("2+3*4-1"));

    QTextStream cin(stdin);
    QTextStream cout(stdout);
    ProgramInterpreter interp;


    while(true)
    {
        cout << QSL("> ");
        cout.flush();
        QString inp = cin.readLine();

        if (inp.toLower() == QSL("q")) {
            break;
        }

        int result;
        if (interp.parse(inp, result)) {
            cout<<result<<endl;
        } else {
            cout<<"Syntax error."<<endl;
        }

    }

    //return a.exec();

    return 0;
}



