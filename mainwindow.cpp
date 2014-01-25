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
    //setWindowTitle("In app payment with the Avangate API");

//    QNetworkProxy* _proxy = new QNetworkProxy(QNetworkProxy::HttpProxy, "proxy.avangate.local", 8080);
//    QNetworkProxyQuery* _query = new QNetworkProxyQuery(QUrl("http://sandbox.avangate.com"));

//    QNetworkProxyFactory* _proxyFactory = new QNetworkProxyFactory(_query);
//    _proxyFactory << _proxy;


    //_network = new urnNetworkAccessManager(ui->webView);
//    _network->setProxyFactory (_proxyFactory);


    //QSslConfiguration _ssl = QSslConfiguration::defaultConfiguration();

    //_cache = new QNetworkDiskCache(ui->webView);
    //_cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/");
    //_cache->setMaximumCacheSize(1000000); //set the cache to 10megs
    //_network->setCache(_cache);
    //ui->webView->page()->setNetworkAccessManager (_network);

    //ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    //ui->webView->page()->setForwardUnsupportedContent(true);

    //qDebug() << ui->webView->page ()->forwardUnsupportedContent ();

    connect(ui->webView->page(), &QWebPage::unsupportedContent,
            this, &MainWindow::slotUnsupportedContent);
    /**/
    connect(ui->webView->page()->networkAccessManager (), &QNetworkAccessManager::finished,
                         this, &MainWindow::slotHandleReply);

    connect(this, &MainWindow::signalOrderPlaced, this, &MainWindow::slotOrderPlaced);
//    connect(ui->webView, &QWebView::urlChanged,
//            this, &MainWindow::slotUrlChanged);

//    connect(_network, &QNetworkAccessManager::fi,
//            this, &MainWindow::slotHandleReply);

    show();
}

void MainWindow::slotHandleReply (QNetworkReply *reply)
{
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << reply->rawHeaderPairs ();
    qDebug() << reply->url();

    if ( reply->url().scheme() == "urn" &&  reply->url().path() == "return") {
        // received redirect back from avangate
        QUrlQuery q(reply->url().query(QUrl::FullyDecoded));

        if (q.queryItemValue("status") == "NOK") {
            Response *madeup;
            madeup->setError(
                Response::Error {
                    -1,
                    q.queryItemValue("message")
                }
            );
            madeup->setId(6);

            emit signalError(madeup);
        }
        if (q.queryItemValue("status") == "OK") {
            emit signalSuccess();
        }
        if (q.queryItemValue("status") == "PENDING") {
            emit signalSuccess();
            QString refNo = q.queryItemValue("refNo");
            emit signalOrderPlaced(refNo);
        }
    } else {
        // some error
    }
}

void MainWindow::slotUnsupportedContent (QNetworkReply * reply)
{
    qDebug() << reply->readAll();
}

void MainWindow::slotUrlChanged (const QUrl& url)
{
    qDebug() << "url changes:" << url;
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
    ui->webView->setEnabled(true);

    QFile formFile(":/html/form.html");
    formFile.open(QIODevice::ReadOnly);

    QString html = formFile.readAll();

    QUrlQuery query;
    query.addQueryItem("finish", "true");
    query.addQueryItem("redir", "urn:return");

    QUrl u = Config::getUrl();
    QString path = u.path().replace(QString("rpc/"), QString("setpaymentdetails.php"));
    u.setPath(path);
    u.setQuery(query);

    ui->webView->setHtml(QString(html).arg (u.toString()).arg(_session));
}

void MainWindow::slotOrderPlaced(QString refNo)
{
    QErrorMessage* _err = new QErrorMessage(this);

    _err->setWindowTitle("Success !");
    _err->showMessage(QString("Order %1 placed successfully").arg(refNo));
}

MainWindow::~MainWindow()
{
    delete ui;
}
