#include "mainwindow.h"
#include "common/sharedmemory.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //添加字体
    QFontDatabase::addApplicationFont(QApplication::applicationDirPath() + "/fonts/msyh.ttf");
    QFontDatabase::addApplicationFont(QApplication::applicationDirPath() + "/fonts/msyhbd.ttf");
    //设置字体
    QFont font;
    font.setFamily("Microsoft Yahei");
    a.setFont(font);
    //共享区初始化
    SharedMemory::getInstance()->queryAirportInfomation();

    MainWindow w;
    w.resize(1100,600);
    w.show();
    w.showGuide();

    return a.exec();
}
