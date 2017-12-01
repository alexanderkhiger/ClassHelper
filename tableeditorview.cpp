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
    setMinimumSize(300,300);
    setWindowTitle(tr("Работа с таблицами | %1 | %2").arg(uName).arg(uShortname));

    tableTab = new QTabWidget;
    chairWidget = new QWidget;
    facultyWidget = new QWidget;
    teacherWidget = new QWidget;

    tableTab->addTab(facultyWidget,tr("Факультеты"));
    tableTab->addTab(chairWidget,tr("Кафедры"));
    tableTab->addTab(teacherWidget,tr("Преподаватели"));

    connect(tableTab,&QTabWidget::currentChanged,this,&TableEditorView::checkSize);

    getIDTable = new QTableView;
    agree = new QPushButton(tr("ОК"));
    disagree = new QPushButton(tr("Назад"));
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(agree,QDialogButtonBox::AcceptRole);
    buttonBox->addButton(disagree,QDialogButtonBox::RejectRole);
    connect(disagree,&QPushButton::clicked,this,&TableEditorView::setSmallTablesInvisible);
    getIDTable->setVisible(0);
    buttonBox->setVisible(0);
    getIDTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    getIDTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    getIDTable->setSelectionMode(QAbstractItemView::SingleSelection);
    getIDTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    getIDTable->verticalHeader()->setVisible(0);
    connect(getIDTable,&QTableView::clicked,this,&TableEditorView::enableGetIDButtons);

    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(tableTab);
    vLayout->addWidget(getIDTable);
    vLayout->addWidget(buttonBox);
    createChairWidgetUI();
    createFacultyWidgetUI();
    createTeacherWidgetUI();
    resizeTable(facultyTable);
}

void TableEditorView::resizeTable(QTableView *table)
{
    hHeader = table->horizontalHeader();
    setMinimumSize(120,300);
    resize(120,300);
    int neededWidth = 0;

    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    for (int c = 0; c < table->horizontalHeader()->count(); c++)
    {
        neededWidth += hHeader->sectionSize(c) + 20;
    }

    setMinimumSize(neededWidth,300);

    table->horizontalHeader()->setStretchLastSection(true);
}

void TableEditorView::checkSize(int index)
{
    if (index == 0)
    {
        resizeTable(facultyTable);
    }
    else if (index == 1)
    {
        resizeTable(chairTable);
    }
    else
    {
        resizeTable(teacherTable);
    }
}

void TableEditorView::getError(QSqlError error)
{
    QMessageBox::StandardButton errorMsg;
    errorMsg = QMessageBox::critical(this,tr("Ошибка"),error.text(),QMessageBox::Ok);
}


void TableEditorView::setSmallTablesVisible()
{
    agree->setEnabled(0);
    getIDTable->setVisible(1);
    buttonBox->setVisible(1);
    tableTab->setVisible(0);
}

