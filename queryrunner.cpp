#include "queryrunner.h"
#include <QDebug>

QueryRunner::QueryRunner(QObject *parent) : QObject(parent)
{
    defaultModel = new QSqlQueryModel;
}

void QueryRunner::tryAuth(const QString login, const QString password, const QString database, const QString hostname)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostname);
    db.setDatabaseName(database);
    db.setUserName(login);
    db.setPassword(password);
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

void QueryRunner::tryQuery(const QString query, bool isModelNeeded)
{
    if (isModelNeeded)
    {
        defaultModel->setQuery(query);
        emit querySuccessReturnModel(defaultModel);
    }

    else
    {
        check = defaultQuery.exec(query);
        if(check)
        {
            emit querySuccess();
        }
        else
        {
            emit queryError(defaultQuery.lastError());
        }
    }
}
