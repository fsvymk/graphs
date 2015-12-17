#include "graphs.h"
#include "ui_graphs.h"
#include <QUdpSocket>
#include <QtMath>
#include <QMap>
#include <testweb.h>
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

// Buttons:
// Main Tab:

void graphs::on_BTN_HELP_clicked()
{
    useCase_help();
}

void graphs::on_BTN_OPEN_clicked()
{
    usecase_OpenFile();
}

void graphs::on_BTN_TEST_clicked()
{
    splitBlocks();
}

void graphs::on_BTN_SEND_clicked()
{
    usecase_SendScript();
}

void graphs::on_BTN_START_clicked()
{

    useCase_send_bbbb();
}

void graphs::on_BTN_NEXT_clicked()
{
    useCase_send_aaaa();
}

void graphs::on_BTN_CANCEL_clicked()
{
    useCase_send_1111();
}

void graphs::on_BTN_PAUSE_clicked()
{
    useCase_send_0000();
}

void graphs::on_BTN_STOP_clicked()
{
    useCase_send_ffff();
}

// Buttons Parser Tab:


// Buttons Settings:


// Buttons other:


// Buttons Plots Tab:



void graphs::on_PARSER_BUTTON_COMPILE_clicked()
{
    //processScript();
}

void graphs::on_PARSER_BUTTON_split_clicked()
{
    splitBlocks();
}

void graphs::on_PARSER_BUTTON_X_clicked()
{

}

void graphs::on_PARSER_BUTTON_SEND_clicked()
{

}


// CLIENT BUTTONS:

void graphs::on_clientAdd_clicked()
{

}

void graphs::on_pushButton_minus_clicked()
{

}

void graphs::on_pushButton_plus_clicked()
{

}

void graphs::on_client_ButtonSend_clicked()
{

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
    ui->widget->removeGraph(ui->widget->graph(0));
    ui->widget->setLayout(new QVBoxLayout);
    ui->widget->clearGraphs();

    graphCount  = 20;   // max graph count;

    plots   = new QCustomPlot[graphCount];
    vbox    = (QVBoxLayout*)ui->widget->layout();

    address    = QHostAddress(ui->lineEdit_clientHost->text());
    clientPort = ui->line_clientPort->text().toInt();

    ui->btn_SettingsSave->setDisabled(true);
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
        double x = result.x/1000.0;
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

            plots[plotRow].xAxis->setTickLabelType(QCPAxis::ltDateTime);
            plots[plotRow].xAxis->setDateTimeFormat("hh:mm:ss");
            //plots[plotRow].xAxis->setAutoTickStep(false);
            //plots[plotRow].xAxis->setTickStep(2);
            //plots[plotRow].axisRect()->setupFullAxesBox();

        }
        plots[plotRow].graph(0)->addData(x, y);  // 0 - потому что других там графиков не должно быть
    }
    for(i=0; i<graphT.size(); i++){
        plots[i].replot();
        // удаление лишних данных
        //quint32 current = QDateTime::currentMSecsSinceEpoch();
        quint32 current = QTime::currentTime().msecsSinceStartOfDay();
        plots[i].graph(0)->removeDataBefore(current-shownSize);

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
    Testweb *help = new Testweb();
    help->show();
    help->setGeometry(20, 60, 760, 540);
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
    //udpSocketSend = new QUdpSocket(this);
    //udpSocketGet  = new QUdpSocket(this);
    QHostAddress host("192.168.1.101");
    QHostAddress bcast("192.168.1.255");

    udpSocketSend.connectToHost(bcast, clientPort);
    //udpSocketGet->bind(host, udpSocketSend->localPort());
    udpSocketGet.bind(address, clientPort);
    connect(&udpSocketGet,SIGNAL(readyRead()),this,SLOT(readUdpDatagrams()));

    QByteArray datagram = makeNewDatagram(); // data from external function
    udpSocketSend.write(datagram);
}

void graphs::readUdpDatagrams_by_PM()
{
    //p("Read Udp Datagrams");
    while (udpSocketGet.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocketGet.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocketGet.readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        processUdpDatagram(datagram);
        QHostAddress receivedAddress = sender;
        ui->main_text_top->append("Получен пакет от ip " + receivedAddress.toString());

        //parser->setAddress(receivedAddress);
        ui->PARSER_LINE_PORT->setText(receivedAddress.toString());
    }
}

