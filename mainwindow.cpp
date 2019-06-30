#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::protocolName(QAbstractSocket::NetworkLayerProtocol protocol)
{
    //通过协议类型返回协议名称
    switch (protocol) {
        case QAbstractSocket::IPv4Protocol:
            return "IPv4 Protocol";
        case QAbstractSocket::IPv6Protocol:
            return "IPv6 Protocol";
        case QAbstractSocket::AnyIPProtocol:
            return "Any Ip Protocol";
        default:
            return "Unknown Network Layer Protocl";
    }
}

void MainWindow::on_btn_localHostName_clicked()
{
    //QHostInfo获取本机主机名
    QString hostName = QHostInfo::localHostName();//本地主机名
    this->ui->plainTextEdit->appendPlainText("本地主机名:" + hostName);
    this->ui->plainTextEdit->appendPlainText("-----------------------------------");


}

void MainWindow::on_btn_fromNameArgQString_clicked()
{
    QString hostName = QHostInfo::localHostName();//本地主机名
    QHostInfo hostINfo = QHostInfo::fromName(hostName);//本机IP地址
    QList<QHostAddress> addList = hostINfo.addresses();//IP地址列表
    if(!addList.isEmpty()){
        for(int i = 0; i < addList.count(); ++i){
            QHostAddress aHost = addList.at(i);//每一项是一个QHostAddress
            bool show = this->ui->chkOnlyIPv4->isChecked();//是否只显示IPV4
            if(show){
                show = (QAbstractSocket::IPv4Protocol == aHost.protocol());
            }else{
                show = true;
            }
            if(show){
                this->ui->plainTextEdit->appendPlainText("协议：" +  protocolName(aHost.protocol()));
                this->ui->plainTextEdit->appendPlainText("本机IP地址:" + aHost.toString());
                this->ui->plainTextEdit->appendPlainText("-----------------------------------");
            }


        }
    }
}

void MainWindow::on_btn_lookupHost_clicked()
{
    //查找主机信息
    QString hostName = this->ui->editHost->text();
    this->ui->plainTextEdit->appendPlainText("正在查找主机信息: " + hostName);
    QHostInfo::lookupHost(hostName, this, SLOT(lookedUpHostInfo(QHostInfo)));

}

void MainWindow::lookedUpHostInfo(const QHostInfo &host)
{
    //查找主机信息的槽函数
    QList<QHostAddress> addList = host.addresses();
    if(!addList.isEmpty()){
        for(int i = 0; i < addList.count(); ++i){
            QHostAddress aHost = addList.at(i);
            bool show = this->ui->chkOnlyIPv4->isChecked();//只显示IPV4
            if(show){
                show = QAbstractSocket::IPv4Protocol == aHost.protocol();
            }else{
                show = true;
            }
            if(show){
                this->ui->plainTextEdit->appendPlainText("协议: " + protocolName(aHost.protocol()));
                this->ui->plainTextEdit->appendPlainText(aHost.toString());
            }
        }
    }
}

void MainWindow::on_btnALLInterface_clicked()
{
    QList<QNetworkInterface > list = QNetworkInterface::allInterfaces();
    for(int i = 0; i < list.count(); ++i){
        QNetworkInterface aInterface = list.at(i);
        if(!aInterface.isValid()){
            continue;
        }
        this->ui->plainTextEdit->appendPlainText("设备名称: " + aInterface.humanReadableName());
        this->ui->plainTextEdit->appendPlainText("硬件地址: " + aInterface.hardwareAddress());
        QList<QNetworkAddressEntry> entryList = aInterface.addressEntries();
        for(int j = 0; j < entryList.count(); ++j){
            QNetworkAddressEntry aEntry = entryList.at(j);
            this->ui->plainTextEdit->appendPlainText("IP 地址: " + aEntry.ip().toString());
            this->ui->plainTextEdit->appendPlainText("子网掩码: " + aEntry.netmask().toString());
            this->ui->plainTextEdit->appendPlainText("广播地址:" + aEntry.broadcast().toString());
            this->ui->plainTextEdit->appendPlainText("--------------------------------------");
        }
        this->ui->plainTextEdit->appendPlainText("\n");
    }
}

void MainWindow::on_btn_allAddress_clicked()
{
    QList<QHostAddress> addList = QNetworkInterface::allAddresses();
    if(!addList.isEmpty()){
        for(int i = 0; i < addList.count(); ++i){
            QHostAddress aHost = addList.at(i);
            bool show = this->ui->chkOnlyIPv4->isChecked();
            if(show){
                show = QAbstractSocket::IPv4Protocol == aHost.protocol();
            }else{
                show = true;
            }
            if(show){
                this->ui->plainTextEdit->appendPlainText("协议: " + protocolName(aHost.protocol()));
                this->ui->plainTextEdit->appendPlainText("IP 地址: " + aHost.toString());
                this->ui->plainTextEdit->appendPlainText("--------------------------");
            }
        }
    }
}
