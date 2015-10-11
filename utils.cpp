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
            lhs << "'" << val.toString() << "'";
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
