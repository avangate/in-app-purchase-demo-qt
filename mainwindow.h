#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tvplot.h"

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
    TVPlot *d_plot;

signals:
    void signalError (QString);
private slots:
    void showPaymentWindow();
    void slotError (QString);

};

#endif // MAINWINDOW_H
