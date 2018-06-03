#ifndef CUSTOMQUERYMODEL_H
#define CUSTOMQUERYMODEL_H

#include <QObject>
#include <QWidget>
#include <QSqlQueryModel>
#include <QDebug>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

class CustomQueryModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    CustomQueryModel();

    Qt::DropActions CustomQueryModel::supportedDropActions() const
    {
        return Qt::CopyAction | Qt::MoveAction;
    }

    Qt::ItemFlags CustomQueryModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags defaultFlags = QSqlQueryModel::flags(index);

        if (index.isValid())
            return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags |Qt::ItemIsSelectable;

        return defaultFlags;
    }

    virtual bool CustomQueryModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
    {
        return true;
    }


    virtual bool CustomQueryModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
    {
        emit targetIndex(parent);
        return true;
    }

signals:

    void targetIndex(const QModelIndex &index);

};





#endif // CUSTOMQUERYMODEL_H
