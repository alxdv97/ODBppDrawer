#include "odbppdrawer.h"

ODBppDrawer::ODBppDrawer(QWidget *parent) :
    QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing);
    scene = new QGraphicsScene();
    setScene(scene);
}

ODBppDrawer::~ODBppDrawer()
{

}

void ODBppDrawer::wheelEvent(QWheelEvent *event){
    int delta = event->delta();
    double scaleFactor = 1.0 + (double)delta/1000;
    scale(scaleFactor, scaleFactor);
    centerOn(mapToScene(event->pos()));
}

void ODBppDrawer::createLayer(QString filePath, QColor color){
    NewLayer *newLayer = new NewLayer(filePath, color);
    layersOrders.append(newLayer->getOrder());
    newLayerList << newLayer;
    QList<QGraphicsItem *> *layItml = new QList<QGraphicsItem *>;
    listOfLayerItemList << layItml;
    painter( newLayerList[iter], listOfLayerItemList[iter]);
    iter++;
    if (layItml->isEmpty()!=true)
        for (int i=0;i<layItml->size(); i++){
            scene->addItem(layItml->at(i));
        }
}

void ODBppDrawer::getNet(Netlist::Net *net){
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setBrush(Qt::red);
    for (int i=0;i<net->myNetPointList.size();i++){
        if (net->myNetPointList.at(i)->getRadius()==0.0){
            QGraphicsRectItem *rect = new QGraphicsRectItem(net->myNetPointList.at(i)->getX()-net->myNetPointList.at(i)->getW()/2,
                                                           net->myNetPointList.at(i)->getY()-net->myNetPointList.at(i)->getH()/2,
                                                           net->myNetPointList.at(i)->getW(),
                                                           net->myNetPointList.at(i)->getH());
            rect->setPen(pen);
            currentNet.append(rect);
        }
        else{
            QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(net->myNetPointList.at(i)->getX()-net->myNetPointList.at(i)->getRadius(),
                                                                     net->myNetPointList.at(i)->getY()-net->myNetPointList.at(i)->getRadius(),
                                                                     net->myNetPointList.at(i)->getRadius()*2,
                                                                     net->myNetPointList.at(i)->getRadius()*2);
            ellipse->setPen(pen);
            currentNet.append(ellipse);
        }  
    }
    illuminateNet(net);
}

void ODBppDrawer::illuminateNet(Netlist::Net *net){
    QPen pen;
    pen.setColor(Qt::red);
    if (currentNet.isEmpty()!=1){
    for (int i=0;i<net->myNetPointList.size();i++){

    if(net->myNetPointList.at(i)->getSide()=="T"){
        int minLayerNumber = minElement(layersOrders);
        doOnlyOneLayerVisible(necessaryLayer(minLayerNumber));
        scene->addItem(currentNet.at(i));
        findNet(currentNet.at(i));
        //currentNet.at(i)->setVisible(false);
        doAllLayerVisible();
    }
    else if(net->myNetPointList.at(i)->getSide()=="D"){
        int maxLayerNumber = maxElement(layersOrders);
        doOnlyOneLayerVisible(necessaryLayer(maxLayerNumber));
        scene->addItem(currentNet.at(i));
        findNet(currentNet.at(i));
        //currentNet.at(i)->setVisible(false);
        doAllLayerVisible();
    }
    else if(net->myNetPointList.at(i)->getSide()=="B"){
        int minLayerNumber = minElement(layersOrders);
        doOnlyOneLayerVisible(necessaryLayer(minLayerNumber));
        scene->addItem(currentNet.at(i));
        findNet(currentNet.at(i));
        doAllLayerVisible();
        int maxLayerNumber = maxElement(layersOrders);
        doOnlyOneLayerVisible(necessaryLayer(maxLayerNumber));
        findNet(currentNet.at(i));
        //currentNet.at(i)->setVisible(false);
        doAllLayerVisible();

    }
    else if(net->myNetPointList.at(i)->getSide()=="I"){
        int id = net->myNetPointList.at(i)->getID();
        doOnlyOneLayerVisible(id);
        scene->addItem(currentNet.at(i));
        findNet(currentNet.at(i));
        doAllLayerVisible();
    }
        if (currentNetList.isEmpty()!=1){

            for(int j =0;j<currentNetList.size(); j++){
                QGraphicsItem *itm = currentNetList.at(j);
                switch (itm->type())
                {
                    case QGraphicsLineItem::Type: {
                        //qDebug() << "Line";
                        QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem *>(itm);
                        line->setPen(pen);
                        break;
                    }
                    case QGraphicsRectItem::Type: {
                        //qDebug() << "Rect";
                        QGraphicsRectItem *rect = qgraphicsitem_cast<QGraphicsRectItem *>(itm);
                        rect->setPen(pen);
                        break;
                    }
                    case QGraphicsEllipseItem::Type: {
                        //qDebug() << "Ellipse";
                        QGraphicsEllipseItem *ellipse = qgraphicsitem_cast<QGraphicsEllipseItem *>(itm);
                        ellipse->setPen(pen);
                        break;
                    }
                }
            }
        }
        else {
            qDebug() << "point isn't colliding with lines";
        }

        }
    }
    else qDebug() << "no points avaliable";
    currentNet.clear();
    currentNetList.clear();
}

