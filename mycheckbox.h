#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H
#include <QCheckBox>
#include <QTableWidgetItem>

class myCheckBox : public QCheckBox
{
    Q_OBJECT
    public:
        myCheckBox(QWidget *parent =0);
        myCheckBox(int ord);
        ~myCheckBox();

        int order;
    private slots:
        // Создаём новый сигнал
        void doEmitStateChanged(int i);

    signals:
        void stateChanged(int, int);
};

#endif // MYCHECKBOX_H
