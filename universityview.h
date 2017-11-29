#ifndef UNIVERSITYVIEW_H
#define UNIVERSITYVIEW_H

#include <QWidget>
#include <QtWidgets>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

#include <queryrunner.h>
#include <universitymodel.h>
#include <mainwindow.h>

class QTableView;
class QPushButton;
class QDialogButtonBox;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
class QHeaderView;
class QLineEdit;

class UniversityView : public QWidget
{
    Q_OBJECT
public:
    explicit UniversityView(QWidget *parent = nullptr);

signals:

public slots:
    void choiceConfirmed();
    void enableButtons();
    void deleteRecord();
    void addRecord();
    void changeAddButtonStyle();
    void getModel();
    void setModel(QSqlTableModel *model);
    void resizeTable();
    void getError(QSqlError error);

private:
    QueryRunner *runner;
    UniversityModel *uModel;
    QGroupBox *workField;
    QTableView *universityTableView;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *confirmButton;
    QPushButton *exitButton;
    QDialogButtonBox *buttonBox;
    QHBoxLayout *topHLayout;
    QHBoxLayout *botHLayout;
    QVBoxLayout *internalVLayout;
    QVBoxLayout *externalVLayout;
    QHeaderView *hHeader;
    QSqlTableModel *modelReference = Q_NULLPTR;
    QLineEdit *nameField;
    QLineEdit *shortnameField;
    QPushButton *confirmAddition;
    enum buttonState {INACTIVE,ACTIVE};
    int addButtonState = buttonState::INACTIVE;

private slots:
    void createUI();
};

#endif // UNIVERSITYVIEW_H
