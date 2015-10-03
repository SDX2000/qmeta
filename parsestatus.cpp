#include "parsestatus.h"
#include "utils.h"

ParseStatus* ParseStatus::success()
{
    static ParseStatus ps(true);
    return &ps;
}

ParseStatus* ParseStatus::failure(QStringRef pos, QString msg)
{
    return new ParseStatus(false, pos, msg);
}

ParseStatus* ParseStatus::failure(QStringRef pos, QString msg, ParseStatus *innerFailure)
{
    return new ParseStatus(false, pos, msg, innerFailure);
}

ParseStatus::~ParseStatus()
{
    safeDelete(m_innerFailure);
}

ParseStatus::ParseStatus(bool status)
    : m_status(status)
    , m_innerFailure(nullptr)
{
}

ParseStatus::ParseStatus(bool status, QStringRef pos, QString msg)
    : m_status(status)
    , m_msg(msg)
    , m_innerFailure(nullptr)
    , m_pos(pos)
{
}

ParseStatus::ParseStatus(bool status, QStringRef pos, QString msg, ParseStatus *innerFailure)
    : m_status(status)
    , m_msg(msg)
    , m_innerFailure(innerFailure)
    , m_pos(pos)
{
}
