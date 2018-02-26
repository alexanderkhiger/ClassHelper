#ifndef OUTPUTTOFILEVIEW_H
#define OUTPUTTOFILEVIEW_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QHeaderView>
#include <QSqlQueryModel>
#include "queryrunner.h"
#include <QFile>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlDatabase>

class OutputToFileView : public QWidget
{
    Q_OBJECT
public:
    explicit OutputToFileView(QString uID, QWidget *parent = 0);

public slots:

    void setTeachersModel(QSqlQueryModel *model);
    void setChosenTeacher(QModelIndex index);
    void process();

private:

    QModelIndex savedIndex;
    QTableView *teachersList;
    QLineEdit *chosenTeacher;
    QLineEdit *fileSavePath;
    QPushButton *confirmOutput;
    QComboBox *outputFormat;
    QVBoxLayout *internalVBoxLayout;
    QHBoxLayout *externalHBoxLayout;

    QSqlQueryModel *teachersModel;
    QueryRunner *runner;
    QString receivedID;

};

#endif // OUTPUTTOFILEVIEW_H
