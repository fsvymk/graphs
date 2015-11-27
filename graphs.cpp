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

void graphs::t(QString str){
    ui->main_text_top->append(str);
}

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

        //QString output = "QHash db ["+QString::number(db.count())+"].";
        //ui->main_text_top->append(output);

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


// four.cpp


void graphs::useCase_parser_show()
{

    //parser->show();
}

void graphs::useCase_mainwindow()
{
    //MainWindow *MW = new MainWindow();
    //MW->show();
}

void graphs::useCase_help()
{
    //Testweb *help = new Testweb();
    //help->show();
   //help->setGeometry(20,60,760,540);
}

void graphs::setNonEditMode( QWidget& Widget , bool bNonEditMode )
{
    // Проход по QLineEdit - там для соответствующего изменения их ReadOnly и FocusPolicy
    QList<QTextEdit*> LE_List = Widget.findChildren<QTextEdit*>();

    for ( QList<QTextEdit*>::iterator it = LE_List.begin(); it != LE_List.end(); it++ )
    {
        (*it)->setReadOnly( bNonEditMode );
        if ( bNonEditMode )
            (*it)->setFocusPolicy(Qt::NoFocus);
        else
            (*it)->setFocusPolicy(Qt::StrongFocus);
    };

    // Проход по QLabel - лам для соответствующего изменения их Enabled
    QList<QLabel*> LB_List = Widget.findChildren<QLabel*>();

    for ( QList<QLabel*>::iterator it = LB_List.begin(); it != LB_List.end(); it++ )
    {
        (*it)->setEnabled( !bNonEditMode );
    };
}

QByteArray graphs::makeNewDatagram(){
    return QByteArray();
}

void graphs::processUdpDatagram(const QByteArray &data)
{

}

/*
void graphs::t(const QString &message)
{
    qDebug() << message;
}*/

void graphs::oprosb()
{
    udpSocketSend = new QUdpSocket(this);
    udpSocketGet  = new QUdpSocket(this);
    QHostAddress host("192.168.1.101");
    QHostAddress bcast("192.168.1.255");

    udpSocketSend->connectToHost(bcast,16000);
    //udpSocketGet->bind(host, udpSocketSend->localPort());
    udpSocketGet->bind(QHostAddress("127.0.0.1"), 16000);
    connect(udpSocketGet,SIGNAL(readyRead()),this,SLOT(readUdpDatagrams()));

    QByteArray datagram = makeNewDatagram(); // data from external function
    udpSocketSend->write(datagram);
}

void graphs::readUdpDatagrams_by_PM()
{
    //p("Read Udp Datagrams");
    while (udpSocketGet->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocketGet->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocketGet->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        processUdpDatagram(datagram);
        QHostAddress receivedAddress = sender;
        ui->main_text_top->append("Получен пакет от ip " + receivedAddress.toString());

        //parser->setAddress(receivedAddress);
        ui->PARSER_LINE_PORT->setText(receivedAddress.);
    }
}

void graphs::useCase_settings()
{
port = ui->lineEdit_clientPort;
address = QHostAddress(ui->lineEdit_clientHost);
}

void graphs::useCase_window_testweb()
{
    //testweb->show();
}

void graphs::useCase_window_mainwindow()
{
    //mainwindow->show();
}

void graphs::useCase_window_parser()
{
    //parser->show();
}

void graphs::fsetAddress(const QHostAddress &addr)
{
    address = addr;
}

void graphs::fsetPort(const quint16 &prt)
{
    port = prt;
}

void graphs::useCase_send_ffff()
{
    if (address.isNull()) {
        t("IP not set");
        return;
    }

    QByteArray Mess;
    int m = 0xffff;
    Mess.append((char*)&m, sizeof(m));

    //QString message = ui->textEdit_Result->toPlainText();

    QString ip = address.toString();
    //udpSocket.writeDatagram(message.toUtf8(), QHostAddress(ip), port);
    // отправка пакета.
    udpSocketSend->writeDatagram(Mess, address, port);
}

void graphs::useCase_send_0000()
{
    if (address.isNull()) {
        t("IP not set");
        return;
    }
    QByteArray Mess;
    int m = 0x0000;
    Mess.append((char*)&m, sizeof(m));

    //QString message = ui->textEdit_Result->toPlainText();

    QString ip = address.toString();
    //udpSocket.writeDatagram(message.toUtf8(), QHostAddress(ip), port);
    // отправка пакета.
    udpSocketSend->writeDatagram(Mess, address, port);
}

void graphs::useCase_send_1111()
{
    if (address.isNull()) {
        t("IP not set");
        return;
    }
    QByteArray Mess;
    int m = 0x1111;
    Mess.append((char*)&m, sizeof(m));

    //QString message = ui->textEdit_Result->toPlainText();

    QString ip = address.toString();
    //udpSocket.writeDatagram(message.toUtf8(), QHostAddress(ip), port);
    // отправка пакета.
    udpSocketSend->writeDatagram(Mess, address, port);
}

void graphs::useCase_send_aaaa()
{
    if (address.isNull()) {
        t("IP not set");
        return;
    }
    QByteArray Mess;
    int m = 0xaaaa;
    Mess.append((char*)&m, sizeof(m));

    //QString message = ui->textEdit_Result->toPlainText();

    QString ip = address.toString();

    //udpSocket.writeDatagram(message.toUtf8(), QHostAddress(ip), port);
    // отправка пакета.
    udpSocketSend->writeDatagram(Mess, address, port);
}


void graphs::useCase_send_bbbb()
{
    if (address.isNull()) {
        t("IP not set");
        return;
    }
    QByteArray Mess;
    int m = 0xbbbb;
    Mess.append((char*)&m, sizeof(m));

    //QString message = ui->textEdit_Result->toPlainText();

    QString ip = address.toString();

    //udpSocket.writeDatagram(message.toUtf8(), QHostAddress(ip), port);
    // отправка пакета.
    udpSocketSend->writeDatagram(Mess, address, port);
}


void graphs::usecase_OpenFile()
{
    QString fileName;

            fileName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Открыть файл"),
                                QDir::currentPath(),
                                "All files (*.*)");

    QFile file(fileName);
         if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;

         ui->main_text_bottom->clear();
         QTextStream in(&file);
         while (!in.atEnd()) {
             QString line = in.readLine();
             p(line);
         }
}

void graphs::usecase_CheckScript()
{
    //parser->splitBlocks();
}

void graphs::usecase_SendScript()
{
    //parser->send();
}
