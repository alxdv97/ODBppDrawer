#include "odbppclient.h"

ODBppClient::ODBppClient(const QString &strHost, int nPort)
{
    m_pTcpSocket = new QTcpSocket(this);//создается сокет
    host = strHost;
    port = nPort;
}

ODBppClient::~ODBppClient()
{

}
//метод, читающий файл и преобразующий его в массив байт
void ODBppClient::readFile(QString filePath){
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)){
        return;
    }
    fileSize = QString::number(file.size());
    fileCheckSum = calcCheckSum(filePath);
    fileName = filePath.split("\\").at(filePath.split("\\").size()-2);
    QByteArray dataArray;
    while(!file.atEnd()){
        dataArray = file.read(1024);
        byteVector.push_back(dataArray);
    }
    file.close();
}

QString ODBppClient::calcCheckSum(QString filePath)
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

void ODBppClient::slotSendToServer()
{
    //формирование и отправка предпакета
    m_pTcpSocket->connectToHost(host, port);
    QString prepacket = "INFO: "+fileName +"_"+ fileSize
            + "_"+ fileType+ "_" +fileCheckSum
            + "_"+ QString::number(red)+ "_" +QString::number(green)+"_" +QString::number(blue)+"ENDINFO";

    m_pTcpSocket->write(prepacket.toUtf8());
    foreach(QByteArray byteArr, byteVector){
        m_pTcpSocket->write(byteArr);
        while(!m_pTcpSocket->waitForBytesWritten());
    }
    byteVector.clear();
    m_pTcpSocket->disconnectFromHost();
    m_pTcpSocket->waitForDisconnected();
}

void ODBppClient::sendLayer(QString filePath, int colorRed, int colorGreen, int colorBlue){
    readFile(filePath);
    fileType = "layer";
    red = colorRed;
    green = colorGreen;
    blue = colorBlue;
    slotSendToServer();
}
void ODBppClient::sendNetlist(QString filePath){
    readFile(filePath);
    fileType = "netlist";
    red = 0;
    green = 0;
    blue = 0;
    slotSendToServer();
}
void ODBppClient::illuminateNet(QString netName){
    sendNetToServer(netName);
}
void ODBppClient::sendNetToServer(QString netName){
    m_pTcpSocket->connectToHost(host, port);
    QString package = "ILLUMINATE:"+netName+"ILLUMINATEEND";
    m_pTcpSocket->write(package.toLocal8Bit());
    m_pTcpSocket->disconnectFromHost();
    m_pTcpSocket->waitForDisconnected();

}
