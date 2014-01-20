#ifndef APIRESPONSE_H
#define APIRESPONSE_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

class APIResponse : public QObject
{
    Q_OBJECT

public:
    struct Error {
        qint16 code;
        QString message;
    };

private:
    ushort _id;
    Error _error;
    float _jsonrpc;
    QVariant* _result;

public:
    explicit APIResponse(const ushort id, const float jsonrpc, QObject *parent = 0);

    bool isError ();

    void setError (Error err);
    void setResult (QVariant *result);

    Error *error();
    QVariant *result ();
    ushort id() const;
    float jsonRPC() const;

signals:

public slots:

};

#endif // APIRESPONSE_H
