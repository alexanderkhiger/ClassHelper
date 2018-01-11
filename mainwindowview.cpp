#include "mainwindowview.h"

MainWindowView::MainWindowView(QString uID, QString uName, QString uShortname, QWidget *ref, QWidget *parent) : QMainWindow(parent)
{

    savedTeacherIndex = QModelIndex();
    savedClassIndex = QModelIndex();
    chosenClassID = 0;
    chosenTeacherID = 0;

    parentReference = ref;
    receivedID = uID;
    receivedName = uName;
    receivedShortname = uShortname;
    mwModel = new MainWindowModel;
    teacherModelReference = new QSqlQueryModel;
    classModelReference = new QSqlQueryModel;
    diffModelReference = new QSqlQueryModel;
    runner = new QueryRunner;
    connect(runner,SIGNAL(queryError(QSqlError)),this,SLOT(getError(QSqlError)));
    createUI();
    connect(dropArea, SIGNAL(changed(const QObject*,const QMimeData*)), mwModel, SLOT(updateParameters(const QObject*,const QMimeData*)));
    connect(mwModel,SIGNAL(sendData(QString,QString)),this,SLOT(getData(QString,QString)));
    connect(dropArea, SIGNAL(clearParameters()), this, SLOT(clearParameters()));
    connect(clearButton, SIGNAL(pressed()), dropArea, SLOT(clear()));
    connect(backButton,SIGNAL(pressed()),this,SLOT(back()));

    connect(classesList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickClassUpdate(QModelIndex)));
    connect(teachersList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickTeacherUpdate(QModelIndex)));
    connect(newFileAction, SIGNAL(triggered(bool)), this, SLOT(newFile()));

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
    connect(teachersRunner,SIGNAL(querySuccessReturnModel(QSqlQueryModel*)),this,SLOT(setTeachersModel(QSqlQueryModel*)));
    connect(classesRunner,SIGNAL(querySuccessReturnModel(QSqlQueryModel*)),this,SLOT(setClassesModel(QSqlQueryModel*)));
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

    val1 = new CustomDoubleValidator(0, 1000, 1, this);
    val1->setNotation(QDoubleValidator::StandardNotation);
    val1->setLocale(valLocale);

    val2 = new CustomDoubleValidator(0, 1000, 1, this);
    val2->setNotation(QDoubleValidator::StandardNotation);
    val2->setLocale(valLocale);

    val3 = new CustomDoubleValidator(0, 1000, 1, this);
    val3->setNotation(QDoubleValidator::StandardNotation);
    val3->setLocale(valLocale);

    val4 = new CustomDoubleValidator(0, 1000, 1, this);
    val4->setNotation(QDoubleValidator::StandardNotation);
    val4->setLocale(valLocale);

    val5 = new CustomDoubleValidator(0, 1000, 1, this);
    val5->setNotation(QDoubleValidator::StandardNotation);
    val5->setLocale(valLocale);

    val6 = new CustomDoubleValidator(0, 1000, 1, this);
    val6->setNotation(QDoubleValidator::StandardNotation);
    val6->setLocale(valLocale);

    val7 = new CustomDoubleValidator(0, 1000, 1, this);
    val7->setNotation(QDoubleValidator::StandardNotation);
    val7->setLocale(valLocale);

    val8 = new CustomDoubleValidator(0, 1000, 1, this);
    val8->setNotation(QDoubleValidator::StandardNotation);
    val8->setLocale(valLocale);

    val9 = new CustomDoubleValidator(0, 1000, 1, this);
    val9->setNotation(QDoubleValidator::StandardNotation);
    val9->setLocale(valLocale);

    val10 = new CustomDoubleValidator(0, 1000, 1, this);
    val10->setNotation(QDoubleValidator::StandardNotation);
    val10->setLocale(valLocale);

    val11 = new CustomDoubleValidator(0, 1000, 1, this);
    val11->setNotation(QDoubleValidator::StandardNotation);
    val11->setLocale(valLocale);

    val12 = new CustomDoubleValidator(0, 1000, 1, this);
    val12->setNotation(QDoubleValidator::StandardNotation);
    val12->setLocale(valLocale);

    val13 = new CustomDoubleValidator(0, 1000, 1, this);
    val13->setNotation(QDoubleValidator::StandardNotation);
    val13->setLocale(valLocale);

    val14 = new CustomDoubleValidator(0, 1000, 1, this);
    val14->setNotation(QDoubleValidator::StandardNotation);
    val14->setLocale(valLocale);

    val15 = new CustomDoubleValidator(0, 1000, 1, this);
    val15->setNotation(QDoubleValidator::StandardNotation);
    val15->setLocale(valLocale);

    val16 = new CustomDoubleValidator(0, 1000, 1, this);
    val16->setNotation(QDoubleValidator::StandardNotation);
    val16->setLocale(valLocale);

    lecLE->setValidator(val1);
    semLE->setValidator(val2);
    labLE->setValidator(val3);
    contLE->setValidator(val4);
    consLE->setValidator(val5);
    zachLE->setValidator(val6);
    examLE->setValidator(val7);
    kursLE->setValidator(val8);
    uchPrLE->setValidator(val9);
    proizvPrLE->setValidator(val10);
    predPrLE->setValidator(val11);
    vklLE->setValidator(val12);
    obzLE->setValidator(val13);
    gekLE->setValidator(val14);
    nirsLE->setValidator(val15);
    aspLE->setValidator(val16);


    confirmDistr = new QPushButton(tr("Добавить запись"));
    connect(confirmDistr,SIGNAL(clicked(bool)),this,SLOT(distributeHours()));

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
//    disconnect(classesRunner,&QueryRunner::returnTableModel,this,&MainWindowView::setClassesModel);
//    connect(classesRunner,&QueryRunner::returnTableModel,this,&MainWindowView::setClassesModel);
//    classesRunner->tryQuery("Select id_zapisi as 'ID',concat(nazvanie_potoka,' | ',nazvanie_discipliny) as 'Название', lekcii_chasov,seminary_chasov,lab_chasov,kontrol_chasov,konsultacii_chasov,"
//                            "zachet_chasov,ekzamen_chasov,kursovie_chasov,ucheb_praktika_chasov,proizv_praktika_chasov,preddipl_praktika_chasov,vkl_chasov,obz_lekcii_chasov,gek_chasov,nirs_chasov,"
//                            "asp_dokt_chasov from zanyatost left join disciplina on zanyatost.id_discipliny = disciplina.id_discipliny left join potok on zanyatost.id_potoka = potok.id_potoka",1);
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
    runner->tryQuery(query);

    runner->tryQuery("START TRANSACTION;");
    runner->tryQuery(QString("UPDATE zanyatost SET lekcii_chasov = lekcii_chasov - %1 WHERE id_zapisi = %2").arg(lecLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET seminary_chasov = seminary_chasov - %1 WHERE id_zapisi = %2").arg(semLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET lab_chasov = lab_chasov - %1 WHERE id_zapisi = %2" ).arg(labLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET kontrol_chasov = kontrol_chasov - %1 WHERE id_zapisi = %2").arg(contLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET konsultacii_chasov = konsultacii_chasov - %1 WHERE id_zapisi = %2").arg(consLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET zachet_chasov = zachet_chasov - %1 WHERE id_zapisi = %2").arg(zachLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET ekzamen_chasov = ekzamen_chasov - %1 WHERE id_zapisi = %2").arg(examLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET kursovie_chasov = kursovie_chasov - %1 WHERE id_zapisi = %2").arg(kursLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET ucheb_praktika_chasov = ucheb_praktika_chasov - %1 WHERE id_zapisi = %2").arg(uchPrLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET proizv_praktika_chasov = proizv_praktika_chasov - %1 WHERE id_zapisi = %2").arg(proizvPrLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET preddipl_praktika_chasov = preddipl_praktika_chasov - %1 WHERE id_zapisi = %2").arg(predPrLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET vkl_chasov = vkl_chasov - %1 WHERE id_zapisi = %2").arg(vklLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET obz_lekcii_chasov = obz_lekcii_chasov - %1 WHERE id_zapisi = %2").arg(obzLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET gek_chasov = gek_chasov - %1 WHERE id_zapisi = %2").arg(gekLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET nirs_chasov = nirs_chasov - %1 WHERE id_zapisi = %2").arg(nirsLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery(QString("UPDATE zanyatost SET asp_dokt_chasov = asp_dokt_chasov - %1 WHERE id_zapisi = %2").arg(aspLE->text().toDouble()).arg(chosenClassID));
    runner->tryQuery("COMMIT;");

    lecLE->clear();
    semLE->clear();
    labLE->clear();
    contLE->clear();
    consLE->clear();
    zachLE->clear();
    examLE->clear();
    kursLE->clear();
    uchPrLE->clear();
    proizvPrLE->clear();
    predPrLE->clear();
    vklLE->clear();
    obzLE->clear();
    gekLE->clear();
    nirsLE->clear();
    aspLE->clear();

    doubleClickClassUpdate(savedClassIndex);
    doubleClickTeacherUpdate(savedTeacherIndex);
}


void MainWindowView::checkFields()
{
    if ((chosenClass->text() != "") && (chosenTeacher->text() != ""))
    {
        this->setStyleSheet("QGroupBox#workField {background-color: white; border-radius: 3px; border: 2px solid gray}");
        workField->setEnabled(true);          
        classesInfoGroupBox->setVisible(1);
        teachersInfoGroupBox->setVisible(1);
    }
}

void MainWindowView::doubleClickClassUpdate(const QModelIndex index)
{
    savedClassIndex = index;
    classesTextEdit->clear();
    chosenClass->setText(classModelReference->data(index).toString());
    chosenClassID = classModelReference->data(classModelReference->index(index.row(),0,QModelIndex())).toInt();

    QString query = QString("Select id_zapisi, lekcii_chasov,seminary_chasov,lab_chasov,kontrol_chasov,konsultacii_chasov,"
                            "zachet_chasov,ekzamen_chasov,kursovie_chasov,ucheb_praktika_chasov,proizv_praktika_chasov,preddipl_praktika_chasov,vkl_chasov,obz_lekcii_chasov,gek_chasov,nirs_chasov,"
                            "asp_dokt_chasov from zanyatost where id_zapisi = %1").arg(chosenClassID);
    QSqlQuery myQuery = runner->tryQuery(query);
    myQuery.next();

//    classesTextEdit->append("Часы лекций: "+(classModelReference->data(classModelReference->index(index.row(),2,QModelIndex())).toString()));
//    classesTextEdit->append("Часы семинаров: "+(classModelReference->data(classModelReference->index(index.row(),3,QModelIndex())).toString()));
//    classesTextEdit->append("Часы лабораторных: "+(classModelReference->data(classModelReference->index(index.row(),4,QModelIndex())).toString()));
//    classesTextEdit->append("Часы контрольных: "+(classModelReference->data(classModelReference->index(index.row(),5,QModelIndex())).toString()));
//    classesTextEdit->append("Часы консультаций: "+(classModelReference->data(classModelReference->index(index.row(),6,QModelIndex())).toString()));
//    classesTextEdit->append("Часы зачета: "+(classModelReference->data(classModelReference->index(index.row(),7,QModelIndex())).toString()));
//    classesTextEdit->append("Часы экзамена: "+(classModelReference->data(classModelReference->index(index.row(),8,QModelIndex())).toString()));
//    classesTextEdit->append("Часы курсовых: "+(classModelReference->data(classModelReference->index(index.row(),9,QModelIndex())).toString()));
//    classesTextEdit->append("Часы учебной практики: "+(classModelReference->data(classModelReference->index(index.row(),10,QModelIndex())).toString()));
//    classesTextEdit->append("Часы произв. практики: "+(classModelReference->data(classModelReference->index(index.row(),11,QModelIndex())).toString()));
//    classesTextEdit->append("Часы преддипл. практики: "+(classModelReference->data(classModelReference->index(index.row(),12,QModelIndex())).toString()));
//    classesTextEdit->append("Часы вкл: "+(classModelReference->data(classModelReference->index(index.row(),13,QModelIndex())).toString()));
//    classesTextEdit->append("Часы обз. лекций: "+(classModelReference->data(classModelReference->index(index.row(),14,QModelIndex())).toString()));
//    classesTextEdit->append("Часы ГЭК: "+(classModelReference->data(classModelReference->index(index.row(),15,QModelIndex())).toString()));
//    classesTextEdit->append("Часы НИРС: "+(classModelReference->data(classModelReference->index(index.row(),16,QModelIndex())).toString()));
//    classesTextEdit->append("Часы асп/докторских: "+(classModelReference->data(classModelReference->index(index.row(),17,QModelIndex())).toString()));

    val1->setTop(myQuery.value(1).toDouble());
    val2->setTop(myQuery.value(2).toDouble());
    val3->setTop(myQuery.value(3).toDouble());
    val4->setTop(myQuery.value(4).toDouble());
    val5->setTop(myQuery.value(5).toDouble());
    val6->setTop(myQuery.value(6).toDouble());
    val7->setTop(myQuery.value(7).toDouble());
    val8->setTop(myQuery.value(8).toDouble());
    val9->setTop(myQuery.value(9).toDouble());
    val10->setTop(myQuery.value(10).toDouble());
    val11->setTop(myQuery.value(11).toDouble());
    val12->setTop(myQuery.value(12).toDouble());
    val13->setTop(myQuery.value(13).toDouble());
    val14->setTop(myQuery.value(14).toDouble());
    val15->setTop(myQuery.value(15).toDouble());
    val16->setTop(myQuery.value(16).toDouble());

    classesTextEdit->append("Часы лекций: "+myQuery.value(1).toString());
    classesTextEdit->append("Часы семинаров: "+myQuery.value(2).toString());
    classesTextEdit->append("Часы лабораторных: "+myQuery.value(3).toString());
    classesTextEdit->append("Часы контрольных: "+myQuery.value(4).toString());
    classesTextEdit->append("Часы консультаций: "+myQuery.value(5).toString());
    classesTextEdit->append("Часы зачета: "+myQuery.value(6).toString());
    classesTextEdit->append("Часы экзамена: "+myQuery.value(7).toString());
    classesTextEdit->append("Часы курсовых: "+myQuery.value(8).toString());
    classesTextEdit->append("Часы учебной практики: "+myQuery.value(9).toString());
    classesTextEdit->append("Часы произв. практики: "+myQuery.value(10).toString());
    classesTextEdit->append("Часы преддипл. практики: "+myQuery.value(11).toString());
    classesTextEdit->append("Часы вкл: "+myQuery.value(12).toString());
    classesTextEdit->append("Часы обз. лекций: "+myQuery.value(13).toString());
    classesTextEdit->append("Часы ГЭК: "+myQuery.value(14).toString());
    classesTextEdit->append("Часы НИРС: "+myQuery.value(15).toString());
    classesTextEdit->append("Часы асп/докторских: "+myQuery.value(16).toString());

    MainWindowView::checkFields();
}

void MainWindowView::doubleClickTeacherUpdate(const QModelIndex index)
{
    savedTeacherIndex = index;
    teachersTextEdit->clear();
    chosenTeacher->setText(teacherModelReference->data(index).toString());
    chosenTeacherID = teacherModelReference->data(teacherModelReference->index(index.row(),0,QModelIndex())).toInt();
    QString query = QString("SELECT "
                            "tab.id_prep,"
                            "normativy.lekcii_chasov - tab.A,"
                            "normativy.seminary_chasov - tab.B,"
                            "normativy.lab_chasov - tab.C,"
                            "normativy.kontrol_chasov - tab.D,"
                            "normativy.lekcii_chasov - tab.E,"
                            "normativy.zachet_chasov - tab.F,"
                            "normativy.ekzamen_chasov - tab.G,"
                            "normativy.kursovie_chasov - tab.H,"
                            "normativy.ucheb_praktika_chasov - tab.I,"
                            "normativy.proizv_praktika_chasov - tab.J,"
                            "normativy.preddipl_praktika_chasov - tab.K,"
                            "normativy.vkl_chasov - tab.L,"
                            "normativy.obz_lekcii_chasov - tab.M,"
                            "normativy.gek_chasov - tab.N,"
                            "normativy.nirs_chasov - tab.O,"
                            "normativy.asp_dokt_chasov - tab.P "
                            "FROM normativy,"
                            "(SELECT "
                            "raspredelenie.id_prep,"
                            "sum(raspredelenie.lekcii_chasov) as A,"
                            "sum(raspredelenie.seminary_chasov) as B,"
                            "sum(raspredelenie.lab_chasov) as C,"
                            "sum(raspredelenie.kontrol_chasov) as D,"
                            "sum(raspredelenie.konsultacii_chasov) as E,"
                            "sum(raspredelenie.zachet_chasov) as F,"
                            "sum(raspredelenie.ekzamen_chasov) as G,"
                            "sum(raspredelenie.kursovie_chasov) as H,"
                            "sum(raspredelenie.ucheb_praktika_chasov) as I,"
                            "sum(raspredelenie.proizv_praktika_chasov) as J,"
                            "sum(raspredelenie.preddipl_praktika_chasov) as K,"
                            "sum(raspredelenie.vkl_chasov) as L,"
                            "sum(raspredelenie.obz_lekcii_chasov) as M,"
                            "sum(raspredelenie.gek_chasov) as N,"
                            "sum(raspredelenie.nirs_chasov) as O,"
                            "sum(raspredelenie.asp_dokt_chasov) as P FROM raspredelenie WHERE id_prep = %1 group by id_prep) as tab;").arg(chosenTeacherID);
    QSqlQuery myQuery = runner->tryQuery(query);

    myQuery.next();
    teachersTextEdit->append("Часы лекций: "+myQuery.value(1).toString());
    teachersTextEdit->append("Часы семинаров: "+myQuery.value(2).toString());
    teachersTextEdit->append("Часы лабораторных: "+myQuery.value(3).toString());
    teachersTextEdit->append("Часы контрольных: "+myQuery.value(4).toString());
    teachersTextEdit->append("Часы консультаций: "+myQuery.value(5).toString());
    teachersTextEdit->append("Часы зачета: "+myQuery.value(6).toString());
    teachersTextEdit->append("Часы экзамена: "+myQuery.value(7).toString());
    teachersTextEdit->append("Часы курсовых: "+myQuery.value(8).toString());
    teachersTextEdit->append("Часы учебной практики: "+myQuery.value(9).toString());
    teachersTextEdit->append("Часы произв. практики: "+myQuery.value(10).toString());
    teachersTextEdit->append("Часы преддипл. практики: "+myQuery.value(11).toString());
    teachersTextEdit->append("Часы вкл: "+myQuery.value(12).toString());
    teachersTextEdit->append("Часы обз. лекций: "+myQuery.value(13).toString());
    teachersTextEdit->append("Часы ГЭК: "+myQuery.value(14).toString());
    teachersTextEdit->append("Часы НИРС: "+myQuery.value(15).toString());
    teachersTextEdit->append("Часы асп/докторских: "+myQuery.value(16).toString());
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
