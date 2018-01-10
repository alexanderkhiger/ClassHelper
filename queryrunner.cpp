#include "queryrunner.h"

QueryRunner::QueryRunner(QObject *parent) : QObject(parent)
{
    if (QSqlDatabase::contains("dbConnection"))
    {
        db = QSqlDatabase::database("dbConnection");
    }
    else
        db = QSqlDatabase::addDatabase("QMYSQL","dbConnection");
    defaultQuery = new QSqlQuery(db);
    defaultModel = new QSqlQueryModel;
    defaultTableModel = new QSqlTableModel(this,db);
}

void QueryRunner::tryAuth(const QString login, const QString password, const QString database, const QString hostname)
{
    db.setHostName(hostname);
    db.setDatabaseName(database);
    db.setUserName(login);
    db.setPassword(password);
    db.close();
    if (login == "" || password == "" || database == "" || hostname == "")
        emit authError(tr("Необходимо заполнить все поля"));
    else
    {
        db.open();
        if (db.isOpen() == 0)
            emit authError(tr("Ошибка авторизации, проверьте правильность данных"));
        else
        {
            emit authSuccess();
        }
    }
}

QSqlQuery QueryRunner::tryQuery(const QString query, bool isModelNeeded)
{
    if (isModelNeeded)
    {
        defaultModel->setQuery(query,db);
        emit querySuccessReturnModel(defaultModel);
    }

    else
    {
        check = defaultQuery->exec(query);
        if(check)
        {
            emit querySuccess();
        }
        else
        {
            emit queryError(defaultQuery->lastError());
        }
        return *defaultQuery;
    }
}

void QueryRunner::tryTableModel(const QString tableName)
{
    defaultTableModel->setTable(tableName);
    defaultTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    defaultTableModel->select();
    emit returnTableModel(defaultTableModel);
}

