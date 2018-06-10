#ifndef LOADNEWFILEMODEL_H
#define LOADNEWFILEMODEL_H

#include <QObject>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDate>
#include <QTimer>
#include <QEventLoop>
#include "queryrunner.h"
#include "dataselectorview.h"
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QList>
#include <QSqlError>
#include <QMessageBox>
#include <QThread>


class LoadNewFileModel : public QObject
{
    Q_OBJECT
public:
    explicit LoadNewFileModel(QString uID, int skipAll = 0, QObject *parent = 0, int year = 0);
    QString rtfToPlainText(const QString rtf);
    QueryRunner *runner;

signals:
    void sendProgress(const int percentage);
    void processingFinished(double expectedTotal, double countedTotal);
    void sendInformation(const QString info);
    void committed();

public slots:
    void processData(const QString dir);
    void convertRtf(const QString dir);
    void setData(QList<double> list);
    void commit();

private:
    int isSkipping;
    QList<double> myList;
    QString receivedID;
    QString singleLine;
    QString query;
    QStringList streamInfo;
    DataSelectorView *mySelector;
    QString facultyName;
    int year;
    QString specialty;
    int studentsCount;
    int groupsCount;
    int subgroupsCount;
    QStringList disciplineInfo;

    QString disciplineName;
    double lectureMultiplier;
    double seminarMultiplier;
    double labMultiplier;
    double kont; // consider rename
    double refr; // consider rename

    QStringList semesterInfo;

    int semester;
    int weeks;
    int individual;

    double lectureHours;
    double seminarHours;
    double labHours;

    double consultTek; // consider rename
    double consultEx; // consider rename

    double kontHours; // consider rename
    double refrHours; // consider rename

    double zExamHours; // consider rename
    double examHours; // consider rename
    double gExamHours; // consider rename

    double practiceHours;
    double qualHours;
    double totalHours;

    int god;




};

#endif // LOADNEWFILELOGIC_H
