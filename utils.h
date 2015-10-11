#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QTextStream>
#include <iostream>

#define QSL(str) QStringLiteral(str)

//QTextStream qStdOut();

template <typename T>
void safeDelete(T* &p) {
    delete p;
    p = nullptr;
}


template <typename T>
void safeDeleteArray(T* &p) {
    delete[] p;
    p = nullptr;
}

#define TAB_SPACES 4

void printIndent();
void printIndent(int indentation);
void print(QVariant &val);

extern int g_indentLevel;

class EntryExitLogger
{
    const char * m_funcName;

public:
    EntryExitLogger(const char *funcName)
        : m_funcName(funcName)
    {
        printIndent();
        std::cout<<"Entering: "<<m_funcName<<std::endl;
        g_indentLevel++;
    }

    ~EntryExitLogger()
    {
        g_indentLevel--;
        printIndent();
        std::cout<<"Leaving: "<<m_funcName<<std::endl;

    }
};

#define LOG() EntryExitLogger __entryExitLogger(__FUNCTION__);

#endif // UTILS_H
