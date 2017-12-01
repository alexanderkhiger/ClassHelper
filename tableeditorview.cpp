#include "tableeditorview.h"

TableEditorView::TableEditorView(QString uID, QString uName, QString uShortname, QWidget *parent) : QWidget(parent)
{
    receivedUID = uID;
    receivedName = uName;
    receivedShortname = uShortname;
    teModel = new TableEditorModel;
    connect(teModel,&TableEditorModel::updateError,this,&TableEditorView::getError);
    chairRunner = new QueryRunner;
    facultyRunner = new QueryRunner;
    teacherRunner = new QueryRunner;
    setMinimumSize(500,500);
    setWindowTitle(tr("Работа с таблицами | %1 | %2").arg(uName).arg(uShortname));

    tableTab = new QTabWidget;
    chairWidget = new QWidget;
    facultyWidget = new QWidget;
    teacherWidget = new QWidget;
    tableTab->addTab(facultyWidget,tr("Факультеты"));
    tableTab->addTab(chairWidget,tr("Кафедры"));
    tableTab->addTab(teacherWidget,tr("Преподаватели"));
    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(tableTab);
    createChairWidgetUI();
    createFacultyWidgetUI();
    createTeacherWidgetUI();

}

void TableEditorView::enableFacultyButtons()
{
    facultyDeleteButton->setEnabled(1);
}

void TableEditorView::enableChairButtons()
{
    chairDeleteButton->setEnabled(1);
}

void TableEditorView::enableTeacherButtons()
{
    teacherDeleteButton->setEnabled(1);
}


void TableEditorView::resizeTable(QTableView *table)
{
    hHeader = table->horizontalHeader();

    int neededWidth = 0;

    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    for (int c = 0; c < table->horizontalHeader()->count(); c++)
    {
        neededWidth += hHeader->sectionSize(c) + 20;
    }

    if (neededWidth > 300)
        setMinimumSize(neededWidth,300);

    table->horizontalHeader()->setStretchLastSection(true);
}

void TableEditorView::createChairWidgetUI()
{
    chairTable = new QTableView;
    chairTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    chairTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    chairTable->setSelectionMode(QAbstractItemView::SingleSelection);
    chairTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    chairTable->verticalHeader()->setVisible(0);

    chairName = new QLineEdit;
    chairShortname = new QLineEdit;
    chairFacultyID = new QLineEdit;
    chairConfirmAddition = new QPushButton(tr("ОК"));
    chairAddButton = new QPushButton(tr("Добавить"));
    chairDeleteButton = new QPushButton(tr("Удалить"));
    chairDeleteButton->setEnabled(0);

    chairName->setPlaceholderText(tr("Название"));
    chairName->setToolTip(tr("Название факультета"));

    chairShortname->setPlaceholderText(tr("Аббревиатура"));
    chairShortname->setToolTip(tr("Аббревиатура факультета"));

    chairFacultyID->setPlaceholderText(tr("ID факультета"));
    chairFacultyID->setToolTip(tr("ID факультета"));

    connect(chairAddButton,&QPushButton::clicked,this,&TableEditorView::changeChairAddButtonStyle);
    connect(chairConfirmAddition,&QPushButton::clicked,this,&TableEditorView::chairAddRecord);
    connect(chairDeleteButton,&QPushButton::clicked,this,&TableEditorView::chairDeleteRecord);
    connect(chairTable,&QTableView::clicked,this,&TableEditorView::enableChairButtons);

    chairConfirmAddition->setVisible(0);
    chairName->setVisible(0);
    chairShortname->setVisible(0);
    chairFacultyID->setVisible(0);

    chairTopHLayout = new QHBoxLayout;
    chairTopHLayout->addWidget(chairName);
    chairTopHLayout->addWidget(chairShortname);
    chairTopHLayout->addWidget(chairFacultyID);
    chairTopHLayout->addWidget(chairConfirmAddition);

    chairBotHLayout = new QHBoxLayout;
    chairBotHLayout->addWidget(chairAddButton);
    chairBotHLayout->addWidget(chairDeleteButton);

    chairVLayout = new QVBoxLayout;
    chairVLayout->addWidget(chairTable);
    chairVLayout->addLayout(chairTopHLayout);
    chairVLayout->addLayout(chairBotHLayout);
    chairWidget->setLayout(chairVLayout);

    getChairModel();
    resizeTable(chairTable);
}

