#include "request.h"

#include <QJsonArray>

using namespace AvangateAPI;

Request::Request()
{
}

void Request::read(const QJsonObject &json)
{
    m_id = json["id"].toInt ();
    m_jsonrpc = json["jsonrpc"].toDouble ();
    m_method = json["method"].toString ();
    m_params = json["params"].toArray ().toVariantList ();
}

void Request::write(QJsonObject &json) const
{
    json["id"] = m_id;
    json["jsonrpc"] = 2.0;
    json["method"] = m_method;
    json["params"] = QJsonArray::fromVariantList (m_params);
}

QJsonObject Request::jsonObject ()
{
    QJsonObject j;
    write(j);

    return j;
}

ushort Request::id() const { return m_id; }

void Request::setId(const ushort id) { m_id = id; }

void Request::setMethod(const QString &method) { m_method = method; }

QString Request::method() const { return m_method; }

void Request::setParams(QVariantList &params) { m_params = params; }

QVariantList Request::params() { return m_params; }

float Request::jsonrpc() const { return m_jsonrpc; }

void Request::setJsonRPC(const float version) { m_jsonrpc = version; }
