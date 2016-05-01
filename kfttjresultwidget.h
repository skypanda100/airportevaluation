#ifndef KFTTJRESULTWIDGET
#define KFTTJRESULTWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/monthsummary.h"
#include "bean/extremum.h"

class KfttjResultWidget : public QWidget{
    Q_OBJECT
public:
    KfttjResultWidget(QWidget *parent = 0);
    ~KfttjResultWidget();

private:
    void initData();
    void query();

private:
    PgDataBase *pgdb;
    QList<Monthsummary> summaryList;
    QList<Extremum> extremumList;
};

#endif // KFTTJRESULTWIDGET