void TableEditorView::createFacultyWidgetUI()
{
    facultyTable = new QTableView;
    facultyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    facultyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    facultyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    facultyTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    facultyTable->verticalHeader()->setVisible(0);

    facultyName = new QLineEdit;
    facultyShortname = new QLineEdit;
    facultyConfirmAddition = new QPushButton(tr("ОК"));
    facultyAddButton = new QPushButton(tr("Добавить"));
    facultyDeleteButton = new QPushButton(tr("Удалить"));
    facultyDeleteButton->setEnabled(0);

    facultyName->setPlaceholderText(tr("Название"));
    facultyName->setToolTip(tr("Название факультета"));

    facultyShortname->setPlaceholderText(tr("Аббревиатура"));
    facultyShortname->setToolTip(tr("Аббревиатура факультета"));

    connect(facultyAddButton,&QPushButton::clicked,this,&TableEditorView::changeFacultyAddButtonStyle);
    connect(facultyConfirmAddition,&QPushButton::clicked,this,&TableEditorView::facultyAddRecord);
    connect(facultyDeleteButton,&QPushButton::clicked,this,&TableEditorView::facultyDeleteRecord);
    connect(facultyTable,&QTableView::clicked,this,&TableEditorView::enableFacultyButtons);

    facultyConfirmAddition->setVisible(0);
    facultyName->setVisible(0);
    facultyShortname->setVisible(0);

    facultyTopHLayout = new QHBoxLayout;
    facultyTopHLayout->addWidget(facultyName);
    facultyTopHLayout->addWidget(facultyShortname);
    facultyTopHLayout->addWidget(facultyConfirmAddition);

    facultyBotHLayout = new QHBoxLayout;
    facultyBotHLayout->addWidget(facultyAddButton);
    facultyBotHLayout->addWidget(facultyDeleteButton);

    facultyVLayout = new QVBoxLayout;
    facultyVLayout->addWidget(facultyTable);
    facultyVLayout->addLayout(facultyTopHLayout);
    facultyVLayout->addLayout(facultyBotHLayout);
    facultyWidget->setLayout(facultyVLayout);

    getFacultyModel();
    resizeTable(facultyTable);
}