void TableEditorView::setSmallTablesInvisible()
{
    getIDTable->setVisible(0);
    buttonBox->setVisible(0);
    tableTab->setVisible(1);
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

void TableEditorView::enableGetIDButtons()
{
    agree->setEnabled(1);
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
    chairChooseFaculty = new CustomLineEdit(this);
    chairChooseFaculty->setPlaceholderText(tr("ID факультета"));
    chairChooseFaculty->setToolTip(tr("ID факультета"));

    connect(chairChooseFaculty->chooseButton,&QToolButton::clicked,this,&TableEditorView::openFacultyList);

    chairConfirmAddition = new QPushButton(tr("ОК"));
    chairAddButton = new QPushButton(tr("Добавить"));
    chairDeleteButton = new QPushButton(tr("Удалить"));
    chairDeleteButton->setEnabled(0);

    chairName->setPlaceholderText(tr("Название"));
    chairName->setToolTip(tr("Название факультета"));

    chairShortname->setPlaceholderText(tr("Аббревиатура"));
    chairShortname->setToolTip(tr("Аббревиатура факультета"));

    connect(chairAddButton,&QPushButton::clicked,this,&TableEditorView::changeChairAddButtonStyle);
    connect(chairConfirmAddition,&QPushButton::clicked,this,&TableEditorView::chairAddRecord);
    connect(chairDeleteButton,&QPushButton::clicked,this,&TableEditorView::chairDeleteRecord);
    connect(chairTable,&QTableView::clicked,this,&TableEditorView::enableChairButtons);

    chairConfirmAddition->setVisible(0);
    chairName->setVisible(0);
    chairShortname->setVisible(0);
    chairChooseFaculty->setVisible(0);

    chairTopHLayout = new QHBoxLayout;
    chairTopHLayout->addWidget(chairName);
    chairTopHLayout->addWidget(chairShortname);
    chairTopHLayout->addWidget(chairChooseFaculty);
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
    teacherPost = new QLineEdit;

    teacherTitle = new QComboBox;
    teacherDegree = new QComboBox;

    teacherTitle->addItem(tr("Отсутствует"));
    teacherTitle->addItem(tr("Доцент"));
    teacherTitle->addItem(tr("Профессор"));

    teacherDegree->addItem(tr("Отсутствует"));
    teacherDegree->addItem(tr("Кандидат наук"));
    teacherDegree->addItem(tr("Доктор наук"));

    teacherChooseChair = new CustomLineEdit(this);
    connect(teacherChooseChair->chooseButton,&QToolButton::clicked,this,&TableEditorView::openChairList);

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

    teacherTitle->setToolTip(tr("Ученое звание преподавателя"));
    teacherDegree->setToolTip(tr("Ученая степень преподавателя"));

    teacherPost->setPlaceholderText(tr("Должность"));
    teacherPost->setToolTip(tr("Должность преподавателя"));

    teacherChooseChair->setPlaceholderText(tr("ID кафедры"));
    teacherChooseChair->setToolTip(tr("ID кафедры"));

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
    teacherChooseChair->setVisible(0);

    titleLabel = new QLabel(tr("Ученое звание "));
    degreeLabel = new QLabel(tr("Ученая степень"));

    titleLabel->setVisible(0);
    degreeLabel->setVisible(0);

    teacherTopHLayout = new QHBoxLayout;
    teacherTopHLayout->addWidget(teacherSurname);
    teacherTopHLayout->addWidget(teacherFirstName);
    teacherTopHLayout->addWidget(teacherMiddleName);
    teacherTopHLayout->addWidget(teacherChooseChair);
    teacherTopHLayout->addWidget(teacherConfirmAddition);

    teacherMidHLayout = new QHBoxLayout;
    teacherMidHLayout->addWidget(titleLabel);
    teacherMidHLayout->addWidget(teacherTitle);
    teacherMidHLayout->addWidget(degreeLabel);
    teacherMidHLayout->addWidget(teacherDegree);
    teacherMidHLayout->addWidget(teacherPost);


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
    chairChooseFaculty->clear();
    if (chairAddButtonState == buttonState::INACTIVE)
    {
        chairName->setVisible(1);
        chairShortname->setVisible(1);
        chairConfirmAddition->setVisible(1);
        chairChooseFaculty->setVisible(1);
        chairAddButtonState = buttonState::ACTIVE;
        chairAddButton->setStyleSheet("background-color:gray; border-color:black; color:white");
    }
    else
    {
        chairName->setVisible(0);
        chairShortname->setVisible(0);
        chairConfirmAddition->setVisible(0);
        chairChooseFaculty->setVisible(0);
        chairAddButtonState = buttonState::INACTIVE;
        chairAddButton->setStyleSheet("");
    }

}

void TableEditorView::changeTeacherAddButtonStyle()
{
    teacherFirstName->clear();
    teacherMiddleName->clear();
    teacherSurname->clear();
    teacherTitle->setCurrentIndex(0);
    teacherDegree->setCurrentIndex(0);
    teacherPost->clear();
    teacherChooseChair->clear();

    if (teacherAddButtonState == buttonState::INACTIVE)
    {
        teacherConfirmAddition->setVisible(1);
        teacherFirstName->setVisible(1);
        teacherMiddleName->setVisible(1);
        teacherSurname->setVisible(1);
        teacherTitle->setVisible(1);
        teacherDegree->setVisible(1);
        titleLabel->setVisible(1);
        degreeLabel->setVisible(1);
        teacherPost->setVisible(1);
        teacherChooseChair->setVisible(1);
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
        titleLabel->setVisible(0);
        degreeLabel->setVisible(0);
        teacherPost->setVisible(0);
        teacherChooseChair->setVisible(0);
        teacherAddButtonState = buttonState::INACTIVE;
        teacherAddButton->setStyleSheet("");

    }

}

void TableEditorView::openFacultyList()
{
    setSmallTablesVisible();
    getIDTable->setModel(facultyModelReference);
    resizeTable(getIDTable);
    connect(agree,&QPushButton::clicked,this,&TableEditorView::getFacultyID);

}

void TableEditorView::openChairList()
{
    setSmallTablesVisible();
    getIDTable->setModel(chairModelReference);
    resizeTable(getIDTable);
    connect(agree,&QPushButton::clicked,this,&TableEditorView::getChairID);
}

void TableEditorView::getFacultyID()
{
    QModelIndex idIndex = getIDTable->model()->index(getIDTable->currentIndex().row(),0,QModelIndex());
    QString idData = getIDTable->model()->data(idIndex).toString();
    chairChooseFaculty->setText(idData);
    setSmallTablesInvisible();
    resizeTable(chairTable);
    disconnect(agree,&QPushButton::clicked,this,&TableEditorView::getFacultyID);
}


void TableEditorView::getChairID()
{
    QModelIndex idIndex = getIDTable->model()->index(getIDTable->currentIndex().row(),0,QModelIndex());
    QString idData = getIDTable->model()->data(idIndex).toString();
    teacherChooseChair->setText(idData);
    setSmallTablesInvisible();
    resizeTable(teacherTable);
    disconnect(agree,&QPushButton::clicked,this,&TableEditorView::getChairID);
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

    if (facultyModelReference == 0)
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
    if (facultyModelReference == 0)
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
    if (chairChooseFaculty->text() == "")
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Поле ID факультета обязательно для заполнения"),QMessageBox::Ok);
        return;
    }

    if (chairName->text() == "" || chairShortname->text() == "" || chairChooseFaculty->text() == "")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Не все поля заполнены. Все равно добавить запись?")), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    if (chairModelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    teModel->updateFacultyModel(chairModelReference,TableEditorModel::operationType::fINSERT,chairTable->currentIndex().row(),chairName->text(),chairShortname->text(),chairChooseFaculty->text());
    chairName->clear();
    chairShortname->clear();
    chairChooseFaculty->clear();

}

