#ifndef QUERYRUNNER_H
#define QUERYRUNNER_H

#include <QObject>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlRecord>
#include <QList>

#include <QFile>
#include <QPrintDialog>
#include <QPrinter>
#include <QFileDialog>
#include <QTextDocument>

class QueryRunner : public QObject
{
    Q_OBJECT
public:
    explicit QueryRunner(QObject *parent = 0);
    QSqlDatabase db;

signals:
    void authError(const QString error);
    void authSuccess();
    void queryError(const QSqlError error);
    void querySuccess();
    void querySuccessReturnModel(QSqlQueryModel *model);
    void returnTableModel(QSqlTableModel *model);
    void returnValues(QList<double> myList);
    void returnHtml(QString htmlData);

public slots:
    int tryQuery(const QString query, bool isModelNeeded = 0, bool isDataNeeded = 0);
    void tryAuth(const QString login, const QString password, const QString database, const QString hostname);
    void tryTableModel(const QString tableName);
    void outputToFile(int teacherID, int uID, QString name);

private:
    int check;
    QSqlQuery defaultQuery;
    QSqlQueryModel *defaultModel;
    QSqlTableModel *defaultTableModel;
};

#endif // QUERYRUNNER_H
