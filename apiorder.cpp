#include "apiorder.h"
#include "order.h"

#include <QDebug>
#include <QtCrypto>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonDocument>


APIOrder::APIOrder(QUrl url, QObject *parent) :
    QObject(parent),
    _cnt(0)
{
   //_client = new APIOrder(url, this);

   connect (this, &APIOrder::signalError,
            this, &APIOrder::slotError);
   connect (this, &APIOrder::signalSuccess,
            this, &APIOrder::slotSuccess);

   networkManager = new QNetworkAccessManager();
   apiRequest = new QNetworkRequest(url);
   apiRequest->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
   apiRequest->setHeader(QNetworkRequest::UserAgentHeader, QCoreApplication::applicationName() + ":" + QCoreApplication::applicationVersion());

   connect(networkManager, &QNetworkAccessManager::finished,
              this, &APIOrder::handleNetworkData);
}

void APIOrder::executeRequest (const QString method, QVariantList *params)
{
    APIRequest* request = APIRequest::createRequest(method, *params);
    request->setId(_cnt);
    request->setJsonRPC(2.0);

    QJsonDocument d;
    d.setObject(request->toJsonObject());

    qDebug() << "Request:" << d.toJson();

    _cnt++;
    networkManager->post(*apiRequest, d.toJson());
}

void APIOrder::handleNetworkData(QNetworkReply *networkReply)
{
    //QUrl _url = networkReply->url();
    if ( !networkReply->error() ) {
        QByteArray c(networkReply->readAll());

        QJsonDocument d = QJsonDocument::fromJson(c);
        qDebug() << "Response:" << d.toJson();

//        APIResponse* resp = APIOrder::fromJsonDocument(&d, state());
        parseResponse(&d);

      } else {
        APIOrder::Response _r;
        APIResponse::Error _err;
        _err.code = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        _err.message = networkReply->errorString();

        qDebug() << _err.message;

        _r.status = false;
        _r.error = &_err;
        //emit signalError (state(), &_r);
    }

    networkReply->deleteLater();
}

APIOrder::State APIOrder::state()
{
    return m_state;
}

QMap<ushort, APIOrder::Response>* APIOrder::states()
{
    return m_states;
}

QString APIOrder::getCallMethod (APIOrder::State m_state)
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

void APIOrder::parseResponse(QJsonDocument* r)
{
    QVariant v = r->toVariant();
    QVariantMap response = v.value<QVariantMap>();

    ushort m_id = response.find("id").value().toUInt();
    float m_version = response.find("jsonrpc").value().toFloat();

    APIOrder::Response r_resp;//states()->find(m_id).value();
    /**/
    if (response.contains("error")) {
        QVariantMap _err = response.find("error").value().value<QVariantMap>();

        qint16 mcode = static_cast<qint16>(_err.find("code").value().toInt());
        APIResponse::Error m_error {
            mcode,
            _err.find("message").value().toString()
        };

        r_resp.error = &m_error;
        //emit signalError(m_id, &r_resp);
    }

    if (response.contains("result")) {
        QVariant _result = response.find("result").value();

        switch (m_state){
        case State::LOGIN:
            _sessionHash = _result.toString();
            emit signalSessionStarted(_sessionHash);
            break;
        case State::ADDPRODUCT:
        case State::SETBILLINGDETAILS:
        case State::SETCOUNTRY:
        case State::SETCURRENCY:
        case State::SETIP:
        case State::SETLANGUAGE:
        case State::SETPAYMENTDETAILS:
            qDebug() << r_resp.call << r_resp.error->message << r_resp.status;
//            if (_result.toBool())  {
//                emit signalSuccess(m_id, &r_resp);
//            } else {
//                APIResponse::Error _e {-1, QString("Unkown error")};
//                r_resp.error = &_e;
//                emit signalError(m_id, &r_resp);
//            }
            break;
        case State::PLACEORDER:
        case State::GETORDER:
            Order* o = new Order();
            break;
        }
        qDebug() << m_id << r_resp.call << r_resp.error->message << r_resp.status;
        //emit signalSuccess(m_id, &r_resp);
    }
}

void APIOrder::login(const QString Identifier, const QString SecretKey)
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
    _params->append(QVariant(Identifier));
    _params->append(QVariant(now));
    _params->append(QVariant(hash));

    executeRequest(getCallMethod(m_state), _params);
}

void APIOrder::setLanguage (const QString IsoLang)
{
    m_state = SETLANGUAGE;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(IsoLang);

    executeRequest(getCallMethod(m_state), _params);
}

void APIOrder::setCountry (const QString IsoCountry)
{
    m_state = SETCOUNTRY;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(IsoCountry);

    executeRequest(getCallMethod(m_state), _params);
}

void APIOrder::setCurrency (const QString IsoCurrency)
{
    m_state = SETCURRENCY;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(IsoCurrency);

    executeRequest(getCallMethod(m_state), _params);
}

void APIOrder::setIP(const QString IP)
{
    m_state = SETIP;
    emit signalBusy(true);
    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(IP);

    executeRequest(getCallMethod(m_state), _params);
}

void APIOrder::setBillingDetails(const BillingDetails* Billing)
{
    m_state = SETBILLINGDETAILS;
    emit signalBusy(true);
    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);
    _params->append(Billing);

    executeRequest(getCallMethod(m_state), _params);
}

void APIOrder::addProduct (const quint32 ProductId, quint8 Quantity, QStringList PriceOptions)
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

void APIOrder::placeOrder()
{
    m_state = PLACEORDER;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append(_sessionHash);

    executeRequest(getCallMethod(m_state), _params);
}

void APIOrder::slotError (const ushort id, APIOrder::Response* response)
{
    emit signalBusy(false);

    states()->insert(id, *response);
}

void APIOrder::slotSuccess (const ushort id, APIOrder::Response* response)
{
    m_state = IDLE;
    emit signalBusy(false);

    states()->insert(id, *response);
}
