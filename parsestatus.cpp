#include "parsestatus.h"


ParseStatus ParseStatus::success()
{
    return ParseStatus(true);
}

ParseStatus ParseStatus::failure(QStringRef pos)
{
    return ParseStatus(false, pos);
}

ParseStatus::ParseStatus(bool status)
    : m_status(status)
{

}

ParseStatus::ParseStatus(bool status, QStringRef pos)
    : m_status(status)
    , m_pos(pos)
{

}
