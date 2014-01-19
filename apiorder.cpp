#include "apiorder.h"

#include <QDebug>
#include <QtCrypto>
#include <QJsonObject>
#include <QJsonDocument>


APIOrder::APIOrder(QUrl url, QObject *parent) :
    QObject(parent)
{
   //_client = new APIOrder(url, this);

   connect (this, &APIOrder::signalError,
            this, &APIOrder::slotError);
   connect (this, &APIOrder::signalSuccess,
            this, &APIOrder::slotSuccess);

   networkManager = new QNetworkAccessManager();
   apiRequest = new QNetworkRequest(url);
   apiRequest->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

   connect(networkManager, &QNetworkAccessManager::finished,
              this, &APIOrder::handleNetworkData);
}

void APIOrder::executeRequest (const QString method, QVariantList *params) const
{
    APIRequest* request = APIRequest::createRequest(method, *params);
    request->setId(_cnt);
    request->setJsonRPC(2.0);

    QJsonDocument d;
    d.setObject(request->toJsonObject());

    qDebug() << "Request:" << d.toJson();

    networkManager->post(*apiRequest, d.toJson());
}

void APIOrder::handleNetworkData(QNetworkReply *networkReply)
{
    //QUrl _url = networkReply->url();
    if ( !networkReply->error() ) {
        QByteArray c(networkReply->readAll());

        QJsonDocument d = QJsonDocument::fromJson(c);
        qDebug() << "Response:" << d.toJson();

        APIResponse* resp = APIOrder::fromJsonDocument(&d, state());

        _cnt++;
        if (resp->isError()) {
            emit signalError(resp->error());
        } else {
            qDebug() << resp->result();
            emit signalSuccess(resp);
        }
    } else {
        APIResponse::Error _err;
        _err.code = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        _err.message = networkReply->errorString();
        emit signalError (&_err);
    }

    networkReply->deleteLater();
}

APIOrder::State APIOrder::state()
{
    return m_state;
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


APIResponse* APIOrder::fromJsonDocument(QJsonDocument* r, APIOrder::State m_state)
{
    QVariant v = r->toVariant();
    QVariantMap response = v.value<QVariantMap>();

    qDebug() << response;
    ushort m_id = response.find("id").value().toUInt();
    float m_version = response.find("jsonrpc").value().toFloat();
    APIResponse* _response = new APIResponse(m_id, m_version);

    /**/
    if (response.contains("error")) {
        QVariantMap _err = response.find("error").value().value<QVariantMap>();
         //= _errVariant.value<QVariantMap>();

        APIResponse::Error m_error;
        m_error.code = _err.find("code").value().toInt();
        m_error.message = _err.find("message").value().toString();

        _response->setError(m_error);

    }
    if (response.contains("result")) {
//        QVariant _result = response.find("result").value();
//        qDebug() << "result" << _result;
        _response->setResult(&response.find("result").value());

        qDebug() << "__noerr__" << _response->result();
    }
    qDebug() << "__err__" << _response->result();
    /**/
    return _response;
}


void APIOrder::login(const QString Identifier, const QString SecretKey) const
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

void APIOrder::slotError (APIResponse::Error *error)
{
    emit signalError(error);
    emit signalBusy(false);
}

void APIOrder::slotSuccess (APIResponse* r)
{

    switch (m_state){
    case State::LOGIN:
        _sessionHash = r->result()->toString();
        qDebug () << "Sess:" << r->result();//_sessionHash;
//        emit signalSessionStarted(_sessionHash);
        break;
    case State::ADDPRODUCT:

        break;
    case State::SETBILLINGDETAILS:

        break;
    case State::SETCOUNTRY:

        break;
    case State::SETCURRENCY:

        break;
    case State::SETIP:

        break;
    case State::SETLANGUAGE:

        break;
    case State::SETPAYMENTDETAILS:

        break;
    case State::PLACEORDER:

        break;
    }

    m_state = IDLE;
    emit signalBusy(false);
}
