#ifndef WEATHERPARAM
#define WEATHERPARAM

#include <QObject>

class WeatherParam{
public:
    WeatherParam();
    ~WeatherParam();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    int choose_type() const;
    void setChoose_type(int choose_type);

    int limit_type() const;
    void setLimit_type(int limit_type);

private:
    int m_id;
    QString m_name;
    int m_choose_type;
    int m_limit_type;
};

#endif // WEATHERPARAM

