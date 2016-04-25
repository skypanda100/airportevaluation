#include "bean/airport.h"

Airport::Airport(){

}

Airport::~Airport(){

}

QString Airport::code() const
{
    return m_code;
}

void Airport::setCode(const QString &code)
{
    m_code = code;
}

QString Airport::name() const
{
    return m_name;
}

void Airport::setName(const QString &name)
{
    m_name = name;
}

float Airport::longitude() const
{
    return m_longitude;
}

void Airport::setLongitude(float longitude)
{
    m_longitude = longitude;
}

float Airport::latitude() const
{
    return m_latitude;
}

void Airport::setLatitude(float latitude)
{
    m_latitude = latitude;
}

float Airport::altitude() const
{
    return m_altitude;
}

void Airport::setAltitude(float altitude)
{
    m_altitude = altitude;
}

float Airport::direction() const
{
    return m_direction;
}

void Airport::setDirection(float direction)
{
    m_direction = direction;
}

QString Airport::type() const
{
    return m_type;
}

void Airport::setType(const QString &type)
{
    m_type = type;
}
