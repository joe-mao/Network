#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LabListen = new QLabel("监听状态:");
    LabListen->setMinimumWidth(150);
    this->ui->statusBar->addWidget(LabListen);

    LabSocketStateServer = new QLabel("Socket 状态");
    LabSocketStateServer->setMinimumWidth(200);
    this->ui->statusBar->addWidget(LabSocketStateServer);

    QString localIP = getLocalIP();//本机IP
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    tcpClient = new QTcpSocket(this);//创建socket变量
    LabSocketStateClient = new QLabel("Socket状态:  ");//状态栏标签
    LabSocketStateClient->setMinimumWidth(250);
    this->ui->statusBar->addWidget(LabSocketStateClient);

    connect(tcpClient, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(tcpClient, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChangeClient(QAbstractSocket::SocketState)));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(onSocketReadyReadClient()));

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

QString MainWindow::getLocalIP()
{
    //获取本机IP地址
    QString hostName = QHostInfo::localHostName();//本地主机名
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    QString localIP = "";
    QList<QHostAddress> addList = hostInfo.addresses();
    if(!addList.isEmpty()){
        for(int i = 0; i < addList.count(); ++i){
            QHostAddress aHost = addList.at(i);
         //   if(QAbstractSocket::IPv4Protocol == aHost.protocol()){
                localIP = aHost.toString();
                this->ui->comboIPTCPServer->addItem(localIP);
                this->ui->comboIPTCPClient->addItem(localIP);
               // break;
         //   }
        }
    }
    return localIP;
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

void MainWindow::onNewConnection()
{
    //当有客户端连入时
    tcpSocket = tcpServer->nextPendingConnection();//获取连入的客户端Socket
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(onClientConnected()));
    onClientConnected();
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    connect(tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    onSocketStateChange(tcpSocket->state());
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

void MainWindow::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    //socket状态变化时
    switch (socketState) {
        case QAbstractSocket::UnconnectedState:
            LabSocketStateServer->setText("socket状态: UnconnectedState"); break;
        case QAbstractSocket::HostLookupState:
            LabSocketStateServer->setText("socket状态: HostLookupState"); break;
        case QAbstractSocket::ConnectingState:
            LabSocketStateServer->setText("socket状态: ConnectingState"); break;
        case QAbstractSocket::BoundState:
            LabSocketStateServer->setText("socket状态: BoundState"); break;
        case QAbstractSocket::ClosingState:
            LabSocketStateServer->setText("socket状态: ClosingState"); break;
        case QAbstractSocket::ConnectedState:
            LabSocketStateServer->setText("socket状态: ConnectedState"); break;
        case QAbstractSocket::ListeningState:
            LabSocketStateServer->setText("socket 状态: ListeningState");
    }
}

void MainWindow::onClientConnected()
{
    //客户端接入时
    this->ui->plainTextEdit->appendPlainText("**client socket connected");
    this->ui->plainTextEdit->appendPlainText("**peer address:" + tcpSocket->peerAddress().toString());
    this->ui->plainTextEdit->appendPlainText("**peer port:" + QString::number(tcpSocket->peerPort()));
}

void MainWindow::onClientDisconnected()
{
    //客户端断开连接时
    this->ui->plainTextEdit->appendPlainText("**client socket disconnected");
    tcpSocket->deleteLater();
}

void MainWindow::onSocketReadyRead()
{
    while(tcpSocket->canReadLine()){
        this->ui->plainTextEdit->appendPlainText("[in] " + tcpSocket->readLine());
    }
}

void MainWindow::onConnected()
{
    this->ui->plainTextEdit->appendPlainText("**已连接到服务器");
    this->ui->plainTextEdit->appendPlainText("**peer address:" + tcpClient->peerAddress().toString());
    this->ui->plainTextEdit->appendPlainText("**peer port:" + QString::number(tcpClient->peerPort()));
    this->ui->actConnectToServer->setEnabled(false);
    this->ui->actDisconnectFromServer->setEnabled(true);
}

void MainWindow::onDisconnected()
{
    this->ui->plainTextEdit->appendPlainText("**已断开与服务器的连接");
    this->ui->actConnectToServer->setEnabled(true);
    this->ui->actDisconnectFromServer->setEnabled(false);
}

void MainWindow::onSocketStateChangeClient(QAbstractSocket::SocketState socketState)
{
    //socket状态变化时
    switch (socketState) {
        case QAbstractSocket::UnconnectedState:
            LabSocketStateClient->setText("socket状态: UnconnectedState"); break;
        case QAbstractSocket::HostLookupState:
            LabSocketStateClient->setText("socket状态: HostLookupState"); break;
        case QAbstractSocket::ConnectingState:
            LabSocketStateClient->setText("socket状态: ConnectingState"); break;
        case QAbstractSocket::BoundState:
            LabSocketStateClient->setText("socket状态: BoundState"); break;
        case QAbstractSocket::ClosingState:
            LabSocketStateClient->setText("socket状态: ClosingState"); break;
        case QAbstractSocket::ConnectedState:
            LabSocketStateClient->setText("socket状态: ConnectedState"); break;
        case QAbstractSocket::ListeningState:
            LabSocketStateClient->setText("socket 状态: ListeningState");
    }

}

void MainWindow::onSocketReadyReadClient()
{
    while(tcpClient->canReadLine()){
        this->ui->plainTextEdit->appendPlainText("[in] " + tcpClient->readLine());
    }
}

void MainWindow::on_actStartListening_triggered()
{
    //开始监听
    QString IP = this->ui->comboIPTCPServer->currentText();//IP地址
    quint32 port = this->ui->spinBoxServer->value();//端口
    QHostAddress addr(IP);
    tcpServer->listen(addr, port);//开始监听
    this->ui->plainTextEdit->appendPlainText("**开始监听......");
    this->ui->plainTextEdit->appendPlainText("**服务器地址：" + tcpServer->serverAddress().toString());
    this->ui->plainTextEdit->appendPlainText("**服务器端口: " + QString::number(tcpServer->serverPort()));
    this->ui->actStartListening->setEnabled(false);
    this->ui->actStopListening->setEnabled(true);
    LabListen->setText("监听状态: 正在监听");
}



void MainWindow::on_actStopListening_triggered()
{
    //停止监听
    if(tcpServer->isListening()){
        //tcpServer正在监听
        tcpServer->close();//停止监听
        this->ui->actStartListening->setEnabled(true);
        this->ui->actStopListening->setEnabled(false);
        LabListen->setText("监听状态: 已停止监听");
    }
}

void MainWindow::on_btnSendServer_clicked()
{
    QString msg = this->ui->editMsgServer->text();
    this->ui->plainTextEdit->appendPlainText("[out] " + msg);
    this->ui->editMsgServer->clear();
    this->ui->editMsgServer->setFocus();

    QByteArray str = msg.toUtf8();
    str.append("\n");//添加一个换行符
    tcpSocket->write(str);
}

void MainWindow::on_actConnectToServer_triggered()
{
    //"连接到服务器"按钮
    QString addr = this->ui->comboIPTCPServer->currentText();
    quint32 port = this->ui->spinBoxClient->value();
    tcpClient->connectToHost(addr, port);
}

void MainWindow::on_actDisconnectFromServer_triggered()
{
    //"断开连接"按钮
    if(tcpClient->state() == QAbstractSocket::ConnectedState){
        tcpClient->disconnectFromHost();
    }
}



void MainWindow::on_btnSendClient_clicked()
{
    //发送数据
    QString msg = this->ui->editMsgClient->text();
    this->ui->plainTextEdit->appendPlainText("[out] " + msg);
    this->ui->editMsgClient->clear();
    this->ui->editMsgClient->setFocus();
    QByteArray str = msg.toUtf8();
    str.append("\n");
    tcpClient->write(str);
}