void graphs::useCase_settings()
{
    port    = ui->line_clientPort->text().toInt();
    address = QHostAddress(ui->lineEdit_clientHost->text());
    clientPort = ui->line_clientPort->text().toInt();
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
    t(ip + " " + clientPort);
    //udpSocket.writeDatagram(message.toUtf8(), QHostAddress(ip), port);
    // отправка пакета.
    udpSocketSend.writeDatagram(Mess, address, clientPort);
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
    udpSocketSend.writeDatagram(Mess, address, clientPort);
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
    udpSocketSend.writeDatagram(Mess, address, clientPort);
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
    udpSocketSend.writeDatagram(Mess, address, clientPort);
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
    udpSocketSend.writeDatagram(Mess, address, clientPort);
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




// PARSER METHODS :

void graphs::splitBlocks()
{
    temporary_green();
    ui->PARSER_TEXT_RESULT->clear();

    QMap<QString, int> globalSems;
    fetchSems(":/config.xml", globalSems);
    displaySems(globalSems);

    QString str = ui->parser_codeEditor->toPlainText();
    QString str_copy = str; // т.к. str будет урезаться в процессе разделения на блоки.

    // Проверка парности скобок.
    int BFL = str.count("{");
    int BFR = str.count("}");
    int BCL = str.count("(");
    int BCR = str.count(")");

    if(BFL!=BFR){
        rb("Err. 1: Brakes {} are not pair.");
        return;
    }

    if(BCL!=BCR){
        rb("Err. 2: Brakes () are not pair.");
        return;
    }

    QByteArray BlockResult;
    QRegExp Block("Block(\\s+)(\\w+)",Qt::CaseInsensitive);
    int i = 0;

    QString blockNameAll;
    QStringList blockNameParts;
    QString blockName;

    QChar qc;
    int lineBase = 0;
    int line = 0;
    QString block;

    while(1==1)
    {
        BlockResult.clear();
        i = Block.indexIn(str);

        line = whatLine(str_copy, lineBase + i) + 1;

        if(i<0)
        {
            saveLogs();
            //rb("GlobalResult = ");
            // вывод готового пакета данных.
            //rb(globalResult.toHex());
            return;
        }

        blockNameAll = Block.cap(0);
        blockNameParts = blockNameAll.split(" ");
        blockName = blockNameParts[1];
        //rb("block Name = " + blockName);

        BlockResult += (char)0; // Block sem code
        BlockResult += blockName;

        while (BlockResult.length() < 32){
            BlockResult += (char)0;
        }

        //rb("BL = " + BlockResult.toHex());
        globalResult += BlockResult;


        int j = str.indexOf('{',i);
        qc = str[j];

        int bl=1;
        int br=0;

        while(bl != br)
        {
            j++;
            qc = str[j];
            if(qc=='{') bl++;
            if(qc=='}') br++;
        }
        block = str.mid(i,j-i);
        str = str.right(str.length()-j);
        lineBase += j;
        // и отправим содержимое в следующую функцию

        parseBlock(block, globalSems, line);
   }
    // сюда код лучше не вставлять т.к. выход из цикла - сразу return;
}

void graphs::send()
{
    if (address.isNull()) {
        t("IP not set");
        return;
    }
    quint16 port = ui->PARSER_LINE_PORT->text().toInt();

    //QString message = ui->textEdit_Result->toPlainText();

    QString ip = address.toString();
    t("SEND " + ip + " :" + globalResult.toHex());
    //udpSocket.writeDatagram(message.toUtf8(), QHostAddress(ip), port);
    // отправка пакета.
    udpSocket.writeDatagram(globalResult, address, port);
}

bool graphs::parseSem(QXmlStreamReader &xml, QMap<QString, int> &sems)
{
     //p("MainWindow::parseSem");
     if (xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "sem")
         return false;
     QXmlStreamAttributes attributes = xml.attributes();
     int id = 0;
     if (attributes.hasAttribute("id")) {
         id = attributes.value("id").toInt();
     }
     else
         return false;

     xml.readNext();
     while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "sem"))
     {
         if (xml.tokenType() == QXmlStreamReader::StartElement)
         {
             if (xml.name() == "name"){
                 xml.readNext();
                 QString name = xml.text().toString();
                 if (sems.contains(name))
                     return false;
                 sems.insert(name, id);
             }
         }
         xml.readNext();
     }
     return true;
}

