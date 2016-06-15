#include "mainwindow.h"
#include "common/sharedmemory.h"
#include <QApplication>

int main(int argc, char *argv[])
{
/*
    int delay = 18;
    int interval = 6;
    for(int i = 0;i < 60;i++){
        for(int j = 0;j < 60 / interval;j++){
            int currentTime = (i + j * interval);
            int minute = ((currentTime - delay + 60) % 60) / 6 * 6;
            qDebug() << i << minute;
        }
    }
*/
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
