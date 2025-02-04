#include "tempui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tempUI w;
    w.show();
    return a.exec();
}
