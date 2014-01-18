#include "apirequest.h"

APIRequest::APIRequest(const QString& method, QVariantList& params, const ushort id, QObject *parent) :
    _id (id),
    _jsonrpc(2.0),
    QObject(parent)
{
    setMethod(method);
    setParams(params);
}

QJsonObject APIRequest::toJsonObject()
{

    QVariantMap a;
    a.insert("method", method());
    a.insert("params", params());
    a.insert("id", id());
    a.insert("jsonrpc", (float)jsonrpc());

    return QJsonObject::fromVariantMap(a);
}

ushort APIRequest::id() const
{
    return _id;
}

void APIRequest::setMethod(const QString &method)
{
    _method = method;
}

QString APIRequest::method() const
{
    return _method;
}

void APIRequest::setParams(QVariantList &params)
{
    _params = params;
}

QVariantList APIRequest::params()
{
    return _params;
}

float APIRequest::jsonrpc() const
{
    return _jsonrpc;
}
