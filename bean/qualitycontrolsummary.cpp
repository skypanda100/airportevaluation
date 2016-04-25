#include "bean/qualitycontrolsummary.h"

QualityControlSummary::QualityControlSummary(){

}

QualityControlSummary::~QualityControlSummary(){

}

int QualityControlSummary::id() const
{
    return m_id;
}

void QualityControlSummary::setId(int id)
{
    m_id = id;
}

QString QualityControlSummary::name() const
{
    return m_name;
}

void QualityControlSummary::setName(const QString &name)
{
    m_name = name;
}

float QualityControlSummary::minimum() const
{
    return m_minimum;
}

void QualityControlSummary::setMinimum(float minimum)
{
    m_minimum = minimum;
}

float QualityControlSummary::maximum() const
{
    return m_maximum;
}

void QualityControlSummary::setMaximum(float maximum)
{
    m_maximum = maximum;
}

QString QualityControlSummary::unit() const
{
    return m_unit;
}

void QualityControlSummary::setUnit(const QString &unit)
{
    m_unit = unit;
}


