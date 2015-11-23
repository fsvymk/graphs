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
    QString ss(long x);
    void p(QString str);

    // graphs:
    void entryMassDeserialize(QByteArray* source,
                              QHash<unsigned, struct entry>* DB);
    void readUdpDatagrams();
    void output_vectors(QVector<double> *x, QVector<double> *y);
    void output_db(QHash<unsigned, struct entry> &db);
    void numbers(QHash<unsigned, struct entry> &db);

    //parser:
    void displayError(int line);
    void parseLine(const QString& text, int line, const QMap<QString, int> &sems);
    void processScript(int line, QString type, QString value, QStringList numbers, const QMap<QString, int> &sems);
    void processUdpDatagram(const QByteArray& data);
    bool parseSem(QXmlStreamReader& xml, QMap<QString, int> &sems);

private:
    Ui::graphs *ui;

    // my properties
    int cod;
    QUdpSocket udpSocket;
    QUdpSocket udpServerSocket;
    QList<unsigned> graphT;
    QHash<unsigned, struct entry> db;
    quint32 datagramCounter;
    quint32 datagramMaxSize;
    QCustomPlot* plots;
    QVBoxLayout* vbox;
};

#endif // GRAPHS_H
