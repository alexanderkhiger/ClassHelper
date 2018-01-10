#include "mainwindowview.h"

MainWindowView::MainWindowView(QString uID, QString uName, QString uShortname, QWidget *ref, QWidget *parent) : QMainWindow(parent)
{
    parentReference = ref;
    receivedID = uID;
    receivedName = uName;
    receivedShortname = uShortname;
    mwModel = new MainWindowModel;
    teacherModelReference = new QSqlQueryModel;
    classModelReference = new QSqlQueryModel;
    createUI();
    connect(dropArea, &DropArea::changed, mwModel, &MainWindowModel::updateParameters);
    connect(mwModel,&MainWindowModel::sendData,this,&MainWindowView::getData);
    connect(dropArea, &DropArea::clearParameters, this, &MainWindowView::clearParameters);
    connect(clearButton, &QAbstractButton::pressed, dropArea, &DropArea::clear);
    connect(backButton,&QAbstractButton::pressed,this,&MainWindowView::back);

    connect(classesList, &QListView::doubleClicked, this, &MainWindowView::doubleClickClassUpdate);
    connect(teachersList, &QListView::doubleClicked, this, &MainWindowView::doubleClickTeacherUpdate);
    connect(newFileAction, &QAction::triggered, this, &MainWindowView::newFile);

    TableEditorView *frm = new TableEditorView(receivedID,receivedName,receivedShortname);
    frm->show();
}

