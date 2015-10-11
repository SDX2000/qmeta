#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QCommandLineParser>
#include <QFile>

#include "qmeta_parser.h"
#include "utils.h"

void doREPL()
{
    QTextStream cin(stdin);
    QMetaParser interp;

    while(true)
    {
        QSTDOUT() << QSL("> ");
        QSTDOUT() .flush();
        QString inp = cin.readLine();

        if (inp.toLower() == QSL("q")) {
            break;
        }

        QVariant result;
        ParseStatusPtr ps;
        bool ok = interp.parse(inp, result, ps);
        if (ok) {
            QSTDOUT() << result << endl;
        } else {
            QSTDOUT() <<"PARSE FAILED ";
            do {
                QSTDOUT() <<ps->toString()<<" ";
            } while((ps = ps->getInnerFailure()));
            QSTDOUT() <<endl;
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
        QSTDOUT() << result;
    } else {
        QSTDOUT() << "PARSE FAILED ";
        do {
            QSTDOUT() << ps->toString() << " ";
        }while((ps = ps->getInnerFailure()));
        QSTDOUT() << endl;
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
        QSTDOUT() << "ERROR: Invalid command line options." << endl;
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



