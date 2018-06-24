#ifndef TABLEEDITORVIEW_H
#define TABLEEDITORVIEW_H

#include <QWidget>
#include <QTabWidget>
#include <QSqlTableModel>
#include "queryrunner.h"
#include "tableeditormodel.h"
#include "customlineedit.h"
#include "customtableview.h"
#include <QComboBox>
#include <QLabel>

#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QToolButton>
#include <QMenuBar>

class TableEditorView : public QWidget
{
    Q_OBJECT
public:
    explicit TableEditorView(QString uID = 0, QString uName = 0, QString uShortname = 0,QWidget *parent = 0);

signals:
    void updateError(QSqlError error);
    void dataBeforeUpdate(QString bData);
public slots:

    void setSmallTablesVisible();
    void setSmallTablesInvisible();

    void getFacultyID();
    void getChairID();
    void getSpecialtyID();

    void createFacultyWidgetUI();
    void createChairWidgetUI();
    void createTeacherWidgetUI();
    void createDisciplineWidgetUI();
    void createSpecialtyWidgetUI();
    void createStreamWidgetUI();

    void changeFacultyAddButtonStyle();
    void changeChairAddButtonStyle();
    void changeTeacherAddButtonStyle();
    void changeDisciplineAddButtonStyle();
    void changeSpecialtyAddButtonStyle();
    void changeStreamAddButtonStyle();

    void checkSize(int index);
    void resizeTable(QTableView *table);
    void getError(QSqlError error);

    void enableFacultyButtons();
    void enableChairButtons();
    void enableTeacherButtons();
    void enableDisciplineButtons();
    void enableSpecialtyButtons();
    void enableStreamButtons();

    void enableGetIDButtons();

    void facultyAddRecord();
    void facultyDeleteRecord();
    void facultyEditRecord();

    void chairAddRecord();
    void chairDeleteRecord();
    void chairEditRecord();

    void openFacultyList();
    void openChairList();
    void openSpecialtyList();

    void teacherAddRecord();
    void teacherDeleteRecord();
    void teacherEditRecord();

    void disciplineAddRecord();
    void disciplineDeleteRecord();
    void disciplineEditRecord();

    void specialtyAddRecord();
    void specialtyDeleteRecord();
    void specialtyEditRecord();

    void streamAddRecord();
    void streamDeleteRecord();
    void streamEditRecord();

    void getFacultyModel();
    void setFacultyModel(QSqlTableModel *model);

    void getChairModel();
    void setChairModel(QSqlTableModel *model);

    void getTeacherModel();
    void setTeacherModel(QSqlTableModel *model);

    void getDisciplineModel();
    void setDisciplineModel(QSqlTableModel *model);

    void getSpecialtyModel();
    void setSpecialtyModel(QSqlTableModel *model);

    void getStreamModel();
    void setStreamModel(QSqlTableModel *model);

    void changedFromData(const QItemSelection &selected);
    void changedToData(const QModelIndex &bIndex);

    void disableFacultyWidgets();
    void disableChairWidgets();
    void disableTeacherWidgets();
    void disableDisciplineWidgets();
    void disableSpecialtyWidgets();
    void disableStreamWidgets();

    void clearAll();

    void enableWidgets();

private:
    QueryRunner *chairRunner;
    QueryRunner *facultyRunner;
    QueryRunner *teacherRunner;
    QueryRunner *disciplineRunner;
    QueryRunner *specialtyRunner;
    QueryRunner *streamRunner;

    QTabWidget *tableTab;
    enum buttonState {INACTIVE,ACTIVE};
    TableEditorModel *teModel;
    QString receivedUID;
    QString receivedName;
    QString receivedShortname;
    QHeaderView *hHeader;

    QSqlTableModel *facultyModelReference;
    QSqlTableModel *chairModelReference;
    QSqlTableModel *teacherModelReference;
    QSqlTableModel *disciplineModelReference;
    QSqlTableModel *specialtyModelReference;
    QSqlTableModel *streamModelReference;

