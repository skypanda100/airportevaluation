#include "weatherparamsetup.h"

WeatherParamSetup::WeatherParamSetup(){

}

WeatherParamSetup::~WeatherParamSetup(){

}

QString WeatherParamSetup::code() const
{
    return m_code;
}

void WeatherParamSetup::setCode(const QString &code)
{
    m_code = code;
}

QString WeatherParamSetup::planeName() const
{
    return m_planeName;
}

void WeatherParamSetup::setPlaneName(const QString &planeName)
{
    m_planeName = planeName;
}

int WeatherParamSetup::paramid() const
{
    return m_paramid;
}

void WeatherParamSetup::setParamid(int paramid)
{
    m_paramid = paramid;
}

QString WeatherParamSetup::limits() const
{
    return m_limits;
}

void WeatherParamSetup::setLimits(const QString &limits)
{
    m_limits = limits;
}
