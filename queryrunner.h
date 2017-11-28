#ifndef QUERYRUNNER_H
#define QUERYRUNNER_H

#include <QObject>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

class QueryRunner : public QObject
{
    Q_OBJECT
public:
    explicit QueryRunner(QObject *parent = nullptr);

signals:
    void authError(const QString error);
    void authSuccess();
    void queryError(const QSqlError error);
    void querySuccess();
    void querySuccessReturnModel(QSqlQueryModel *model);

public slots:
    void tryQuery(const QString query, bool isModelNeeded = 0);
    void tryAuth(const QString login, const QString password, const QString database, const QString hostname);

private:
    int check = 0;
    QSqlQuery defaultQuery;
    QSqlQueryModel *defaultModel;
};

#endif // QUERYRUNNER_H
