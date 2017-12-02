#ifndef CUSTOMITEMDELEGATE_H
#define CUSTOMITEMDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QAbstractItemDelegate>
#include <QEvent>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QDebug>

class CustomItemDelegate : public QAbstractItemDelegate
{
public:
    CustomItemDelegate();
    bool editorEvent(QEvent *event,
                             QAbstractItemModel *model,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index);
};

#endif // CUSTOMITEMDELEGATE_H
