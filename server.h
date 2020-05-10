#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QWidget>
#include <QTcpServer>
#include <QDebug>
#include <QByteArray>
#include <QTcpSocket>
#include <QTextEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QCryptographicHash>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
class Server : public QWidget
{
    Q_OBJECT
public:
    explicit Server(int nPort, QWidget *parent = nullptr);

private:
    QTcpServer* m_ptcpServer;  //основа управления сервером
    QTextEdit* m_ptxt;         //многострочное тектовое поле, для отобрадения событий
    quint16 m_nNextBlockSize;  //длина следующего блока полученного от сокета
    QVector<QByteArray> dataVector;
    QString fileName, fileType, fileCheckSum;
    int fileSize;
    int dataVectorByteSize = 0;
    QString illuminatingNetName;
    QColor layerColor;
private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);
    QString calcCheckSum(QString filePath);
    void parsingInfo(QString infoString);
public slots:
    virtual void slotNewConnection();
    void slotReadClient ();
    void slotSaveFile();
signals:
    void fileRecieved();
    void layerRecieved(QString layerFileName, QColor color);
    void netlistRecieved(QString netlistFileName);
    void illuminate(QString netName)
;};

#endif // SERVER_H
