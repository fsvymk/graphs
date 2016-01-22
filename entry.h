#ifndef ENTRY
#define ENTRY

/*
 * Mihail Sidorin - 11.11.2015
 * siddotnet@gmail.com
 * Qt header file
 * Entry is unit, which contains
 * T - number of device/plot
 * N - string number in parsed script
 * Z - result of executing that string
 * V - 0 - minus 1 - plus
 * X - 32 bit time coord.
 * Y - 32 bit value on X.
 */

#include <QString>
#include <QMap>
#include <QDataStream>

struct entry{
    quint32 x;
    qint32  y;   // 64 bits
    quint16 z;
    quint16 v;
    quint16 n;
    quint16 t;   // 64 bits
        // Total : 128 bits
};

static QString ss(int str){
     QString result = QString::number(str);
     return result;
}

static QString toString(entry s){
        QString result;
        QStringList resultDescr;
        resultDescr << "Не определено." << "Успешно." << "Не успешно." << "аварийный стоп.";
        QString rd = "";
        if(s.z > 0 && s.z<4) rd = resultDescr[s.z];

        result.append(QString::number(s.t) + " " +
                      QString::number(s.v) + " " +
                      QString::number(s.x) + " " +
                      QString::number(s.y) + " " +
                      QString::number(s.n) + " " +
                      QString::number(s.z) + " - " + rd);

        return result;
}

static QString output(QMultiMap<unsigned, entry> x){

    QMultiMap<unsigned, entry>::iterator i;
    QString out = "";
    entry point;
    for (i = x.begin(); i != x.end(); ++i){
        out += "\n["+ss(i.key())+"]";
        point = i.value();
        out += toString(point);
    }
    return out;
}

static int serialize(entry M, QByteArray* target){
    bool result = false;

    // Использование встроеного в Qt класса QDataStream оказалось лучшим решением.
    QDataStream niceTool(target, QIODevice::WriteOnly);

    niceTool << M.x;
    niceTool << M.y;

    niceTool << M.t;
    niceTool << M.v;

    niceTool << M.z;
    niceTool << M.n;

    return result;
}

static entry deserialize(QByteArray* source){
    entry result;
    QDataStream prettyTool(source, QIODevice::ReadOnly);
    prettyTool >> result.x >> result.y >> result.t >> result.v >> result.z >> result.n;
    return result;
}

#endif // ENTRY
