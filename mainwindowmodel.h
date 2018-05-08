#ifndef MAINWINDOWMODEL_H
#define MAINWINDOWMODEL_H

#include <QObject>
#include <QByteArray>
#include <QMimeData>
#include <QDataStream>
#include <QDebug>

class MainWindowModel : public QObject
{
    Q_OBJECT
public:
    explicit MainWindowModel(QObject *parent = 0);

signals:
    void sendData(QString objName, QString containedData);

public slots:
    void updateParameters(const QObject *myObject, const QMimeData *mimeData);
    void distributeAllToTeacher(int idZan, int idPrep);

private:
    QString containedData;
};

#endif // MAINWINDOWMODEL_H
