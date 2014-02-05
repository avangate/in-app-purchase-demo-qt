#include "paymentwindow.h"
#include "ui_paymentwindow.h"

#include <QStandardPaths>
#include <QWebPage>
#include <QDebug>
#include <QNetworkProxyFactory>
#include <QFile>
#include <QUrlQuery>
#include <QErrorMessage>

using namespace AvangateAPI;

PaymentWindow::PaymentWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaymentWindow)
{
    ui->setupUi(this);

//    connect(ui->webView->page(), &QWebPage::unsupportedContent,
//            this, &PaymentWindow::slotUnsupportedContent);
    connect(ui->webView->page()->networkAccessManager (), &QNetworkAccessManager::finished,
                         this, &PaymentWindow::slotHandleReply);
    connect(this, &PaymentWindow::signalOrderPlaced, this, &PaymentWindow::slotOrderPlaced);
    connect(this, &PaymentWindow::signalPaymentDetailsAdded, this, &PaymentWindow::slotPaymentDetailsAdded);
    connect(this, &PaymentWindow::signalError, this, &PaymentWindow::slotError);
}

void PaymentWindow::slotHandleReply (QNetworkReply *reply)
{
    //qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << reply->rawHeaderPairs ();
    qDebug() << reply->url().toString();

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

            emit signalError(madeup, SETPAYMENTDETAILS);
        }     
        if (q.queryItemValue("status") == "AUTHRECEIVED" || q.queryItemValue("status") == "PENDING" || q.queryItemValue("status") == "TEST") {
            QString refNo = q.queryItemValue("refNo");
            emit signalOrderPlaced(refNo, q.queryItemValue("status"));
        }
        if (q.queryItemValue("status") == "OK") {
            emit signalPaymentDetailsAdded();
        }
    } else {
        // some error
    }
}

void PaymentWindow::slotUnsupportedContent (QNetworkReply * reply)
{
    qDebug() << reply->readAll();
}

void PaymentWindow::slotUrlChanged (const QUrl& url)
{
    qDebug() << "url changes:" << url;
}

void PaymentWindow::slotError(Response* err)
{
    //ui->webView->setEnabled(false);
    QErrorMessage* _err = new QErrorMessage(this);

    _err->setWindowTitle("Error");
    _err->showMessage(err->error()->message);
}

void PaymentWindow::slotSuccess()
{
//     emit signalSuccess();
}

void PaymentWindow::slotSetSession(QString session)
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

    ui->webView->setHtml(QString(html).arg (u.toString()).arg(_session).arg("CCNOPCI").arg("eur").arg("ro"));
}

void PaymentWindow::slotPaymentDetailsAdded()
{
    QVariant v;
    QVariantMap result;
    result.insert("status", "OK");

    Response madeUp;
    madeUp.setId(m_id);
    madeUp.setResult(&v);

    emit signalSuccess (&madeUp, SETPAYMENTDETAILS);
}

void PaymentWindow::slotOrderPlaced(QString refNo, QString status)
{
    QErrorMessage* _err = new QErrorMessage();

    if (status == "PENDING") {
        _err->setWindowTitle("Failed !");
        _err->showMessage(QString("Order %1 placed, but couldn't finish the payment").arg(refNo));
    }
    if (status == "AUTHRECEIVED" || status == "TEST") {
        _err->setWindowTitle("Success !");
        _err->showMessage(QString("Congratulations!\nOrder %1 placed successfully with status %2").arg(refNo).arg(status));
    }

    QVariant v;
    QVariantMap result;
    result.insert("status", status);
    result.insert("refNo", refNo);

    Response madeUp;
    madeUp.setId(m_id);
    madeUp.setResult(&v);

    close();
    emit signalSuccess (&madeUp, SETPAYMENTDETAILS);
}

PaymentWindow::~PaymentWindow()
{
    delete ui;
}

void PaymentWindow::setId(ushort id)
{
    m_id = id;
}
