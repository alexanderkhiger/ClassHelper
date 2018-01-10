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
    diffModelReference = new QSqlQueryModel;
    runner = new QueryRunner;
    connect(runner,&QueryRunner::queryError,this,&MainWindowView::getError);
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


    teachersList = new QTableView;
    classesList = new QTableView;

    classesList->verticalHeader()->setVisible(0);
    teachersList->verticalHeader()->setVisible(0);
//    classesList->horizontalHeader()->setStretchLastSection(true);
//    teachersList->horizontalHeader()->setStretchLastSection(true);
    teachersList->setDragDropMode(QAbstractItemView::DragDrop);
    classesList->setDragDropMode(QAbstractItemView::DragDrop);

    teachersList->setDragEnabled(true);
    classesList->setDragEnabled(true);


    teachersRunner = new QueryRunner;
    classesRunner = new QueryRunner;
    connect(teachersRunner,&QueryRunner::querySuccessReturnModel,this,&MainWindowView::setTeachersModel);
    connect(classesRunner,&QueryRunner::querySuccessReturnModel,this,&MainWindowView::setClassesModel);
    //classesRunner->tryQuery("Select nazvanie_discipliny from disciplina",1);
    classesRunner->tryQuery("Select id_zapisi as 'ID',concat(nazvanie_potoka,' | ',nazvanie_discipliny) as 'Название', lekcii_chasov,seminary_chasov,lab_chasov,kontrol_chasov,konsultacii_chasov,"
                            "zachet_chasov,ekzamen_chasov,kursovie_chasov,ucheb_praktika_chasov,proizv_praktika_chasov,preddipl_praktika_chasov,vkl_chasov,obz_lekcii_chasov,gek_chasov,nirs_chasov,"
                            "asp_dokt_chasov from zanyatost left join disciplina on zanyatost.id_discipliny = disciplina.id_discipliny left join potok on zanyatost.id_potoka = potok.id_potoka",1);
    teachersRunner->tryQuery("Select id_prep as 'ID',concat(familiya,' ',imya,' ',otchestvo) as 'Имя' from prepodavatel",1);

    classesList->resizeColumnsToContents();
    classesList->resizeRowsToContents();

    classesList->setColumnHidden(2,true);
    classesList->setColumnHidden(3,true);
    classesList->setColumnHidden(4,true);
    classesList->setColumnHidden(5,true);
    classesList->setColumnHidden(6,true);
    classesList->setColumnHidden(7,true);
    classesList->setColumnHidden(8,true);
    classesList->setColumnHidden(9,true);
    classesList->setColumnHidden(10,true);
    classesList->setColumnHidden(11,true);
    classesList->setColumnHidden(12,true);
    classesList->setColumnHidden(13,true);
    classesList->setColumnHidden(14,true);
    classesList->setColumnHidden(15,true);
    classesList->setColumnHidden(16,true);
    classesList->setColumnHidden(17,true);

    teachersList->resizeColumnsToContents();
    teachersList->resizeRowsToContents();

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
    workField->setEnabled(false);

    lecLE = new QLineEdit;
    semLE = new QLineEdit;
    labLE = new QLineEdit;
    contLE = new QLineEdit;
    consLE = new QLineEdit;
    zachLE = new QLineEdit;
    examLE = new QLineEdit;
    kursLE = new QLineEdit;
    uchPrLE = new QLineEdit;
    proizvPrLE = new QLineEdit;
    predPrLE = new QLineEdit;
    vklLE = new QLineEdit;
    obzLE = new QLineEdit;
    gekLE = new QLineEdit;
    nirsLE = new QLineEdit;
    aspLE = new QLineEdit;

    QLocale valLocale(QLocale::C);
    valLocale.setNumberOptions(QLocale::RejectGroupSeparator);

    QDoubleValidator *val = new QDoubleValidator(0, 1000, 1, this);
    val->setLocale(valLocale);

    lecLE->setValidator(val);
    semLE->setValidator(val);
    labLE->setValidator(val);
    contLE->setValidator(val);
    consLE->setValidator(val);
    zachLE->setValidator(val);
    examLE->setValidator(val);
    kursLE->setValidator(val);
    uchPrLE->setValidator(val);
    proizvPrLE->setValidator(val);
    predPrLE->setValidator(val);
    vklLE->setValidator(val);
    obzLE->setValidator(val);
    gekLE->setValidator(val);
    nirsLE->setValidator(val);
    aspLE->setValidator(val);


    confirmDistr = new QPushButton(tr("Добавить запись"));
    connect(confirmDistr,&QPushButton::clicked,this,&MainWindowView::distributeHours);

    lecLabel = new QLabel(tr("Лекции"));
    semLabel = new QLabel(tr("Семинары"));
    labLabel = new QLabel(tr("Лабораторные"));
    contLabel = new QLabel(tr("Контрольные"));
    consLabel = new QLabel(tr("Консультации"));
    zachLabel = new QLabel(tr("Зачет"));
    examLabel = new QLabel(tr("Экзамен"));
    kursLabel = new QLabel(tr("Курсовые"));
    uchPrLabel = new QLabel(tr("Учебная практика"));
    proizvPrLabel = new QLabel(tr("Произв. практика"));
    predPrLabel = new QLabel(tr("Преддипл. практика"));
    vklLabel = new QLabel(tr("Вкл"));
    obzLabel = new QLabel(tr("Обзорные лекции"));
    gekLabel = new QLabel(tr("ГЭК"));
    nirsLabel = new QLabel(tr("НИРС"));
    aspLabel = new QLabel(tr("Аспирантские/докторские"));

    lecLabel->setAlignment(Qt::AlignHCenter);
    semLabel->setAlignment(Qt::AlignHCenter);
    labLabel->setAlignment(Qt::AlignHCenter);
    contLabel->setAlignment(Qt::AlignHCenter);
    consLabel->setAlignment(Qt::AlignHCenter);
    zachLabel->setAlignment(Qt::AlignHCenter);
    examLabel->setAlignment(Qt::AlignHCenter);
    kursLabel->setAlignment(Qt::AlignHCenter);
    uchPrLabel->setAlignment(Qt::AlignHCenter);
    proizvPrLabel->setAlignment(Qt::AlignHCenter);
    predPrLabel->setAlignment(Qt::AlignHCenter);
    vklLabel->setAlignment(Qt::AlignHCenter);
    obzLabel->setAlignment(Qt::AlignHCenter);
    gekLabel->setAlignment(Qt::AlignHCenter);
    nirsLabel->setAlignment(Qt::AlignHCenter);
    aspLabel->setAlignment(Qt::AlignHCenter);


    distrGrid = new QGridLayout;

    distrGrid->addWidget(lecLabel,0,0);
    distrGrid->addWidget(semLabel,0,1);
    distrGrid->addWidget(lecLE,1,0);
    distrGrid->addWidget(semLE,1,1);
    distrGrid->addWidget(labLabel,2,0);
    distrGrid->addWidget(contLabel,2,1);
    distrGrid->addWidget(labLE,3,0);
    distrGrid->addWidget(contLE,3,1);
    distrGrid->addWidget(consLabel,4,0);
    distrGrid->addWidget(zachLabel,4,1);
    distrGrid->addWidget(consLE,5,0);
    distrGrid->addWidget(zachLE,5,1);
    distrGrid->addWidget(examLabel,6,0);
    distrGrid->addWidget(kursLabel,6,1);
    distrGrid->addWidget(examLE,7,0);
    distrGrid->addWidget(kursLE,7,1);

    distrGrid->addWidget(uchPrLabel,8,0);
    distrGrid->addWidget(proizvPrLabel,8,1);
    distrGrid->addWidget(uchPrLE,9,0);
    distrGrid->addWidget(proizvPrLE,9,1);
    distrGrid->addWidget(predPrLabel,10,0);
    distrGrid->addWidget(vklLabel,10,1);
    distrGrid->addWidget(predPrLE,11,0);
    distrGrid->addWidget(vklLE,11,1);
    distrGrid->addWidget(obzLabel,12,0);
    distrGrid->addWidget(gekLabel,12,1);
    distrGrid->addWidget(obzLE,13,0);
    distrGrid->addWidget(gekLE,13,1);
    distrGrid->addWidget(nirsLabel,14,0);
    distrGrid->addWidget(aspLabel,14,1);
    distrGrid->addWidget(nirsLE,15,0);
    distrGrid->addWidget(aspLE,15,1);

    distrGrid->addWidget(confirmDistr,16,0,1,2);


    workField->setLayout(distrGrid);

    externalVLayout = new QVBoxLayout;
    internalMiddleVLayout = new QVBoxLayout;
    internalLeftVLayout = new QVBoxLayout;
    internalRightVLayout = new QVBoxLayout;
    externalHLayout = new QHBoxLayout;
    internalHLayout = new QHBoxLayout;
    classesInfoVLayout = new QVBoxLayout;
    teachersInfoVLayout = new QVBoxLayout;

    classesInfoGroupBox = new QGroupBox;
    teachersInfoGroupBox = new QGroupBox;

    classesTextEdit = new QTextEdit;
    teachersTextEdit = new QTextEdit;

    classesTextEditHeader = new QLabel(tr("Нераспределенных часов"));
    teachersTextEditHeader = new QLabel(tr("Свободных часов"));

    classesTextEditHeader->setAlignment(Qt::AlignHCenter);
    teachersTextEditHeader->setAlignment(Qt::AlignHCenter);

    classesInfoVLayout->addWidget(classesTextEditHeader);
    classesInfoVLayout->addWidget(classesTextEdit);
    teachersInfoVLayout->addWidget(teachersTextEditHeader);
    teachersInfoVLayout->addWidget(teachersTextEdit);

    classesInfoGroupBox->setLayout(classesInfoVLayout);
    teachersInfoGroupBox->setLayout(teachersInfoVLayout);

    classesInfoGroupBox->setMinimumWidth(200);
    teachersInfoGroupBox->setMinimumWidth(200);

    classesInfoGroupBox->setVisible(0);
    teachersInfoGroupBox->setVisible(0);
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
//    internalLeftVLayout->addLayout(classesColumns);
    internalLeftVLayout->addWidget(classesList);

    internalRightVLayout->addWidget(teachersHeader);
    internalRightVLayout->addWidget(teachersList);

    externalHLayout->addLayout(internalLeftVLayout);
    externalHLayout->addWidget(classesInfoGroupBox);
    externalHLayout->addLayout(internalMiddleVLayout);
    externalHLayout->addWidget(teachersInfoGroupBox);
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