void graphs::displaySems(QMap<QString, int> &sems)
{
    QMap<QString, int>::const_iterator it = sems.begin();
    for(;it != sems.end(); ++it)
    {
       ui->PARSER_TEXT_SEMS->append("KEY:" + it.key() + "=" + QString::number(it.value()));
    }
}

void graphs::fetchSems(QString FileName, QMap<QString, int> &sems)
{
    QFile file(FileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                    QMessageBox::critical(this,"Load XML File Problem",
                    "Couldn't open config.xml to load settings for parse",
                    QMessageBox::Ok);
                    return;
            }

    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name() == "sems")
                continue;
            if (xml.name() == "sem"){
                if (!parseSem(xml, sems)) {
                    QMessageBox::critical(this,"XML File Problem",
                    "Error parse XML file",
                    QMessageBox::Ok);
                    return;
                }
            }
        }
    }
}

void graphs::QStringList_print(QStringList list)
{
    int cnt = 0;
    QStringList::const_iterator it;
    for(it=list.constBegin(); it!=list.constEnd(); ++it)
    {
        //b("\nlist["+QString::number(cnt)+"]");
        //b("\n" + *it);
        cnt++;
    }
}

void  graphs::rr(QString str)
{
    ui->PARSER_TEXT_RESULT->append(str);

}

void  graphs::bb(QByteArray str)
{
    QString result;
    int n = str.length();
    for(int i=0;i<n;i++)
    {
        short x = str.at(i);
        result += QString::number(x) + " ";
    }
    ui->PARSER_TEXT_RESULT->append(result);
}

void graphs::tt(QString str)
{
    ui->PARSER_TEXT_RESULT->append(str);
}

void  graphs::rb(QString str)
{
    ui->PARSER_TEXT_SEMS->append(str);
}


bool graphs::checkCommand(QString Command, int line)
{
    bool result = true;

    // первое ключевое слово, должно быть command.
    QRegExp fKey("\\w+");
    QRegExp Cmd("command", Qt::CaseInsensitive);

    int pos = fKey.indexIn(Command);
    QString test = fKey.cap(0);

    //int pot = Cmd.indexIn(test);
    if(Cmd.cap(0)!=test) result = false;

    //int fL = Command.length();
    int fB = Command.indexOf("(");
    int fBe = Command.indexOf(")");
    if(fB==-1) rb("Err. 12: There are not ( after command .. at line " + QString::number(line));

    QString CommandName = Command.left(fB);
    CommandName = CommandName.right(CommandName.length() - pos - test.length());
    fB++; // скобку съели
    //b("NAME = " + CommandName);

    QString Bra = Command.mid(fB, fBe - fB);
    // Внутри скобок

    int poe = Bra.indexOf('='); // позиция знака =
    //b("LINE " + QString::number(line) + " POE " + QString::number(poe));
    QString arg = Bra.left(poe);
    QString val = Bra.right(Bra.length() - poe - 1);

    //проверим то что слева и справа от равно, мало ли.
    QRegExp ArgVal("([A-Za-z0-9-]+)");
    int test_arg = arg.indexOf(ArgVal,0);
    int test_val = val.indexOf(ArgVal,0);
    int test_nam = CommandName.indexOf(ArgVal,0);

    //b(QString::number(test_arg));
    //b(QString::number(test_val));
    if (poe == -1)      rb("Err. 13: Command has not parameters. = is not found at line " + QString::number(line));
    if (test_arg == -1) rb("Err. 14: No argument at line " + QString::number(line));
    if (test_val == -1) rb("Err. 15: No value at line " + QString::number(line));
    if (test_nam == -1) rb("Err. 16: What is your name, command? at line " + QString::number(line));

    if ((poe ==-1)  || (test_arg == -1) ||  (test_val == -1) ||  (test_nam == -1)) result = false;

    //b(Bra);
    //b(arg);
    //b(val);

    // отлично - Bra - Это скобки и всё что внутри них.

    return result;
}

