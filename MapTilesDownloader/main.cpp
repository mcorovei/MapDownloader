#include "startup.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ps::Startup w;
    w.show();

    return a.exec();
}
