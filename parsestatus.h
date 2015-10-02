#ifndef PARSESTATUS_H
#define PARSESTATUS_H

#include <QString>

class ParseStatus
{
public:
    static ParseStatus success();
    static ParseStatus failure(QStringRef pos);

    operator bool () const{
        return m_status;
    }

private:
    explicit ParseStatus(bool status);
    ParseStatus(bool status, QStringRef pos);

    bool m_status;
    QStringRef m_pos;
};

#endif // PARSESTATUS_H
