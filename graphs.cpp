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

// Events

void graphs::on_pushButton_5_clicked()
{
    //ui->tabWidget->addTab(new QWidget); // there are error
}

// Output:



// Use cases:

void graphs::init(){

    connect(&udpServerSocket, SIGNAL(readyRead()),
            this, SLOT(readUdpDatagrams()));
    datagramCounter = 0;
    datagramMaxSize = 0;

    ui->customPlot->clearPlottables();
    ui->customPlot->setLayout(new QVBoxLayout);
    ui->customPlot->clearGraphs();

    graphCount  = 20;   // max graph count;

    plots   = new QCustomPlot[graphCount];
    vbox    = (QVBoxLayout*)ui->customPlot->layout();
}

void graphs::readUdpDatagrams()
{
    while (udpServerSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpServerSocket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpServerSocket.readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        datagramCounter++;
        ui->line_datagrams->setText(ss(datagramCounter));

        if (datagram.size()>datagramMaxSize) datagramMaxSize = datagram.size();
        ui->line_maxdata->setText(ss(datagramMaxSize));

        //QHash<unsigned, struct entry> db;
        entryMassDeserialize(&datagram, &db);

        QString output = "QHash db ["+QString::number(db.count())+"].";
        ui->textVectors->append(output);

        //output_db(db);
        //output_vectors(&_x,&_y);
        //draw(&_x,&_y);
        //numbers(db);

        //realTime_Redraw();
    }
}
