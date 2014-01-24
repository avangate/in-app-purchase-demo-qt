#ifndef RESPONSE_H
#define RESPONSE_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

namespace AvangateAPI
{
class Response
{
public:
    struct Error {
        qint32 code;
        QString message;
    };

private:
    ushort m_id;
    Error m_error;
    float m_jsonrpc;
    QVariant* m_result;

public:
//    explicit Response(const ushort id, const float jsonrpc);
//    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    void setResult (QVariant *result);
    QVariant *result();

    void setError (const Error err);
    Error *error();
    bool isError ();

    void setId (const ushort id);
    ushort id() const;

    void setJsonRPC (const float version);
    float jsonRPC() const;
};
}
#endif // RESPONSE_H
