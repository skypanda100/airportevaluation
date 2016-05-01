#ifndef MONTHSUMMARY
#define MONTHSUMMARY

#include <QObject>

class Monthsummary{

public:
    Monthsummary();
    ~Monthsummary();

    QString datetime() const;
    void setDatetime(const QString &datetime);

    QString winddirection() const;
    void setWinddirection(const QString &winddirection);

    QString windspeed() const;
    void setWindspeed(const QString &windspeed);

    QString gustspeed() const;
    void setGustspeed(const QString &gustspeed);

    QString leadingvisibility() const;
    void setLeadingvisibility(const QString &leadingvisibility);

    QString runway1no() const;
    void setRunway1no(const QString &runway1no);

    QString runway1vr() const;
    void setRunway1vr(const QString &runway1vr);

    QString runway2no() const;
    void setRunway2no(const QString &runway2no);

    QString runway2vr() const;
    void setRunway2vr(const QString &runway2vr);

    QString runway3no() const;
    void setRunway3no(const QString &runway3no);

    QString runway3vr() const;
    void setRunway3vr(const QString &runway3vr);

    QString runway4no() const;
    void setRunway4no(const QString &runway4no);

    QString runway4vr() const;
    void setRunway4vr(const QString &runway4vr);

    QString runway5no() const;
    void setRunway5no(const QString &runway5no);

    QString runway5vr() const;
    void setRunway5vr(const QString &runway5vr);

    QString phenomena() const;
    void setPhenomena(const QString &phenomena);

    QString totalcloudcover() const;
    void setTotalcloudcover(const QString &totalcloudcover);

    QString lowcloudcover() const;
    void setLowcloudcover(const QString &lowcloudcover);

    QString lowcloudstate1() const;
    void setLowcloudstate1(const QString &lowcloudstate1);

    QString lowcloudstate2() const;
    void setLowcloudstate2(const QString &lowcloudstate2);

    QString lowcloudstate3() const;
    void setLowcloudstate3(const QString &lowcloudstate3);

    QString lowcloudstate4() const;
    void setLowcloudstate4(const QString &lowcloudstate4);

    QString lowcloudstate5() const;
    void setLowcloudstate5(const QString &lowcloudstate5);

    QString middlecloudstate1() const;
    void setMiddlecloudstate1(const QString &middlecloudstate1);

    QString middlecloudstate2() const;
    void setMiddlecloudstate2(const QString &middlecloudstate2);

    QString middlecloudstate3() const;
    void setMiddlecloudstate3(const QString &middlecloudstate3);

    QString highcloudstate1() const;
    void setHighcloudstate1(const QString &highcloudstate1);

    QString highcloudstate2() const;
    void setHighcloudstate2(const QString &highcloudstate2);

    QString highcloudstate3() const;
    void setHighcloudstate3(const QString &highcloudstate3);

    QString temperature() const;
    void setTemperature(const QString &temperature);

    QString relativehumidity() const;
    void setRelativehumidity(const QString &relativehumidity);

    QString dewpoint() const;
    void setDewpoint(const QString &dewpoint);

    QString airdromepressure() const;
    void setAirdromepressure(const QString &airdromepressure);

    QString correctedseapressure() const;
    void setCorrectedseapressure(const QString &correctedseapressure);

private:
    QString m_datetime;
    QString m_winddirection;
    QString m_windspeed;
    QString m_gustspeed;
    QString m_leadingvisibility;
    QString m_runway1no;
    QString m_runway1vr;
    QString m_runway2no;
    QString m_runway2vr;
    QString m_runway3no;
    QString m_runway3vr;
    QString m_runway4no;
    QString m_runway4vr;
    QString m_runway5no;
    QString m_runway5vr;
    QString m_phenomena;
    QString m_totalcloudcover;
    QString m_lowcloudcover;
    QString m_lowcloudstate1;
    QString m_lowcloudstate2;
    QString m_lowcloudstate3;
    QString m_lowcloudstate4;
    QString m_lowcloudstate5;
    QString m_middlecloudstate1;
    QString m_middlecloudstate2;
    QString m_middlecloudstate3;
    QString m_highcloudstate1;
    QString m_highcloudstate2;
    QString m_highcloudstate3;
    QString m_temperature;
    QString m_relativehumidity;
    QString m_dewpoint;
    QString m_airdromepressure;
    QString m_correctedseapressure;
};

#endif // MONTHSUMMARY

