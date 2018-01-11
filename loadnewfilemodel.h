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

class LoadNewFileModel : public QObject
{
    Q_OBJECT
public:
    explicit LoadNewFileModel(QString uID, QObject *parent = 0);
    QString rtfToPlainText(const QString rtf);

signals:
    void sendProgress(const int percentage);
    void processingFinished();
    void sendInformation(const QString info);

public slots:
    void processData(const QString dir);
    void convertRtf(const QString dir);

private:
    QString receivedID;
    QString singleLine;
    QueryRunner *runner;
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




};

#endif // LOADNEWFILELOGIC_H
