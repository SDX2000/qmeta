#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>

#include "programinterpreter.h"

int main(/*int argc, char *argv[]*/)
{
    //QCoreApplication a(argc, argv);

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
        const ParseStatus * ps = interp.parse(inp, result);
        if (ps->isOk()) {
            cout<<result<<endl;
        } else {
            do {
                cout<<ps->toString().toStdString().c_str()<<" ";
            }while((ps = ps->getInnerFailure()));
            cout<<endl;
        }
    }

    //return a.exec();

    return 0;
}



