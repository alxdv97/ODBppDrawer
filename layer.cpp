#include "layer.h"

Layer::Layer()
{

}
Layer::Layer(QString filePath, QColor color){
    setLayer(filePath, color);
}
void Layer:: setLayer(QString filePath, QColor color){
    analysisFile(readFile(filePath));
    feature_symbol_names();
    Layer_features_L_analysis();
    Layer_features_P_analysis();
    set_layerColor(color);
}
QStringList Layer::readFile(QString file_name)
{
    QFile file(file_name);

    QByteArray data_array;
    QStringList data_list;
    if (!file.open(QIODevice::ReadOnly))
    {
        data_list.insert(0,"error");
        return data_list;
    }
    int i=0;
    while(!file.atEnd())
    {
        data_array = file.readLine();
        QString data=data_array.data();
        data_list.insert(i,data);
        i++;
    }
    return data_list;
}

Layer::~Layer(){

}
void Layer::analysisFile(QStringList data)
{
    QChar op;                           //current operator: $ - Feature_symbol_name, @ - Feature_attribute_name, & - Feature_attribute_text
                                        // L - Layer_features for line, P -  Layer_features for pad
    QString current_str;                // current string

    //set_Layer_physical_order (data.at(0).right(3).toInt());
    //set_Layer_color(data.at(1).right(10).toLong());

    for (int i=0;i<data.size();i++)
    {
        current_str = data.at(i);
        op = current_str.at(0);
        if (op == '$') add_feature_symbol_names(current_str);
        if (op == 'L') add_Layer_features_L(current_str);
        if (op == 'P') add_Layer_features_P(current_str);

    }
}



void Layer::Layer_features_L_analysis()
{
    QString current_string;
    QStringList current_string_splitted;
    int polarity_symbol_index;
    QVector<double> coordinates;
    int symbol_num;
    for (int i=0;i<Layer_features_L.size();++i)
    {
        Layer_features_L[i].remove(0,2);
        polarity_symbol_index = Layer_features_L.at(i).indexOf('P');
        Layer_features_L[i].remove(polarity_symbol_index-1,10);
        current_string = Layer_features_L[i];
        current_string_splitted=current_string.split(" ");
        for (int j=0;j<4;j++)
        {
            coordinates << current_string_splitted[j].toDouble();
        }
        add_coordinate_list_L(coordinates);
        symbol_num=current_string_splitted[4].toInt();
        add_sym_num_L(symbol_num);
        coordinates.clear();
    }
}

void Layer::Layer_features_P_analysis()
{
    QString current_string;
    QStringList current_string_splitted;
    int polarity_symbol_index;
    QVector<double> coordinates;
    int symbol_num;

    for (int i=0;i<Layer_features_P.size();++i)
    {
       Layer_features_P[i].remove(0,2);
       polarity_symbol_index = Layer_features_P.at(i).indexOf('P'); //ALL POLARITY IS P(POSITIVE)
       Layer_features_P[i].remove(polarity_symbol_index-1,30);
       current_string = Layer_features_P[i];
       current_string_splitted=current_string.split(" ");
       for (int j=0;j<2;j++)
       {
           coordinates << current_string_splitted[j].toDouble();
       }
       add_coordinate_list_P(coordinates);
       symbol_num=current_string_splitted[2].toInt();
       add_sym_num_P(symbol_num);
       coordinates.clear();
    }
}
void Layer::feature_symbol_names()
{
    int space_symbol_index;
    QString current_str;
    int real_string_ending_index; //to delete /n/r symbols
    for (int i=0;i<Feature_symbol_name.size();++i)
    {
        space_symbol_index = Feature_symbol_name.at(i).indexOf(" ");
        Feature_symbol_name[i].remove(0,space_symbol_index+1);
        real_string_ending_index = Feature_symbol_name[i].length()-2;
        Feature_symbol_name[i].remove(real_string_ending_index,3);
        current_str = Feature_symbol_name[i];
        if ((current_str[0] == 'r')&&((current_str[1]<58)&&(current_str[1]>47))) // r - round, after - diameter<double>
        {
            add_typeOfSymNum("round");
            current_str.remove(0,1);
            add_diameters(current_str.toDouble());
            add_diameters(current_str.toDouble());
        }

        if ((current_str[0] == 'r')&&(current_str[1] == 'e')) // rect - rectangle, after - WxH<double>
        {
            add_typeOfSymNum("rect");
            current_str.remove(0,4);
            QStringList current_str_splitted = current_str.split("x");
            add_diameters(current_str_splitted[0].toDouble());
            add_diameters(current_str_splitted[1].toDouble());
        }
        if ((current_str[0] == 's')&&((current_str[1]<58)&&(current_str[1]>47))) // s - sqare, after - width<double>
        {
            add_typeOfSymNum("square");
            current_str.remove(0,1);
            add_diameters(current_str.toDouble());
            add_diameters(current_str.toDouble());
        }
    }
}


//setters and getters

void Layer::add_feature_symbol_names(QString str){
    Feature_symbol_name << str;
}
void Layer::add_Layer_features_L(QString str){
    Layer_features_L << str;
}
void Layer::add_Layer_features_P(QString str){
    Layer_features_P << str;
}
void Layer::add_diameters(double diameter){
    diameters << diameter;
}
void Layer::add_typeOfSymNum(QString str){
    typeOfSymNum << str;
}
void Layer::set_Layer_physical_order(int order){
    Layer_physical_order = order;
}
void Layer::set_Layer_color(long color){
    Layer_color = color;
}
void Layer::set_layerColor(QColor color){
    layerColor = color;
}
void Layer::add_coordinate_list_L(QVector<double> coordinates){
    coordinate_list_L << coordinates;
}
void Layer::add_coordinate_list_P(QVector<double> coordinates){
    coordinate_list_P << coordinates;
}
void Layer::add_sym_num_L(int sym_num){
    sym_num_L << sym_num;
}
void Layer::add_sym_num_P(int sym_num){
    sym_num_P << sym_num;
}

QString Layer::get_feature_symbol_names(int num){
    return Feature_symbol_name[num];
}
QString Layer::get_Layer_features_L(int num){
    return Layer_features_L[num];
}
QString Layer::get_Layer_features_P(int num){
    return Layer_features_P[num];
}
double Layer::get_diameters(int num){
    return diameters[num];
}
QString Layer::get_typeOfSymNum(int num){
    return typeOfSymNum[num];
}
int Layer::get_Layer_physical_order(){
    return Layer_physical_order;
}
long Layer::get_Layer_color(){
    return Layer_color;
}
QColor Layer::get_layerColor(){
    return layerColor;
}

int Layer::get_size_feature_symbol_names(){
    return Feature_symbol_name.size();
}
int Layer::get_size_Layer_features_L(){
    return Layer_features_L.size();
}
int Layer::get_size_Layer_features_P(){
    return Layer_features_P.size();
}
QVector<double> Layer::get_coordinate_list_L(int num){
    return coordinate_list_L[num];
}
QVector<double> Layer::get_coordinate_list_P(int num){
    return coordinate_list_P[num];
}
int Layer::get_sym_num_L(int num){
    return sym_num_L[num];
}
int Layer::get_sym_num_P(int num){
    return sym_num_P[num];
}
