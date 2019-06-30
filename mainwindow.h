#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostInfo>
#include <QNetworkInterface>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    QString protocolName(QAbstractSocket::NetworkLayerProtocol protocol);
private slots:
    void on_btn_localHostName_clicked();

    void on_btn_fromNameArgQString_clicked();

    void on_btn_lookupHost_clicked();

    void lookedUpHostInfo(const QHostInfo & host);

    void on_btnALLInterface_clicked();

    void on_btn_allAddress_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
