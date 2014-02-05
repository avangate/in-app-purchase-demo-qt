#include "order.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageAuthenticationCode>

#include "config.h"
#include "paymentwindow.h"

using namespace AvangateAPI;

Order::Order(QUrl url, QWidget *parent) :
    QObject(parent),
    m_currentState (IDLE),
    _cnt(0),
    m_state (IDLE),
    m_url(url)
{
   connect (this, &Order::signalError,
            this, &Order::slotError);

   connect (this, &Order::signalSuccess,
            this, &Order::slotSuccess);

   connect (this, &Order::signalShowPaymentWindow,
            this, &Order::slotShowPaymentWindow);

   connect (this, &Order::signalBillingDetailsAdded,
            this, &Order::slotBillingDetailsAdded);

   connect (this, &Order::signalPaymentDetailsAdded,
            this, &Order::slotPaymentDetailsAdded);

   connect (this, &Order::signalProductAdded,
            this, &Order::slotProductAdded);

   connect (this, &Order::signalOrderPlaced,
            this, &Order::slotOrderPlaced);

   networkManager = new QNetworkAccessManager();

   connect(networkManager, &QNetworkAccessManager::finished,
              this, &Order::handleNetworkData);

   qDebug() << "Connecting to:" << m_url.toString();
}

void Order::executeRequest (const QString method, QVariantList *params)
{
    Request* request = new Request();
    request->setId (_cnt);
    request->setJsonRPC (2.0);
    request->setMethod (method);
    request->setParams(*params);

    QJsonDocument d;
    d.setObject(request->jsonObject());

    CallStatus stat {m_state, false};
    //m_states->setSharable(true);
    m_states.insert(_cnt, stat);

    //qDebug() << t << stat.call << stat.status;
    _cnt++;

    QNetworkRequest* _request = new QNetworkRequest(m_url);
    _request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //qDebug() << "Request:" << d.toJson();

    networkManager->post(*_request, d.toJson());
}

void Order::handleNetworkData (QNetworkReply *networkReply)
{
    if ( !networkReply->error() ) {
        QByteArray c(networkReply->readAll());

        QJsonDocument d = QJsonDocument::fromJson(c);
        //qDebug() << "Response:" << d.toJson();

        parseResponse(d);

      } else {
        Response* _r = new Response();
        _r->setId (_cnt - 1);
        Response::Error _err;
        _err.code = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        _err.message = networkReply->errorString();

        _r->setError (_err);
        emit signalError (_r, m_state);
    }

    networkReply->deleteLater();
}

Order::State Order::state()
{
    return m_state;
}

//QMap<int, Order::State *> *Order::states()
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
    case State::GETORDER:
        method = "getOrder";
        break;
    case State::IDLE:
    default:
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

    State c_state = m_states.value(resp->id()).call;

    if (json.contains ("error")) {
        resp->setError (Response::Error {
            json["error"].toObject ()["code"].toInt (),
            json["error"].toObject ()["message"].toString ()
        });

        emit signalError(resp, c_state);
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
        case QJsonValue::Null:
            resultVariant = QVariant::fromValue(NULL);
            break;
        case QJsonValue::Double:
            resultVariant = QVariant::fromValue(json["result"].toDouble());
            break;
        case QJsonValue::Array:
            resultVariant = QVariant::fromValue(json["result"].toArray());
            break;
        case QJsonValue::Undefined:
            break;
        }
        resp->setResult (&resultVariant);

        emit signalSuccess(resp, c_state);
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

    QString hash =  QMessageAuthenticationCode::hash(Data, SecretKey.toLatin1(), QCryptographicHash::Md5).toHex();

    m_state = LOGIN;
    emit signalBusy(true);

    QVariantList* _params = new QVariantList();
    _params->append (Identifier);
    _params->append (now);
    _params->append (hash);

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

void Order::slotError (Response* response, Order::State c_state)
{
    emit signalBusy(false);
    qDebug() << response->error()->message << "in state" << getStateName(c_state);
}

