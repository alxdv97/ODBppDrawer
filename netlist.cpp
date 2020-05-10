#include "netlist.h"

Netlist::Netlist(){

}
Netlist::Netlist(QString filePath){
    analysisFile(readFile(filePath));
}

QStringList Netlist::readFile(QString filePath){
    QFile file(filePath);

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

void Netlist::analysisFile(QStringList data){
    QChar firstSymbol;
    QString currentStr;
    QStringList currentStrSplitted;

    QString U;
    QChar op = data.at(0).at(0);
    if (op == 'U') {
        QString unitStr = data.at(0).split("=").at(1);
        unitStr.remove("\r\n");
        unitStr.remove("\n");
        //unitStr = unitStr.remove(unitStr.length()-1,3);
        if (unitStr == "MM") U = "MM";
        else U = "INCH";
    }

    int id;
    for (int i=1;i<data.size();++i)
    {
        currentStr = data.at(i);
        firstSymbol = currentStr.at(0);
        if (firstSymbol == '$') {
            currentStr.remove(0,1);
            currentStrSplitted = currentStr.split(" ");
            createNet(currentStrSplitted.at(0).toInt(), currentStrSplitted.at(1));
        }
        else if ((firstSymbol<58)&&(firstSymbol>47))
        {
            currentStrSplitted = currentStr.split(" ");
            if (currentStrSplitted.at(4)=="I")
            {
                id = currentStrSplitted.last().toInt();
            }
            else id=-1;
            //if ((std::abs(currentStrSplitted.at(1).toDouble() - 0) <std::numeric_limits<double>::epsilon()))
            if (currentStrSplitted.at(1).toDouble()!=0)
                myNetList.at(currentStrSplitted.at(0).toInt())->createNetPoint(currentStrSplitted.at(1).toDouble(),
                                                                           currentStrSplitted.at(2).toDouble(),
                                                                           currentStrSplitted.at(3).toDouble(),
                                                                           currentStrSplitted.at(4),
                                                                           currentStrSplitted.at(5), U, id);

            else
                myNetList.at(currentStrSplitted.at(0).toInt())->createNetPointSquare(currentStrSplitted.at(2).toDouble(),
                                                                                currentStrSplitted.at(3).toDouble(),
                                                                                currentStrSplitted.at(4),
                                                                                currentStrSplitted.at(5).toDouble(),
                                                                                currentStrSplitted.at(6).toDouble(),
                                                                                currentStrSplitted.at(7), U, id);
        }
    }
}

Netlist::~Netlist(){

}

Netlist::Net::Net(int netNum, QString netName){
    setNetNum(netNum);
    setNetName(netName);
}
void Netlist::createNet(int netNum, QString netName){
    Net *net = new Net(netNum, netName);
    myNetList << net;
}

QString Netlist::searchPoint(double x, double y){  //return point name with coordinates x,y
    if (myNetList.size()!=0){
        for (int i=0; i<myNetList.size(); i++){
            if (myNetList.at(i)->searchPointNet(x,y)==true)
              return myNetList.at(i)->getNetName();
            else return "Pad is not found";
        }
    }
    else
        return "No nets found";
}

bool Netlist::Net::searchPointNet(double x, double y){
    if (myNetPointList.size()!=0){
        for (int i=0; i<myNetPointList.size(); i++){
            if((std::abs(myNetPointList.at(i)->getX() - x) <
                    std::numeric_limits<double>::epsilon())&&(std::abs(myNetPointList.at(i)->getY()
                    - y) < std::numeric_limits<double>::epsilon()))
                return true;
            else
                return false;
        }
    }
    else
        return false;
}

void Netlist::Net::createNetPoint(double rad, double xC, double yC, QString s, QString eP, QString units, int id){
    NetPoint *netpoint = new NetPoint(rad, xC, yC, s, 0, 0, eP, units, id);
    myNetPointList << netpoint;
}
void Netlist::Net::createNetPointSquare(double xC, double yC, QString s, double wS,
                                         double hS, QString eP, QString units, int id)
{
    NetPoint *netpoint = new NetPoint(0, xC, yC, s, wS, hS, eP, units, id);
    myNetPointList << netpoint;
}

Netlist::Net* Netlist::searchNetViaName(QString netName){
    for (int i=0; i<myNetList.size();i++){
        if (myNetList.at(i)->getNetName()==netName){
            return myNetList.at(i);
        }
    }
}

Netlist::Net::NetPoint::NetPoint(double rad, double xC, double yC, QString s,
                                 double wS, double hS, QString eP, QString units, int id){
    setRadius(rad, units);
    setX(xC, units);
    setY(yC, units);
    setSide(s);
    setW(wS, units);
    setH(hS, units);
    setEpoint(eP);
    setID(id);
}
void  Netlist::Net::setNetNum(int num){
    netNum = num;
}
void  Netlist::Net::setNetName(QString name){
    netName = name.remove(name.size()-2,2);
}
int  Netlist::Net::getNetNum(){
    return netNum;
}
QString  Netlist::Net::getNetName(){
    return netName;
}


void Netlist::Net::NetPoint::setRadius(double r, QString units){
//    radius = r*25.4*10;
    if (units == "MM")
    radius = r*10;
    else radius = r*25.4*10;
}
void Netlist::Net::NetPoint::setX(double x, QString units){
    //xCoordinate = x*25.4*10;
    if (units == "MM")
    xCoordinate = x*10;
    else xCoordinate = x*25.4*10;
}
void Netlist::Net::NetPoint::setY(double y, QString units){
//    yCoordinate = y*25.4*10;
    if (units == "MM")
    yCoordinate = y*10;
    else yCoordinate = y*25.4*10;
}
void Netlist::Net::NetPoint::setSide(QString s){
    side = s;
}
void Netlist::Net::NetPoint::setW(double w, QString units){
//    wSize = w*25.4*10;
    if (units == "MM")
    wSize = w*10;
    else wSize = w*25.4*10;
}
void Netlist::Net::NetPoint::setH(double h, QString units){
//   hSize = h*25.4*10;
    if (units == "MM")
    hSize = h*10;
    else hSize = h*25.4*10;
}
void Netlist::Net::NetPoint::setEpoint(QString e){
    epoint = e;
}
void Netlist::Net::NetPoint::setID(int id){
    ID =id;
}
double Netlist::Net::NetPoint::getRadius(){
    return radius;
}
double Netlist::Net::NetPoint::getX(){
    return xCoordinate;
}
double Netlist::Net::NetPoint::getY(){
    return yCoordinate;
}
QString Netlist::Net::NetPoint::getSide(){
    return side;
}
double Netlist::Net::NetPoint::getW(){
    return wSize;
}
double Netlist::Net::NetPoint::getH(){
    return hSize;
}
QString Netlist::Net::NetPoint::getEpoint(){
    return epoint;
}
int Netlist::Net::NetPoint::getID(){
    return ID;
}
