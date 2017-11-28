#ifndef UNIVERSITYVIEW_H
#define UNIVERSITYVIEW_H

#include <QWidget>
#include <queryrunner.h>

class QTableView;
class QPushButton;
class QDialogButtonBox;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;

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
    void getModel();
    void setModel(QSqlQueryModel *model);

private:
    QueryRunner *runner;
    QGroupBox *workField;
    QTableView *universityTableView;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *confirmButton;
    QPushButton *exitButton;
    QDialogButtonBox *buttonBox;
    QHBoxLayout *hLayout;
    QVBoxLayout *internalVLayout;
    QVBoxLayout *externalVLayout;

private slots:
    void createUI();
};

#endif // UNIVERSITYVIEW_H
