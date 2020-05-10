#ifndef NETLIST_H
#define NETLIST_H
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QDebug>
class Netlist
{
public: class Net
    {
        class NetPoint
        {
        public:
            NetPoint(double rad, double xC, double yC, QString s,double wS, double hS,
                     QString eP, QString units, int id);
            void    setRadius(double r, QString units);
            void    setX(double x, QString units);
            void    setY(double y, QString units);
            void    setSide(QString s);
            void    setW(double w, QString units);
            void    setH(double h, QString units);
            void    setEpoint(QString e);
            void    setID(int id);
            double  getRadius();
            double  getX();
            double  getY();
            QString getSide();
            double  getW();
            double  getH();
            QString getEpoint();
            int     getID();

        private:
           double   radius;
           double   xCoordinate;
           double   yCoordinate;
           QString  side;
           double   wSize;
           double   hSize;
           QString  epoint;
           int      ID;
        };
    public:
        Net(int netNum, QString netName);
        void    createNetPointSquare(double xC, double yC, QString s,
                            double wS, double hS, QString eP, QString units,int id=-1);
        void    createNetPoint(double rad, double xC, double yC, QString s, QString eP, QString units, int id=-1);
        void    setNetNum(int num);
        void    setNetName(QString name);
        int     getNetNum();
        QString getNetName();
        bool searchPointNet(double x, double y);
        QList<NetPoint *> myNetPointList;
    private:
        int               netNum;
        QString           netName;

    };
public:
    Netlist();
    Netlist(QString filePath);
    ~Netlist();
    void createNet(int netNum, QString netName);
    QString searchPoint(double x, double y);
    //Net searchNetViaName(QString netName);
    Net* searchNetViaName(QString netName);
    QList<Net *> myNetList;
    static QString UNITS;
private:
    QStringList readFile(QString filePath);
    void        analysisFile(QStringList data);

};

#endif // NETLIST_H
