#ifndef MAINWINDOWVIEW_H
#define MAINWINDOWVIEW_H

#include <QMainWindow>
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
#include <QModelIndex>
#include "customdoublevalidator.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QMimeData>
#include <QPushButton>
#include <QTableWidget>
#include "droparea.h"
#include <QListWidget>
#include <QString>
#include <QGroupBox>
#include <QWidget>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QListView>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>


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

    CustomDoubleValidator *val1;
    CustomDoubleValidator *val2;
    CustomDoubleValidator *val3;
    CustomDoubleValidator *val4;
    CustomDoubleValidator *val5;
    CustomDoubleValidator *val6;
    CustomDoubleValidator *val7;
    CustomDoubleValidator *val8;
    CustomDoubleValidator *val9;
    CustomDoubleValidator *val10;
    CustomDoubleValidator *val11;
    CustomDoubleValidator *val12;
    CustomDoubleValidator *val13;
    CustomDoubleValidator *val14;
    CustomDoubleValidator *val15;
    CustomDoubleValidator *val16;

    QueryRunner *teachersRunner;
    QueryRunner *classesRunner;
    QueryRunner *runner;
    MainWindowModel *mwModel;
    QModelIndex savedTeacherIndex = QModelIndex();
    QModelIndex savedClassIndex = QModelIndex();
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
