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

    void setNormalZoom();
    void saveSettings();
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

    //PARSER

    void 		splitBlocks();
    void 		send();
    bool 		parseSem(QXmlStreamReader &xml, QMap<QString, int> &sems);
    void 		displaySems(QMap<QString, int> &sems);
    void 		fetchSems(QString FileName, QMap<QString, int> &sems);
    void 		QStringList_print(QStringList list);
    void  		rr(QString str);
    void  		bb(QByteArray str);
    void 		tt(QString str);
    void  		rb(QString str);
    bool 		checkCommand(QString Command, int line);
    bool 		checkControlBy(QString ControlBy);
    bool 		checkOptions(QString Options);
    QString 	GetCodeBetweenBrackets(QString str, QString regular_expression);
    QStringList TrueThreeParts(QString step, int line);
    QStringList Command_ControlBy_Options(QString step, int line);
    void 		parseBlock(QString Block, QMap<QString,int> &sems, int line);
    void 		parseFragment(QString text, int StepN, const QMap<QString, int> &sems, int line);
    QByteArray 	parseLine(const QString &text, const QMap<QString, int> &sems, int line);
    QByteArray 	processScript(QString value, QStringList numbers, const QMap<QString, int> &sems, int line);
    int 		whatLine(QString text, int position);
    void 		temporary_green();
    void 		saveLogs();


    void        usecase_parser_send();
    void        useCase_parser_Save();

    void        useCase_parserOpen();
    void        useCase_parser_pushTestFile();
    void        parse();
    void        usecase_parser_clear();
    void on_lineEdit_bind_textChanged(const QString &arg1);

    void on_line_clientPort_textChanged(const QString &arg1);

    void on_btn_SettingsSave_clicked();

    void on_verticalSlider_valueChanged(int value);

    void on_verticalSlider_actionTriggered(int action);

    void on_pushButton_clicked();

    void on_line_clientPort_2_textChanged(const QString &arg1);

private:
    Ui::graphs *ui;

    // my properties
    int graphCount;
    QUdpSocket udpSocket;
    QUdpSocket udpServerSocket;
    QUdpSocket udpSocketSend;
    QUdpSocket udpSocketGet;
    QList<unsigned> graphT;
    QHash<unsigned, struct entry> db;
    quint32 datagramCounter;
    quint32 datagramMaxSize;
    QCustomPlot* plots;
    QVBoxLayout* vbox;

    // Settings
    QHostAddress address; // client addr.
    quint16 clientPort;
    quint16 port;        //server port

    quint32 shownSize;

    // Parser
    static const int LineSize = 32;
    QStringList BlockNames;
    QStringList Blocks;
    QByteArray globalResult;
    int __CommandLine;
    int __ControlByLine;
    int __OptionsLine;
    int globalStepNumber;
};

#endif // GRAPHS_H