bool graphs::checkControlBy(QString ControlBy)
{
    bool result = true;
    //rb("CTRLBY="+ControlBy);
    if(ControlBy.length()<14) result = false;
    return result;
}

bool graphs::checkOptions(QString Options)
{
    bool result = true;
    //rb("OPTS="+Options);
    if(Options.length()<11) result = false;
    return result;
}

QString graphs::GetCodeBetweenBrackets(QString str, QString regular_expression)
{
    // Имеем: блок кода, типа Command {...}
    // Задача: получить всё внутри фигурных скобок.
    // Учитывая, что внутри могут быть вложеные блоки.

    QString result;

    QRegExp rx(regular_expression, Qt::CaseInsensitive);
    int pos = rx.indexIn(str);
    QStringList list = rx.capturedTexts();
    //int c = list.length();

    QString capt = list[0];
    int CL = capt.length();
    str = str.right(str.length() - CL - pos);

    int i = str.indexOf('{');
    int j = i;
    QChar qc = str[i];
    i++;// чтобы первая скобка не попала в результат

    int bl=1;
    int br=0;

    while(bl != br)
    {
        j++;
        qc = str[j];
        if(qc=='{') bl++;
        if(qc=='}') br++;
    }
    result = str.mid(i,j-i);

    return result;
}

QStringList graphs::TrueThreeParts(QString step, int line)
{
    QStringList result;
    QString sline = QString::number(line);

    QString Command = GetCodeBetweenBrackets(step, "command\\s");
    QString CtrlBy  = GetCodeBetweenBrackets(step, "control_by");
    QString Options = GetCodeBetweenBrackets(step, "options");

    t(sline);
    t("");
    t(CtrlBy);

    return result;
}

QStringList graphs::Command_ControlBy_Options(QString step, int line)
{
    QStringList result;

    QRegExp QR_Command("command\\s(.+)control_by", Qt::CaseInsensitive);
    QRegExp QR_ControlBy("control_by", Qt::CaseInsensitive);
    QRegExp QR_Options("options", Qt::CaseInsensitive);

    int i = step.indexOf(QR_Command);
    int j = step.indexOf(QR_ControlBy);
    int k = step.indexOf(QR_Options);

    if(i<0) rb("Err. 4. There are not Command in Step " + QString::number(globalStepNumber) + " LINE " + QString::number(line));
    if(j<0) rb("Err. 5. There are not Control By in Step "+QString::number(globalStepNumber)+" LINE " + QString::number(line));
    if(k<0) rb("Err. 6. There are not Options in Step  "+ QString::number(globalStepNumber) + " LINE " + QString::number(line));

    if (i>j) rb("Err 7. Command should stand in front of the ControlBy.\n");
    if (j>k) rb("Err 8. ControlBy should stand in front of the Options.\n");
    if (i>k) rb("Err 9. Command should stand in front of the Options.\n");

    // Проверить, чтобы не было при этом ничего лишнего.
    // Способ 1: Проверять в Step. Можно проверить
    // Способ 2: Проверять в ThreeParts.
    // т.е. создать функцию checkCommand, checkControlBy, checkOptions
    // и проверять перед вызовом ParseFragment....

    __CommandLine = whatLine(step, i);
    __ControlByLine = whatLine(step, j);
    __OptionsLine = whatLine(step, k);

    // нет ли чего лишнего перед Command.
    QString checkTrash = step.left(i);

    QRegExp TrashWord("\\w");
    int checkTrashWord = TrashWord.indexIn(checkTrash);
    if(checkTrashWord!=-1) rb("Err. 10: " + QString::number(checkTrashWord) + " at line " + QString::number(line));

    int FB = checkTrash.count("{");
    if(FB!=1) rb("Err. 11: There are not { after Step " + QString::number(globalStepNumber) + " at line " +  QString::number(line));
    checkTrash.replace("\{", "");

    QString Command =   step.mid(i, j-i);
    //if (checkCommand(Command, line)) t("TRUE");
    //else t("FALSE");

    QString ControlBy = step.mid(j, k-j);
    checkControlBy(ControlBy);

    QString Options =   step.mid(k, step.length()-k);
    checkOptions(Options);

    result.append(Command);
    result.append(ControlBy);
    result.append(Options);

    return result;
}

