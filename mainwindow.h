#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
class QVBoxLayout;
class QHBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:

    void updateParameters(const QObject *myObject, const QMimeData *mimeData);
    void clearParameters();
    void doubleClickClassUpdate(const QListWidgetItem *myItem);
    void doubleClickTeacherUpdate(const QListWidgetItem *myItem);
    void checkFields();
    void newFile();


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

    QVBoxLayout *externalVLayout;
    QVBoxLayout *internalMiddleVLayout;
    QVBoxLayout *internalLeftVLayout;
    QVBoxLayout *internalRightVLayout;
    QHBoxLayout *externalHLayout;
    QHBoxLayout *internalHLayout;

    QWidget *myWorkField;


};

#endif // MAINWINDOW_H
