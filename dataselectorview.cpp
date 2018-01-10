#include "dataselectorview.h"

DataSelectorView::DataSelectorView(QString tableName, QString receivedFacultyName, QString receivedSpecialtyName, QString receivedDisciplineName, QString uID, QWidget *parent) : QWidget(parent)
{
    createUI();
    receivedID = uID;
    facultyName = receivedFacultyName;
    specialtyName = receivedSpecialtyName;
    disciplineName = receivedDisciplineName;
    facultyModelReference = new QSqlTableModel;
    specialtyModelReference = new QSqlTableModel;
    disciplineModelReference = new QSqlTableModel;
    runner = new QueryRunner;

    if (facultyName != "noValue")
    {
        topLabel->setText(tr("Факультет с названием %1 не найден. Выберите подходящий факультет или создайте новую запись").arg(receivedFacultyName));
        createButton->setText(tr("Создать факультет с названием %1").arg(receivedFacultyName));
        connect(runner,&QueryRunner::returnTableModel,this,&DataSelectorView::setFacultyModel);
        runner->tryTableModel(tableName);
    }
    else if (specialtyName != "noValue")
    {
        topLabel->setText(tr("Специальность с названием %1 не найдена. Выберите подходящую специальность или создайте новую запись").arg(receivedSpecialtyName));
        createButton->setText(tr("Создать специальность с названием %1").arg(receivedSpecialtyName));
        connect(runner,&QueryRunner::returnTableModel,this,&DataSelectorView::setSpecialtyModel);
        runner->tryTableModel(tableName);
    }
    else if (disciplineName != "noValue")
    {
        topLabel->setText(tr("Дисциплина с названием %1 не найдена. Выберите подходящую дисциплину или создайте новую запись").arg(disciplineName));
        createButton->setText(tr("Создать дисциплину с названием %1").arg(disciplineName));
        connect(runner,&QueryRunner::returnTableModel,this,&DataSelectorView::setDisciplineModel);
        runner->tryTableModel(tableName);
    }

}

void DataSelectorView::createUI()
{
    topLabel = new QLabel;
    topLabel->setAlignment(Qt::AlignCenter);
    topLabel->setWordWrap(true);
    externalVLayout = new QVBoxLayout(this);
    externalVLayout->addWidget(topLabel);
    myTableView = new QTableView;
    connect(myTableView,&QTableView::doubleClicked,this,&DataSelectorView::setData);
    myTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    myTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    myTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    myTableView->verticalHeader()->setVisible(0);
    externalVLayout->addWidget(myTableView);
    createButton = new QPushButton;
    connect(createButton,&QPushButton::clicked,this,&DataSelectorView::createButtonClicked);
    externalVLayout->addWidget(createButton);
}


void DataSelectorView::setFacultyModel(QSqlTableModel *model)
{
    facultyModelReference = model;
    facultyModelReference->setHeaderData(0,Qt::Horizontal,tr("ID факультета"));
    facultyModelReference->setHeaderData(1,Qt::Horizontal,tr("Название"));
    facultyModelReference->setHeaderData(2,Qt::Horizontal,tr("Аббревиатура"));
    facultyModelReference->setHeaderData(3,Qt::Horizontal,tr("ID университета"));
    myTableView->setModel(model);
    resizeTable();
}

void DataSelectorView::setSpecialtyModel(QSqlTableModel *model)
{
    specialtyModelReference = model;
    specialtyModelReference->setFilter(QString("id_fakulteta = %1").arg(receivedID));
    specialtyModelReference->setHeaderData(0,Qt::Horizontal,tr("ID специальности"));
    specialtyModelReference->setHeaderData(1,Qt::Horizontal,tr("Название"));
    specialtyModelReference->setHeaderData(2,Qt::Horizontal,tr("ID факультета"));
    myTableView->setModel(model);
    resizeTable();
}

void DataSelectorView::setDisciplineModel(QSqlTableModel *model)
{
    disciplineModelReference = model;
    disciplineModelReference->setHeaderData(0,Qt::Horizontal,tr("ID дисциплины"));
    disciplineModelReference->setHeaderData(1,Qt::Horizontal,tr("Название"));
    myTableView->setModel(model);
    resizeTable();
}

void DataSelectorView::resizeTable()
{
    hHeader = myTableView->horizontalHeader();
    setMinimumSize(120,300);
    resize(120,300);
    int neededWidth = 0;

    myTableView->resizeColumnsToContents();
    myTableView->resizeRowsToContents();

    for (int c = 0; c < myTableView->horizontalHeader()->count(); c++)
    {
        neededWidth += hHeader->sectionSize(c) + 20;
    }

    setMinimumSize(neededWidth,300);

    myTableView->horizontalHeader()->setStretchLastSection(true);
}

void DataSelectorView::setData(QModelIndex index)
{
    if (facultyName != "noValue")
    {
        faculty_id = myTableView->model()->data(myTableView->model()->index(index.row(),0,QModelIndex())).toInt();
    }
    else if (specialtyName != "noValue")
    {
        specialty_id = myTableView->model()->data(myTableView->model()->index(index.row(),0,QModelIndex())).toInt();
    }
    else if (disciplineName != "noValue")
    {
        discipline_id = myTableView->model()->data(myTableView->model()->index(index.row(),0,QModelIndex())).toInt();
    }

    emit sendData();
}

void DataSelectorView::createButtonClicked()
{
    if (facultyName != "noValue")

    {
        int rowNumber = facultyModelReference->rowCount();
        facultyModelReference->insertRows(rowNumber, 1);
        facultyModelReference->setData(facultyModelReference->index(rowNumber, 1), facultyName);
        facultyModelReference->setData(facultyModelReference->index(rowNumber, 3), receivedID.toInt());
        int check = facultyModelReference->submitAll();
        if (!check)
        {
            facultyModelReference->revertAll();
        }
        else
        {
            faculty_id = facultyModelReference->data(facultyModelReference->index(rowNumber,0,QModelIndex())).toInt();
            emit sendData();
        }

    }

    else if (specialtyName != "noValue")

    {
        int rowNumber = specialtyModelReference->rowCount();
        specialtyModelReference->insertRows(rowNumber, 1);
        specialtyModelReference->setData(specialtyModelReference->index(rowNumber, 1), specialtyName);
        specialtyModelReference->setData(specialtyModelReference->index(rowNumber, 2), receivedID.toInt());
        int check = specialtyModelReference->submitAll();
        specialtyModelReference->setFilter("1=1 ORDER BY id_spec DESC");
        if (!check)
        {
            specialtyModelReference->revertAll();
        }
        else
        {
//            specialty_id = specialtyModelReference->data(specialtyModelReference->index(rowNumber,0,QModelIndex())).toInt();
            specialty_id = specialtyModelReference->data(specialtyModelReference->index(0,0,QModelIndex())).toInt();
            emit sendData();
        }
    }

    else if (disciplineName != "noValue")

    {
        int rowNumber = disciplineModelReference->rowCount();
        disciplineModelReference->insertRows(rowNumber, 1);
        disciplineModelReference->setData(disciplineModelReference->index(rowNumber, 1), disciplineName);
        int check = disciplineModelReference->submitAll();
        disciplineModelReference->setFilter("1=1 ORDER BY id_discipliny DESC");
        if (!check)
        {
            disciplineModelReference->revertAll();
        }
        else
        {
//            specialty_id = specialtyModelReference->data(specialtyModelReference->index(rowNumber,0,QModelIndex())).toInt();
            discipline_id = disciplineModelReference->data(disciplineModelReference->index(0,0,QModelIndex())).toInt();
            emit sendData();
        }
    }

}



