#include "droparea.h"

DropArea::DropArea(QWidget *parent)
    : QLabel(parent)
{
    setMinimumSize(300, 100);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    setWordWrap(true);
    setAutoFillBackground(true);
    clear();
}

void DropArea::dragEnterEvent(QDragEnterEvent *event)
{

    setText(tr("Перетащите сюда элемент из списка или сделайте двойной клик на элементе списка"));
    setBackgroundRole(QPalette::Highlight);
    event->acceptProposedAction();

}

void DropArea::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropArea::dropEvent(QDropEvent *event)
{

    setBackgroundRole(QPalette::Dark);
    event->acceptProposedAction();
    emit changed(event->source(), event->mimeData());
}

void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    setBackgroundRole(QPalette::Dark);
    event->accept();
}

void DropArea::clear()
{
    setText(tr("Перетащите сюда элемент из списка или сделайте двойной клик на элементе списка"));
    setBackgroundRole(QPalette::Dark);
    emit clearParameters();
}
