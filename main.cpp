#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QCommandLineParser>
#include <QFile>

#include "qmeta_parser.h"

#include <iostream>

using namespace std;

#define TAB_SPACES 4

void printIndent(int indentation) {
    for(int i = 0; i < TAB_SPACES * indentation; i++) {
        cout << " ";
    }
}

void printInternal(QVariant &val, int indentation) {
    if(val.type() == QVariant::List) {
        auto list = val.value<QVariantList>();
        printIndent(indentation);
        cout << "List with " << list.length() << " elements" <<endl;
        foreach(QVariant elem, list) {
            printInternal(elem, indentation + 1);
        }
    } else {
        printIndent(indentation);
        cout << "Single value of type:" << val.typeName() <<endl;
        printIndent(indentation);
        cout << val.toString().toStdString().c_str() << endl;
    }
}

void print(QVariant &val) {
    printInternal(val, 0);
}

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
        ParseStatusPtr ps;
        bool ok = interp.parse(inp, result, ps);
        if (ok) {
            print(result);
        } else {
            cout<<"PARSE FAILED ";
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
    ParseStatusPtr ps;
    bool ok = interp.parse(prog, result, ps);
    if (ok) {
        print(result);
    } else {
        cout<<"PARSE FAILED ";
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



