#ifndef NEWLAYER_H
#define NEWLAYER_H
#include <QString>
#include <QColor>
#include <QFile>
class NewLayer
{
public:
    NewLayer(QString filePath, QColor color);
    ~NewLayer();
    class FSN{
    public:
        FSN(QString dataString, QString units);
        int getId();
        double getSizeX();
        double getSizeY();
        QString getType();
    private:
        QString type;
        double sizeX, sizeY;
        int    id;
    };

    class Line{
    public:
        Line(QString dataString);
        double getXS();
        double getYS();
        double getXE();
        double getYE();
        int    getId();
    private:
        double xS, yS, xE, yE;
        int id;
    };

    class Pad{
    public:
        Pad(QString dataString);
        double getX();
        double getY();
        int    getId();
    private:
        double x, y;
        int id;
    };


public:
    QList<FSN *> getFSNList();
    QList<Line *> getLineList();
    QList<Pad *> getPadList();
    QString getUnits();
    FSN* getFSNviaID(int necessaryId);
    QColor getColor();
    int getOrder();

private:
    QList<FSN *> FSNList;
    QList<Line *> lineList;
    QList<Pad *> padList;

    QString UNITS = "INCH";
    QColor layerColor;
    int     order;

private:
    QStringList readFile(QString fileName);
    void analysisFile(QStringList data);

};

#endif // NEWLAYER_H
