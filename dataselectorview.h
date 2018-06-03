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
    explicit DataSelectorView(QString tableName, QString receivedFacultyName = 0, QString receivedSpecialtyName = 0, QString receivedDisciplineName = 0, QString receivedChairName = 0, QString uID = 0, QWidget *parent = 0, int alwaysInsert = 0);
    QTableView *myTableView;
    QVBoxLayout *externalVLayout;
    QueryRunner *runner;
    QSqlTableModel *facultyModelReference;
    QSqlTableModel *specialtyModelReference;
    QSqlTableModel *disciplineModelReference;
    QSqlTableModel *chairModelReference;
    QHeaderView *hHeader;
    QPushButton *createButton;
    QLabel *topLabel;
    int faculty_id;
    int specialty_id;
    int discipline_id;
    int chair_id;

signals:
    void sendData();

private:
    QString receivedID;
    QString facultyName;
    QString specialtyName;
    QString disciplineName;
    QString chairName;

public slots:
    void setFacultyModel(QSqlTableModel *model);
    void setSpecialtyModel(QSqlTableModel *model);
    void setDisciplineModel(QSqlTableModel *model);
    void setChairModel(QSqlTableModel *model);
    void resizeTable();
    void setData(QModelIndex index);
    void createButtonClicked();
    void createUI();
};

#endif // DATASELECTORVIEW_H