void MainWindowView::setDiffModel(QSqlQueryModel *model)
{
    diffModelReference = model;
}

void MainWindowView::getError(QSqlError error)
{
    QMessageBox::StandardButton errorMsg;
    errorMsg = QMessageBox::critical(this,tr("Ошибка"),error.text(),QMessageBox::Ok);
}

void MainWindowView::distributeHours()
{
    QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,lekcii_chasov,seminary_chasov,lab_chasov,kontrol_chasov,konsultacii_chasov,zachet_chasov,ekzamen_chasov,"
    "kursovie_chasov,ucheb_praktika_chasov,proizv_praktika_chasov,preddipl_praktika_chasov,vkl_chasov,obz_lekcii_chasov,gek_chasov,nirs_chasov,asp_dokt_chasov) "
    "VALUES (%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18)")
            .arg(chosenClassID)
            .arg(chosenTeacherID)
            .arg(lecLE->text().toDouble())
            .arg(semLE->text().toDouble())
            .arg(labLE->text().toDouble())
            .arg(contLE->text().toDouble())
            .arg(consLE->text().toDouble())
            .arg(zachLE->text().toDouble())
            .arg(examLE->text().toDouble())
            .arg(kursLE->text().toDouble())
            .arg(uchPrLE->text().toDouble())
            .arg(proizvPrLE->text().toDouble())
            .arg(predPrLE->text().toDouble())
            .arg(vklLE->text().toDouble())
            .arg(obzLE->text().toDouble())
            .arg(gekLE->text().toDouble())
            .arg(nirsLE->text().toDouble())
            .arg(aspLE->text().toDouble());
    qDebug() << query;
    runner->tryQuery(query);
}


