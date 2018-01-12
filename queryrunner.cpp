#include "queryrunner.h"

QueryRunner::QueryRunner(QObject *parent) : QObject(parent)
{
    check = 0;

    if (QSqlDatabase::contains("dbConnection"))
    {
        db = QSqlDatabase::database("dbConnection");
    }
    else
        db = QSqlDatabase::addDatabase("QMYSQL","dbConnection");
//    defaultQuery = new QSqlQuery(db);
    defaultQuery = QSqlQuery(db);
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

int QueryRunner::tryQuery(const QString query, bool isModelNeeded, bool isDataNeeded)
{
    QList<double> *myList = new QList<double>;
    if (isModelNeeded)
    {
        defaultModel->setQuery(query,db);
        emit querySuccessReturnModel(defaultModel);
    }

    else
    {
        check = defaultQuery.exec(query);
        if(check)
        {
            emit querySuccess();
            if (isDataNeeded)
            {
                while (defaultQuery.next())
                {
                    for (int i = 0; i < defaultQuery.record().count(); i++)
                    {
                        myList->append(defaultQuery.value(i).toDouble());
                    }
                }
                emit returnValues(*myList);
            }
        }
        else
        {
            emit queryError(defaultQuery.lastError());
        }
        return defaultQuery.size();
    }
    return -1;
}


void QueryRunner::tryTableModel(const QString tableName)
{
    defaultTableModel->setTable(tableName);
    defaultTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    defaultTableModel->select();
    emit returnTableModel(defaultTableModel);
}

