#include "homePage.h"
#include "tcpController.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    homePage w;

    w.show();
    return a.exec();
}