void TableEditorView::createTeacherWidgetUI()
{
    teacherTable = new QTableView;
    teacherTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    teacherTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    teacherTable->setSelectionMode(QAbstractItemView::SingleSelection);
    teacherTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    teacherTable->verticalHeader()->setVisible(0);

    teacherFirstName = new QLineEdit;
    teacherMiddleName = new QLineEdit;
    teacherSurname = new QLineEdit;
    teacherTitle = new QLineEdit;
    teacherDegree = new QLineEdit;
    teacherPost = new QLineEdit;
    teacherChairID = new QLineEdit;

    teacherConfirmAddition = new QPushButton(tr("ОК"));
    teacherAddButton = new QPushButton(tr("Добавить"));
    teacherDeleteButton = new QPushButton(tr("Удалить"));
    teacherDeleteButton->setEnabled(0);

    teacherFirstName->setPlaceholderText(tr("Имя"));
    teacherFirstName->setToolTip(tr("Имя преподавателя"));

    teacherMiddleName->setPlaceholderText(tr("Отчество"));
    teacherMiddleName->setToolTip(tr("Отчество преподавателя"));

    teacherSurname->setPlaceholderText(tr("Фамилия"));
    teacherSurname->setToolTip(tr("Фамилия преподавателя"));

    teacherTitle->setPlaceholderText(tr("Ученое звание"));
    teacherTitle->setToolTip(tr("Ученое звание преподавателя"));

    teacherDegree->setPlaceholderText(tr("Ученая степень"));
    teacherDegree->setToolTip(tr("Ученая степень преподавателя"));

    teacherPost->setPlaceholderText(tr("Должность"));
    teacherPost->setToolTip(tr("Должность преподавателя"));

    teacherChairID->setPlaceholderText(tr("ID кафедры"));
    teacherChairID->setToolTip(tr("ID кафедры"));

    connect(teacherAddButton,&QPushButton::clicked,this,&TableEditorView::changeTeacherAddButtonStyle);
    connect(teacherConfirmAddition,&QPushButton::clicked,this,&TableEditorView::teacherAddRecord);
    connect(teacherDeleteButton,&QPushButton::clicked,this,&TableEditorView::teacherDeleteRecord);
    connect(teacherTable,&QTableView::clicked,this,&TableEditorView::enableTeacherButtons);

    teacherConfirmAddition->setVisible(0);
    teacherFirstName->setVisible(0);
    teacherMiddleName->setVisible(0);
    teacherSurname->setVisible(0);
    teacherTitle->setVisible(0);
    teacherDegree->setVisible(0);
    teacherPost->setVisible(0);
    teacherChairID->setVisible(0);

    teacherTopHLayout = new QHBoxLayout;
    teacherTopHLayout->addWidget(teacherSurname);
    teacherTopHLayout->addWidget(teacherFirstName);
    teacherTopHLayout->addWidget(teacherMiddleName);
    teacherTopHLayout->addWidget(teacherConfirmAddition);

    teacherMidHLayout = new QHBoxLayout;
    teacherMidHLayout->addWidget(teacherTitle);
    teacherMidHLayout->addWidget(teacherDegree);
    teacherMidHLayout->addWidget(teacherPost);
    teacherMidHLayout->addWidget(teacherChairID);

    teacherBotHLayout = new QHBoxLayout;
    teacherBotHLayout->addWidget(teacherAddButton);
    teacherBotHLayout->addWidget(teacherDeleteButton);

    teacherVLayout = new QVBoxLayout;
    teacherVLayout->addWidget(teacherTable);
    teacherVLayout->addLayout(teacherTopHLayout);
    teacherVLayout->addLayout(teacherMidHLayout);
    teacherVLayout->addLayout(teacherBotHLayout);
    teacherWidget->setLayout(teacherVLayout);

    getTeacherModel();
    resizeTable(teacherTable);
}

void TableEditorView::changeFacultyAddButtonStyle()
{
    facultyName->clear();
    facultyShortname->clear();
    if (facultyAddButtonState == buttonState::INACTIVE)
    {
        facultyName->setVisible(1);
        facultyShortname->setVisible(1);
        facultyConfirmAddition->setVisible(1);
        facultyAddButtonState = buttonState::ACTIVE;
        facultyAddButton->setStyleSheet("background-color:gray; border-color:black; color:white");
    }
    else
    {
        facultyName->setVisible(0);
        facultyShortname->setVisible(0);
        facultyConfirmAddition->setVisible(0);
        facultyAddButtonState = buttonState::INACTIVE;
        facultyAddButton->setStyleSheet("");
    }

}

void TableEditorView::changeChairAddButtonStyle()
{
    chairName->clear();
    chairShortname->clear();
    chairFacultyID->clear();
    if (chairAddButtonState == buttonState::INACTIVE)
    {
        chairName->setVisible(1);
        chairShortname->setVisible(1);
        chairConfirmAddition->setVisible(1);
        chairFacultyID->setVisible(1);
        chairAddButtonState = buttonState::ACTIVE;
        chairAddButton->setStyleSheet("background-color:gray; border-color:black; color:white");
    }
    else
    {
        chairName->setVisible(0);
        chairShortname->setVisible(0);
        chairConfirmAddition->setVisible(0);
        chairFacultyID->setVisible(0);
        chairAddButtonState = buttonState::INACTIVE;
        chairAddButton->setStyleSheet("");
    }

}

