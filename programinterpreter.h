#ifndef PROGRAMINTERPRETER_H
#define PROGRAMINTERPRETER_H

#include <QString>
#include "parser.h"

class ProgramInterpreter : public Parser
{
public:
    ParseStatus* parse(QStringRef inp, int& result);
    ParseStatus* parse(QString inp, int& result);
private:
    ParseStatus* assignment(QStringRef& inp, int &result);
    ParseStatus* value(QStringRef &inp, int& result);
    ParseStatus* factor(QStringRef& inp, int &result);
    ParseStatus* term(QStringRef &inp, int& result);
    ParseStatus* expression(QStringRef &inp, int& result);
    ParseStatus* program(QStringRef& inp, int &result);
};

#endif // PROGRAMINTERPRETER_H
