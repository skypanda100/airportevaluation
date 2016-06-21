#ifndef KFTTJINPUTWIDGET
#define KFTTJINPUTWIDGET

#include <QtWidgets>

class KfttjInputWidget : public QWidget{
    Q_OBJECT
public:
    KfttjInputWidget(QWidget *parent = 0);
    ~KfttjInputWidget();

signals:
    void executeKfttj();
    void executeExport();

private slots:
    void execute();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QPushButton *executeButton;
    QPushButton *exportButton;
};
#endif // KFTTJINPUTWIDGET