void TableEditorView::changeTeacherAddButtonStyle()
{
    teacherFirstName->clear();
    teacherMiddleName->clear();
    teacherSurname->clear();
    teacherTitle->clear();
    teacherDegree->clear();
    teacherPost->clear();
    teacherChairID->clear();

    if (teacherAddButtonState == buttonState::INACTIVE)
    {
        teacherConfirmAddition->setVisible(1);
        teacherFirstName->setVisible(1);
        teacherMiddleName->setVisible(1);
        teacherSurname->setVisible(1);
        teacherTitle->setVisible(1);
        teacherDegree->setVisible(1);
        teacherPost->setVisible(1);
        teacherChairID->setVisible(1);
        teacherAddButtonState = buttonState::ACTIVE;
        teacherAddButton->setStyleSheet("background-color:gray; border-color:black; color:white");
    }
    else
    {
        teacherConfirmAddition->setVisible(0);
        teacherFirstName->setVisible(0);
        teacherMiddleName->setVisible(0);
        teacherSurname->setVisible(0);
        teacherTitle->setVisible(0);
        teacherDegree->setVisible(0);
        teacherPost->setVisible(0);
        teacherChairID->setVisible(0);
        teacherAddButtonState = buttonState::INACTIVE;
        teacherAddButton->setStyleSheet("");

    }

}

void TableEditorView::getFacultyModel()
{
    connect(facultyRunner,&QueryRunner::returnTableModel,this,&TableEditorView::setFacultyModel);
    facultyRunner->tryTableModel("fakultet");
}


void TableEditorView::setFacultyModel(QSqlTableModel *model)
{
    facultyModelReference = model;
    facultyTable->setModel(model);
}

void TableEditorView::getChairModel()
{
    connect(chairRunner,&QueryRunner::returnTableModel,this,&TableEditorView::setChairModel);
    chairRunner->tryTableModel("kafedra");
}


void TableEditorView::setChairModel(QSqlTableModel *model)
{
    chairModelReference = model;
    chairTable->setModel(model);
}

void TableEditorView::getTeacherModel()
{
    connect(teacherRunner,&QueryRunner::returnTableModel,this,&TableEditorView::setTeacherModel);
    teacherRunner->tryTableModel("prepodavatel");
}


void TableEditorView::setTeacherModel(QSqlTableModel *model)
{
    teacherModelReference = model;
    teacherTable->setModel(model);
}


void TableEditorView::facultyAddRecord()
{
    if (facultyName->text() == "" || facultyShortname->text() == "")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Не все поля заполнены. Все равно добавить запись?")), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    if (facultyModelReference == Q_NULLPTR)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    teModel->updateFacultyModel(facultyModelReference,TableEditorModel::operationType::fINSERT,facultyTable->currentIndex().row(),facultyName->text(),facultyShortname->text(),receivedUID);
    facultyName->clear();
    facultyShortname->clear();

}

void TableEditorView::facultyDeleteRecord()
{
    if (facultyModelReference == Q_NULLPTR)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    QModelIndex idIndex = facultyTable->model()->index(facultyTable->currentIndex().row(),0,QModelIndex());
    QModelIndex nameIndex = facultyTable->model()->index(facultyTable->currentIndex().row(),1,QModelIndex());
    QModelIndex shortnameIndex = facultyTable->model()->index(facultyTable->currentIndex().row(),2,QModelIndex());

    QString idData = facultyTable->model()->data(idIndex).toString();
    QString nameData = facultyTable->model()->data(nameIndex).toString();
    QString shortnameData = facultyTable->model()->data(shortnameIndex).toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Удаление"), QString(tr("Удалить запись %1 | %2 | %3?")).arg(idData).arg(nameData).arg(shortnameData), QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        teModel->updateFacultyModel(facultyModelReference,TableEditorModel::operationType::fDELETE,facultyTable->currentIndex().row());

    }

}

void TableEditorView::chairAddRecord()
{
    if (chairName->text() == "" || chairShortname->text() == "")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Не все поля заполнены. Все равно добавить запись?")), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    if (chairModelReference == Q_NULLPTR)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    teModel->updateFacultyModel(chairModelReference,TableEditorModel::operationType::fINSERT,chairTable->currentIndex().row(),chairName->text(),chairShortname->text(),chairFacultyID->text());
    chairName->clear();
    chairShortname->clear();
    chairFacultyID->clear();

}

