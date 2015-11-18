#ifndef TESTWEB_H
#define TESTWEB_H

#include <QMainWindow>
#include <QWebView>

namespace Ui {
class testweb;
}

class Testweb : public QMainWindow
{
    Q_OBJECT

public:
    explicit Testweb(QWidget *parent = 0);
    ~Testweb();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::testweb *ui;
};

#endif // TESTWEB_H
