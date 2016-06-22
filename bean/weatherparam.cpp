#include "weatherparam.h"

WeatherParam::WeatherParam(){

}

WeatherParam::~WeatherParam(){

}

int WeatherParam::id() const
{
    return m_id;
}

void WeatherParam::setId(int id)
{
    m_id = id;
}

QString WeatherParam::name() const
{
    return m_name;
}

void WeatherParam::setName(const QString &name)
{
    m_name = name;
}

int WeatherParam::choose_type() const
{
    return m_choose_type;
}

void WeatherParam::setChoose_type(int choose_type)
{
    m_choose_type = choose_type;
}

int WeatherParam::limit_type() const
{
    return m_limit_type;
}

void WeatherParam::setLimit_type(int limit_type)
{
    m_limit_type = limit_type;
}
