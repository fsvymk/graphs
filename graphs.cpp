#include "graphs.h"
#include "ui_graphs.h"
#include <QUdpSocket>
#include <QtMath>
#include <QMap>

#include <qcustomplot.h>

graphs::graphs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::graphs)
{
    ui->setupUi(this);
}

graphs::~graphs()
{
    delete ui;
}

void graphs::on_pushButton_5_clicked()
{
    ui->tabWidget->addTab(); // there are error
}
