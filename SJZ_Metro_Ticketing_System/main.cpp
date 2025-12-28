#include "enter.h"

#include <QApplication>
#include <ElaApplication.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElaApplication::getInstance()->init();
    enter w;
    w.show();
    return a.exec();
}