void graphs::parseBlock(QString Block, QMap<QString,int> &sems, int line)
{
    //b("BLOCK. Line = " + QString::number(line));
    //b(Block);
    //b("\r\n\r\n\r\n\r\n\r\n");

    QString str = Block; // вот! всегдя нужно нажимать Crtl+Mouse чтобы понять где объявлена переменная.
    QString str_copy = str; // тот же принцип что и в предыдущей функции

    QRegExp QR("[S|s]tep\\d+\\s");
    QR.setMinimal(true);
    QChar qc;
    QRegExp QRStepN("\\d+");
    QString StepArgs;

    QString StepN;
    QString step;
    QStringList ThreeParts;

    int lineBase = 0;  // позиция текущего шага из блока (в символах)
    int lineInner = 0; // линия внутри блока. line - из аргументов функции - линия, с которой начинается Block.

    while(1==1)
    {
        int i = QR.indexIn(str);
        if(i<0) return;


        lineInner = whatLine(str_copy, lineBase + i); // линия внутри блока! первая в блоке - нулевая здесь.
        if(lineBase == 0) lineBase = i; // только в первом проходе, чтобы Block name{ попали сюда...

        StepArgs = QR.cap(0);
        str = str.right(str.length() - StepArgs.length());

        // получение номера Step

        QRStepN.indexIn(StepArgs);
        StepN = QRStepN.cap(0);
        globalStepNumber = StepN.toInt();
        //globalResult += globalStepNumber;

        int j = str.indexOf('{',i);

        qc = str[j];

        int bl=1;
        int br=0;

        while(bl != br)
        {
            j++;
            qc = str[j];
            if(qc=='{') bl++;
            if(qc=='}') br++;
        }

        // Получение одного шага из оставшихся нескольких
        step = str.mid(i,j-i);
        str = str.right(str.length() - j);
        lineBase += j + 1;

        // Разбиваем шаг на составляющие
        ThreeParts = Command_ControlBy_Options(step, line + lineInner);
        parseFragment(ThreeParts[0], globalStepNumber, sems, line + lineInner + __CommandLine);
        parseFragment(ThreeParts[1], globalStepNumber, sems, line + lineInner + __ControlByLine);
        parseFragment(ThreeParts[2], globalStepNumber, sems, line + lineInner + __OptionsLine);

        //ThreeParts = TrueThreeParts(step, line+lineInner);
    }

}

void graphs::parse()
{

    // А считаем ка для начала config.xml
    QMap<QString, int> globalSems;
    fetchSems(":/config.xml", globalSems);
    displaySems(globalSems);

    // 2. Правильность написания Block, Control by, Options

    // 3. Переводить все содержимое {Command..} в одну строку

        // Variant A
        // Разбить Block на Step внутри него.

    QString str  = ui->parser_codeEditor->toPlainText();

        // Первый Step после Block ... Первая фигурная скобка после Block, а после неё...
    QRegExp QR("[S|s]tep\\d+\\s");

                            //сюда можно добавить:
                            //procedure\\(\\)");
                            //(ui->textEdit_5->toPlainText());

        // находим позицию символа в строке.
    QR.setMinimal(true);

    int i = QR.indexIn(str);
    QString StepArgs = QR.cap(0);

    str = str.right(str.length() - StepArgs.length());

    // получение номера Step
    QRegExp QRStepN("\\d+");
    QRStepN.indexIn(StepArgs);
    QString StepN = QRStepN.cap(0);
    globalStepNumber = StepN.toInt();
    globalResult += globalStepNumber;
    //r(QString::number(globalStepNumber));

    //ui->textEdit_2->append("StepN = " + StepN);

    //ui->textEdit_2->append(QString::number(i));

        // находим позицию первой { после найденой (i)

    int j = str.indexOf('{',i);
    //ui->textEdit_2->append("Первая фигурная скобка после Step1 " + QString::number(j));

    // проверим тот ли это символ
    QChar qc = str[j];

    int bl=1;
    int br=0;

    while(bl != br)
    {
        j++;
        qc = str[j];
        if(qc=='{') bl++;
        if(qc=='}') br++;
    }

    //ui->textEdit_2->append(QString::number(j));

    QString STEP1 = str.mid(i,j-i);
    //ui->textEdit_2->append("STEP1");

    QStringList ThreeParts = Command_ControlBy_Options(STEP1, 0);

    parseFragment(ThreeParts[0], globalStepNumber, globalSems ,0);
    parseFragment(ThreeParts[1], globalStepNumber, globalSems ,0);
    parseFragment(ThreeParts[2], globalStepNumber, globalSems ,0);


}


