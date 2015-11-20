#ifndef GRAPHS_H
#define GRAPHS_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QMap>
#include <QtMath>

#include <qcustomplot.h>

namespace Ui {
class graphs;
}

class graphs : public QMainWindow
{
    Q_OBJECT

public:
    explicit graphs(QWidget *parent = 0);
    ~graphs();

private slots:
    void on_pushButton_5_clicked();

private:
    Ui::graphs *ui;
};

#endif // GRAPHS_H