void MainWindowView::checkFields()
{
    if ((chosenClass->text() != "") && (chosenTeacher->text() != ""))
    {
        QString query = QString("SELECT id_prep, (normativy.lekcii_chasov-sum(raspredelenie.lekcii_chasov)),"
                         "(normativy.seminary_chasov-sum(raspredelenie.seminary_chasov)),"
                         "(normativy.lab_chasov-sum(raspredelenie.lab_chasov)),"
                         "(normativy.kontrol_chasov-sum(raspredelenie.kontrol_chasov)),"
                         "(normativy.lekcii_chasov-sum(raspredelenie.konsultacii_chasov)),"
                         "(normativy.zachet_chasov-sum(raspredelenie.zachet_chasov)),"
                         "(normativy.ekzamen_chasov-sum(raspredelenie.ekzamen_chasov)),"
                         "(normativy.kursovie_chasov-sum(raspredelenie.kursovie_chasov)),"
                         "(normativy.ucheb_praktika_chasov-sum(raspredelenie.ucheb_praktika_chasov)),"
                         "(normativy.proizv_praktika_chasov-sum(raspredelenie.proizv_praktika_chasov)),"
                         "(normativy.preddipl_praktika_chasov-sum(raspredelenie.preddipl_praktika_chasov)),"
                         "(normativy.vkl_chasov-sum(raspredelenie.vkl_chasov)),"
                         "(normativy.obz_lekcii_chasov-sum(raspredelenie.obz_lekcii_chasov)),"
                         "(normativy.gek_chasov-sum(raspredelenie.gek_chasov)),"
                         "(normativy.nirs_chasov-sum(raspredelenie.nirs_chasov)),"
                         "(normativy.asp_dokt_chasov-sum(raspredelenie.asp_dokt_chasov)) FROM raspredelenie,normativy WHERE id_prep = %1 group by id_prep;").arg(chosenTeacherID);

        QSqlQuery myQuery = runner->tryQuery(query);

        myQuery.next();
        teachersTextEdit->clear();
        teachersTextEdit->append("Часы лекций: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы семинаров: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы лабораторных: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы контрольных: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы консультаций: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы зачета: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы экзамена: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы курсовых: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы учебной практики: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы произв. практики: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы преддипл. практики: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы вкл"+myQuery.value(1).toString());
        teachersTextEdit->append("Часы обз. лекций: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы ГЭК: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы НИРС: "+myQuery.value(1).toString());
        teachersTextEdit->append("Часы асп/докторских: "+myQuery.value(1).toString());

        this->setStyleSheet("QGroupBox#workField {background-color: white; border-radius: 3px; border: 2px solid gray}");
        workField->setEnabled(true);          
        classesInfoGroupBox->setVisible(1);
        teachersInfoGroupBox->setVisible(1);
    }
}

