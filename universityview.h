#ifndef UNIVERSITYVIEW_H
#define UNIVERSITYVIEW_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QDebug>

#include <queryrunner.h>
#include <universitymodel.h>
#include "customtableview.h"
#include <mainwindowview.h>

#include <QTableView>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLineEdit>

class UniversityView : public QWidget
{
    Q_OBJECT
public:
    explicit UniversityView(QWidget *parent = 0);
//    virtual bool UniversityView::eventFilter(QObject *obj, QEvent *event);

signals:
    void updateError(QSqlError error);
public slots:
    void choiceConfirmed();
    void enableButtons();
    void deleteRecord();
    void addRecord();
    void editRecord();
    void changeAddButtonStyle();
    void getModel();
    void setModel(QSqlTableModel *model);
    void resizeTable();
    void getError(QSqlError error);
    void changedFrom(const QItemSelection &selected);
    void changedTo(const QModelIndex &bIndex);
    void turnOffButtons();
    void turnOnButtons();

private:
    QueryRunner *runner;
    UniversityModel *uModel;
    QGroupBox *workField; //
    CustomTableView *universityTableView; //
    QPushButton *addButton; //
    QPushButton *deleteButton; //
    QPushButton *confirmButton; //
    QPushButton *exitButton;
    QDialogButtonBox *buttonBox;
    QHBoxLayout *topHLayout;
    QHBoxLayout *botHLayout;
    QVBoxLayout *internalVLayout;
    QVBoxLayout *externalVLayout;
    QHeaderView *hHeader;
    QSqlTableModel *modelReference;
    QLineEdit *nameField;
    QLineEdit *shortnameField;
    QPushButton *confirmAddition;
    enum buttonState {INACTIVE,ACTIVE};
    int addButtonState;
    QString universityBeforeUpdate;
    QString universityAfterUpdate;

private slots:
    void createUI();
};

#endif // UNIVERSITYVIEW_H
