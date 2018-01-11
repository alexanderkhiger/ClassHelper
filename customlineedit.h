#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include <QWidget>
#include <QToolButton>
#include <QStyle>
#include <QLabel>


class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CustomLineEdit(QWidget *ref);
    QToolButton *chooseButton;

protected:
    void resizeEvent(QResizeEvent *);

signals:
    void needInfo();

private slots:
    void updateClearButton(const QString text);

private:
    QToolButton *clearButton;
    QWidget *parentReference;
};


#endif // CUSTOMLINEEDIT_H
