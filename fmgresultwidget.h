#ifndef FMGRESULTWIDGET
#define FMGRESULTWIDGET

#include <QtWidgets>

class FmgResultWidget : public QSplitter{
    Q_OBJECT
public:
    FmgResultWidget(QWidget *parent = 0);
    ~FmgResultWidget();

signals:
    void setProgressValue(int);

private:
    void initData();
    void initUI();
    void initConnect();
};

#endif // FMGRESULTWIDGET

