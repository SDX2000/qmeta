#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QCommandLineParser>
#include <QFile>

#include "programinterpreter.h"

#include <iostream>

using namespace std;

void doREPL()
{
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
}

void execute(QString prog)
{
    ProgramInterpreter interp;
    int result;
    const ParseStatus * ps = interp.parse(prog, result);
    if (ps->isOk()) {
        cout<<result<<endl;
    } else {
        do {
            cout<<ps->toString().toStdString().c_str()<<" ";
        }while((ps = ps->getInnerFailure()));
        cout<<endl;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("ushell");
    QCoreApplication::setApplicationVersion("0.0.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("A modern crossplatform shell script and REPL.");
    parser.addHelpOption();
    parser.addVersionOption();

    if (!parser.parse(QCoreApplication::arguments())) {
        cout << "ERROR: Invalid command line options." <<endl;
        return 1;
    }

    const QStringList args = parser.positionalArguments();

    if (args.isEmpty()) {
        doREPL();
        return 0;
    }

    foreach(QString fileName, args) {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text))
            continue;
        QTextStream in(&file);
        execute(in.readAll());
    }

    //TODO: Return a relevant error code.
    return 0;
}



