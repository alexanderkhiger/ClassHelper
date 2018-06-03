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
#include "outputtofileview.h"
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
#include <QToolButton>
#include <QToolBar>
#include <customquerymodel.h>

class MainWindowView : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindowView(QString uID, QString uName, QString uShortname, QWidget *ref, QWidget *parent = 0);

public slots:

    void clearParameters();
    void doubleClickClassUpdate(const QModelIndex index);
    void doubleClickTeacherUpdate(const QModelIndex index);
    void updateDataForSelectedClass();
    void checkFields();
    void newFile();
    void back();
    void getData(QString objName, QString containedData);
    void setClassesModel(CustomQueryModel *model);
    void setTeachersModel(CustomQueryModel *model);
    void setDiffModel(QSqlQueryModel *model);
    void getError(QSqlError error);
    void distributeHours();
    void setData(QList<double> list);
    void distributeDiscipline();
    void distributeAll(int classID, int teacherID, QString className, QString teacherName);
    void changeCurrentSemester();
    void setTableEditorAsCentral();
    void setWorkFieldAsCentral();
    void chooseColumns(int state);
    void outputToFile();
    void toggleClassesChecks();
    void toggleTeachersChecks();
    void classToTeacherDrag(QModelIndex index);
    void teacherToClassDrag(QModelIndex index);
    void receiveModels();

private:

    QString eventSource;

    QToolBar *leftBar;
    QToolBar *topBar;
    QToolButton *clearTool;
    QToolButton *changeSemesterTool;
    QList<double> totalHoursDistributedList;
    QList<double> hoursDistributedForSelectedList;
    QList<double> totalHoursLeftList;
    QList<double> myList;
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
    QModelIndex savedTeacherIndex;
    QModelIndex savedClassIndex;
    QWidget *parentReference;

    QSqlQueryModel *classModelReference;
    QSqlQueryModel *teacherModelReference;
    QSqlQueryModel *diffModelReference;

    CustomQueryModel *customClass;
    CustomQueryModel *customTeacher;

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

//    QPushButton *clearButton;
//    QPushButton *backButton;
//    QDialogButtonBox *buttonBox;

    QVBoxLayout *externalVLayout;
    QVBoxLayout *internalMiddleVLayout;
    QVBoxLayout *internalLeftVLayout;
    QVBoxLayout *internalRightVLayout;
    QHBoxLayout *externalHLayout;
    QHBoxLayout *internalHLayout;

    QVBoxLayout *classesInfoVLayout;
    QVBoxLayout *teachersInfoVLayout;

    QVBoxLayout *classesColumns;
    QVBoxLayout *teachersColumns;

    QCheckBox *teachersIDCheck;
    QCheckBox *teachersChairCheck;
    QCheckBox *teachersDegreeCheck;
    QCheckBox *teachersTitleCheck;
    QCheckBox *teachersPostCheck;

    QCheckBox *classesIDCheck;

    QGroupBox *classesInfoGroupBox;
    QGroupBox *teachersInfoGroupBox;

    QLabel *classesTextEditHeader;
    QLabel *teachersTextEditHeader;

    QPushButton *classesShowChecks;
    QPushButton *teachersShowChecks;


    QWidget *myWorkField;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QAction *newFileAction;
    QAction *outputAction;
    QAction *tableEditorAction;
    QAction *backToUniversityList;




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

    QToolButton *lecButtonAdd;
    QToolButton *semButtonAdd;
    QToolButton *labButtonAdd;
    QToolButton *contButtonAdd;
    QToolButton *consButtonAdd;
    QToolButton *zachButtonAdd;
    QToolButton *examButtonAdd;
    QToolButton *kursButtonAdd;
    QToolButton *uchPrButtonAdd;
    QToolButton *proizvPrButtonAdd;
    QToolButton *predPrButtonAdd;
    QToolButton *vklButtonAdd;
    QToolButton *obzButtonAdd;
    QToolButton *gekButtonAdd;
    QToolButton *nirsButtonAdd;
    QToolButton *aspButtonAdd;

    QLabel *lecLabelAdd;
    QLabel *semLabelAdd;
    QLabel *labLabelAdd;
    QLabel *contLabelAdd;
    QLabel *consLabelAdd;
    QLabel *zachLabelAdd;
    QLabel *examLabelAdd;
    QLabel *kursLabelAdd;
    QLabel *uchPrLabelAdd;
    QLabel *proizvPrLabelAdd;
    QLabel *predPrLabelAdd;
    QLabel *vklLabelAdd;
    QLabel *obzLabelAdd;
    QLabel *gekLabelAdd;
    QLabel *nirsLabelAdd;
    QLabel *aspLabelAdd;

    QToolButton *lecButtonRemove;
    QToolButton *semButtonRemove;
    QToolButton *labButtonRemove;
    QToolButton *contButtonRemove;
    QToolButton *consButtonRemove;
    QToolButton *zachButtonRemove;
    QToolButton *examButtonRemove;
    QToolButton *kursButtonRemove;
    QToolButton *uchPrButtonRemove;
    QToolButton *proizvPrButtonRemove;
    QToolButton *predPrButtonRemove;
    QToolButton *vklButtonRemove;
    QToolButton *obzButtonRemove;
    QToolButton *gekButtonRemove;
    QToolButton *nirsButtonRemove;
    QToolButton *aspButtonRemove;

    QLabel *lecLabelRemove;
    QLabel *semLabelRemove;
    QLabel *labLabelRemove;
    QLabel *contLabelRemove;
    QLabel *consLabelRemove;
    QLabel *zachLabelRemove;
    QLabel *examLabelRemove;
    QLabel *kursLabelRemove;
    QLabel *uchPrLabelRemove;
    QLabel *proizvPrLabelRemove;
    QLabel *predPrLabelRemove;
    QLabel *vklLabelRemove;
    QLabel *obzLabelRemove;
    QLabel *gekLabelRemove;
    QLabel *nirsLabelRemove;
    QLabel *aspLabelRemove;


    QGridLayout *distrGrid;
    QGridLayout *hoursLeftGrid;
    QGridLayout *hoursDistrGrid;

    int chosenClassID;
    int chosenTeacherID;
    int chosenSemester;
    int currentSemester;

    TableEditorView *tableEditor;
    LoadNewFileView *loadNewFile;
    OutputToFileView *outFile;

private slots:
    void createUI();
    void createWorkfieldWidget();

};

#endif // MAINWINDOW_H
