#ifndef DISTRIBUTIONVIEW_H
#define DISTRIBUTIONVIEW_H

#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include "tableeditorview.h"
#include "loadnewfileview.h"
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
#include <QComboBox>

class DistributionView : public QWidget
{
    Q_OBJECT
public:
    explicit DistributionView(QString uID = 0, QString uName = 0, QString uShortname = 0,QWidget *parent = 0);

signals:
    void semesterChanged(int semester);

public slots:
    void clearParameters();
    void doubleClickClassUpdate(const QModelIndex index);
    void doubleClickTeacherUpdate(const QModelIndex index);
    void updateDataForSelectedClass();
    void checkFields();
    void getData(QString objName, QString containedData);
    void setClassesModel(CustomQueryModel *model);
    void setTeachersModel(CustomQueryModel *model);
    void getError(QSqlError error);
    void distributeHours();
    void setData(QList<double> list);
    void distributeDiscipline();
    void distributeAll(int classID, int teacherID, QString className, QString teacherName);
    void chooseColumns(int state);
    void toggleClassesChecks();
    void toggleTeachersChecks();
    void classToTeacherDrag(QModelIndex index);
    void teacherToClassDrag(QModelIndex index);
    void receiveModels();
    void getChairList();
    void getYearList();
    void setChairModel(CustomQueryModel *model);
    void filterTeachers();
    void setYearModel(CustomQueryModel *model);
    void changeCurrentSemester();

private:
    QString eventSource;
    QComboBox *chairComboBox;
    QComboBox *yearComboBox;
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
    QueryRunner *chairRunner;
    QueryRunner *yearRunner;
    QModelIndex savedTeacherIndex;
    QModelIndex savedClassIndex;

    QSqlQueryModel *classModelReference;
    QSqlQueryModel *teacherModelReference;
    QSqlQueryModel *diffModelReference;

    CustomQueryModel *customClass;
    CustomQueryModel *customTeacher;
    CustomQueryModel *customChair;
    CustomQueryModel *customYear;

    QString query;
    QString receivedID;
    QString receivedName;
    QString receivedShortname;

    QGroupBox *workField;

    QString listViewData;

    QTableView *classesList;
    QTableView *teachersList;

    QLineEdit *chosenClass;
    QLineEdit *chosenTeacher;

    QLabel *classesHeader;
    QLabel *teachersHeader;

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
    QCheckBox *teachersHoursCheck;

    QComboBox *teachersFilterComboBox;
    QLineEdit *teachersHoursEdit;
    QHBoxLayout *teachersFilterLayout;

    QCheckBox *classesIDCheck;
    QCheckBox *classesUndistributedCheck;

    QGroupBox *classesInfoGroupBox;
    QGroupBox *teachersInfoGroupBox;

    QLabel *classesTextEditHeader;
    QLabel *teachersTextEditHeader;

    QPushButton *classesShowChecks;
    QPushButton *teachersShowChecks;

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
    int chosenYear;

    bool showOnlyUndistributedClasses;
    bool filterTeachersHours;

private slots:
    void createUI();
};

#endif // DISTRIBUTIONVIEW_H
