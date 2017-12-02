#include "customitemdelegate.h"

CustomItemDelegate::CustomItemDelegate()
{

}

bool CustomItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::InputMethod)
    qDebug() << "works";
    else
    return QAbstractItemDelegate::editorEvent(event,model,option,index);
}
