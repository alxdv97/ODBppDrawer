#ifndef ODBPPDRAWER_H
#define ODBPPDRAWER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QStringList>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QVector>
#include <QPen>
#include <QWheelEvent>
#include <QMap>
#include <QGraphicsItemGroup>
#include "layer.h"
#include "netlist.h"
#include <QGraphicsEffect>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsObject>
#include "newlayer.h"

class ODBppDrawer : public QGraphicsView
{
public:
    ODBppDrawer(QWidget *parent = nullptr);
    ~ODBppDrawer();
    void createLayer(QString filePath, QColor color); //create layer inside odbppdrawer
    //QList<QGraphicsItemGroup *> graphicsLayerList;
    QList<QList<QGraphicsItem *>*> listOfLayerItemList;
    QList<QList<QGraphicsItem *>> listOfLayItmL;
    void getNet(Netlist::Net *net);
    void illuminateNet(Netlist::Net *net);
    int order = 0;
    QList<QGraphicsItem *> currentNetList; // for colliding
    QList<QAbstractGraphicsShapeItem *> shapeList;
    QList<QGraphicsItem *> currentNet;     // for drawing
private:
    QGraphicsScene *scene;
    QVector<int> layersOrders;
    int             Layer_physical_order;
    long            Layer_color;
    QVector<double> diameters;
    QStringList     typeOfSymNum;

    QStringList     Feature_symbol_name;
    QStringList     Layer_features_L;
    QStringList     Layer_features_P;

    //QList<Layer *> layerList;
    QList<NewLayer *> newLayerList;
    int iter=0;
    //for drawing
    void doOnlyOneLayerVisible(int layerNumber);
    void doAllLayerVisible();
    int maxElement(QVector<int> vector);
    int minElement(QVector<int> vector);
    int necessaryLayer(int order);
    void findNet(QGraphicsItem *itm);
    void draw_l(QVector<double> coordinates, int sym_num, QVector<double> diameters, QColor color, QList<QGraphicsItem *> *layerItemList
                /*QGraphicsItemGroup *graphicsLayer*/);
    void draw_p(QVector<double> coordinates, int sym_num, QString typeSymNum, QVector<double> diameters, QList<QGraphicsItem *> *layerItemList
                /*QGraphicsItemGroup *graphicsLayer*/);
    void drawLine(NewLayer::Line *line, NewLayer::FSN *fsn, QString units, QColor color, QList<QGraphicsItem *> *layerItemList);
    void drawPad(NewLayer::Pad *pad, NewLayer::FSN *fsn, QString units, QColor color, QList<QGraphicsItem *> *layerItemList);
    void painter(/*Layer *layer*/ NewLayer *layer, QList<QGraphicsItem *> *layerItemList);
    bool isUnique(QGraphicsItem *itm);
    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event);

};

#endif // ODBPPDRAWER_H
