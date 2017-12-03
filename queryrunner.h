#ifndef QUERYRUNNER_H
#define QUERYRUNNER_H

#include <QObject>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QDebug>

class QueryRunner : public QObject
{
    Q_OBJECT
public:
    explicit QueryRunner(QObject *parent = nullptr);
    QSqlDatabase db;

signals:
    void authError(const QString error);
    void authSuccess();
    void queryError(const QSqlError error);
    void querySuccess();
    void querySuccessReturnModel(QSqlQueryModel *model);
    void returnTableModel(QSqlTableModel *model);

public slots:
    void tryQuery(const QString query, bool isModelNeeded = 0);
    void tryAuth(const QString login, const QString password, const QString database, const QString hostname);
    void tryTableModel(const QString tableName);

private:
    int check = 0;
    QSqlQuery *defaultQuery;
    QSqlQueryModel *defaultModel;
    QSqlTableModel *defaultTableModel;
};

#endif // QUERYRUNNER_H
