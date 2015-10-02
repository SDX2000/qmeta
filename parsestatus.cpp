#include "parsestatus.h"
#include "utils.h"

ParseStatus* ParseStatus::success()
{
    static ParseStatus ps(true);
    return &ps;
}

ParseStatus* ParseStatus::failure(QStringRef pos)
{
    return new ParseStatus(false, pos);
}

ParseStatus* ParseStatus::failure(QStringRef pos, ParseStatus *innerFailure)
{
    return new ParseStatus(false, pos, innerFailure);
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

ParseStatus::ParseStatus(bool status, QStringRef pos)
    : m_status(status)
    , m_innerFailure(nullptr)
    , m_pos(pos)
{
}

ParseStatus::ParseStatus(bool status, QStringRef pos, ParseStatus *innerFailure)
    : m_status(status)
    , m_innerFailure(innerFailure)
    , m_pos(pos)
{
}
