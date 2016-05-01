#ifndef EXTREMUM
#define EXTREMUM

#include <QObject>

class Extremum{
public:
    Extremum();
    ~Extremum();

    QString datetime() const;
    void setDatetime(const QString &datetime);

    QString hightemperature() const;
    void setHightemperature(const QString &hightemperature);

    QString lowtemperature() const;
    void setLowtemperature(const QString &lowtemperature);

    QString precipitation() const;
    void setPrecipitation(const QString &precipitation);

    QString maxsnowdepth() const;
    void setMaxsnowdepth(const QString &maxsnowdepth);

    QString evolution1() const;
    void setEvolution1(const QString &evolution1);

    QString evolution2() const;
    void setEvolution2(const QString &evolution2);

    QString evolution3() const;
    void setEvolution3(const QString &evolution3);

    QString evolution4() const;
    void setEvolution4(const QString &evolution4);

    QString evolution5() const;
    void setEvolution5(const QString &evolution5);

    QString evolution6() const;
    void setEvolution6(const QString &evolution6);

    QString evolution7() const;
    void setEvolution7(const QString &evolution7);

    QString evolution8() const;
    void setEvolution8(const QString &evolution8);

    QString evolution9() const;
    void setEvolution9(const QString &evolution9);

    QString evolution10() const;
    void setEvolution10(const QString &evolution10);

private:
    QString m_datetime;
    QString m_hightemperature;
    QString m_lowtemperature;
    QString m_precipitation;
    QString m_maxsnowdepth;
    QString m_evolution1;
    QString m_evolution2;
    QString m_evolution3;
    QString m_evolution4;
    QString m_evolution5;
    QString m_evolution6;
    QString m_evolution7;
    QString m_evolution8;
    QString m_evolution9;
    QString m_evolution10;
};

#endif // EXTREMUM

