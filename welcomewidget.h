#ifndef WELCOMEWIDGET
#define WELCOMEWIDGET

#include <QtWidgets>

class WelcomeWidget : public QWidget{
    Q_OBJECT
public:
    WelcomeWidget(QWidget *parent = 0);
    ~WelcomeWidget();

private:
    void initData();
    void initUI();
    void initConnect();

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    QPixmap *welcomePixmap;
};

#endif // WELCOMEWIDGET

