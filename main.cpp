#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QCommandLineParser>
#include <QFile>

#include "qmeta_parser.h"

#include <iostream>

using namespace std;

void doREPL()
{
    QTextStream cin(stdin);
    QTextStream cout(stdout);
    QMetaParser interp;

    while(true)
    {
        cout << QSL("> ");
        cout.flush();
        QString inp = cin.readLine();

        if (inp.toLower() == QSL("q")) {
            break;
        }

        QVariant result;
        const ParseStatus * ps = interp.parse(inp, result);
        if (ps) {
            cout<<result.toString().toStdString().c_str()<<endl;
        } else {
            do {
                cout<<ps->toString().toStdString().c_str()<<" ";
            } while((ps = ps->getInnerFailure()));
            cout<<endl;
        }
    }
}

void execute(QString prog)
{
    QMetaParser interp;
    QVariant result;
    const ParseStatus * ps = interp.parse(prog, result);
    if (ps) {
        cout<<result.toString().toStdString().c_str()<<endl;
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
    QCoreApplication::setApplicationName("qmeta");
    QCoreApplication::setApplicationVersion("0.0.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("An ometa parser/interpreter in Qt.");
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



