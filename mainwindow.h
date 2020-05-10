#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QStringList>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QVector>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QTableWidgetItem>
#include "odbppdrawer.h"
#include "layer.h"
#include "mycheckbox.h"
#include "netlist.h"
#include "server.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void CheckBoxStateChanged(int state, int order);
    void CheckBoxStateChangedNetlist(int state, int order);
    void slotLayerProcessing(QString layerFileName, QColor color);
    void slotNetlistProcessing(QString netlistFileName);
    void slotIlluminateNet(QString netName);

private:
    void sendNet(Netlist::Net *net);
    void createNetlist(QString netlistName);
    void setTableNetlist(QString netlistName);
    void setNetToTable(QString netName);
    void setTable(QString layerName);

    QList<QCheckBox *> checkBoxList;
    QList<QCheckBox *> checkBoxListNetlist;
    Ui::MainWindow *ui;
    QString layer_name;
    QString netlist_name;
    QString filter;
    QList<QTableWidgetItem> itemList;
    int     order = 0;
    Netlist *netlist;
    Server *server;
};

#endif // MAINWINDOW_H
