#ifndef MAINWINDOWVIEW_H
#define MAINWINDOWVIEW_H

#include <QMainWindow>
#include <QtWidgets>
#include "tableeditorview.h"
#include "droparea.h"
#include "loadnewfileview.h"
#include "mainwindowmodel.h"

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
class QMenu;
class QAction;

class MainWindowView : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindowView(QString uID, QString uName, QString uShortname, QWidget *ref, QWidget *parent = nullptr);

public slots:

    void clearParameters();
    void doubleClickClassUpdate(const QListWidgetItem *myItem);
    void doubleClickTeacherUpdate(const QListWidgetItem *myItem);
    void checkFields();
    void newFile();
    void back();
    void getData(QString objName, QString containedData);

private:

    MainWindowModel *mwModel;
    QWidget *parentReference;

    QString receivedID;
    QString receivedName;
    QString receivedShortname;

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
    QPushButton *backButton;
    QDialogButtonBox *buttonBox;

    QVBoxLayout *externalVLayout;
    QVBoxLayout *internalMiddleVLayout;
    QVBoxLayout *internalLeftVLayout;
    QVBoxLayout *internalRightVLayout;
    QHBoxLayout *externalHLayout;
    QHBoxLayout *internalHLayout;

    QWidget *myWorkField;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QAction *newFileAction;

private slots:
    void createUI();

};

#endif // MAINWINDOW_H
