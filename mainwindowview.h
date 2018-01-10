#ifndef MAINWINDOWVIEW_H
#define MAINWINDOWVIEW_H

#include <QMainWindow>
#include <QtWidgets>
#include "tableeditorview.h"
#include "droparea.h"
#include "loadnewfileview.h"
#include "mainwindowmodel.h"
#include <QSqlTableModel>
#include <QCheckBox>
#include "queryrunner.h"
#include <QTextEdit>
#include <QHeaderView>
#include <QTableView>

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
class QListView;
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
    void doubleClickClassUpdate(const QModelIndex index);
    void doubleClickTeacherUpdate(const QModelIndex index);
    void checkFields();
    void newFile();
    void back();
    void getData(QString objName, QString containedData);
    void getModel();
    void setClassesModel(QSqlQueryModel *model);
    void setTeachersModel(QSqlQueryModel *model);
    void setDiffModel(QSqlQueryModel *model);
    void getError(QSqlError error);
    void distributeHours();

private:

    QueryRunner *teachersRunner;
    QueryRunner *classesRunner;
    QueryRunner *runner;
    MainWindowModel *mwModel;
    QWidget *parentReference;
    QSqlQueryModel *classModelReference;
    QSqlQueryModel *teacherModelReference;
    QSqlQueryModel *diffModelReference;
    QString query;
    QString receivedID;
    QString receivedName;
    QString receivedShortname;

    DropArea *dropArea;

    QGroupBox *workField;

    QString listViewData;

    QTableView *classesList;
    QTableView *teachersList;

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
    QHBoxLayout *classesColumns;
    QHBoxLayout *teachersColumns;
    QVBoxLayout *classesInfoVLayout;
    QVBoxLayout *teachersInfoVLayout;

    QGroupBox *classesInfoGroupBox;
    QGroupBox *teachersInfoGroupBox;

    QLabel *classesTextEditHeader;
    QLabel *teachersTextEditHeader;

    QTextEdit *classesTextEdit;
    QTextEdit *teachersTextEdit;

    QWidget *myWorkField;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QAction *newFileAction;





    QLineEdit *lecLE;
    QLineEdit *semLE;
    QLineEdit *labLE;
    QLineEdit *contLE;
    QLineEdit *consLE;
    QLineEdit *zachLE;
    QLineEdit *examLE;
    QLineEdit *kursLE;
    QLineEdit *uchPrLE;
    QLineEdit *proizvPrLE;
    QLineEdit *predPrLE;
    QLineEdit *vklLE;
    QLineEdit *obzLE;
    QLineEdit *gekLE;
    QLineEdit *nirsLE;
    QLineEdit *aspLE;

    QPushButton *confirmDistr;

    QLabel *lecLabel;
    QLabel *semLabel;
    QLabel *labLabel;
    QLabel *contLabel;
    QLabel *consLabel;
    QLabel *zachLabel;
    QLabel *examLabel;
    QLabel *kursLabel;
    QLabel *uchPrLabel;
    QLabel *proizvPrLabel;
    QLabel *predPrLabel;
    QLabel *vklLabel;
    QLabel *obzLabel;
    QLabel *gekLabel;
    QLabel *nirsLabel;
    QLabel *aspLabel;



    QGridLayout *distrGrid;

    int chosenClassID = 0;
    int chosenTeacherID = 0;





private slots:
    void createUI();

};

#endif // MAINWINDOW_H
