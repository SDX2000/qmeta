#include "utils.h"

using namespace std;

int g_indentLevel = 0;


void printIndent() {
    printIndent(g_indentLevel);
}

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
        cout << val.typeName() << " = " << val.toString().toStdString().c_str() << endl;
    }
}

void print(QVariant &val) {
    printInternal(val, g_indentLevel);
}