void graphs::usecase_parser_clear()
{
    //ui->textEdit_prepared->clear();
    ui->PARSER_TEXT_RESULT->clear();
    //ui->textEdit_2->clear();
    ui->PARSER_TEXT_SEMS->clear();
    ui->PARSER_TEXT_RESULT->clear();

    globalResult = "";
}

void graphs::parseFragment(QString text, int StepN, const QMap<QString, int> &sems, int line)
{
        QByteArray result;

        result += StepN;

        text.replace("}"," ");
        text.replace("{"," ");
        text.replace("\'", "");
        //text.replace("\‘", "");
        //text.replace("\’", "");

        text.replace("Command ", "", Qt::CaseInsensitive);
        //text.replace("Options", "", Qt::CaseInsensitive); // потому что у Options есть код.
        text.replace("Control_by", "", Qt::CaseInsensitive);

        QStringList lines = text.split("\n");

        for (int i = 0; i < lines.length(); i++)
        {
            result += parseLine(lines[i], sems, line);
        }

        int t = result.length();
        for(int i=0; i<(LineSize-1-t); i++){
            result.append((char)0);
        }

        result.append(line);

        globalResult += result;
        rr(result.toHex());
}

QByteArray graphs::parseLine(const QString &text, const QMap<QString, int> &sems, int line)
{
    //r("PARSE LINE " + text);

    int i = text.indexOf(' ');
    QString type = text.mid(0, i);
    int lastPosition = 0;

    i = text.indexOf('(', lastPosition);
    QString value = text.mid(lastPosition, i - lastPosition).trimmed();
    lastPosition = i;
    i = text.indexOf(')', lastPosition);

    //Текст между скобками
    QString betweenBrackets =  text.mid(lastPosition+1, i - lastPosition - 1);
    //Массив чисел
    QStringList numbers = betweenBrackets.split(",", QString::SkipEmptyParts);

    QByteArray result = processScript(value, numbers, sems, line);

    return result;
}

QByteArray graphs::processScript(QString value, QStringList numbers, const QMap<QString, int> &sems, int line)
{
    //  Самое главное - одному вызову processScript соответствует один вызов parseLine
    //  смысл функции - конкретную строку превратить в байт-код.
    //  line - номер линии
    //  value - команда
    //  numbers - параметры (если есть)
    //  sems - всегда одинаковый набор, можно было бы запихнуть это в глобальную область, если не лень.

    QByteArray result;
    QString SLine = QString::number(line);

    value = value.toLower();
    //Ищем, есть ли нужный семафор из xml файла
    if (!sems.contains(value)) // нет такого ключевого слова
    {
        int VL = value.length();
        //int test_options = value.indexOf("options", Qt::CaseInsensitive);


        if(VL > 0) // если ключевое слово длинее нуля символов
        {
            rb("Err.3 Wrong keyword. " + value + " at line " + SLine); // // + " has not sem. length = " + QString::number(VL));
        }
    }
    else
    {
        result.append(sems[value]);
    }

    //обработка символа *
      for (int i = 0; i < numbers.size(); i++) {
        QString number = numbers[i].trimmed();
            if (number == "*")
                result += (char)0x19;
            else {
                bool ok = false;
                int e = number.indexOf("=");
                QString leftPart  = number.left(e).trimmed();
                QString rightPart = number.right(number.size()-e-1).trimmed();

                QString leftPartSem = "";
                QString rightPartSem= "";

                if (!sems.contains(leftPart)){

                    leftPartSem += "QDEBUG: erro sedm " + leftPart;
                }
                else{
                    result.append(sems[leftPart]);
                    leftPartSem += QString::number(sems[leftPart]);
                }

                if (!sems.contains(rightPart)){
                    // преобразование правой части параметра в число
                    result += (char)rightPart.toUInt(&ok);
                    //rightPartSem += (int)rightPart.toUInt(&ok);
                }
                else{
                    result.append(sems[rightPart]);
                    //rightPartSem += QString::number(sems[rightPart]);
                }
            }
    }

    return result;
}

