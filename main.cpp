#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置字体
    QFont font;
    font.setFamily("Microsoft Yahei");
    a.setFont(font);

    MainWindow w;
    w.resize(800,600);
    w.show();

    return a.exec();
}