void Order::slotSuccess (Response* response, Order::State c_state)
{
    m_state = IDLE;
    emit signalBusy(false);

    switch (c_state) {
    case State::LOGIN:
        _sessionHash = response->result()->toString();
        m_currentState |= LOGIN;
        emit signalSessionStarted(_sessionHash);
        break;
    case State::ADDPRODUCT:
        emit signalProductAdded();
        break;
    case State::SETBILLINGDETAILS:
        emit signalBillingDetailsAdded();
        break;
    case State::SETPAYMENTDETAILS:
        //qDebug() << response->result()->toString();
        emit signalPaymentDetailsAdded();
        break;
    case State::PLACEORDER:
//        qDebug() << response->result()->t;
        emit signalOrderPlaced();
        break;
    case State::SETCOUNTRY:
        m_currentState |= SETCOUNTRY;
        break;
    case State::SETCURRENCY:
        m_currentState |= SETCURRENCY;
        break;
    case State::SETIP:
        m_currentState |= SETIP;
        break;
    case State::SETLANGUAGE:
        m_currentState |= SETLANGUAGE;
        break;
    case State::GETORDER:
        m_currentState |= GETORDER;
        break;
    case State::IDLE:
        break;
    }

//    qDebug() << "last state" << c_state;
    if (
         ((m_currentState & State::LOGIN) == State::LOGIN) &&
         ((m_currentState & State::SETBILLINGDETAILS) == State::SETBILLINGDETAILS) &&
         ((m_currentState & State::ADDPRODUCT) == State::ADDPRODUCT) &&
         ((m_currentState & State::SETPAYMENTDETAILS) != State::SETPAYMENTDETAILS)
            ) {

        emit signalSetupFinished();
    }
    qDebug() << "Success on call:" << response->id () << "FULL state:" << getActiveStatesLabels(m_currentState);
 }

void  Order::setPaymentDetails (PaymentDetails *Payment)
{
    m_state = SETPAYMENTDETAILS;
    emit signalBusy(true);

    // conditional about PCI compliance
    if (Payment == 0) {
        // this is for no PCI compliance - showing webview with card form
        emit signalShowPaymentWindow();
    } else {
        // PCI compliant
        QVariantMap p;
        p.insert("Type", Payment->type());
        p.insert("Currency", Payment->currency());
        p.insert("CustomerIP", Payment->customerIP());

        QVariantList* _params = new QVariantList();
        _params->append(_sessionHash);
        _params->append(p);

        executeRequest(getCallMethod(m_state), _params);
    }
}

void Order::slotShowPaymentWindow()
{

    PaymentWindow* w = new PaymentWindow(_cnt);

    w->slotSetSession(_sessionHash);
//    connect(this, &Order::signalSessionStarted, w, &PaymentWindow::slotSetSession);
//    connect(this, &Order::signalSetupFinished, w, &PaymentWindow::show);

    connect(w, &PaymentWindow::signalError, this, &Order::slotError);
    connect(this, &Order::signalError, w, &PaymentWindow::slotError);

    connect(w, &PaymentWindow::signalSuccess, this, &Order::slotSuccess);

    w->show();
}

void Order::slotProductAdded()
{
    m_currentState |= ADDPRODUCT;
}

void Order::slotBillingDetailsAdded()
{
    m_currentState |= SETBILLINGDETAILS;
}

void Order::slotPaymentDetailsAdded()
{
    m_currentState |= SETPAYMENTDETAILS;
}

void Order::slotOrderPlaced()
{
    m_currentState |= PLACEORDER;
}

QString Order::getStateName(State state)
{
    QString label;
    switch (state) {
    case State::LOGIN:
        label = "Login";
        break;
    case State::ADDPRODUCT:
        label = "Product added";
        break;
    case State::SETBILLINGDETAILS:
        label = "Billing Details set";
        break;
    case State::SETCOUNTRY:
        label = "Country set";
        break;
    case State::SETCURRENCY:
        label = "Currency set";
        break;
    case State::SETIP:
        label = "Ip set";
        break;
    case State::SETLANGUAGE:
        label = "Language set";
        break;
    case State::SETPAYMENTDETAILS:
        label = "Payment Details set";
        break;
    case State::PLACEORDER:
        label = "Order placed";
        break;
    case State::IDLE:
        label = "Idle";
        break;
    case State::GETORDER:
        label = "Get order";
        break;
    }

    return label;
}

QStringList Order::getActiveStatesLabels(int state)
{
    QStringList stateLabels;
    if ((state & LOGIN) == LOGIN) {
        stateLabels << getStateName(LOGIN);
    }
    if ((state & ADDPRODUCT) == ADDPRODUCT) {
        stateLabels << getStateName(ADDPRODUCT);
    }
    if ((state & SETBILLINGDETAILS) == SETBILLINGDETAILS) {
        stateLabels << getStateName(SETBILLINGDETAILS);
    }
    if ((state & SETPAYMENTDETAILS) == SETPAYMENTDETAILS) {
        stateLabels << getStateName(SETPAYMENTDETAILS);
    }
    if ((state & SETCOUNTRY) == SETCOUNTRY) {
        stateLabels << getStateName(SETCOUNTRY);
    }
    if ((state & SETCURRENCY) == SETCURRENCY) {
        stateLabels << getStateName(SETCURRENCY);
    }
    if ((state & SETLANGUAGE) == SETLANGUAGE) {
        stateLabels << getStateName(SETLANGUAGE);
    }
    if ((state & SETIP) == SETIP) {
        stateLabels << getStateName(SETIP);
    }
    if ((state & PLACEORDER) == PLACEORDER) {
        stateLabels << getStateName(PLACEORDER);
    }

    return stateLabels;
}
