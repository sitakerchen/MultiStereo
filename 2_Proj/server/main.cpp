#include "homePage.h"
#include "tcpController.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    homePage w;
    w.show();
    return a.exec();
}