void MainWindowView::doubleClickClassUpdate(const QModelIndex index)
{
    classesTextEdit->clear();
    chosenClass->setText(classModelReference->data(index).toString());
    chosenClassID = classModelReference->data(classModelReference->index(index.row(),0,QModelIndex())).toInt();

    classesTextEdit->append("Часы лекций: "+(classModelReference->data(classModelReference->index(index.row(),2,QModelIndex())).toString()));
    classesTextEdit->append("Часы семинаров: "+(classModelReference->data(classModelReference->index(index.row(),3,QModelIndex())).toString()));
    classesTextEdit->append("Часы лабораторных: "+(classModelReference->data(classModelReference->index(index.row(),4,QModelIndex())).toString()));
    classesTextEdit->append("Часы контрольных: "+(classModelReference->data(classModelReference->index(index.row(),5,QModelIndex())).toString()));
    classesTextEdit->append("Часы консультаций: "+(classModelReference->data(classModelReference->index(index.row(),6,QModelIndex())).toString()));
    classesTextEdit->append("Часы зачета: "+(classModelReference->data(classModelReference->index(index.row(),7,QModelIndex())).toString()));
    classesTextEdit->append("Часы экзамена: "+(classModelReference->data(classModelReference->index(index.row(),8,QModelIndex())).toString()));
    classesTextEdit->append("Часы курсовых: "+(classModelReference->data(classModelReference->index(index.row(),9,QModelIndex())).toString()));
    classesTextEdit->append("Часы учебной практики: "+(classModelReference->data(classModelReference->index(index.row(),10,QModelIndex())).toString()));
    classesTextEdit->append("Часы произв. практики: "+(classModelReference->data(classModelReference->index(index.row(),11,QModelIndex())).toString()));
    classesTextEdit->append("Часы преддипл. практики: "+(classModelReference->data(classModelReference->index(index.row(),12,QModelIndex())).toString()));
    classesTextEdit->append("Часы вкл"+(classModelReference->data(classModelReference->index(index.row(),13,QModelIndex())).toString()));
    classesTextEdit->append("Часы обз. лекций: "+(classModelReference->data(classModelReference->index(index.row(),14,QModelIndex())).toString()));
    classesTextEdit->append("Часы ГЭК: "+(classModelReference->data(classModelReference->index(index.row(),15,QModelIndex())).toString()));
    classesTextEdit->append("Часы НИРС: "+(classModelReference->data(classModelReference->index(index.row(),16,QModelIndex())).toString()));
    classesTextEdit->append("Часы асп/докторских: "+(classModelReference->data(classModelReference->index(index.row(),17,QModelIndex())).toString()));
    MainWindowView::checkFields();
}

void MainWindowView::doubleClickTeacherUpdate(const QModelIndex index)
{
    teachersTextEdit->clear();
    chosenTeacher->setText(teacherModelReference->data(index).toString());
    chosenTeacherID = teacherModelReference->data(teacherModelReference->index(index.row(),0,QModelIndex())).toInt();
    MainWindowView::checkFields();
}

void MainWindowView::clearParameters()
{
    chosenClass->clear();
    chosenTeacher->clear();
    this->setStyleSheet("QGroupBox#workField {}");
    workField->setEnabled(false);
    classesInfoGroupBox->setVisible(0);
    teachersInfoGroupBox->setVisible(0);
    classesTextEdit->clear();
    teachersTextEdit->clear();
}

void MainWindowView::newFile()
{
    LoadNewFileView *frm = new LoadNewFileView(receivedID);
    frm->show();
}
