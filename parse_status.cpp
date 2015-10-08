#include "parse_status.h"
#include "utils.h"

ParseStatusPtr ParseStatus::success()
{
    static ParseStatus ps(true);
    return &ps;
}

ParseStatusPtr ParseStatus::failure(int pos, QString msg)
{
    return new ParseStatus(false, pos, msg);
}

ParseStatusPtr ParseStatus::failure(int pos, QString msg, ParseStatusPtr innerFailure)
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

ParseStatus::ParseStatus(bool status, int pos, QString msg)
    : m_status(status)
    , m_msg(msg)
    , m_innerFailure(nullptr)
    , m_pos(pos)
{
}

ParseStatus::ParseStatus(bool status, int pos, QString msg, ParseStatusPtr innerFailure)
    : m_status(status)
    , m_msg(msg)
    , m_innerFailure(innerFailure)
    , m_pos(pos)
{
}