void TableEditorView::chairDeleteRecord()
{
    if (chairModelReference == Q_NULLPTR)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    QModelIndex idIndex = chairTable->model()->index(chairTable->currentIndex().row(),0,QModelIndex());
    QModelIndex nameIndex = chairTable->model()->index(chairTable->currentIndex().row(),1,QModelIndex());
    QModelIndex shortnameIndex = chairTable->model()->index(chairTable->currentIndex().row(),2,QModelIndex());
    QModelIndex facultyIDIndex = chairTable->model()->index(chairTable->currentIndex().row(),3,QModelIndex());

    QString idData = chairTable->model()->data(idIndex).toString();
    QString nameData = chairTable->model()->data(nameIndex).toString();
    QString shortnameData = chairTable->model()->data(shortnameIndex).toString();
    QString facultyIDData = chairTable->model()->data(facultyIDIndex).toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Удаление"), QString(tr("Удалить запись %1 | %2 | %3 | %4?")).arg(idData).arg(nameData).arg(shortnameData).arg(facultyIDData), QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        teModel->updateFacultyModel(chairModelReference,TableEditorModel::operationType::fDELETE,chairTable->currentIndex().row());

    }

}

void TableEditorView::teacherAddRecord()
{
    if (teacherFirstName->text() == "" || teacherMiddleName->text() == "" || teacherSurname->text() == "" || teacherDegree->text() == "" || teacherTitle->text() == "" || teacherPost->text() == "" || teacherChairID->text() == "")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Не все поля заполнены. Все равно добавить запись?")), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    if (teacherModelReference == Q_NULLPTR)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    teModel->updateTeacherModel(teacherModelReference,TableEditorModel::operationType::fINSERT,teacherTable->currentIndex().row(),teacherFirstName->text(),teacherSurname->text(),teacherMiddleName->text(),teacherDegree->text(),teacherChairID->text(),teacherPost->text(),teacherTitle->text());
    teacherFirstName->clear();
    teacherMiddleName->clear();
    teacherSurname->clear();
    teacherTitle->clear();
    teacherDegree->clear();
    teacherPost->clear();
    teacherChairID->clear();
}

void TableEditorView::teacherDeleteRecord()
{
    if (teacherModelReference == Q_NULLPTR)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    QModelIndex idIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),0,QModelIndex());
    QModelIndex firstNameIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),1,QModelIndex());
    QModelIndex surnameIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),2,QModelIndex());
    QModelIndex middleNameIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),3,QModelIndex());
    QModelIndex degreeIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),4,QModelIndex());
    QModelIndex chairIDIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),5,QModelIndex());
    QModelIndex postIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),6,QModelIndex());
    QModelIndex titleIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),7,QModelIndex());

    QString idData = teacherTable->model()->data(idIndex).toString();
    QString firstNameData = teacherTable->model()->data(firstNameIndex).toString();
    QString surnameData = teacherTable->model()->data(surnameIndex).toString();
    QString middleNameData = teacherTable->model()->data(middleNameIndex).toString();
    QString degreeData = teacherTable->model()->data(degreeIndex).toString();
    QString chairIDData = teacherTable->model()->data(chairIDIndex).toString();
    QString postData = teacherTable->model()->data(postIndex).toString();
    QString titleData = teacherTable->model()->data(titleIndex).toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Удаление"), QString(tr("Удалить запись %1 | %2 | %3 | %4 | %5 | %6 | %7 | %8?")).arg(idData).arg(firstNameData).arg(surnameData).arg(middleNameData).arg(degreeData).arg(chairIDData).arg(postData).arg(titleData), QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        teModel->updateTeacherModel(teacherModelReference,TableEditorModel::operationType::fDELETE,teacherTable->currentIndex().row());
    }

}

void TableEditorView::getError(QSqlError error)
{
    QMessageBox::StandardButton errorMsg;
    errorMsg = QMessageBox::critical(this,tr("Ошибка"),error.text(),QMessageBox::Ok);
}
