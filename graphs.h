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
    void setNonEditMode(QWidget& Widget, bool bNonEditMode);
    void processUdpDatagram(const QByteArray &data);
    void oprosb();
    void readUdpDatagrams_by_PM();
    void fsetAddress(const QHostAddress &addr);
    void fsetPort(const quint16 &prt);

    void useCase_parser_show();
    void useCase_mainwindow();
    void useCase_help();
    void useCase_settings();
    void useCase_window_testweb();
    void useCase_window_mainwindow();
    void useCase_window_parser();

    void useCase_send_ffff();
    void useCase_send_0000();
    void useCase_send_1111();
    void useCase_send_aaaa();
    void useCase_send_bbbb();

    void usecase_OpenFile();
    void usecase_CheckScript();
    void usecase_SendScript();

    void on_BTN_HELP_clicked();
    void on_BTN_OPEN_clicked();
    void on_BTN_TEST_clicked();
    void on_BTN_SEND_clicked();
    void on_BTN_START_clicked();
    void on_BTN_NEXT_clicked();
    void on_BTN_CANCEL_clicked();
    void on_BTN_PAUSE_clicked();
    void on_BTN_STOP_clicked();
    void on_PARSER_BUTTON_COMPILE_clicked();
    void on_PARSER_BUTTON_split_clicked();
    void on_PARSER_BUTTON_X_clicked();
    void on_PARSER_BUTTON_SEND_clicked();
    void on_clientAdd_clicked();
    void on_pushButton_minus_clicked();
    void on_pushButton_plus_clicked();
    void on_client_ButtonSend_clicked();

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
};

#endif // GRAPHS_H
