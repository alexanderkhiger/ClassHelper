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

void QueryRunner::outputToFile(int teacherID, QString path)
{
    QFile startFile("start.htm");
    if (!startFile.open(QIODevice::ReadOnly|QIODevice::Text)) return;
    QString text = startFile.readAll();


    QFile middleFile("middle.htm");
    if (!middleFile.open(QIODevice::ReadOnly|QIODevice::Text)) return;


    QString queryStr = QString("SELECT "
    "fakultet.nazvanie_fakulteta,"
    "potok.kurs,"
    "specialnost.nazvanie_spec,"
    "potok.kolvo_studentov,"
    "potok.kolvo_grupp,"
    "potok.kolvo_podgrupp,"
    "disciplina.nazvanie_discipliny,"
    "zanyatost.semestr,"
    "zanyatost.nedeli,"
    "sum(raspredelenie.lekcii_chasov),sum(raspredelenie.seminary_chasov),sum(raspredelenie.lab_chasov),"
    "sum(raspredelenie.kontrol_chasov),sum(raspredelenie.konsultacii_chasov),sum(raspredelenie.zachet_chasov),"
    "sum(raspredelenie.ekzamen_chasov),sum(raspredelenie.kursovie_chasov),sum(raspredelenie.ucheb_praktika_chasov),"
    "sum(raspredelenie.proizv_praktika_chasov),sum(raspredelenie.preddipl_praktika_chasov),sum(raspredelenie.vkl_chasov),"
    "sum(raspredelenie.obz_lekcii_chasov),sum(raspredelenie.gek_chasov),sum(raspredelenie.nirs_chasov),sum(raspredelenie.asp_dokt_chasov) "
    "FROM raspredelenie "
    "LEFT JOIN zanyatost on raspredelenie.id_zanyatosti = zanyatost.id_zapisi "
    "LEFT JOIN disciplina on zanyatost.id_discipliny = disciplina.id_discipliny "
    "LEFT JOIN potok on zanyatost.id_potoka = potok.id_potoka "
    "LEFT JOIN specialnost on potok.id_spec = specialnost.id_spec "
    "LEFT JOIN fakultet on specialnost.id_fakulteta = fakultet.id_fakulteta "
    "where id_prep = %1 group by id_zanyatosti").arg(teacherID);

    defaultQuery.exec(queryStr);
    qDebug () << defaultQuery.size();

    for (int i = 0; i < defaultQuery.size(); i++)
    {
        defaultQuery.next();
        text += middleFile.readAll();
        middleFile.reset();

        text.replace(QString("%faculty%"),defaultQuery.value(0).toString());
        text.replace(QString("%year%"),defaultQuery.value(1).toString());
        text.replace(QString("%specialty%"),defaultQuery.value(2).toString());
        text.replace(QString("%students%"),defaultQuery.value(3).toString());
        text.replace(QString("%groups%"),defaultQuery.value(4).toString());
        text.replace(QString("%subgroups%"),defaultQuery.value(5).toString());
        text.replace(QString("%discipline%"),defaultQuery.value(6).toString());
        text.replace(QString("%semester%"),defaultQuery.value(7).toString());
        text.replace(QString("%weeks%"),defaultQuery.value(8).toString());
        text.replace(QString("%ind%"),"-");
        text.replace(QString("%lec%"),defaultQuery.value(9).toString());
        text.replace(QString("%sem%"),defaultQuery.value(10).toString());
        text.replace(QString("%lab%"),defaultQuery.value(11).toString());
        text.replace(QString("%ctek%"),defaultQuery.value(12).toString());
        text.replace(QString("%cekz%"),defaultQuery.value(13).toString());
        text.replace(QString("%cont%"),defaultQuery.value(14).toString());
        text.replace(QString("%refr%"),defaultQuery.value(15).toString());
        text.replace(QString("%zach%"),defaultQuery.value(16).toString());
        text.replace(QString("%ekz%"),defaultQuery.value(17).toString());
        text.replace(QString("%gek%"),defaultQuery.value(18).toString());
        text.replace(QString("%prak%"),defaultQuery.value(19).toString());
        text.replace(QString("%dipl%"),defaultQuery.value(20).toString());
        text.replace(QString("%total%"),"-");

    }

    QFile endFile("end.htm");
    if (!endFile.open(QIODevice::ReadOnly|QIODevice::Text)) return;
    text += endFile.readAll();

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(path);
    QTextDocument textdocument;
    textdocument.setHtml(text);
    textdocument.print(&printer);


}
