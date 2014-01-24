#include "response.h"

#include <QDebug>
#include <QJsonDocument>

using namespace AvangateAPI;

//Response::Response(const ushort id, const float version) :
//    QJsonObject()/*,
//    _id (id),
//    _jsonrpc(version)*/
//{
//}

//void Response::read(const QJsonObject &json)
//{
//    m_id = json["id"].toInt ();
//    m_jsonrpc = json["jsonrpc"].toDouble ();
//    if (json.contains ("error")) {
//        m_error = Response::Error {
//            json["error"].toObject ()["code"].toInt (),
//            json["error"].toObject ()["message"].toString ()
//        };
//    }
//    //QVariant v = json["result"].type ();
//    qDebug() << json["result"].type ();
////    m_result = &v;
//}

void Response::write(QJsonObject &json) const
{
    json["id"] = m_id;
    json["jsonrpc"] = 2.0;

    QVariantMap _e;
    _e.insert ("code", m_error.code);
    _e.insert ("message", m_error.message);

    json["error"] = QJsonObject::fromVariantMap (_e);
//    json["result"] = QJsonObject::fromVariantMap (*m_result);
}


void Response::setId (const ushort id) { m_id = id; }
ushort Response::id () const {  return m_id; }

void Response::setError (const Error err) { m_error = err; }
Response::Error* Response::error() { return &m_error;}
bool Response::isError () { return !m_error.message.isEmpty(); }

void Response::setResult (QVariant *result) { m_result = result; }
QVariant* Response::result () { return m_result; }

void Response::setJsonRPC (float version) { m_jsonrpc = version; }
float Response::jsonRPC () const { return m_jsonrpc;}

