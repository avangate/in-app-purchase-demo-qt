#include <QApplication>
#include <QDebug>

#include "config.h"
#include "order.h"
#include "response.h"
#include "billingdetails.h"
#include "paymentwindow.h"
#include "mainwindow.h"

using namespace AvangateAPI;

bool Config::devel;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    Config::devel = true;
    
    MainWindow w;
    w.show();

    return a.exec();
}