int ODBppDrawer::maxElement(QVector<int> vector){
    int max = 0;
    foreach ( int temp, vector) if( max < temp) max = temp;
    return max;
}
int ODBppDrawer::minElement(QVector<int> vector){
    int min = 100;
    foreach ( int temp, vector) if( min > temp) min = temp;
    return min;
}
int ODBppDrawer::necessaryLayer(int order){
    for(int i = 0;i<layersOrders.size();i++){
        if (layersOrders.at(i)==order)
            return i;
    }
    return -1;
}
void ODBppDrawer::doOnlyOneLayerVisible(int layerNumber){
    for (int i = 0; i<listOfLayerItemList.size(); i++){
        if (i!=layerNumber)
        for (int j = 0; j<listOfLayerItemList.at(i)->size(); j++){
            listOfLayerItemList.at(i)->at(j)->setVisible(false);
        }
    }
}

void ODBppDrawer::doAllLayerVisible(){
    for (int i = 0; i<listOfLayerItemList.size(); i++){
        for (int j = 0; j<listOfLayerItemList.at(i)->size(); j++){
            listOfLayerItemList.at(i)->at(j)->setVisible(true);
        }
    }
}

bool ODBppDrawer::isUnique(QGraphicsItem *itm){
    if (currentNetList.indexOf(itm)>-1)
        return false;
    else return true;
}
void ODBppDrawer::findNet(QGraphicsItem *itm){
    QList<QGraphicsItem*> itemList = itm->collidingItems();
    if (itemList.isEmpty()) return;

    for (int i=0;i<itemList.size(); i++)
    {
        if (isUnique(itemList.at(i))){
            currentNetList << itemList.at(i);
            findNet(itemList.at(i));
        }
    }
}

void ODBppDrawer::painter(/*Layer *layer*/NewLayer *newLayer, QList<QGraphicsItem *> *layerItemList){
    //QList<NewLayer::FSN *> FSNList = newLayer->getFSNList();
    QList<NewLayer::Line *> lineList = newLayer->getLineList();
    QList<NewLayer::Pad *> padList = newLayer->getPadList();
    QString units = newLayer->getUnits();
    QColor color = newLayer->getColor();

    foreach(NewLayer::Line *line, lineList){
        int id = line->getId();
        NewLayer::FSN *fsn = newLayer->getFSNviaID(id);
        drawLine(line, fsn, units, color, layerItemList);

    }
    foreach(NewLayer::Pad *pad, padList){
        int id = pad->getId();
        NewLayer::FSN *fsn = newLayer->getFSNviaID(id);
        drawPad(pad, fsn, units, color, layerItemList);
    }

//    QVector<double> diam;
//    for (int i=0;i<layer->get_size_Layer_features_L();i++){
//        diam << layer->get_diameters(2*layer->get_sym_num_L(i));
//        diam << layer->get_diameters(2*layer->get_sym_num_L(i)+1);
//        draw_l(layer->get_coordinate_list_L(i), layer->get_sym_num_L(i), diam, layer->get_layerColor(),layerItemList);
//        diam.clear();
//    }
//    diam.clear();
//    QString type;
//    int current_s_n;
//    for (int i=0;i<layer->get_size_Layer_features_P();i++){
//        current_s_n = layer->get_sym_num_P(i);
//        type = layer->get_typeOfSymNum(current_s_n);
//        diam << layer->get_diameters(2*layer->get_sym_num_P(i));
//        diam << layer->get_diameters(2*layer->get_sym_num_P(i)+1);
//        draw_p(layer->get_coordinate_list_P(i), layer->get_sym_num_P(i), type, diam, layerItemList);
//        diam.clear();
//    }
}

//------------------------------drawing--------------------

void ODBppDrawer::draw_l(QVector<double> coordinates, int sym_num, QVector<double> diameters, QColor color,
                         QList<QGraphicsItem *> *layerItemList)
{
    const double toMM = 25.4;
    const int scale = 10;
    const double pixel2mm = 0.264583;
    QPen pen;
    qreal lineWidth;
    lineWidth = diameters[1]*pixel2mm;
    pen.setWidthF(lineWidth);
    pen.setColor(color);
    pen.setCapStyle(Qt::RoundCap);
    QGraphicsLineItem *line;
    line = new QGraphicsLineItem(coordinates[0]*toMM*scale,coordinates[1]*toMM*scale,
            coordinates[2]*scale*toMM, coordinates[3]*scale*toMM);
    line->setPen(pen);
    layerItemList->append(line);

}

