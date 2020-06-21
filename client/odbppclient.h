#ifndef ODBPPCLIENT_H
#define ODBPPCLIENT_H

#include <QObject>
#include <QCryptographicHash>
#include <QFile>
#include <QColor>
#include <QTcpSocket>

class ODBppClient : public QObject
{
    Q_OBJECT
public:
    explicit ODBppClient(const QString& strHost, int nPort);
    ~ODBppClient();
public:

    void sendLayer(QString filePath, int colorRed, int colorGreen, int colorBlue);
    void sendNetlist(QString filePath);
    void illuminateNet(QString netName);
private:
    QString host;
    int port;
    QTcpSocket* m_pTcpSocket;//атрибут для управления клинетом
    //QByteArray byteArrayFromFile;//считанный из файла массив байт\

    //для предпакета
    QString fileCheckSum;//контрольная сумма файла
    QString fileName;    //имя файла
    QString fileSize;    //размер файла
    QString fileType;    //тип файла: netlist/layer
    int red, green, blue;
    //
    QVector<QByteArray> byteVector;

private:
    void readFile(QString filePath);
    void sendNetToServer(QString netName);
    QString calcCheckSum(QString filePath);
private slots:
    void slotSendToServer();
    //void slotConnected ();


signals:

};

#endif // ODBPPCLIENT_H
