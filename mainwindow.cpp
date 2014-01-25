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

//    connect(ui->webView->page(), &QWebPage::unsupportedContent,
//            this, &MainWindow::slotUnsupportedContent);
    connect(ui->webView->page()->networkAccessManager (), &QNetworkAccessManager::finished,
                         this, &MainWindow::slotHandleReply);
    connect(this, &MainWindow::signalOrderPlaced, this, &MainWindow::slotOrderPlaced);
    connect(this, &MainWindow::signalError, this, &MainWindow::slotError);

    show();
}

void MainWindow::slotHandleReply (QNetworkReply *reply)
{
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << reply->rawHeaderPairs ();
    qDebug() << reply->url();

    if ( reply->url().scheme() == "urn" &&  reply->url().path() == "return") {
        // received redirect back from avangate
        QUrlQuery q(reply->url().query());

        if (q.queryItemValue("status") == "NOK") {
            Response *madeup = new Response();

            madeup->setError(
              Response::Error {
                  -1,
                  QString(q.queryItemValue("message", QUrl::FullyDecoded)).replace("+", " ")
              }
            );
            madeup->setId(6);

            emit signalError(madeup);
        }     
        if (q.queryItemValue("status") == "AUTHRECEIVED" || q.queryItemValue("status") == "PENDING") {
            QString refNo = q.queryItemValue("refNo");
            emit signalOrderPlaced(refNo, q.queryItemValue("status"));
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
    //ui->webView->setEnabled(false);
    QErrorMessage* _err = new QErrorMessage(this);

    _err->setWindowTitle("Error");
    _err->showMessage(err->error()->message);
}

void MainWindow::slotSuccess()
{
     emit signalSuccess();
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

void MainWindow::slotOrderPlaced(QString refNo, QString status)
{
    QErrorMessage* _err = new QErrorMessage();

    if (status == "PENDING") {
        _err->setWindowTitle("Failed !");
        _err->showMessage(QString("Order %1 placed, but couldn't finish the payment").arg(refNo));
    }
    if (status == "AUTHRECEIVED") {
        _err->setWindowTitle("Success !");
        _err->showMessage(QString("Congratulations!\nOrder %1 placed successfully").arg(refNo));
    }
    ui->centralWidget->close();
    ui->webView->close();

    setCentralWidget(_err);

    connect (_err, &QErrorMessage::accepted, this, &MainWindow::slotSuccess);
}

MainWindow::~MainWindow()
{
    delete ui;
}
