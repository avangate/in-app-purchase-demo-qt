#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>

#include "response.h"

namespace Ui {
class MainWindow;
}
using namespace AvangateAPI;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString _session;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void slotUrlChanged (const QUrl &url);

private:
    Ui::MainWindow *ui;

signals:
    void signalError(Response*);
    void signalSuccess();
    void signalOrderPlaced(QString);

public slots:
    void slotUnsupportedContent (QNetworkReply * reply);
    void slotHandleReply (QNetworkReply * reply);
    void slotError (Response*);
    void slotSetSession (QString);
    void slotOrderPlaced (QString);

};

#endif // MAINWINDOW_H
