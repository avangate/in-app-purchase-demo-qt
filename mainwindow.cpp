#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardPaths>
#include <QWebPage>
#include <QDebug>
#include <QNetworkProxyFactory>

using namespace AvangateAPI;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


//    QNetworkProxy* _proxy = new QNetworkProxy(QNetworkProxy::HttpProxy, "proxy.avangate.local", 8080);
//    QNetworkProxyQuery* _query = new QNetworkProxyQuery(QUrl("http://sandbox.avangate.com"));

//    QNetworkProxyFactory* _proxyFactory = new QNetworkProxyFactory(_query);
//    _proxyFactory << _proxy;


    _network = new urnNetworkAccessManager(ui->webView);
//    _network->setProxyFactory (_proxyFactory);


    QSslConfiguration _ssl = QSslConfiguration::defaultConfiguration();

    //_cache = new QNetworkDiskCache(ui->webView);
    //_cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/");
    //_cache->setMaximumCacheSize(1000000); //set the cache to 10megs
    //_network->setCache(_cache);
    ui->webView->page()->setNetworkAccessManager (_network);

    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    ui->webView->page()->setForwardUnsupportedContent(true);

    //qDebug() << ui->webView->page ()->forwardUnsupportedContent ();

    connect(ui->webView->page(), &QWebPage::unsupportedContent,
            this, &MainWindow::slotUnsupportedContent);
    /**/
    QObject::connect(ui->webView->page()->networkAccessManager (), &QNetworkAccessManager::finished,
                         this, &MainWindow::slotHandleReply);

    connect(ui->webView, &QWebView::urlChanged,
            this, &MainWindow::slotUrlChanged);

//    connect(ui->webView->page(), &QWebPage::,
//            this, &MainWindow::slotHandleReply);

//    QUrl r("http://google.com");
    QUrl r(QStringLiteral ("qrc:html/form.html"));

    ui->webView->setUrl( r );
}

void MainWindow::slotHandleReply (QNetworkReply *reply)
{
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << reply->rawHeaderPairs () << reply->readAll ();
}

void MainWindow::slotUnsupportedContent (QNetworkReply * reply)
{
    qDebug() << reply->readAll();
}

void MainWindow::slotUrlChanged (const QUrl& url)
{
    qDebug() << url;
}

MainWindow::~MainWindow()
{
    delete ui;
}
