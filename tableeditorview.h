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
    virtual bool TableEditorView::eventFilter(QObject *obj, QEvent *event);



signals:
    void updateError(QSqlError error);
    void dataBeforeUpdate(QString bData);
public slots:

    void setSmallTablesVisible();
    void setSmallTablesInvisible();
    void getFacultyID();
    void getChairID();

    void createFacultyWidgetUI();
    void createChairWidgetUI();
    void createTeacherWidgetUI();

    void changeFacultyAddButtonStyle();
    void changeChairAddButtonStyle();
    void changeTeacherAddButtonStyle();

    void checkSize(int index);
    void resizeTable(QTableView *table);
    void getError(QSqlError error);

    void enableFacultyButtons();
    void enableChairButtons();
    void enableTeacherButtons();
    void enableGetIDButtons();

    void facultyAddRecord();
    void facultyDeleteRecord();
    void facultyEditRecord();

    void chairAddRecord();
    void chairDeleteRecord();
    void chairEditRecord();

    void openFacultyList();
    void openChairList();

    void teacherAddRecord();
    void teacherDeleteRecord();
    void teacherEditRecord();

    void getFacultyModel();
    void setFacultyModel(QSqlTableModel *model);

    void getChairModel();
    void setChairModel(QSqlTableModel *model);

    void getTeacherModel();
    void setTeacherModel(QSqlTableModel *model);

    void changedFromData(const QItemSelection &selected);
    void changedToData(const QModelIndex &bIndex);

    void disableFacultyWidgets();
    void disableChairWidgets();
    void disableTeacherWidgets();

    void enableWidgets();

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
    QSqlTableModel *facultyModelReference;
    QSqlTableModel *chairModelReference;
    QSqlTableModel *teacherModelReference;

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
