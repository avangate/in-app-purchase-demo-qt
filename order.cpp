#include "order.h"
//#include "order.h"

#include <QDebug>
#include <QtCrypto>
#include <QJsonObject>
#include <QJsonDocument>

using namespace AvangateAPI;

Order::Order(QUrl url, QObject *parent) :
    QObject(parent),
    _cnt(0)
{
   //_client = new Order(url, this);

   m_url = url;
   connect (this, &Order::signalError,
            this, &Order::slotError);
   connect (this, &Order::signalSuccess,
            this, &Order::slotSuccess);

   networkManager = new QNetworkAccessManager();

   connect(networkManager, &QNetworkAccessManager::finished,
              this, &Order::handleNetworkData);
}

void Order::executeRequest (const QString method, QVariantList *params)
{
    Request* request = new Request();
    request->setId (_cnt);
    request->setJsonRPC (2.0);
    request->setMethod (method);
    request->setParams(*params);

//    if (method == "setBillingDetails") {
//        QString _sess = params->begin ();
//        BillingDetails* = params->end<Bikl ();
//    } else {

//    }

    QJsonDocument d;
    d.setObject(request->jsonObject());

//    states()->insert(_cnt, );

    _cnt++;

    QNetworkRequest* _request = new QNetworkRequest(m_url);
    _request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    qDebug() << "Request:" << d.toJson();

    networkManager->post(*_request, d.toJson());
}

void Order::handleNetworkData (QNetworkReply *networkReply)
{
    if ( !networkReply->error() ) {
        QByteArray c(networkReply->readAll());

        QJsonDocument d = QJsonDocument::fromJson(c);
        qDebug() << "Response:" << d.toJson();

        parseResponse(d);

      } else {
        Response* _r = new Response();
        _r->setId (_cnt - 1);
        Response::Error _err;
        _err.code = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        _err.message = networkReply->errorString();

        _r->setError (_err);
        emit signalError (_r);
    }

    networkReply->deleteLater();
}

Order::State Order::state()
{
    return m_state;
}

//QMap<ushort, Response>* Order::states()
//{
//    return m_states;
//}

QString Order::getCallMethod (Order::State m_state)
{
    QString method;
    switch (m_state) {
    case State::LOGIN:
        method = "login";
        break;
    case State::ADDPRODUCT:
        method = "addProduct";
        break;
    case State::SETBILLINGDETAILS:
        method = "setBillingDetails";
        break;
    case State::SETCOUNTRY:
        method = "setCountry";
        break;
    case State::SETCURRENCY:
        method = "setCurrency";
        break;
    case State::SETIP:
        method = "setIp";
        break;
    case State::SETLANGUAGE:
        method = "setLanguage";
        break;
    case State::SETPAYMENTDETAILS:
        method = "setPaymentDetails";
        break;
    case State::PLACEORDER:
        method = "placeOrder";
        break;
    }

    return method;
}

void Order::parseResponse(QJsonDocument jsonDoc)
{
    QJsonObject json = jsonDoc.object ();

    Response* resp = new Response();
    resp->setId (json["id"].toInt ());
    resp->setJsonRPC (json["jsonrpc"].toDouble ());
    if (json.contains ("error")) {
        resp->setError (Response::Error {
            json["error"].toObject ()["code"].toInt (),
            json["error"].toObject ()["message"].toString ()
        });

        emit signalError(resp);
    } else {
        QVariant resultVariant;
        switch (json["result"].type ()) {
        case QJsonValue::String:
            resultVariant = QVariant::fromValue(json["result"].toString ());
            break;
        case QJsonValue::Bool:
            resultVariant = QVariant::fromValue(json["result"].toBool ());
            break;
        case QJsonValue::Object:
            resultVariant = QVariant::fromValue(json["result"].toObject ().toVariantMap ());
            break;
        }
        resp->setResult (&resultVariant);


        switch (m_state) {
        case State::LOGIN:
            _sessionHash = resp->result ()->toString();
            emit signalSessionStarted(_sessionHash);
            break;
        case State::ADDPRODUCT:
        case State::SETBILLINGDETAILS:
        case State::SETCOUNTRY:
        case State::SETCURRENCY:
        case State::SETIP:
        case State::SETLANGUAGE:
        case State::SETPAYMENTDETAILS:
//            if (_result.toBool())  {
//                emit signalSuccess(m_id, &r_resp);
//            } else {
//                Response::Error _e {-1, QString("Unkown error")};
//                r_resp.error = &_e;
//                emit signalError(m_id, &r_resp);
//            }
            break;
        case State::PLACEORDER:
        case State::GETORDER:
//            Order* o = new Order();
            break;
        }
        emit signalSuccess(resp);
        /**/
    }
}

void Order::login(const QString Identifier, const QString SecretKey)
{

    QDateTime _sessionStart = QDateTime::currentDateTimeUtc();
    QString now = _sessionStart.toString("yyyy-MM-dd hh:mm:ss");

    QByteArray Data;

    Data.append(QString::number(Identifier.length())) \
          .append(Identifier) \
          .append(QString::number(now.length())) \
          .append (now);

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
    m_state = LOGIN;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append (Identifier);
    _params->append (now);
    _params->append (hash);
//    _params << Identifier;
//    _params << now;
//    _params << hash;

    executeRequest(getCallMethod(m_state), _params);
}

void Order::setLanguage (const QString IsoLang)
{
    m_state = SETLANGUAGE;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(IsoLang);

    executeRequest(getCallMethod(m_state), _params);
}

void Order::setCountry (const QString IsoCountry)
{
    m_state = SETCOUNTRY;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(IsoCountry);

    executeRequest(getCallMethod(m_state), _params);
}

void Order::setCurrency (const QString IsoCurrency)
{
    m_state = SETCURRENCY;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(IsoCurrency);

    executeRequest(getCallMethod(m_state), _params);
}

void Order::setIP(const QString IP)
{
    m_state = SETIP;
    emit signalBusy(true);
    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(IP);

    executeRequest(getCallMethod(m_state), _params);
}

void Order::setBillingDetails(BillingDetails *Billing)
{
    m_state = SETBILLINGDETAILS;

    QVariantMap b;
    b.insert("FirstName", Billing->FirstName());
    b.insert("LastName", Billing->LastName());
    b.insert("Email", Billing->Email());
    b.insert("City", Billing->City());
    b.insert("Country", Billing->Country());
    b.insert("State", Billing->State());
    b.insert("PostalCode", Billing->PostalCode());
    b.insert("Address", Billing->Address());

    emit signalBusy(true);
    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(QVariant::fromValue(b));

    executeRequest(getCallMethod(m_state), _params);
}

void Order::addProduct (const quint32 ProductId, quint8 Quantity, QStringList PriceOptions)
{
    m_state = ADDPRODUCT;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(ProductId);
    _params->append(Quantity);
    _params->append(PriceOptions.join(','));

    executeRequest(getCallMethod(m_state), _params);
}

void Order::placeOrder()
{
    m_state = PLACEORDER;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);

    executeRequest(getCallMethod(m_state), _params);
}

void Order::slotError ( Response* response)
{
    emit signalBusy(false);
//    states()->insert(id, *response);
}

void Order::slotSuccess (Response* response)
{
    m_state = IDLE;
    emit signalBusy(false);

    qDebug() << "Success:" << response->id ();
//    states()->insert(id, *response);
}