void ODBppDrawer::draw_p(QVector<double> coordinates, int sym_num, QString typeSymNum,
                         QVector<double> diameters,QList<QGraphicsItem *> *layerItemList)
{
    const double toMM = 25.4;
    const double scale = 10;
    const double toMMd = 0.0254;
    double toCenterX = (diameters[0]/2)*toMMd*scale;
    double toCenterY = (diameters[1]/2)*toMMd*scale;

    if (typeSymNum=="round")
    {    
        QGraphicsEllipseItem *ellipse;
        ellipse = new QGraphicsEllipseItem((coordinates[0]*toMM*scale)-toCenterX,(coordinates[1]*toMM*scale)-toCenterY,
                diameters[0]*toMMd*scale,diameters[1]*toMMd*scale);
        layerItemList->append(ellipse);
    }
    if (typeSymNum=="rect")
    {
        QGraphicsRectItem *rect;
        rect = new QGraphicsRectItem((coordinates[0]*toMM*scale)-toCenterX,(coordinates[1]*toMM*scale)-toCenterY,
                diameters[0]*toMMd*scale,diameters[1]*toMMd*scale);
        layerItemList->append(rect);
    }
    if (typeSymNum=="square")
    {
        QGraphicsRectItem *square;
        square = new QGraphicsRectItem((coordinates[0]*toMM*scale)-toCenterX,(coordinates[1]*toMM*scale)-toCenterY,
                diameters[0]*toMMd*scale,diameters[1]*toMMd*scale);
        layerItemList->append(square);

    }
}

void ODBppDrawer::drawPad(NewLayer::Pad *pad, NewLayer::FSN *fsn, QString units, QColor color, QList<QGraphicsItem *> *layerItemList){
    double scale;
    double toMM;
    double toMMd;
    if (units == "MM"){
        toMM = 1;
        scale = 10;
        toMMd = 0.001;
    }
    else {
        toMM = 25.4;
        scale = 10;
        toMMd = 0.0254;
        //if (units == "MM") toMMd = 0.0254;
        //else toMMd = 0.0254;
    }
   // double toMMd = 0.0254/*1*/;
    double toCenterX = (fsn->getSizeX()/2)*toMMd*scale;
    double toCenterY = (fsn->getSizeY()/2)*toMMd*scale;
    QString typeSymNum = fsn->getType();
    if (typeSymNum=="round")
    {
        QGraphicsEllipseItem *ellipse;
        ellipse = new QGraphicsEllipseItem((pad->getX()*scale*toMM)-toCenterX,((pad->getY()*scale*toMM))-toCenterY,
                fsn->getSizeX()*toMMd*scale,fsn->getSizeY()*toMMd*scale);
        layerItemList->append(ellipse);
    }
    if (typeSymNum=="rect")
    {
        QGraphicsRectItem *rect;
        rect = new QGraphicsRectItem((pad->getX()*scale*toMM)-toCenterX,((pad->getY()*scale*toMM))-toCenterY,
                fsn->getSizeX()*toMMd*scale,fsn->getSizeY()*toMMd*scale);
        layerItemList->append(rect);
    }
    if (typeSymNum=="square")
    {
        QGraphicsRectItem *square;
        square = new QGraphicsRectItem((pad->getX()*scale*toMM)-toCenterX,((pad->getY()*scale*toMM))-toCenterY,
                fsn->getSizeX()*toMMd*scale,fsn->getSizeY()*toMMd*scale);
        layerItemList->append(square);

    }
    if (typeSymNum=="oval")
    {
        QGraphicsEllipseItem *ellipse;
        ellipse = new QGraphicsEllipseItem((pad->getX()*scale*toMM)-toCenterX,((pad->getY()*scale*toMM))-toCenterY,
                fsn->getSizeX()*toMMd*scale,fsn->getSizeY()*toMMd*scale);
        layerItemList->append(ellipse);

    }

}
void ODBppDrawer::drawLine(NewLayer::Line *line, NewLayer::FSN *fsn, QString units, QColor color, QList<QGraphicsItem *> *layerItemList){
    double scale;
    double pixel2mm;
////    if (units == "MM") {
////        pixel2mm = 0.264583/100;
////        scale = /*10*/10;
////    }
////    else {
//    if (units == "MM") pixel2mm = 0.264583/10;
//    else pixel2mm = 0.264583;
//        scale = 254;
////    }
        if (units == "MM") {
            scale = 10;
            pixel2mm = 0.264583/50; //толщина линий
        }
        else {
            pixel2mm = 0.264583;
            scale = 254;
        }
    QPen pen;
    qreal lineWidth;
    lineWidth = fsn->getSizeX()*pixel2mm;
    pen.setWidthF(lineWidth);
    pen.setColor(color);
    pen.setCapStyle(Qt::RoundCap);

    QGraphicsLineItem *grLine;
    grLine = new QGraphicsLineItem(line->getXS()*scale, line->getYS()*scale, line->getXE()*scale,
                                   line->getYE()*scale);
    grLine->setPen(pen);
    layerItemList->append(grLine);
}
