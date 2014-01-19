#include "apirequest.h"

APIRequest::APIRequest(QObject *parent) :
    QObject(parent)
{
}

APIRequest* APIRequest::createRequest (const char* method, QVariantList &params)
{
    return APIRequest::createRequest(QString(method), params);
}

APIRequest* APIRequest::createRequest (const QString& method, QVariantList &params)
{
    APIRequest* req = new APIRequest();

    req->setMethod(method);
    req->setParams(params);

    return req;
}

QJsonObject APIRequest::toJsonObject()
{

    QVariantMap a;
    a.insert("method", method());
    a.insert("params", params());
    a.insert("id", id());
    a.insert("jsonrpc", jsonrpc());

    return QJsonObject::fromVariantMap(a);
}

ushort APIRequest::id() const
{
    return _id;
}

void APIRequest::setId(const ushort id)
{
    _id = id;
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

void APIRequest::setJsonRPC(const float version)
{
    _jsonrpc = version;
}
