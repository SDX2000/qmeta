#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QTextStream>

#define QSL(str) QStringLiteral(str)

#define QSTDOUT() printIndent(); qStdOut()

QTextStream& qStdOut();

QTextStream& operator << (QTextStream& lhs, const QVariant& val);
QTextStream& operator << (QTextStream& lhs, bool val);

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
//void print(QVariant &val);

extern int g_indentLevel;

//class EntryExitLogger
//{
//    const char * m_funcName;

//public:
//    EntryExitLogger(const char *funcName)
//        : m_funcName(funcName)
//    {
//        printIndent();
//        qStdOut()<<"Entering: "<<m_funcName<<endl;
//        g_indentLevel++;
//    }

//    ~EntryExitLogger()
//    {
//        g_indentLevel--;
//        printIndent();
//        qStdOut()<<"Leaving: "<<m_funcName<<endl;

//    }
//};

//#define LOG() EntryExitLogger __entryExitLogger(__FUNCTION__);

#endif // UTILS_H
