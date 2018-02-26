#include "outputtofileview.h"

OutputToFileView::OutputToFileView(QString uID, QWidget *parent) : QWidget(parent)
{
    receivedID = uID;
    teachersModel = new QSqlQueryModel;
    runner = new QueryRunner;

    connect(runner,SIGNAL(querySuccessReturnModel(QSqlQueryModel*)),this,SLOT(setTeachersModel(QSqlQueryModel*)));


    teachersList = new QTableView;

    teachersList->verticalHeader()->setVisible(0);

    teachersList->setDragDropMode(QAbstractItemView::DragDrop);

    teachersList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    teachersList->setSelectionBehavior(QAbstractItemView::SelectRows);
    teachersList->setSelectionMode(QAbstractItemView::SingleSelection);

    teachersList->setDragEnabled(true);

    connect(teachersList,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setChosenTeacher(QModelIndex)));

    runner->tryQuery(QString("Select id_prep as 'ID',concat(familiya, ' ' ,imya,' ' , otchestvo) as 'Имя' from prepodavatel LEFT JOIN kafedra on prepodavatel.id_kafedry=kafedra.id_kafedry "
                             "LEFT JOIN fakultet on kafedra.id_fakulteta=fakultet.id_fakulteta LEFT JOIN universitet on fakultet.id_universiteta=universitet.id_universiteta"
                             " WHERE universitet.id_universiteta = %1").arg(receivedID),1);


//    teachersList->setColumnHidden(0,true);
//    teachersList->setColumnHidden(1,true);
//    teachersList->setColumnHidden(5,true);
//    teachersList->setColumnHidden(6,true);
//    teachersList->setColumnHidden(7,true);

    teachersList->resizeColumnsToContents();
    teachersList->resizeRowsToContents();
    teachersList->horizontalHeader()->setStretchLastSection(1);

    chosenTeacher = new QLineEdit;
    chosenTeacher->setReadOnly(1);
    fileSavePath = new QLineEdit;

    confirmOutput = new QPushButton("OK");
    connect(confirmOutput,SIGNAL(clicked(bool)),this,SLOT(process()));

    outputFormat = new QComboBox;
    internalVBoxLayout = new QVBoxLayout;
    internalVBoxLayout->addWidget(chosenTeacher);
    internalVBoxLayout->addWidget(outputFormat);
    internalVBoxLayout->addWidget(confirmOutput);

    externalHBoxLayout = new QHBoxLayout(this);
    externalHBoxLayout->addWidget(teachersList);
    externalHBoxLayout->addLayout(internalVBoxLayout);




}

void OutputToFileView::setTeachersModel(QSqlQueryModel *model)
{
    teachersModel = model;
    teachersList->setModel(teachersModel);
}

void OutputToFileView::setChosenTeacher(QModelIndex index)
{
    savedIndex = index;
    chosenTeacher->setText(teachersList->model()->data(index).toString());
}

void OutputToFileView::process()
{
    QString filename = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("Сохранить"),
                               ":/",
                               ("PDF (*.pdf)"));

    runner->outputToFile(teachersList->model()->data(teachersList->model()->index(savedIndex.row(),0)).toInt(),filename);










    /*
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
    "where id_prep = %1 group by id_zanyatosti").arg(teachersList->model()->data(savedIndex).toInt());


    QSqlDatabase db;

    if (QSqlDatabase::contains("dbConnection"))
    {
        db = QSqlDatabase::database("dbConnection");
    }
    else
        db = QSqlDatabase::addDatabase("QMYSQL","dbConnection");

    QSqlQuery query(db);
    query.exec(queryStr);
    qDebug() << query.size();

    for (int i = 0; i < query.size(); i++)
    {
        query.next();
        text += middleFile.readAll();

        text.replace(QString("%faculty%"),query.value(0).toString());
        text.replace(QString("%year%"),query.value(1).toString());
        text.replace(QString("%specialty%"),query.value(2).toString());
        text.replace(QString("%students%"),query.value(3).toString());
        text.replace(QString("%groups%"),query.value(4).toString());
        text.replace(QString("%subgroups%"),query.value(5).toString());
        text.replace(QString("%discipline%"),query.value(6).toString());
        text.replace(QString("%semester%"),query.value(7).toString());
        text.replace(QString("%weeks%"),query.value(8).toString());
        text.replace(QString("%ind%"),"-");
        text.replace(QString("%lec%"),query.value(9).toString());
        text.replace(QString("%sem%"),query.value(10).toString());
        text.replace(QString("%lab%"),query.value(11).toString());
        text.replace(QString("%ctek%"),query.value(12).toString());
        text.replace(QString("%cekz%"),query.value(13).toString());
        text.replace(QString("%cont%"),query.value(14).toString());
        text.replace(QString("%refr%"),query.value(15).toString());
        text.replace(QString("%zach%"),query.value(16).toString());
        text.replace(QString("%ekz%"),query.value(17).toString());
        text.replace(QString("%gek%"),query.value(18).toString());
        text.replace(QString("%prak%"),query.value(19).toString());
        text.replace(QString("%dipl%"),query.value(20).toString());
        text.replace(QString("%total%"),"-");

    }

    QFile endFile("end.htm");
    if (!endFile.open(QIODevice::ReadOnly|QIODevice::Text)) return;
    text += endFile.readAll();


    QString filename = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("Сохранить"),
                               ":/",
                               ("PDF (*.pdf)"));
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(filename);
    QTextDocument textdocument;
    textdocument.setHtml(text);
    textdocument.print(&printer);*/


}
