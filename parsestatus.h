#ifndef PARSESTATUS_H
#define PARSESTATUS_H

#include <QString>

class ParseStatus
{
public:
    static ParseStatus* success();
    static ParseStatus* failure(QStringRef pos);
    static ParseStatus* failure(QStringRef pos, ParseStatus *innerFailure);

    bool isOk() const{
        return m_status;
    }

    ~ParseStatus();

private:
    explicit ParseStatus(bool status);
    ParseStatus(bool status, QStringRef pos);
    ParseStatus(bool status, QStringRef pos, ParseStatus *innerFailure);

    bool m_status;
    ParseStatus *m_innerFailure;
    QStringRef m_pos;

};

#endif // PARSESTATUS_H
