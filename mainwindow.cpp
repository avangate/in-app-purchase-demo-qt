#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "order.h"
#include "config.h"

#include <QAction>
#include <QErrorMessage>
#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qcombobox.h>

using namespace AvangateAPI;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    d_plot = new TVPlot( this );
    setCentralWidget( d_plot );

    QToolBar *toolBar = new QToolBar( this );

    QComboBox *typeBox = new QComboBox( toolBar );
    typeBox->addItem( "Outline" );
    typeBox->addItem( "Columns" );
    typeBox->addItem( "Lines" );
    typeBox->addItem( "Column Symbol" );
    typeBox->setCurrentIndex( 0 );
    typeBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    QToolButton *btnExport = new QToolButton( toolBar );
    btnExport->setText( "Export" );
    btnExport->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    connect( btnExport, SIGNAL( clicked() ), d_plot, SLOT( exportPlot() ) );

    toolBar->addWidget( typeBox );
    toolBar->addWidget( btnExport );
    addToolBar( toolBar );

    d_plot->setMode( typeBox->currentIndex() );
    connect( typeBox, SIGNAL( currentIndexChanged( int ) ),
             d_plot, SLOT( setMode( int ) ) );

    connect(ui->actionNew_Purchase, &QAction::triggered, this, &MainWindow::showPaymentWindow);
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);

    QObject::connect(this, &MainWindow::signalError, this, &MainWindow::slotError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showPaymentWindow()
{
    Order* order = new Order(Config::getUrl(), this);
    QObject::connect(order, &Order::signalError, [=](Response *response) {
        QString mess = QString::fromStdString("id[%1] %2: %3").arg(response->id ()).arg(response->error ()->code).arg(response->error ()->message);
        qDebug() << "Error: " << mess;
        emit signalError(mess);
    });
}

void MainWindow::slotError(QString err)
{
    QErrorMessage* _err = new QErrorMessage(this);

    _err->setWindowTitle("Error");
    _err->showMessage(err);
}
