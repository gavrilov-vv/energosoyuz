#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    int _result = w.init();

    if(_result == ResultCode::ALL_RIGHT)
    {
        w.show();
    }
    else
    {
        exit(_result);
    }


    return a.exec();
}
