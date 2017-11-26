#ifndef LOADNEWFILELOGIC_H
#define LOADNEWFILELOGIC_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDate>

class LoadNewFile;
class QString;
class QStringList;

class LoadNewFileLogic : public QObject
{
    Q_OBJECT
public:
    explicit LoadNewFileLogic(QObject *parent = nullptr);

signals:
    void sendProgress(const int percentage);
    void processingFinished();
    void sendInformation(const QString info);

public slots:
    void processData(const QString dir);

private:

    QString singleLine;

    QStringList streamInfo;

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
