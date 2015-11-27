#ifndef GRAPHS_H
#define GRAPHS_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QMap>
#include <QtMath>

#include <entry.h>
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
    // event listeners:
    void on_pushButton_5_clicked();

    // use cases:
    void init();
    void remove(QLayout* layout);
    void deleteSome();
    QString ss(long x);
    void t(QString str);
    void p(QString str);

    // graphs:
    void entryMassDeserialize(QByteArray* source,
                              QHash<unsigned, struct entry>* DB);
    void readUdpDatagrams();
    void output_vectors(QVector<double> *x, QVector<double> *y);
    void output_db(QHash<unsigned, struct entry> &db);
    void numbers(QHash<unsigned, struct entry> &db);
    void delete_graphs(QVBoxLayout* plotsLayout);
    void clear_plots(QCustomPlot* pl);
    void bindUdpPort();

    void on_main_button_server_start_clicked();

    //four.h

    QByteArray makeNewDatagram();

    void useCase_parser_show();
    void useCase_mainwindow();
    void useCase_help();
    void setNonEditMode( QWidget& Widget , bool bNonEditMode );
    void processUdpDatagram(const QByteArray &data);
    void t(const QString &message);
    void oprosb();
    void readUdpDatagrams_by_PM();
    void useCase_settings();
    void useCase_window_testweb();
    void useCase_window_mainwindow();
    void useCase_window_parser();
    void fsetAddress(const QHostAddress &addr);
    void fsetPort(const quint16 &prt);
    void useCase_send_ffff();
    void useCase_send_0000();
    void useCase_send_1111();
    void useCase_send_aaaa();
    void useCase_send_bbbb();
    void usecase_OpenFile();
    void usecase_CheckScript();
    void usecase_SendScript();

private:
    Ui::graphs *ui;

    // my properties
    int graphCount;
    QUdpSocket udpSocket;
    QUdpSocket udpServerSocket;
    QUdpSocket *udpSocketSend;
    QUdpSocket *udpSocketGet;
    QList<unsigned> graphT;
    QHash<unsigned, struct entry> db;
    quint32 datagramCounter;
    quint32 datagramMaxSize;
    QCustomPlot* plots;
    QVBoxLayout* vbox;

    // four.cpp
    QHostAddress address;
    quint16 port;

    //parser
    QHostAddress address;
};

#endif // GRAPHS_H
