#include "apiclient.h"
#include "apirequest.h"
#include "apiresponse.h"
#include "billingdetails.h"

#include <QVariant>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtCrypto>

APIClient::APIClient(const QUrl url, const QString Merch, const QString Sk, QObject *parent) :
    QObject(parent),
    _MerchantCode (Merch),
    _SecretKey (Sk)
{

    networkManager = new QNetworkAccessManager();
    apiRequest = new QNetworkRequest(url);
    apiRequest->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(networkManager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(handleNetworkData(QNetworkReply*)));

    login(Merch, Sk);
}


bool APIClient::sessionStarted ()
{
    return (_cnt > 0 && !_sessionHash.isEmpty());
}

void APIClient::handleError (QString error)
{

}

void APIClient::handleSuccess (APIResponse* resp)
{

}

void APIClient::handleNetworkData(QNetworkReply *networkReply)
{
    QUrl _url = networkReply->url();
    if ( !networkReply->error() ) {
        QByteArray c(networkReply->readAll());

        APIResponse* resp = new APIResponse(_cnt);

        qDebug() << "Response:" << c;

        handleSuccess(resp);

        _cnt++;
    } else {
        handleError("HTTP Error: " + networkReply->errorString());
    }

    networkReply->deleteLater();
}

void APIClient::login(const QString Identifier, const QString SecretKey) const
{

    QDateTime _sessionStart = QDateTime::currentDateTimeUtc();
    QString now = _sessionStart.toString("yyyy-MM-dd hh:mm:ss");

    QByteArray Data;
//    qDebug () << Identifier.length() << Identifier << now.length() << now;

    Data.append(QString::number(Identifier.length())) \
          .append(Identifier) \
          .append(QString::number(now.length())) \
          .append (now);

//    Data.append ("8AVANTEST192014-01-18 17:34:5");
    QString hash;
    QCA::Initializer init;

    QCA::SecureArray key(SecretKey.toLatin1());
    if( !QCA::isSupported("hmac(md5)") ) {
        qDebug() << "hmac(md5) is not supported";
    } else {
        // create the required object using HMAC with MD5, and an
        // empty key.
        QCA::MessageAuthenticationCode hmacObject(  "hmac(md5)", QCA::SecureArray() );

        // create the key
        QCA::SymmetricKey keyObject(key);

        // set the HMAC object to use the key
        hmacObject.setup(key);
        // that could also have been done in the
        // QCA::MessageAuthenticationCode constructor

        // we split it into two parts to show incremental update
        QCA::SecureArray SecData(Data);
        hmacObject.update(SecData);


        // no more updates after calling final.
        QCA::SecureArray resultArray = hmacObject.final();

        // convert the result into printable hexadecimal.
        hash = QCA::arrayToHex(resultArray.toByteArray());

        //qDebug ()  << "HMAC(MD5) of" << Data.data() << "with" << key.data() << " = [" << hash.toLatin1().data() << "]\n";
    }

    QVariantList* _params = new QVariantList();
    _params->append(QVariant(Identifier));
    _params->append(QVariant(now));
    _params->append(QVariant(hash));

    APIRequest* r = new APIRequest(QString("login"), *_params, _cnt);

    QJsonDocument d;
    d.setObject(r->toJsonObject());

    qDebug() << "Request:" << d.toJson();

    networkManager->post(*apiRequest, d.toJson());
}

void APIClient::setLanguage (QString IsoLang)
{
    if (_sessionHash.isNull()) {
        login(_MerchantCode, _SecretKey);
    }

    QVariantList* _params = new QVariantList();
    _params->append(QVariant(_sessionHash));
    _params->append(QVariant(IsoLang));

    APIRequest* r = new APIRequest(QString("setLanguage"), *_params, _cnt);
    QJsonDocument d;
    d.setObject(r->toJsonObject());

    qDebug() << "Request:" << d.toJson();

    networkManager->post(*apiRequest, d.toJson());
}

void APIClient::setCountry (QString IsoCountry)
{

    QVariantList* _params = new QVariantList();
    _params->append(QVariant(_sessionHash));
    _params->append(QVariant(IsoCountry));

    APIRequest* r = new APIRequest(QString("setCountry"), *_params, _cnt);
    QJsonDocument d;
    d.setObject(r->toJsonObject());

    qDebug() << "Request:" << d.toJson();

    networkManager->post(*apiRequest, d.toJson());
}

void APIClient::setCurrency (QString IsoCurrency)
{

    QVariantList* _params = new QVariantList();
    _params->append(QVariant(_sessionHash));
    _params->append(QVariant(IsoCurrency));

    APIRequest* r = new APIRequest(QString("setCurrency"), *_params, _cnt);
    QJsonDocument d;
    d.setObject(r->toJsonObject());

    qDebug() << "Request:" << d.toJson();

    networkManager->post(*apiRequest, d.toJson());
}


void APIClient::setIP (QString IP)
{
    QVariantList* _params = new QVariantList();
    _params->append(QVariant(_sessionHash));
    _params->append(QVariant(IP));

    APIRequest* r = new APIRequest(QString("setIP"), *_params, _cnt);
    QJsonDocument d;
    d.setObject(r->toJsonObject());

    qDebug() << "Request:" << d.toJson();

    networkManager->post(*apiRequest, d.toJson());
}

void APIClient::setBillingDetails (const BillingDetails* Billing)
{
    QVariantList* _params = new QVariantList();
    _params->append(QVariant(_sessionHash));
    _params->append(QVariant(Billing));

    APIRequest* r = new APIRequest(QString("setBillingDetails"), *_params, _cnt);
    QJsonDocument d;
    d.setObject(r->toJsonObject());

    qDebug() << "Request:" << d.toJson();

    networkManager->post(*apiRequest, d.toJson());
}

void APIClient::placeOrder()
{
    QVariantList* _params = new QVariantList();
    _params->append(QVariant(_sessionHash));

    APIRequest* r = new APIRequest(QString("placeOrder"), *_params, _cnt);
    QJsonDocument d;
    d.setObject(r->toJsonObject());

    qDebug() << "Request:" << d.toJson();

    networkManager->post(*apiRequest, d.toJson());
}
