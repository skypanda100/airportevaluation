#include "bean/qualitycontrolsource.h"

QualityControlSource::QualityControlSource(){

}

QualityControlSource::~QualityControlSource(){

}

int QualityControlSource::id() const
{
    return m_id;
}

void QualityControlSource::setId(int id)
{
    m_id = id;
}

QString QualityControlSource::name() const
{
    return m_name;
}

void QualityControlSource::setName(const QString &name)
{
    m_name = name;
}

QString QualityControlSource::fields() const
{
    return m_fields;
}

void QualityControlSource::setFields(const QString &fields)
{
    m_fields = fields;
}

