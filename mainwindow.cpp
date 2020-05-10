#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("ODBppDrawer");
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setColumnWidth(0,300);

    ui->tableWidget_2->setShowGrid(true);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->horizontalHeader()->hide();
    ui->tableWidget_2->setColumnCount(1);
    ui->tableWidget_2->setColumnWidth(0,300);

    server = new Server(10000);
    server->show();
    connect(server, SIGNAL(layerRecieved(QString, QColor)), this, SLOT(slotLayerProcessing(QString, QColor)));
    connect(server, SIGNAL(netlistRecieved(QString)), this, SLOT(slotNetlistProcessing(QString)));
    connect(server, SIGNAL(illuminate(QString)), this, SLOT(slotIlluminateNet(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CheckBoxStateChanged(int state, int order){

    if (state == Qt::Checked) {
        for(int j=0;j<ui->drawer->listOfLayerItemList.at(order)->size();j++)
            ui->drawer->listOfLayerItemList.at(order)->at(j)->setVisible(true);
    }
    else {
        for(int j=0;j<ui->drawer->listOfLayerItemList.at(order)->size();j++)
            ui->drawer->listOfLayerItemList.at(order)->at(j)->setVisible(false);
    }
}
void MainWindow::CheckBoxStateChangedNetlist(int state, int order){

    if (state == Qt::Checked) {

    }
    else {
        Netlist::Net* net =  MainWindow::netlist->searchNetViaName(netlist->myNetList.at(order)->getNetName());
        sendNet(net);
    }
}

void MainWindow::setTable(QString layerName){
    MainWindow::ui->tableWidget->insertRow(order);
    myCheckBox *checkbox = new myCheckBox(order);
    checkbox->QCheckBox::setCheckState(Qt::Checked);
    checkbox->QCheckBox::setText(layerName);
    checkBoxList << checkbox;
    ui->tableWidget->setCellWidget(order,0,checkbox);
}
void MainWindow::setTableNetlist(QString netlistName){
//    for (int i = 0; i<netlist->myNetList.size();i++)
//    {
//    MainWindow::ui->tableWidget_2->insertRow(i);
//    myCheckBox *checkbox = new myCheckBox(i);
//    checkbox->QCheckBox::setCheckState(Qt::Checked);
//    checkbox->QCheckBox::setText(netlist->myNetList.at(i)->getNetName());
//    checkBoxListNetlist << checkbox;
//    ui->tableWidget_2->setCellWidget(i,0,checkbox);
//    connect(checkBoxListNetlist.at(i), SIGNAL(stateChanged(int, int)), this, SLOT(CheckBoxStateChangedNetlist(int, int)));
//    }
}
void MainWindow::setNetToTable(QString netName){
    int i = MainWindow::ui->tableWidget_2->rowCount();
    QTableWidgetItem *itm = new QTableWidgetItem(netName);
    MainWindow::ui->tableWidget_2->insertRow(i);
    MainWindow::ui->tableWidget_2->setItem(i,0,itm);
    qDebug() << "end";
    //MainWindow::ui->tableWidget_2->item(MainWindow::ui->tableWidget_2->rowCount()-1,1)->setText(netName);
}

void MainWindow::sendNet(Netlist::Net *net){
    ui->drawer->getNet(net);
}

void MainWindow::slotLayerProcessing(QString layerFileName, QColor color){
    QString fileName = layerFileName.split("/").at(layerFileName.split("/").size() -1);
    ui->drawer->createLayer(layerFileName, color);
    setTable(fileName);
    connect(checkBoxList[order], SIGNAL(stateChanged(int, int)), this, SLOT(CheckBoxStateChanged(int, int)));
    order++;
}
void MainWindow::slotNetlistProcessing(QString netlistFileName){
    netlist = new Netlist(netlistFileName);
    //setTableNetlist(netlistFileName);
}

void MainWindow::slotIlluminateNet(QString netName){
    Netlist::Net* net =  MainWindow::netlist->searchNetViaName(netName);
    sendNet(net);
    setNetToTable(netName);
}
