#include "mycheckbox.h"

myCheckBox::myCheckBox(QWidget *parent) : QCheckBox(parent){
// Connect original signal to slot
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(doEmitStateChanged(int)));
    }

myCheckBox::~myCheckBox(){

    }
myCheckBox::myCheckBox(int ord){
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(doEmitStateChanged(int)));
    order = ord;
}
void myCheckBox::doEmitStateChanged(int i) {
    emit stateChanged(i, order);
    }