int graphs::whatLine(QString text, int position)
{
    text = text.left(position);
    return text.count("\n"); // т.к. перед первой строкой нет перевода строки, но это уже одна строка.
}

void graphs::temporary_green()
{

}

void graphs::saveLogs()
{
    QString fileName1 = "log_result.txt";
    QString fileName2 = "log_errors.txt";

    QString log1 = ui->parser_codeEditor->toPlainText();
    QString log2 = ui->PARSER_TEXT_RESULT->toPlainText();
    log1.replace("\n","\r\n");
    log2.replace("\n","\r\n");

    QFile file1(fileName1);
    QFile file2(fileName2);

    QFile::OpenMode FileMode1 = QIODevice::WriteOnly;
    QFile::OpenMode FileMode2 = QIODevice::WriteOnly;

    if(file1.exists(fileName1)) FileMode1 = QIODevice::Append;
    if(file2.exists(fileName2)) FileMode2 = QIODevice::Append;

    file1.open(FileMode1);
    file2.open(FileMode2);

    QTextStream stream1(&file1);
    QTextStream stream2(&file2);

    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString date_str = date.toString() + " " + time.toString();

    stream1 << "\r\n\r\n##### " + date_str + " #####\r\n";
    stream2 << "\r\n\r\n===== " + date_str + " =====\r\n";

    stream1 << log1;
    stream2 << log2;

    file1.close();
    file2.close();
}


void graphs::usecase_parser_send()
{
    send();
}

void graphs::useCase_parser_Save()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                QString::fromUtf8("Сохранить файл"),
                                QDir::currentPath(),
                                "All files (*.*)");
    QFile file(fileName);
    QString str = ui->parser_codeEditor->toPlainText();

    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );

        stream << str;
        file.close();
    }


}

void graphs::useCase_parserOpen()
{
    QString fileName;

            fileName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Открыть файл"),
                                QDir::currentPath(),
                                "All files (*.*)");

    QFile file(fileName);
         if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;

         ui->parser_codeEditor->clear();
         QTextStream in(&file);
         while (!in.atEnd()) {
             QString line = in.readLine();
             ui->parser_codeEditor->appendPlainText(line);
         }
}

void graphs::useCase_parser_pushTestFile()
{
    //QString fileName = QFileDialog::getSaveFileName(this,QString::fromUtf8("Сохранить файл"),QDir::currentPath(),"All files (*.*)");

    QString fileName = "findme.txt";

    QFile file(fileName);
    QString str = ui->parser_codeEditor->toPlainText();

    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );

        stream << str;
        file.close();
    }

}

void graphs::on_lineEdit_bind_textChanged(const QString &arg1)
{
    ui->main_text_top->append("Server port = " + arg1);
    ui->btn_SettingsSave->setEnabled(true);
}

void graphs::on_line_clientPort_textChanged(const QString &arg1)
{
    ui->btn_SettingsSave->setEnabled(true);
    ui->main_text_top->append("Client port = " + arg1);
}

void graphs::on_btn_SettingsSave_clicked()
{
    address = QHostAddress(ui->lineEdit_clientHost->text());
    port =  ui->lineEdit_bind->text().toInt();
    clientPort =ui->line_clientPort->text().toInt();

    ui->btn_SettingsSave->setDisabled(true);
}

void graphs::on_verticalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(ss(value));
}
