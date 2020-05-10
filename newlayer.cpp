#include "newlayer.h"

NewLayer::NewLayer(QString filePath, QColor color)
{
    analysisFile(readFile(filePath));
    layerColor = color;
}

NewLayer::FSN::FSN(QString dataString, QString units){

    QStringList stringList = dataString.split(" ");
    QString IDstr = stringList.at(0);
    IDstr = IDstr.remove(0,1);             //remove $
    id = IDstr.toInt();
    QString data = stringList.at(1);
    data.remove("\r\n");
    data.remove("\n");
    if (data.contains("rect")){
        type = "rect";
        data = data.remove(0,4);           //remove rect
        QStringList size = data.split("x");
        sizeX = size.at(0).toDouble();
        sizeY = size.at(1).toDouble();
        return;
    } else if (data.contains("r")){
        type = "round";
        data = data.remove(0,1);           //remove r
        sizeX = data.toDouble();
        sizeY = data.toDouble();
        return;
    }
    if (data.contains("s")){
        type = "square";
        data = data.remove(0,1);           //remove s
        sizeX = data.toDouble();
        sizeY = data.toDouble();
        return;
    }
    if (data.contains("oval")){
        type = "oval";
        data = data.remove(0,4);           //remove oval
        QStringList size = data.split("x");
        sizeX = size.at(0).toDouble();
        sizeY = size.at(1).toDouble();
        return;
    }
}

NewLayer::Line::Line(QString dataString){
    QStringList data = dataString.split(" ");
    xS = data.at(1).toDouble();
    yS = data.at(2).toDouble();
    xE = data.at(3).toDouble();
    yE = data.at(4).toDouble();
    id = data.at(5).toInt();
}

NewLayer::Pad::Pad(QString dataString){
    QStringList data = dataString.split(" ");
    x = data.at(1).toDouble();
    y = data.at(2).toDouble();
    id = data.at(3).toInt();
}
NewLayer::~NewLayer(){

}

QStringList NewLayer::readFile(QString fileName)
{
    QFile file(fileName);

    QByteArray dataArray;
    QStringList dataList;
    if (!file.open(QIODevice::ReadOnly))
    {
        dataList.insert(0,"error");
        return dataList;
    }
    int i=0;
    while(!file.atEnd())
    {
        dataArray = file.readLine();
        QString data=dataArray.data();
        dataList.insert(i,data);
        i++;
    }
    return dataList;
}

void NewLayer::analysisFile(QStringList data)
{
    QChar op;

    QString currentStr;

    for (int i=0;i<data.size();i++)
    {
        currentStr = data.at(i);


        op = currentStr.at(0);

        if (i<10){                                                      //searching for order or ID
            if (currentStr.contains("ID=")){
                QString stringID = currentStr.split("=").at(1);
                stringID.remove("\n");
                order = stringID.toInt();
            } else if(currentStr.contains("Layer_Physical_Order")){
                QString stringID = currentStr.split("=").at(1);
                stringID.remove("\r\n");
                order = stringID.toInt();
            }
        }
        if (op == 'U') {
            QString unitStr = currentStr.split("=").at(1);
            unitStr = unitStr.remove(unitStr.length()-1,3);
            if (unitStr == "MM") UNITS = "MM";
        }
        if (op == '$') FSNList.append(new FSN(currentStr, UNITS));
        if (op == 'L') lineList.append(new Line(currentStr));
        if (op == 'P') padList.append(new Pad(currentStr));

    }
}
//-------Layer getters
QList<NewLayer::FSN *> NewLayer::getFSNList(){
    return FSNList;
}
QList<NewLayer::Line *> NewLayer::getLineList(){
    return lineList;
}
QList<NewLayer::Pad *> NewLayer::getPadList(){
    return padList;
}
QString NewLayer::getUnits(){
    return UNITS;
}
NewLayer::FSN* NewLayer::getFSNviaID(int necessaryId){
    foreach(NewLayer::FSN* fsn, FSNList){
        if (necessaryId == fsn->getId())
            return fsn;
    }
}
QColor NewLayer::getColor(){
    return layerColor;
}
int NewLayer::getOrder(){
    return order;
}
//-------FSN getters
int NewLayer::FSN::getId(){
    return id;
}
double NewLayer::FSN::getSizeX(){
    return sizeX;
}
double NewLayer::FSN::getSizeY(){
    return sizeY;
}
QString NewLayer::FSN::getType(){
    return type;
}
//-------Line getters
double NewLayer::Line::getXS(){
    return xS;
}
double NewLayer::Line::getYS(){
    return yS;
}
double NewLayer::Line::getXE(){
    return xE;
}
double NewLayer::Line::getYE(){
    return yE;
}
int NewLayer::Line::getId(){
    return id;
}
//-------Pad getters
double NewLayer::Pad::getX(){
    return x;
}
double NewLayer::Pad::getY(){
    return y;
}
int NewLayer::Pad::getId(){
    return id;
}