void TableEditorView::chairDeleteRecord()
{
    if (chairModelReference == 0)
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

    if (teacherChooseChair->text() == "")
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Поле ID кафедры обязательно для заполнения"),QMessageBox::Ok);
        return;
    }

    if (teacherFirstName->text() == "" || teacherMiddleName->text() == "" || teacherSurname->text() == "" || teacherDegree->currentText() == "" || teacherTitle->currentText() == "" || teacherPost->text() == "" || teacherChooseChair->text() == "")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Не все поля заполнены. Все равно добавить запись?")), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    if (teacherModelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    teModel->updateTeacherModel(teacherModelReference,TableEditorModel::operationType::fINSERT,teacherTable->currentIndex().row(),teacherSurname->text(),teacherFirstName->text(),teacherMiddleName->text(),teacherChooseChair->text(),teacherPost->text(),teacherDegree->currentText(),teacherTitle->currentText());
    teacherFirstName->clear();
    teacherMiddleName->clear();
    teacherSurname->clear();
    teacherTitle->setCurrentIndex(0);
    teacherDegree->setCurrentIndex(0);
    teacherPost->clear();
    teacherChooseChair->clear();
}

void TableEditorView::teacherDeleteRecord()
{
    if (teacherModelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    QModelIndex idIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),0,QModelIndex());
    QModelIndex firstNameIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),2,QModelIndex());
    QModelIndex surnameIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),1,QModelIndex());
    QModelIndex middleNameIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),3,QModelIndex());
    QModelIndex degreeIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),6,QModelIndex());
    QModelIndex chairIDIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),4,QModelIndex());
    QModelIndex postIndex = teacherTable->model()->index(teacherTable->currentIndex().row(),5,QModelIndex());
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

