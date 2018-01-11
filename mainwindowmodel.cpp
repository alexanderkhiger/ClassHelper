#include "mainwindowmodel.h"

MainWindowModel::MainWindowModel(QObject *parent) : QObject(parent)
{

}

void MainWindowModel::updateParameters(const QObject *myObject, const QMimeData *mimeData)
{
    if (myObject->objectName() != "classesList" && myObject->objectName() != "teachersList")
        return;
    QByteArray encoded = mimeData->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;
        containedData = roleDataMap[0].toString();
    }

    if (myObject->objectName()=="classesList")
    {
        emit sendData("classesList",containedData);
    }
    else if (myObject->objectName()=="teachersList")
    {
        emit sendData("teachersList",containedData);
    }
    else
        return;
}
