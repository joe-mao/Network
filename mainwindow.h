#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>


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

    //自定义槽函数
    void onNewConnection();//QTcpServer的newConnection()信号
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void onClientConnected();//Client socket connected
    void onClientDisconnected();//Client Socket disconnected
    void onSocketReadyRead();//读取socket传入的数据

    void onConnected();
    void onDisconnected();
    void onSocketStateChangeClient(QAbstractSocket::SocketState socketState);
    void onSocketReadyReadClient();//读取socket传入的数据




    void on_actStartListening_triggered();

    void on_actStopListening_triggered();

    void on_btnSendServer_clicked();

    void on_actConnectToServer_triggered();

    void on_actDisconnectFromServer_triggered();

    void on_btnSendClient_clicked();

protected:
//    void closeEvent(QCloseEvent * event);


private:
    Ui::MainWindow *ui;
    QLabel * LabListen;//状态栏标签
    QLabel * LabSocketStateServer;//状态栏标签
    QLabel * LabSocketStateClient;
    QTcpServer * tcpServer;//Tcp服务器
    QTcpSocket * tcpClient;
    QTcpSocket * tcpSocket;//Tcp通信的Socket
    QString getLocalIP();//获取本机IP地址


};

#endif // MAINWINDOW_H
