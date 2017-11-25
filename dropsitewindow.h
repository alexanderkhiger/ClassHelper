
#ifndef DROPSITEWINDOW_H
#define DROPSITEWINDOW_H

#include <QWidget>

class QDialogButtonBox;
class QLabel;
class QMimeData;
class QPushButton;
class QTableWidget;
class DropArea;
class QListWidget;
class QString;
class QGroupBox;
class QWidget;
class QLineEdit;
class QListWidgetItem;
class QDebug;

class DropSiteWindow : public QWidget
{
    Q_OBJECT

public:
    DropSiteWindow();

public slots:

    void updateParameters(const QObject *myObject, const QMimeData *mimeData);
    void clearParameters();
    void doubleClickClassUpdate(const QListWidgetItem *myItem);
    void doubleClickTeacherUpdate(const QListWidgetItem *myItem);
    void checkFields();


private:
    DropArea *dropArea;

    QGroupBox *workField;

    QString listViewData;

    QListWidget *classesList;
    QListWidget *teachersList;

    QLineEdit *chosenClass;
    QLineEdit *chosenTeacher;

    QLabel *classesHeader;
    QLabel *teachersHeader;

    QPushButton *clearButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
};

#endif // DROPSITEWINDOW_H
