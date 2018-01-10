#ifndef DATASELECTORVIEW_H
#define DATASELECTORVIEW_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QLabel>
#include "queryrunner.h"

class DataSelectorView : public QWidget
{
    Q_OBJECT
public:
    explicit DataSelectorView(QString tableName, QString receivedFacultyName = 0, QString receivedSpecialtyName = 0, QString receivedDisciplineName = 0, QString uID = 0, QWidget *parent = nullptr);
    QTableView *myTableView;
    QVBoxLayout *externalVLayout;
    QueryRunner *runner;
    QSqlTableModel *facultyModelReference;
    QSqlTableModel *specialtyModelReference;
    QSqlTableModel *disciplineModelReference;
    QHeaderView *hHeader;
    QPushButton *createButton;
    QLabel *topLabel;
    int faculty_id = 0;
    int specialty_id = 0;
    int discipline_id = 0;

signals:
    void sendData();

private:
    QString receivedID;
    QString facultyName;
    QString specialtyName;
    QString disciplineName;

public slots:
    void setFacultyModel(QSqlTableModel *model);
    void setSpecialtyModel(QSqlTableModel *model);
    void setDisciplineModel(QSqlTableModel *model);
    void resizeTable();
    void setData(QModelIndex index);
    void createButtonClicked();
    void createUI();
};

#endif // DATASELECTORVIEW_H
