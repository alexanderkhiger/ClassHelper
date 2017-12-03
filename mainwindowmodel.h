#ifndef MAINWINDOWMODEL_H
#define MAINWINDOWMODEL_H

#include <QObject>
#include <QByteArray>
#include <QMimeData>
#include <QDataStream>

class MainWindowModel : public QObject
{
    Q_OBJECT
public:
    explicit MainWindowModel(QObject *parent = nullptr);

signals:
    void sendData(QString objName, QString containedData);

public slots:
    void updateParameters(const QObject *myObject, const QMimeData *mimeData);

private:
    QString containedData;
};

#endif // MAINWINDOWMODEL_H
