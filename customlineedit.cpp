#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QWidget *ref)
{
    parentReference = ref;
    chooseButton = new QToolButton(this);
    chooseButton->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    chooseButton->setCursor(Qt::ArrowCursor);

    clearButton = new QToolButton(this);
    clearButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    clearButton->setCursor(Qt::ArrowCursor);
    this->setReadOnly(1);


    clearButton->hide();
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(updateClearButton(const QString)));

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(2*clearButton->sizeHint().width() + frameWidth + 1));

    QSize msz = minimumSizeHint();
    setMinimumSize(qMax(msz.width(), clearButton->sizeHint().height() + frameWidth * 2 + 2),
                   qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
}

void CustomLineEdit::resizeEvent(QResizeEvent *)
{
    QSize sz = clearButton->sizeHint();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    chooseButton->move(rect().right() - frameWidth - sz.width(),
                       (rect().bottom() + 1 - sz.height())/2);
    clearButton->move(rect().right() - frameWidth - sz.width() - sz.width(),
                      (rect().bottom() + 1 - sz.height())/2);
}

void CustomLineEdit::updateClearButton(const QString text)
{
    clearButton->setVisible(!text.isEmpty());
}


