#include "apiresponse.h"

#include <QDebug>
#include <QJsonDocument>

APIResponse::APIResponse(const ushort id, const float version, QObject *parent) :
    QObject(parent),
    _id (id),
    _jsonrpc(version)
{
}

ushort APIResponse::id() const
{
    return _id;
}

bool APIResponse::isError ()
{
    return !_error.message.isEmpty();
}

APIResponse::Error* APIResponse::error()
{
    return &_error;
}

void APIResponse::setError (Error err)
{
    _error = err;
}

void APIResponse::setResult (QVariant *result)
{
    _result = result;
}

QVariant* APIResponse::result ()
{
    return _result;
}

float APIResponse::jsonRPC () const
{
    return _jsonrpc;
}

