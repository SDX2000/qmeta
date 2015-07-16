#include <QCoreApplication>
#include <QString>
#include <QTextStream>

/*
expr    = term (( "+" | "-" ) term)*
term    = factor (( "*" | "/" ) factor)*
factor  = value | "(" expr ")"
*/

QStringRef expr(QStringRef inp);

QTextStream& qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}

QStringRef expect(QStringRef inp, QString str)
{
    if (!inp.startsWith(str)) {
        qStdOut() << "ERROR: Expected " << str << endl;
        exit(1);
    }

    return inp.mid(str.length());
}

QStringRef value(QStringRef inp)
{
    if(inp.isEmpty())
        return inp;

    int count = 0;

    for(; count < inp.length() && inp.at(count).isDigit(); count++) {
    }

    return inp.mid(count);
}

QStringRef factor(QStringRef inp)
{
    if(inp.isEmpty())
        return inp;

    QStringRef inp0 = inp;
    inp = value(inp0);

    if(inp != inp0)
        return inp;

    inp = expect(inp, QStringLiteral("("));

    inp = expr(inp.mid(1));

    inp = expect(inp, QStringLiteral(")"));

    return inp;
}

QStringRef term(QStringRef inp)
{
    if(inp.isEmpty())
        return inp;

    inp = factor(inp);

    while(!inp.isEmpty() && (inp.at(0) == QStringLiteral("*") || inp.at(0) == QStringLiteral("/")))
    {
        inp = inp.mid(1);
        inp = factor(inp);
    }

    return inp;
}

QStringRef expr(QStringRef inp)
{
    if(inp.isEmpty())
        return inp;

    inp = term(inp);

    while (!inp.isEmpty() && (inp.at(0) == QStringLiteral("+") || inp.at(0) == QStringLiteral("-")))
    {
        inp = inp.mid(1);
        inp = term(inp);
    }

    return inp;
}


int main(/*int argc, char *argv[]*/)
{
    //QCoreApplication a(argc, argv);

    QString inp(QStringLiteral("1+1"));
    expr(inp.midRef(0));

    //return a.exec();

    return 0;
}
