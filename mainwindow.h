#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>

#include "urnnetworkaccessmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    AvangateAPI::urnNetworkAccessManager* _network;
    //QNetworkDiskCache* _cache;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void slotUrlChanged (const QUrl &url);
    void slotUnsupportedContent (QNetworkReply * reply);
    void slotHandleReply (QNetworkReply * reply);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
