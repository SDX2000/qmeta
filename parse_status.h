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
    static ParseStatusPtr failure(QStringRef pos, QString msg);
    static ParseStatusPtr failure(QStringRef pos, QString msg, ParseStatusPtr innerFailure);

    operator bool() const{
        return m_status;
    }

    QString toString() const {
        return QString(QSL("ERROR %1")).arg(m_msg);
    }

    ConstParseStatusPtr getInnerFailure() const{
        return m_innerFailure;
    }

    ~ParseStatus();

private:
    explicit ParseStatus(bool status);
    ParseStatus(bool status, QStringRef pos, QString msg);
    ParseStatus(bool status, QStringRef pos, QString msg, ParseStatusPtr innerFailure);

    bool            m_status;
    QString         m_msg;
    ParseStatusPtr  m_innerFailure;
    QStringRef      m_pos;
};



#endif // PARSESTATUS_H
