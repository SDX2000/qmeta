#ifndef PROGRAMINTERPRETER_H
#define PROGRAMINTERPRETER_H

#include <QString>
#include "parser.h"

class ProgramInterpreter : public Parser
{
public:
    bool parse(QStringRef inp, int& result);
    bool parse(QString inp, int& result);
private:
    bool assignment(QStringRef& inp, int &result);
    bool value(QStringRef &inp, int& result);
    bool factor(QStringRef& inp, int &result);
    bool term(QStringRef &inp, int& result);
    bool expression(QStringRef &inp, int& result);
    bool program(QStringRef& inp, int &result);
};

#endif // PROGRAMINTERPRETER_H
