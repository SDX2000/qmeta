#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QTextStream>

#define QSL(str) QStringLiteral(str)

#define QSTDOUT() printIndent(); qStdOut()

#define TRACE_LAST(X) qStdOut() << #X << " = " << X;
#define TRACE(X) qStdOut() << #X << " = " << X << ", ";

#define TRACE1(X)       TRACE_LAST(X)
#define TRACE2(X, ...)  TRACE(X)TRACE1(__VA_ARGS__)
#define TRACE3(X, ...)  TRACE(X)TRACE2(__VA_ARGS__)
#define TRACE4(X, ...)  TRACE(X)TRACE3(__VA_ARGS__)
#define TRACE5(X, ...)  TRACE(X)TRACE4(__VA_ARGS__)
#define TRACE6(X, ...)  TRACE(X)TRACE5(__VA_ARGS__)


#define GET_TRACER(_1,_2,_3,_4,_5,_6,TRACEER,...) TRACEER
#define TRACEV(...) \
  GET_TRACER(__VA_ARGS__,TRACE6, TRACE5, TRACE4, TRACE3, TRACE2, TRACE1)(__VA_ARGS__)

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

extern int g_indentLevel;


#endif // UTILS_H
