#include "bean/qualitycontrolautomatic.h"

QualityControlAutomatic::QualityControlAutomatic(){

}

QualityControlAutomatic::~QualityControlAutomatic(){

}

int QualityControlAutomatic::id() const
{
    return m_id;
}

void QualityControlAutomatic::setId(int id)
{
    m_id = id;
}

QString QualityControlAutomatic::name() const
{
    return m_name;
}

void QualityControlAutomatic::setName(const QString &name)
{
    m_name = name;
}

float QualityControlAutomatic::minimum() const
{
    return m_minimum;
}

void QualityControlAutomatic::setMinimum(float minimum)
{
    m_minimum = minimum;
}

float QualityControlAutomatic::maximum() const
{
    return m_maximum;
}

void QualityControlAutomatic::setMaximum(float maximum)
{
    m_maximum = maximum;
}

float QualityControlAutomatic::suspiciousLimit() const
{
    return m_suspiciousLimit;
}

void QualityControlAutomatic::setSuspiciousLimit(float suspiciousLimit)
{
    m_suspiciousLimit = suspiciousLimit;
}

float QualityControlAutomatic::errorLimit() const
{
    return m_errorLimit;
}

void QualityControlAutomatic::setErrorLimit(float errorLimit)
{
    m_errorLimit = errorLimit;
}

QString QualityControlAutomatic::unit() const
{
    return m_unit;
}

void QualityControlAutomatic::setUnit(const QString &unit)
{
    m_unit = unit;
}

