#include "server.h"

Server::Server(int nPort, QWidget *parent) : QWidget(parent)
{
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        QMessageBox::critical(0, "Server Error", "Unable to start the server:"+ m_ptcpServer->errorString());
        m_ptcpServer->close();
        return;
    }
    //если прослушивание без ошибок, соединяем со слотом slotNewConnection()
    connect(m_ptcpServer, SIGNAL(newConnection()),this, SLOT(slotNewConnection()));

    connect(this,SIGNAL(fileRecieved()), this, SLOT(slotSaveFile()));
    m_ptxt = new QTextEdit;//для отображения информации
    m_ptxt->setReadOnly(true);

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;

    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(m_ptxt);

    setLayout(pvbxLayout);
}

/*virtual*/ void Server::slotNewConnection()//вызывается каждый раз для нового клиента
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();//подтверждение соединения с клиентом
    //nextPendingConnection() возвращает сокет, по которому идет связь с клиентом
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));//слот для отсоединения клиента
    connect(pClientSocket, SIGNAL(readyRead()),this, SLOT(slotReadClient()));//слот для чтения запросов клиента

    //sendToClient(pClientSocket, "Server Response: Connected!");
}

void Server::slotReadClient()
{
    QTcpSocket* pClientSocket = qobject_cast<QTcpSocket*>(sender()); //преобразование указателя,

    //возвращаемого методом sender(), к типу QTcpSocket
    //QByteArray byteArr = pClientSocket->read(1024);
    QByteArray byteArr = pClientSocket->readAll();//INFO>data>info>data

    QString dataString(byteArr);
    QString infoSplitted;

    if (dataString.contains("INFO:")){
        fileName ="";
        fileType ="";
        fileCheckSum ="";
        fileSize =0;
        dataVectorByteSize = 0;
        dataVector.clear();

        infoSplitted = dataString.split("ENDINFO").at(0);
        QByteArray data = dataString.split("ENDINFO").at(1).toLocal8Bit();
        parsingInfo(infoSplitted);

        if (!data.isEmpty()) {
            dataVectorByteSize = data.size();
            dataVector.push_back(data);
            m_ptxt->append(data);
        }
    }
    else if (dataString.contains("ILLUMINATE:")){
        illuminatingNetName = dataString.split("ILLUMINATEEND").at(0);
        illuminatingNetName.remove("ILLUMINATE:");
        emit illuminate(illuminatingNetName);
    } else {
        dataVectorByteSize += byteArr.size();
        dataVector.push_back(byteArr);
    }

    if(dataVectorByteSize == fileSize){
        dataVectorByteSize = 0;
        qDebug() << "file Reacieved";
        emit fileRecieved();
    }
//    QTcpSocket* pClientSocket = qobject_cast<QTcpSocket*>(sender());//преобразование указателя,
//    //возвращаемого методом sender(), к типу QTcpSocket
//    //QByteArray byteArr = pClientSocket->read(1024);
//    QByteArray byteArr = pClientSocket->readAll();//INFO>data>info>data
//    QString dataString(byteArr);
//    QString infoSplitted;
//    //QByteArray packetWithoutInfo;
//    qDebug() << "dataStringSize" << dataString.size();
//    if (dataString.contains("ILLUMINATE:")){
//        illuminatingNetName = dataString.split("ILLUMINATEEND").at(0);
//        illuminatingNetName.remove("ILLIMINATE:");
//        emit illuminate(illuminatingNetName);
//    }
//    //qDebug() << "str51";
//    if (dataString.contains("INFO:")){
//        fileName ="";
//        fileType ="";
//        fileCheckSum ="";
//        fileSize =0;
//        dataVectorByteSize = 0;
//        infoSplitted = dataString.split("ENDINFO").at(0);
//        byteArr =  dataString.split("ENDINFO").at(1).toUtf8();
//        dataVectorByteSize = byteArr.size();
//        parsingInfo(infoSplitted);
//        /*if (!dataVector.isEmpty())*/ dataVector.clear();
//    }
//    else dataVectorByteSize += byteArr.size();
//    qDebug() << "str51";

//    qDebug() << "recieved size: "<< dataVectorByteSize;
//    qDebug() << "file size: "<< fileSize;

//    if(dataVectorByteSize == fileSize){
//        dataVectorByteSize = 0;
//        dataVector.push_back(byteArr);
//        qDebug() << "file Recieved";
//        emit fileRecieved();
//    }
//    m_ptxt->append(byteArr);
//    dataVector.push_back(byteArr);
//    qDebug() << "str51";
}

//void Server::sendToClient(QTcpSocket* pSocket, const QString& str)//формируются данные для отправки клиенту
//{
//    QByteArray arrBlock;
//    QDataStream out(&arrBlock, QIODevice::WriteOnly);
//    out << quint16(0) << QTime::currentTime() << str;

//    out.device()->seek(0);
//    quint16 sizeofQuint16 = sizeof(quint16);
//    out << quint16(arrBlock.size()-sizeofQuint16);

//    pSocket->write(arrBlock);
//}

QString Server::calcCheckSum(QString filePath)
{
    QCryptographicHash crypto(QCryptographicHash::Md5);
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    while(!file.atEnd()){
      crypto.addData(file.read(1024));
    }
    QString result = crypto.result().toHex().data();
    file.close();
    return result;
}
void Server::parsingInfo(QString infoString){
    infoString.remove("INFO: ");
    infoString.remove("\n");
    fileName = infoString.split("_").at(0);
    fileSize = infoString.split("_").at(1).toInt();
    fileType = infoString.split("_").at(2);
    fileCheckSum = infoString.split("_").at(3);
    int red = infoString.split("_").at(4).toInt();
    int green = infoString.split("_").at(5).toInt();
    int blue = infoString.split("_").at(6).toInt();
    layerColor = QColor(red, green, blue);

}

void Server::slotSaveFile(){
    QString tmpDirPath = QCoreApplication::applicationDirPath()+"/tmp/";
    //qDebug() << tmpDirPath;
    QDir tmpDir;

    if(!tmpDir.exists(tmpDirPath)){
        QDir().mkdir(tmpDirPath);

    }
    QFile file(tmpDirPath+fileName);
    if (file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        for(int i = 0; i<dataVector.size(); i++){
            file.write(dataVector.at(i));
        }
    }
    QString newFileCheckSum = calcCheckSum(tmpDirPath+fileName);
    file.close();
    if (fileType=="netlist"){
        emit netlistRecieved(tmpDirPath+fileName);
    }
    else if (fileType=="layer"){
        emit layerRecieved(tmpDirPath+fileName, layerColor);
    }
}
