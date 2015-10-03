#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QTextStream>

#define QSL(str) QStringLiteral(str)

QStringRef mid(QStringRef lhs, QStringRef rhs);
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

#endif // UTILS_H