void MainWindowView::createUI()
{

    setMinimumHeight(500);
    dropArea = new DropArea;
    dropArea->setFixedHeight(75);

    clearButton = new QPushButton(tr("Очистить"));
    backButton = new QPushButton(tr("Назад"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(clearButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(backButton, QDialogButtonBox::RejectRole);


    teachersList = new QListView;
    classesList = new QListView;

    teachersList->setDragDropMode(QAbstractItemView::DragDrop);
    classesList->setDragDropMode(QAbstractItemView::DragDrop);

    teachersList->setDragEnabled(true);
    classesList->setDragEnabled(true);


    teachersRunner = new QueryRunner;
    classesRunner = new QueryRunner;
    connect(teachersRunner,&QueryRunner::querySuccessReturnModel,this,&MainWindowView::setTeachersModel);
    connect(classesRunner,&QueryRunner::querySuccessReturnModel,this,&MainWindowView::setClassesModel);
    //classesRunner->tryQuery("Select nazvanie_discipliny from disciplina",1);
    classesRunner->tryQuery("Select concat(nazvanie_potoka,' | ',nazvanie_discipliny) from zanyatost left join disciplina on zanyatost.id_discipliny = disciplina.id_discipliny left join potok on zanyatost.id_potoka = potok.id_potoka",1);
    teachersRunner->tryQuery("Select concat(familiya,' ',imya,' ',otchestvo) from prepodavatel",1);



    teachersList->setFixedWidth(300);
    classesList->setFixedWidth(300);

    teachersList->setObjectName("teachersList");
    classesList->setObjectName("classesList");

    chosenClass = new QLineEdit;
    chosenTeacher = new QLineEdit;

    classesHeader = new QLabel(tr("Дисциплины"));
    teachersHeader = new QLabel(tr("Преподаватели"));

    classesHeader->setAlignment(Qt::AlignHCenter);
    teachersHeader->setAlignment(Qt::AlignHCenter);

    chosenClass->setPlaceholderText(tr("Дисциплина"));
    chosenTeacher->setPlaceholderText(tr("Преподаватель"));

    chosenClass->setReadOnly(true);
    chosenTeacher->setReadOnly(true);

    chosenClass->setAlignment(Qt::AlignHCenter);
    chosenTeacher->setAlignment(Qt::AlignHCenter);

    workField = new QGroupBox;
    workField->setObjectName("workField");

    externalVLayout = new QVBoxLayout;
    internalMiddleVLayout = new QVBoxLayout;
    internalLeftVLayout = new QVBoxLayout;
    internalRightVLayout = new QVBoxLayout;
    externalHLayout = new QHBoxLayout;
    internalHLayout = new QHBoxLayout;
//    classesColumns = new QHBoxLayout;

//    QCheckBox *cb1 = new QCheckBox;
//    QCheckBox *cb2 = new QCheckBox;
//    QCheckBox *cb3 = new QCheckBox;
//    QCheckBox *cb4 = new QCheckBox;
//    QCheckBox *cb5 = new QCheckBox;
//    QCheckBox *cb6 = new QCheckBox;

//    classesColumns->addWidget(cb1);
//    classesColumns->addWidget(cb2);
//    classesColumns->addWidget(cb3);
//    classesColumns->addWidget(cb4);
//    classesColumns->addWidget(cb5);
//    classesColumns->addWidget(cb6);

    internalHLayout->addWidget(chosenClass);
    internalHLayout->addWidget(chosenTeacher);

    internalMiddleVLayout->addWidget(dropArea);
    internalMiddleVLayout->addLayout(internalHLayout);
    internalMiddleVLayout->addWidget(workField);

    internalLeftVLayout->addWidget(classesHeader);
    internalLeftVLayout->addLayout(classesColumns);
    internalLeftVLayout->addWidget(classesList);

    internalRightVLayout->addWidget(teachersHeader);
    internalRightVLayout->addWidget(teachersList);

    externalHLayout->addLayout(internalLeftVLayout);
    externalHLayout->addLayout(internalMiddleVLayout);
    externalHLayout->addLayout(internalRightVLayout);
    externalVLayout->addLayout(externalHLayout);
    externalVLayout->addWidget(buttonBox);

    myWorkField = new QWidget();
    myWorkField->setLayout(externalVLayout);

    this->setCentralWidget(myWorkField);

    fileMenu = new QMenu(tr("Файл"));
    editMenu = new QMenu(tr("Правка"));
    toolsMenu = new QMenu(tr("Инструменты"));
    helpMenu = new QMenu(tr("Справка"));

    this->menuBar()->addMenu(fileMenu);
    this->menuBar()->addMenu(editMenu);
    this->menuBar()->addMenu(toolsMenu);
    this->menuBar()->addMenu(helpMenu);

    newFileAction = new QAction(tr("Загрузить файл данных"), this);
    newFileAction->setShortcuts(QKeySequence::New);
    newFileAction->setStatusTip(tr("Загрузить новый файл данных"));
    fileMenu->addAction(newFileAction);

    setWindowTitle(QString(tr("Time Tracker | %1 | %2")).arg(receivedName).arg(receivedShortname));
}

void MainWindowView::back()
{
    parentReference->show();
    QWidget::close();
}

void MainWindowView::getData(QString objName, QString containedData)
{
    if (objName == "classesList")
    {
        chosenClass->setText(containedData);
    }
    else if (objName == "teachersList")
    {
        chosenTeacher->setText(containedData);
    }
    checkFields();
}

void MainWindowView::getModel()
{
//    connect(runner,&QueryRunner::returnTableModel,this,&MainWindowView::setModel);
//    runner->tryTableModel("universitet");
}


void MainWindowView::setClassesModel(QSqlQueryModel *model)
{
    classModelReference = model;
    classesList->setModel(classModelReference);
}

void MainWindowView::setTeachersModel(QSqlQueryModel *model)
{
    teacherModelReference = model;
    teachersList->setModel(teacherModelReference);
}

void MainWindowView::getError(QSqlError error)
{
    QMessageBox::StandardButton errorMsg;
    errorMsg = QMessageBox::critical(this,tr("Ошибка"),error.text(),QMessageBox::Ok);
}


void MainWindowView::checkFields()
{
    if ((chosenClass->text() != "") && (chosenTeacher->text() != ""))
    {
        this->setStyleSheet("QGroupBox#workField {background-color: white; border-radius: 3px; border: 2px solid gray}");
        workField->setEnabled(true);
    }
}

void MainWindowView::doubleClickClassUpdate(const QModelIndex index)
{
    chosenClass->setText(classModelReference->data(index).toString());
    MainWindowView::checkFields();
}

void MainWindowView::doubleClickTeacherUpdate(const QModelIndex index)
{
    chosenTeacher->setText(teacherModelReference->data(index).toString());
    MainWindowView::checkFields();
}

void MainWindowView::clearParameters()
{
    chosenClass->clear();
    chosenTeacher->clear();
    this->setStyleSheet("QGroupBox#workField {}");
    workField->setEnabled(false);
}

void MainWindowView::newFile()
{
    LoadNewFileView *frm = new LoadNewFileView(receivedID);
    frm->show();
}
