#ifndef KFTTJRESULTWIDGET
#define KFTTJRESULTWIDGET

#include <QtWidgets>
#include "database/pgdatabase.h"
#include "bean/monthsummary.h"
#include "bean/extremum.h"
#include "model/tablemodel.h"

/***东经110度以东***/
const int JAN_DAY_E[11] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const int FEB_DAY_E[12] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int MAR_DAY_E[13] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int APR_DAY_E[14] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int MAY_DAY_E[15] = {21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int JUN_DAY_E[15] = {21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int JUL_DAY_E[15] = {21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int AUG_DAY_E[15] = {21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int SEP_DAY_E[13] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int OCT_DAY_E[13] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int NOV_DAY_E[11] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const int DEC_DAY_E[11] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const int HALF_DAY_E = 2;
const int WHOLE_DAY_E = 6;
/***东经95度以东~东经110度***/
const int JAN_DAY_M[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int FEB_DAY_M[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int MAR_DAY_M[13] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int APR_DAY_M[14] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int MAY_DAY_M[15] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int JUN_DAY_M[15] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int JUL_DAY_M[15] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int AUG_DAY_M[15] = {22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int SEP_DAY_M[13] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int OCT_DAY_M[13] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int NOV_DAY_M[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int DEC_DAY_M[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int HALF_DAY_M = 3;
const int WHOLE_DAY_M = 7;
/***东经95度以西***/
const int JAN_DAY_W[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int FEB_DAY_W[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int MAR_DAY_W[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int APR_DAY_W[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int MAY_DAY_W[15] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int JUN_DAY_W[15] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int JUL_DAY_W[15] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int AUG_DAY_W[15] = {23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int SEP_DAY_W[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int OCT_DAY_W[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int NOV_DAY_W[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int DEC_DAY_W[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int HALF_DAY_W = 4;
const int WHOLE_DAY_W = 8;

class KfttjResultWidget : public QSplitter{
    Q_OBJECT
public:
    KfttjResultWidget(QWidget *parent = 0);
    ~KfttjResultWidget();

private:
    void initData();
    void initUI();
    void initConnect();

    void query();
    void analysis();

private:
    QList<QString> titleList;
    PgDataBase *pgdb;
    QList<Monthsummary> summaryList;
    QList<Extremum> extremumList;

    TableModel *tableModel;
    QTableView *tableView;
};

#endif // KFTTJRESULTWIDGET

