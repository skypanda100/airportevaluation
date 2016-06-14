#include "mainwindow.h"
#include "common/sharedmemory.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置字体
    QFont font;
    font.setFamily("Microsoft Yahei");
    a.setFont(font);
    //共享区初始化
    SharedMemory::getInstance()->queryAirportInfomation();

    MainWindow w;
    w.resize(1100,600);
    w.show();

    return a.exec();
}
