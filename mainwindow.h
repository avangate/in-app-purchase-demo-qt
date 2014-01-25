#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>

#include "urnnetworkaccessmanager.h"
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
    AvangateAPI::urnNetworkAccessManager* _network;
    //QNetworkDiskCache* _cache;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void slotUrlChanged (const QUrl &url);

private:
    Ui::MainWindow *ui;

public slots:
    void slotUnsupportedContent (QNetworkReply * reply);
    void slotHandleReply (QNetworkReply * reply);
    void slotError (Response*);
    void slotSetSession (QString);

};

#endif // MAINWINDOW_H
