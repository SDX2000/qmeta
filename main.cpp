#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QCommandLineParser>
#include <QFile>
#include <QDebug>

#include "QMetaParser.h"
#include "utils.h"

void doREPL()
{
    QTextStream cin(stdin);
    QMetaParser interp;

    while(true)
    {
        qStdOut() << QSL("> ");
        qStdOut() .flush();
        QString inp = cin.readLine();

        if (inp.toLower() == QSL("q")) {
            break;
        }

        QVariant result;

        bool ok = interp.parse((int)QMetaParser::RULES, inp, result);
        if (ok) {
            qStdOut() << endl << result << endl;
        } else {
            qStdOut() << "Generated errors... " <<endl;
            interp.getError()->print(qStdOut());
        }
    }
}

void execute(QString prog)
{
    QMetaParser interp;
    QVariant result;

    bool ok = interp.parse(QMetaParser::GRAMMAR, prog, result);

    if (ok) {
        qStdOut() << endl << result << endl;
    } else {
        qStdOut() << "Generated errors... " <<endl;
        interp.getError()->print(qStdOut());
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("qmeta");
    QCoreApplication::setApplicationVersion("0.0.0");

    qStdOut() << "Arguments..." <<endl;

    for (int i = 0; i < argc; i++) {
        qStdOut() << i + 1 << ") " << argv[i] << endl;
    }

    QCommandLineParser parser;
    parser.setApplicationDescription("An ometa parser/interpreter in Qt.");
    parser.addHelpOption();
    parser.addVersionOption();

    if (!parser.parse(QCoreApplication::arguments())) {
        qStdOut() << "ERROR: Invalid command line options." << endl;
        return 1;
    }

    const QStringList args = parser.positionalArguments();

    if (args.isEmpty()) {
        doREPL();
        return 0;
    }

    foreach(QString fileName, args) {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            qWarning() << "Could not open file: " << fileName << endl;
            continue;
        }
        QTextStream in(&file);
        execute(in.readAll());
    }

    //TODO: Return a relevant error code.
    return 0;
}



