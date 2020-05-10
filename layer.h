#ifndef LAYER_H
#define LAYER_H

#include <QStringList>
#include <QVector>
#include <QString>
#include <QByteArray>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
//#include "odbppdrawer.h"

class Layer
{
public:    
    Layer(QString filePath, QColor color);
    Layer();
    ~Layer();
    void setLayer(QString filePath, QColor color);
    // setters and getters
    void add_feature_symbol_names(QString str);
    void add_Layer_features_L(QString str);
    void add_Layer_features_P(QString str);
    void add_diameters(double diameter);
    void add_typeOfSymNum(QString str);
    void add_coordinate_list_L(QVector<double> coordinates);
    void add_coordinate_list_P(QVector<double> coordinates);
    void add_sym_num_L(int sym_num);
    void add_sym_num_P(int sym_num);
    void set_Layer_physical_order(int order);
    void set_Layer_color(long color); // unused
    void set_layerColor(QColor color);

    QVector<double> get_coordinate_list_L(int num);
    QVector<double> get_coordinate_list_P(int num);
    QString get_feature_symbol_names(int num);
    QString get_Layer_features_L(int num);
    QString get_Layer_features_P(int num);
    QString get_typeOfSymNum(int num);

    double get_diameters(int num);
    int get_sym_num_L(int num);
    int get_sym_num_P(int num);
    long get_Layer_color();
    QColor get_layerColor();
    int get_Layer_physical_order();

    int get_size_feature_symbol_names();
    int get_size_Layer_features_L();
    int get_size_Layer_features_P();



private:
    QStringList readFile(QString file_name);
    void analysisFile(QStringList data);

    void feature_symbol_names();
    void Layer_features_L_analysis();
    void Layer_features_P_analysis();

    int             Layer_physical_order;
    long            Layer_color; //unused
    QColor          layerColor;

    QStringList     Feature_symbol_name;
    QStringList     Layer_features_L;
    QStringList     Layer_features_P;

//important
    QVector<double>         diameters;
    QStringList             typeOfSymNum;
    QList<QVector<double>>  coordinate_list_L;
    QList<QVector<double>>  coordinate_list_P;
    QVector<int>            sym_num_L;
    QVector<int>            sym_num_P;
};

#endif // LAYER_H
