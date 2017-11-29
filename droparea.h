#ifndef DROPAREA_H
#define DROPAREA_H

#include <QLabel>
#include <QObject>
#include <QDragEnterEvent>
#include <QMimeData>

class QMimeData;

class DropArea : public QLabel
{
    Q_OBJECT

public:
    DropArea(QWidget *parent = 0);

public slots:
    void clear();

signals:
    void changed(const QObject *myObject = 0, const QMimeData *mimeData = 0);
    void clearParameters();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QLabel *label;
};

#endif // DROPAREA_H