    QWidget *facultyWidget;
    CustomTableView *facultyTable;
    QVBoxLayout *facultyVLayout;
    QHBoxLayout *facultyTopHLayout;
    QHBoxLayout *facultyBotHLayout;
    QLineEdit *facultyName;
    QLineEdit *facultyShortname;
    QPushButton *facultyConfirmAddition;
    QPushButton *facultyAddButton;
    QPushButton *facultyDeleteButton;
    int facultyAddButtonState;
    QString facultyBeforeEditing;
    QString facultyAfterEditing;

    QWidget *chairWidget;
    CustomTableView *chairTable;
    QVBoxLayout *chairVLayout;
    QHBoxLayout *chairTopHLayout;
    QHBoxLayout *chairBotHLayout;
    QLineEdit *chairName;
    QLineEdit *chairShortname;
    CustomLineEdit *chairChooseFaculty;
    QPushButton *chairConfirmAddition;
    QPushButton *chairAddButton;
    QPushButton *chairDeleteButton;
    int chairAddButtonState;
    QString chairBeforeEditing;
    QString chairAfterEditing;

    QWidget *disciplineWidget;
    CustomTableView *disciplineTable;
    QHBoxLayout *disciplineTopHLayout;
    QVBoxLayout *disciplineVLayout;
    QHBoxLayout *disciplineBotHLayout;
    QLineEdit *disciplineName;
    QPushButton *disciplineConfirmAddition;
    QPushButton *disciplineAddButton;
    QPushButton *disciplineDeleteButton;
    int disciplineAddButtonState;
    QString disciplineBeforeEditing;
    QString disciplineAfterEditing;

    QWidget *specialtyWidget;
    CustomTableView *specialtyTable;
    QHBoxLayout *specialtyTopHLayout;
    QVBoxLayout *specialtyVLayout;
    QHBoxLayout *specialtyBotHLayout;
    QLineEdit *specialtyName;
    CustomLineEdit *specialtyChooseFaculty;
    QPushButton *specialtyConfirmAddition;
    QPushButton *specialtyAddButton;
    QPushButton *specialtyDeleteButton;
    int specialtyAddButtonState;
    QString specialtyBeforeEditing;
    QString specialtyAfterEditing;

    QWidget *streamWidget;
    CustomTableView *streamTable;
    QVBoxLayout *streamVLayout;
    QHBoxLayout *streamTopHLayout;
    QHBoxLayout *streamMidHLayout;
    QHBoxLayout *streamBotHLayout;
    CustomLineEdit *streamChooseSpecialty;

    QLineEdit *streamName;
    QLineEdit *streamShortName;
    QLineEdit *streamYear;

    QLineEdit *streamStudents;
    QLineEdit *streamGroups;
    QLineEdit *streamSubgroups;

    QPushButton *streamConfirmAddition;
    QPushButton *streamAddButton;
    QPushButton *streamDeleteButton;
    int streamAddButtonState;
    QString streamBeforeEditing;
    QString streamAfterEditing;


    QWidget *teacherWidget;
    CustomTableView *teacherTable;
    QVBoxLayout *teacherVLayout;
    QHBoxLayout *teacherTopHLayout;
    QHBoxLayout *teacherMidHLayout;
    QHBoxLayout *teacherBotHLayout;
    CustomLineEdit *teacherChooseChair;

    QLineEdit *teacherFirstName;
    QLineEdit *teacherMiddleName;
    QLineEdit *teacherSurname;

    QLineEdit *teacherPost;
    QComboBox *teacherTitle;
    QComboBox *teacherDegree;

    QLabel *titleLabel;
    QLabel *degreeLabel;

    QPushButton *teacherConfirmAddition;
    QPushButton *teacherAddButton;
    QPushButton *teacherDeleteButton;
    int teacherAddButtonState;
    QString teacherBeforeEditing;
    QString teacherAfterEditing;

    QVBoxLayout *vLayout;
    QTableView *getIDTable;
    QPushButton *agree;
    QPushButton *disagree;
    QDialogButtonBox *buttonBox;

};

#endif // TABLEEDITORVIEW_H
