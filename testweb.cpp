#include "testweb.h"
#include "ui_testweb.h"
#include <QWebView>

Testweb::Testweb(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::testweb)
{
    ui->setupUi(this);
    ui->webView->load(QUrl("qrc:/help.html"));
}

Testweb::~Testweb()
{
    delete ui;
}

void Testweb::on_pushButton_clicked()
{
    //ui->webView->setUrl(QUrl("http://google.com/"));
}

void Testweb::on_pushButton_2_clicked()
{
    //ui->webView->load(QUrl("qrc:/help.html"));
}

































