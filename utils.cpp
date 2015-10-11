#include "utils.h"

using namespace std;

int g_indentLevel = 0;

QTextStream& qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}

void printIndent() {
    printIndent(g_indentLevel);
}

void printIndent(int indentation) {
    for(int i = 0; i < TAB_SPACES * indentation; i++) {
        qStdOut() << " ";
    }
}

QString escape(QChar c) {
    if ('\'' == c)
        return QSL("\\'");
    if ('"' == c)
        return QSL("\\\"");
    if ('?' == c)
        return QSL("\\?");
    if ('\\' == c)
        return QSL("\\\\");
    if ('\a' == c)
        return QSL("\\a");
    if ('\b' == c)
        return QSL("\\b");
    if ('\f' == c)
        return QSL("\\f");
    if ('\n' == c)
        return QSL("\\n");
    if ('\r' == c)
        return QSL("\\r");
    if ('\t' == c)
        return QSL("\\t");
    if ('\v' == c)
        return QSL("\\v");

    return QSL("");
}

QTextStream& operator << (QTextStream& lhs, const QVariant& val) {
    if(val.type() == QVariant::List) {
        lhs << "[";
        auto list = val.value<QVariantList>();
        int i = list.length();
        foreach(QVariant elem, list) {
            i--;

            lhs << elem;

            if(i) {
                lhs << ", ";
            }
        }
        lhs << "]";
    } else {
        if(val.type() == QVariant::String) {
            lhs << '"' << val.toString() << '"';
        } else if (val.type() == QVariant::Char) {
            QChar c = val.toChar();
            QString escaped = escape(c);
            if(!escaped.isEmpty()) {
                lhs << "'" << escaped << "'";
            } else if (c.isPrint()) {
                lhs << "'" << c << "'";
            } else {
                lhs << "'" << "\\x" << hex << c.unicode() << "'";
            }
        } else {
            lhs << val.toString() << " : " << val.typeName();
        }
    }
    return lhs;
}

QTextStream& operator << (QTextStream& lhs, bool val) {
    if (val) {
        lhs << "true";
    } else {
        lhs << "false";
    }
    return lhs;
}
