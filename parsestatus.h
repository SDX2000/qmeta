#ifndef PARSESTATUS_H
#define PARSESTATUS_H

#include <QString>
#include "utils.h"

class ParseStatus
{
public:
    static ParseStatus* success();
    static ParseStatus* failure(QStringRef pos, QString msg);
    static ParseStatus* failure(QStringRef pos, QString msg, ParseStatus *innerFailure);

    bool isOk() const{
        return m_status;
    }

    QString toString() const {
        return QString(QSL("ERROR %1")).arg(m_msg);
    }

    const ParseStatus* getInnerFailure() const{
        return m_innerFailure;
    }

    ~ParseStatus();

private:
    explicit ParseStatus(bool status);
    ParseStatus(bool status, QStringRef pos, QString msg);
    ParseStatus(bool status, QStringRef pos, QString msg, ParseStatus *innerFailure);

    bool m_status;
    QString m_msg;
    ParseStatus *m_innerFailure;
    QStringRef m_pos;

};

#endif // PARSESTATUS_H
