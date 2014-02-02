#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "order.h"
#include "config.h"

#include <QAction>

using namespace AvangateAPI;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionNew_Purchase, &QAction::triggered, this, &MainWindow::showPaymentWindow);
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showPaymentWindow()
{
    Order* order = new Order(Config::getUrl(), this);

    QObject::connect(order, &Order::signalError, [=](Response *response) {
        qDebug() << "Error: id[" << response->id () << "]" << response->error ()->code << response->error ()->message;
    });


    QObject::connect(order, &Order::signalSessionStarted, [=](QString _sessionHash) {
        qDebug() << "Session started [" << _sessionHash << "]";

        order->setLanguage("en");
        order->setCurrency("eur");

        BillingDetails* b = new BillingDetails();
        b->setFirstName("API");
        b->setLastName("TEST");
        b->setEmail("sandbox.avangate@avangate.com");
        b->setCity("Bucharest");
        b->setAddress("Some Street, no 666");
        b->setCountry("ro");
        b->setState("Bucharest");
        b->setPostalCode("101222");

        order->setBillingDetails(b);

        order->addProduct(Config::getProductId(), 1, Config::getPriceOptions());
    });

    connect(order, &Order::signalSetupFinished, [=]() {
       order->setPaymentDetails();
    });

//    connect(order, &Order::signalPaymentDetailsAdded, [=]() {
//        order->placeOrder();
//    });

    order->login(Config::getMerchantCode(), Config::getSecretKey());
}
