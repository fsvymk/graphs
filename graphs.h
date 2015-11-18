#ifndef GRAPHS_H
#define GRAPHS_H

#include <QMainWindow>

namespace Ui {
class graphs;
}

class graphs : public QMainWindow
{
    Q_OBJECT

public:
    explicit graphs(QWidget *parent = 0);
    ~graphs();

private:
    Ui::graphs *ui;
};

#endif // GRAPHS_H
