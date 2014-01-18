#ifndef APIRESPONSE_H
#define APIRESPONSE_H

#include <QObject>
#include <QJsonObject>

class APIResponse : public QObject
{
    Q_OBJECT

private:
    const ushort _id;

public:
    explicit APIResponse(const ushort id, QObject *parent = 0);

    QJsonObject toJsonObject();
    void fromJsonObject(QJsonObject r);

    bool isError ();
    QString getError ();
    QString getResponse ();

    ushort id() const;

signals:

public slots:

};

#endif // APIRESPONSE_H
