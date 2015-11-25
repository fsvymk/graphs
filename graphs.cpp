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
    init();             // Точка входа. Будет занят порт UDP для входящих сообщений.
}

graphs::~graphs()
{
    delete ui;
}

// Events
void graphs::on_main_button_server_start_clicked()
{
    bindUdpPort();
}

void graphs::on_pushButton_5_clicked()
{
    //ui->tabWidget->addTab(new QWidget); // there are error
}

// Output:



// Use cases:

void graphs::p(QString str){
    ui->main_text_bottom->append(str);
}

QString graphs::ss(long x){
    return QString::number(x);
}

void graphs::init(){

    connect(&udpServerSocket, SIGNAL(readyRead()),
            this, SLOT(readUdpDatagrams()));
    datagramCounter = 0;
    datagramMaxSize = 0;

    ui->widget->clearPlottables();
    ui->widget->setLayout(new QVBoxLayout);
    ui->widget->clearGraphs();

    graphCount  = 20;   // max graph count;

    plots   = new QCustomPlot[graphCount];
    vbox    = (QVBoxLayout*)ui->widget->layout();
}

void graphs::delete_graphs(QVBoxLayout* plotsLayout)
{
    while (QLayoutItem* item = plotsLayout->takeAt(0)) {    delete item;    }
}

void graphs::clear_plots(QCustomPlot* pl)
{
    pl->clearGraphs();
}

void graphs::deleteSome(){

    ui->main_text_bottom->clear();
    ui->main_text_top->clear();

    delete_graphs(vbox);

    int o = graphT.size();
    int i;
    for(i=0;i<o;i++){
        plots[i].graph(0)->clearData();
        plots[i].xAxis->rescale();
        plots[i].replot();
        vbox->removeWidget(&plots[i]);
    }
    vbox->replaceWidget(&plots[0], &plots[o], Qt::FindChildrenRecursively);
    graphT.clear();
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

        entryMassDeserialize(&datagram, &db);

        QString output = "QHash db ["+QString::number(db.count())+"].";
        ui->main_text_top->append(output);

        output_db(db);
        //output_vectors(&_x,&_y);
        //draw(&_x,&_y);
        //numbers(db);
        //realTime_Redraw();
    }
}

// from PLOT-DEMO
void graphs::entryMassDeserialize(QByteArray* source,
                                    QHash<unsigned, struct entry>* DB){
    int i;
    entry result;
    QDataStream prettyTool(source, QIODevice::ReadOnly);

    int cnt = source->length()/sizeof(entry);
    for(i=0; i<cnt; i++){
        prettyTool >> result.x >> result.y >> result.t >> result.v >> result.z >> result.n;
        //p(entryToString(result));
        DB->insert(result.t, result);
        double x = result.x;
        double y = result.y;

        quint16 T = result.t;
        //Добавим графики прямо здесь

        int plotRow;
        if(graphT.contains(T)){ // График с этим ключом T уже есть
            //p("T="+ss(T)+" уже есть. "+ss(result.x)+" "+ss(result.y));
            plotRow = graphT.indexOf(T); // Вычисляем, где этот график. Позиция ключа в graphT совпадает с
                                         // позицией нужного графика в виджете по вертикали.
        }
        else{
            //p("T="+ss(T)+" создаём.");
            graphT.append(T);                // создаём ключ
            plotRow = graphT.size()-1;
            //p("Это будет "+ss(plotRow)+"-й график "+ss(result.x)+" "+ss(result.y));

            vbox->addWidget(&plots[plotRow]);    // QVBoxLayout PL1
            plots[plotRow].addGraph();          // В этой координатной сетке он будет всегда один.
            plots[plotRow].xAxis->setLabel(ss(T)+" X (time)");
            plots[plotRow].yAxis->setLabel("y");


        }
        plots[plotRow].graph(0)->addData(x, y);  // 0 - потому что других там графиков не должно быть
    }
    for(i=0; i<graphT.size(); i++){
        plots[i].replot();
        plots[i].xAxis->rescale();
        plots[i].yAxis->rescale();
   }
}

void graphs::output_db(QHash<unsigned, entry> &db){
     QHash<unsigned, entry>::const_iterator i;
     for (i = db.constBegin(); i != db.constEnd(); ++i){
            ui->main_text_bottom->append(QString::number(i.key()) + ":");
             ui->main_text_bottom->append(toString(i.value()));
         }
}

void graphs::numbers(QHash<unsigned, struct entry> &db){
    int it = 0;
    db.insert(0, deserialize(NULL));
}


void graphs::remove(QLayout* layout)
{
    QLayoutItem* child;
    while(layout->count()!=0)
    {
        child = layout->takeAt(0);
        if(child->layout() != 0)
        {
            remove(child->layout());
        }
        else if(child->widget() != 0)
        {
            delete child->widget();
        }

        delete child;
    }
}



void graphs::output_vectors(QVector<double>* x, QVector<double>* y){
    int cntx = x->size();
    int cnty = y->size();
    int i;
    QString outstr;
    for(i=0; i<cntx; i++){
        outstr = "" + QString::number(i) + " ";
        outstr += QString::number(x->at(i)) + " ";
        outstr += QString::number(y->at(i)) + "?";

        p(outstr);
    }
}


void graphs::bindUdpPort()
{
    quint16 port = ui->lineEdit_bind->text().toInt();
    if (udpServerSocket.bind(port)) { ui->main_button_server_start->setEnabled(false);}
    else {      p("Ошибка запуска сервера");     }
}
