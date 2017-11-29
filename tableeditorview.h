#ifndef TABLEEDITORVIEW_H
#define TABLEEDITORVIEW_H

#include <QWidget>
#include <QtWidgets>
#include <QTabWidget>
#include <QSqlTableModel>
#include "queryrunner.h"
#include "tableeditormodel.h"

class QLineEdit;
class QPushButton;
class QTableView;
class QDialogButtonBox;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
class QHeaderView;
class QMessageBox;

class TableEditorView : public QWidget
{
    Q_OBJECT
public:
    explicit TableEditorView(QString uID, QString uName, QString uShortname,QWidget *parent = nullptr);

signals:

public slots:
    void createFacultyWidgetUI();
    void createChairWidgetUI();
    void createTeacherWidgetUI();

    void changeFacultyAddButtonStyle();
    void changeChairAddButtonStyle();
    void changeTeacherAddButtonStyle();

    void resizeTable(QTableView *table);
    void getError(QSqlError error);

    void enableFacultyButtons();
    void enableChairButtons();
    void enableTeacherButtons();

    void facultyAddRecord();
    void facultyDeleteRecord();

    void chairAddRecord();
    void chairDeleteRecord();

    void teacherAddRecord();
    void teacherDeleteRecord();

    void getFacultyModel();
    void setFacultyModel(QSqlTableModel *model);

    void getChairModel();
    void setChairModel(QSqlTableModel *model);

    void getTeacherModel();
    void setTeacherModel(QSqlTableModel *model);
private:
    QueryRunner *chairRunner;
    QueryRunner *facultyRunner;
    QueryRunner *teacherRunner;

    QTabWidget *tableTab;
    enum buttonState {INACTIVE,ACTIVE};
    TableEditorModel *teModel;
    QString receivedUID;
    QString receivedName;
    QString receivedShortname;
    QHeaderView *hHeader;
    QSqlTableModel *facultyModelReference = Q_NULLPTR;
    QSqlTableModel *chairModelReference = Q_NULLPTR;
    QSqlTableModel *teacherModelReference = Q_NULLPTR;

    QWidget *facultyWidget;
    QTableView *facultyTable;
    QVBoxLayout *facultyVLayout;
    QHBoxLayout *facultyTopHLayout;
    QHBoxLayout *facultyBotHLayout;
    QSqlTableModel *facultyModel;
    QLineEdit *facultyName;
    QLineEdit *facultyShortname;
    QPushButton *facultyConfirmAddition;
    QPushButton *facultyAddButton;
    QPushButton *facultyDeleteButton;
    int facultyAddButtonState = buttonState::INACTIVE;

    QWidget *chairWidget;
    QTableView *chairTable;
    QVBoxLayout *chairVLayout;
    QHBoxLayout *chairTopHLayout;
    QHBoxLayout *chairBotHLayout;
    QSqlTableModel *chairModel;
    QLineEdit *chairName;
    QLineEdit *chairShortname;
    QLineEdit *chairFacultyID;
    QPushButton *chairConfirmAddition;
    QPushButton *chairAddButton;
    QPushButton *chairDeleteButton;
    int chairAddButtonState = buttonState::INACTIVE;

    QWidget *teacherWidget;
    QTableView *teacherTable;
    QVBoxLayout *teacherVLayout;
    QHBoxLayout *teacherTopHLayout;
    QHBoxLayout *teacherMidHLayout;
    QHBoxLayout *teacherBotHLayout;
    QSqlTableModel *teacherModel;

    QLineEdit *teacherFirstName;
    QLineEdit *teacherMiddleName;
    QLineEdit *teacherSurname;

    QLineEdit *teacherTitle;
    QLineEdit *teacherDegree;
    QLineEdit *teacherPost;
    QLineEdit *teacherChairID;


    QPushButton *teacherConfirmAddition;
    QPushButton *teacherAddButton;
    QPushButton *teacherDeleteButton;
    int teacherAddButtonState = buttonState::INACTIVE;

    QVBoxLayout *vLayout;


};

#endif // TABLEEDITORVIEW_H
