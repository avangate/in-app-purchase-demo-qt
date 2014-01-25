#include "mainwindow.h"
#include "config.h"
#include "ui_mainwindow.h"

#include <QStandardPaths>
#include <QWebPage>
#include <QDebug>
#include <QNetworkProxyFactory>
#include <QFile>
#include <QUrlQuery>
#include <QErrorMessage>

using namespace AvangateAPI;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("In app payment with the Avangate API");

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

void MainWindow::slotError(Response* err)
{
    ui->webView->setEnabled(false);
    QErrorMessage* _err = new QErrorMessage(this);

    _err->setWindowTitle("Error");
    _err->showMessage(err->error()->message);
}

void MainWindow::slotSetSession(QString session)
{
    _session = session;

    QFile formFile(":/html/form.html");
    formFile.open(QIODevice::ReadOnly);

    QString html = formFile.readAll();

    QUrlQuery query;
    query.addQueryItem("finish", "false");
    query.addQueryItem("redir", "urn:return");

    QUrl u = Config::getUrl();
    QString path = u.path().replace(QString("rpc/"), QString("setpaymentdetails.php"));
    u.setPath(path);
    u.setQuery(query);

    ui->webView->setHtml(QString(html).arg (u.toString()).arg(_session));

    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
