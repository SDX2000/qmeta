#ifndef PARSESTATUS_H
#define PARSESTATUS_H

#include <QString>
#include "utils.h"

class ParseStatus;

typedef ParseStatus* ParseStatusPtr;
typedef const ParseStatus* ConstParseStatusPtr;

class ParseStatus
{
public:
    static ParseStatusPtr success();
    static ParseStatusPtr failure(int pos, QString msg);
    static ParseStatusPtr failure(int pos, QString msg, ParseStatusPtr innerFailure);

    void chain(const ParseStatusPtr ps) {
        if(m_innerFailure) {
            m_innerFailure->chain(ps);
        } else {
            m_innerFailure = ps;
        }
    }

    QString toString() const {
        return QString(QSL("ERROR %1")).arg(m_msg);
    }

    ParseStatusPtr getInnerFailure() const{
        return m_innerFailure;
    }

    ~ParseStatus();

private:
    explicit ParseStatus(bool status);
    ParseStatus(bool status, int pos, QString msg);
    ParseStatus(bool status, int pos, QString msg, ParseStatusPtr innerFailure);

    bool            m_status;
    QString         m_msg;
    ParseStatusPtr  m_innerFailure;
    int             m_pos;
};



#endif // PARSESTATUS_H
