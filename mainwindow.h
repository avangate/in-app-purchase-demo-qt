#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
using namespace AvangateAPI;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

signals:
    void signalError (QString);
private slots:
    void showPaymentWindow();
    void slotError (QString);

};

#endif // MAINWINDOW_H
