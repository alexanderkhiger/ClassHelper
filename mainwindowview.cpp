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
    connect(runner,SIGNAL(returnValues(QList<double>)),this,SLOT(setData(QList<double>)));
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
    classesRunner->tryQuery(QString("Select id_zapisi as 'ID',concat(nazvanie_potoka,' | ',nazvanie_discipliny) as 'Название', lekcii_chasov,seminary_chasov,lab_chasov,kontrol_chasov,konsultacii_chasov,"
                            "zachet_chasov,ekzamen_chasov,kursovie_chasov,ucheb_praktika_chasov,proizv_praktika_chasov,preddipl_praktika_chasov,vkl_chasov,obz_lekcii_chasov,gek_chasov,nirs_chasov,"
                            "asp_dokt_chasov from zanyatost left join disciplina on zanyatost.id_discipliny = disciplina.id_discipliny left join potok on zanyatost.id_potoka = potok.id_potoka LEFT JOIN "
                                    "specialnost on potok.id_spec = specialnost.id_spec LEFT JOIN fakultet on specialnost.id_fakulteta = fakultet.id_fakulteta LEFT JOIN universitet on "
                                    "fakultet.id_universiteta = universitet.id_universiteta WHERE universitet.id_universiteta = %1 ORDER BY id_zapisi").arg(receivedID),1);
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
    hoursLeftGrid = new QGridLayout;
    hoursDistrGrid = new QGridLayout;

    lecButtonAdd = new QToolButton;
    semButtonAdd = new QToolButton;
    labButtonAdd = new QToolButton;
    contButtonAdd = new QToolButton;
    consButtonAdd = new QToolButton;
    zachButtonAdd = new QToolButton;
    examButtonAdd = new QToolButton;
    kursButtonAdd = new QToolButton;
    uchPrButtonAdd = new QToolButton;
    proizvPrButtonAdd = new QToolButton;
    predPrButtonAdd = new QToolButton;
    vklButtonAdd = new QToolButton;
    obzButtonAdd = new QToolButton;
    gekButtonAdd = new QToolButton;
    nirsButtonAdd = new QToolButton;
    aspButtonAdd = new QToolButton;

    lecButtonAdd->setObjectName("lecButtonAdd");
    semButtonAdd->setObjectName("semButtonAdd");
    labButtonAdd->setObjectName("labButtonAdd");
    contButtonAdd->setObjectName("contButtonAdd");
    consButtonAdd->setObjectName("consButtonAdd");
    zachButtonAdd->setObjectName("zachButtonAdd");
    examButtonAdd->setObjectName("examButtonAdd");
    kursButtonAdd->setObjectName("kursButtonAdd");
    uchPrButtonAdd->setObjectName("uchPrButtonAdd");
    proizvPrButtonAdd->setObjectName("proizvButtonAdd");
    predPrButtonAdd->setObjectName("predPrButtonAdd");
    vklButtonAdd->setObjectName("vklButtonAdd");
    obzButtonAdd->setObjectName("obzButtonAdd");
    gekButtonAdd->setObjectName("gekButtonAdd");
    nirsButtonAdd->setObjectName("nirsButtonAdd");
    aspButtonAdd->setObjectName("aspButtonAdd");



    connect(lecButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(semButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(labButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(contButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(consButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(zachButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(examButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(kursButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(uchPrButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(proizvPrButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(predPrButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(vklButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(obzButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(gekButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(nirsButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(aspButtonAdd,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));

    lecLabelAdd = new QLabel;
    semLabelAdd = new QLabel;
    labLabelAdd = new QLabel;
    contLabelAdd = new QLabel;
    consLabelAdd = new QLabel;
    zachLabelAdd = new QLabel;
    examLabelAdd = new QLabel;
    kursLabelAdd = new QLabel;
    uchPrLabelAdd = new QLabel;
    proizvPrLabelAdd = new QLabel;
    predPrLabelAdd = new QLabel;
    vklLabelAdd = new QLabel;
    obzLabelAdd = new QLabel;
    gekLabelAdd = new QLabel;
    nirsLabelAdd = new QLabel;
    aspLabelAdd = new QLabel;



    lecButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    semButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    labButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    contButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    consButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    zachButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    examButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    kursButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    uchPrButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    proizvPrButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    predPrButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    vklButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    obzButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    gekButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    nirsButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    aspButtonAdd->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));


    hoursLeftGrid->addWidget(lecLabelAdd,0,0);
    hoursLeftGrid->addWidget(semLabelAdd,1,0);
    hoursLeftGrid->addWidget(labLabelAdd,2,0);
    hoursLeftGrid->addWidget(contLabelAdd,3,0);
    hoursLeftGrid->addWidget(consLabelAdd,4,0);
    hoursLeftGrid->addWidget(zachLabelAdd,5,0);
    hoursLeftGrid->addWidget(examLabelAdd,6,0);
    hoursLeftGrid->addWidget(kursLabelAdd,7,0);
    hoursLeftGrid->addWidget(uchPrLabelAdd,8,0);
    hoursLeftGrid->addWidget(proizvPrLabelAdd,9,0);
    hoursLeftGrid->addWidget(predPrLabelAdd,10,0);
    hoursLeftGrid->addWidget(vklLabelAdd,11,0);
    hoursLeftGrid->addWidget(obzLabelAdd,12,0);
    hoursLeftGrid->addWidget(gekLabelAdd,13,0);
    hoursLeftGrid->addWidget(nirsLabelAdd,14,0);
    hoursLeftGrid->addWidget(aspLabelAdd,15,0);

    hoursLeftGrid->addWidget(lecButtonAdd,0,1);
    hoursLeftGrid->addWidget(semButtonAdd,1,1);
    hoursLeftGrid->addWidget(labButtonAdd,2,1);
    hoursLeftGrid->addWidget(contButtonAdd,3,1);
    hoursLeftGrid->addWidget(consButtonAdd,4,1);
    hoursLeftGrid->addWidget(zachButtonAdd,5,1);
    hoursLeftGrid->addWidget(examButtonAdd,6,1);
    hoursLeftGrid->addWidget(kursButtonAdd,7,1);
    hoursLeftGrid->addWidget(uchPrButtonAdd,8,1);
    hoursLeftGrid->addWidget(proizvPrButtonAdd,9,1);
    hoursLeftGrid->addWidget(predPrButtonAdd,10,1);
    hoursLeftGrid->addWidget(vklButtonAdd,11,1);
    hoursLeftGrid->addWidget(obzButtonAdd,12,1);
    hoursLeftGrid->addWidget(gekButtonAdd,13,1);
    hoursLeftGrid->addWidget(nirsButtonAdd,14,1);
    hoursLeftGrid->addWidget(aspButtonAdd,15,1);

    lecButtonRemove = new QToolButton;
    semButtonRemove = new QToolButton;
    labButtonRemove = new QToolButton;
    contButtonRemove = new QToolButton;
    consButtonRemove = new QToolButton;
    zachButtonRemove = new QToolButton;
    examButtonRemove = new QToolButton;
    kursButtonRemove = new QToolButton;
    uchPrButtonRemove = new QToolButton;
    proizvPrButtonRemove = new QToolButton;
    predPrButtonRemove = new QToolButton;
    vklButtonRemove = new QToolButton;
    obzButtonRemove = new QToolButton;
    gekButtonRemove = new QToolButton;
    nirsButtonRemove = new QToolButton;
    aspButtonRemove = new QToolButton;

    lecButtonRemove->setObjectName("lecButtonRemove");
    semButtonRemove->setObjectName("semButtonRemove");
    labButtonRemove->setObjectName("labButtonRemove");
    contButtonRemove->setObjectName("contButtonRemove");
    consButtonRemove->setObjectName("consButtonRemove");
    zachButtonRemove->setObjectName("zachButtonRemove");
    examButtonRemove->setObjectName("examButtonRemove");
    kursButtonRemove->setObjectName("kursButtonRemove");
    uchPrButtonRemove->setObjectName("uchPrButtonRemove");
    proizvPrButtonRemove->setObjectName("proizvPrButtonRemove");
    predPrButtonRemove->setObjectName("predPrButtonRemove");
    vklButtonRemove->setObjectName("vklButtonRemove");
    obzButtonRemove->setObjectName("obzButtonRemove");
    gekButtonRemove->setObjectName("gekButtonRemove");
    nirsButtonRemove->setObjectName("nirsButtonRemove");
    aspButtonRemove->setObjectName("aspButtonRemove");

    connect(lecButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(semButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(labButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(contButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(consButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(zachButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(examButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(kursButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(uchPrButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(proizvPrButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(predPrButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(vklButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(obzButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(gekButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(nirsButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));
    connect(aspButtonRemove,SIGNAL(clicked(bool)),this,SLOT(distributeAllHours()));

    lecLabelRemove = new QLabel;
    semLabelRemove = new QLabel;
    labLabelRemove = new QLabel;
    contLabelRemove = new QLabel;
    consLabelRemove = new QLabel;
    zachLabelRemove = new QLabel;
    examLabelRemove = new QLabel;
    kursLabelRemove = new QLabel;
    uchPrLabelRemove = new QLabel;
    proizvPrLabelRemove = new QLabel;
    predPrLabelRemove = new QLabel;
    vklLabelRemove = new QLabel;
    obzLabelRemove = new QLabel;
    gekLabelRemove = new QLabel;
    nirsLabelRemove = new QLabel;
    aspLabelRemove = new QLabel;

    lecButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    semButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    labButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    contButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    consButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    zachButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    examButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    kursButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    uchPrButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    proizvPrButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    predPrButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    vklButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    obzButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    gekButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    nirsButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    aspButtonRemove->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));


    hoursDistrGrid->addWidget(lecButtonRemove,0,0);
    hoursDistrGrid->addWidget(semButtonRemove,1,0);
    hoursDistrGrid->addWidget(labButtonRemove,2,0);
    hoursDistrGrid->addWidget(contButtonRemove,3,0);
    hoursDistrGrid->addWidget(consButtonRemove,4,0);
    hoursDistrGrid->addWidget(zachButtonRemove,5,0);
    hoursDistrGrid->addWidget(examButtonRemove,6,0);
    hoursDistrGrid->addWidget(kursButtonRemove,7,0);
    hoursDistrGrid->addWidget(uchPrButtonRemove,8,0);
    hoursDistrGrid->addWidget(proizvPrButtonRemove,9,0);
    hoursDistrGrid->addWidget(predPrButtonRemove,10,0);
    hoursDistrGrid->addWidget(vklButtonRemove,11,0);
    hoursDistrGrid->addWidget(obzButtonRemove,12,0);
    hoursDistrGrid->addWidget(gekButtonRemove,13,0);
    hoursDistrGrid->addWidget(nirsButtonRemove,14,0);
    hoursDistrGrid->addWidget(aspButtonRemove,15,0);

    hoursDistrGrid->addWidget(lecLabelRemove,0,1);
    hoursDistrGrid->addWidget(semLabelRemove,1,1);
    hoursDistrGrid->addWidget(labLabelRemove,2,1);
    hoursDistrGrid->addWidget(contLabelRemove,3,1);
    hoursDistrGrid->addWidget(consLabelRemove,4,1);
    hoursDistrGrid->addWidget(zachLabelRemove,5,1);
    hoursDistrGrid->addWidget(examLabelRemove,6,1);
    hoursDistrGrid->addWidget(kursLabelRemove,7,1);
    hoursDistrGrid->addWidget(uchPrLabelRemove,8,1);
    hoursDistrGrid->addWidget(proizvPrLabelRemove,9,1);
    hoursDistrGrid->addWidget(predPrLabelRemove,10,1);
    hoursDistrGrid->addWidget(vklLabelRemove,11,1);
    hoursDistrGrid->addWidget(obzLabelRemove,12,1);
    hoursDistrGrid->addWidget(gekLabelRemove,13,1);
    hoursDistrGrid->addWidget(nirsLabelRemove,14,1);
    hoursDistrGrid->addWidget(aspLabelRemove,15,1);


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

    classesTextEditHeader = new QLabel(tr("Нераспределенных часов"));
    teachersTextEditHeader = new QLabel(tr("Свободных часов"));

    classesTextEditHeader->setAlignment(Qt::AlignHCenter);
    teachersTextEditHeader->setAlignment(Qt::AlignHCenter);

    classesInfoVLayout->addWidget(classesTextEditHeader);
    classesInfoVLayout->addWidget(classesInfoGroupBox);
    teachersInfoVLayout->addWidget(teachersTextEditHeader);
    teachersInfoVLayout->addWidget(teachersInfoGroupBox);

//    classesInfoGroupBox->setLayout(classesInfoVLayout);
//    teachersInfoGroupBox->setLayout(teachersInfoVLayout);

    classesInfoGroupBox->setLayout(hoursLeftGrid);
    teachersInfoGroupBox->setLayout(hoursDistrGrid);

    classesInfoGroupBox->setMinimumWidth(200);
    teachersInfoGroupBox->setMinimumWidth(200);

    classesInfoGroupBox->setVisible(0);
    teachersInfoGroupBox->setVisible(0);
    classesTextEditHeader->setVisible(0);
    teachersTextEditHeader->setVisible(0);
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
    externalHLayout->addLayout(classesInfoVLayout);
    externalHLayout->addLayout(internalMiddleVLayout);
    externalHLayout->addLayout(teachersInfoVLayout);
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

void MainWindowView::setData(QList<double> list)
{
    myList = list;
}


void MainWindowView::distributeAllHours()
{
 //   qDebug() << sender();
    doubleClickClassUpdate(savedClassIndex);
    if (sender()->objectName() == "lecButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,lekcii_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(1));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET lekcii_chasov = lekcii_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(1)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "semButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,seminary_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(2));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET seminary_chasov = seminary_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(2)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "labButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,lab_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(3));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET lab_chasov = lab_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(3)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "contButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,kontrol_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(4));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET kontrol_chasov = kontrol_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(4)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "consButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,konsultacii_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(5));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET konsultacii_chasov = konsultacii_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(5)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "zachButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,zachet_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(6));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET zachet_chasov = zachet_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(6)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "examButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,ekzamen_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(7));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET ekzamen_chasov = ekzamen_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(7)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "kursButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,kursovie_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(8));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET kursovie_chasov = kursovie_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(8)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "uchPrButtonAdd")
    {


        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,ucheb_praktika_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(9));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET ucheb_praktika_chasov = ucheb_praktika_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(9)).arg(chosenClassID));

    }
    else if (sender()->objectName() == "proizvPrButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,proizv_praktika_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(10));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET proizv_praktika_chasov = proizv_praktika_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(10)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "predPrButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,preddipl_praktika_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(11));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET preddipl_praktika_chasov = preddipl_praktika_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(11)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "vklButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,vkl_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(12));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET vkl_chasov = vkl_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(12)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "obzButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,obz_lekcii_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(13));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET obz_lekcii_chasov = obz_lekcii_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(13)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "gekButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,gek_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(14));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET gek_chasov = gek_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(14)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "nirsButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,nirs_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(15));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET nirs_chasov = nirs_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(15)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "aspButtonAdd")
    {
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,asp_dokt_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(totalHoursLeftList.value(16));
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET asp_dokt_chasov = asp_dokt_chasov - %1 WHERE id_zapisi = %2").arg(totalHoursLeftList.value(16)).arg(chosenClassID));
    }
    else if (sender()->objectName() == "lecButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(1);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,lekcii_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET lekcii_chasov = lekcii_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "semButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(2);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,seminary_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET seminary_chasov = seminary_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "labButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(3);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,lab_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET lab_chasov = lab_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "contButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(4);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,kontrol_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET kontrol_chasov = kontrol_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "consButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(5);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,konsultacii_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET konsultacii_chasov = konsultacii_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "zachButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(6);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,zachet_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET zachet_chasov = zachet_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "examButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(7);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,ekzamen_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET ekzamen_chasov = ekzamen_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "kursButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(8);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,kursovie_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET kursovie_chasov = kursovie_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "uchPrButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(9);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,ucheb_praktika_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET ucheb_praktika_chasov = ucheb_praktika_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));

    }
    else if (sender()->objectName() == "proizvPrButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(10);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,proizv_praktika_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET proizv_praktika_chasov = proizv_praktika_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "predPrButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(11);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,preddipl_praktika_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET preddipl_praktika_chasov = preddipl_praktika_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "vklButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(12);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,vkl_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET vkl_chasov = vkl_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "obzButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(13);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,obz_lekcii_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET obz_lekcii_chasov = obz_lekcii_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "gekButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(14);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,gek_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET gek_chasov = gek_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "nirsButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(15);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,nirs_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET nirs_chasov = nirs_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
    else if (sender()->objectName() == "aspButtonRemove")
    {
        double val = -1 * hoursDistributedForSelectedList.value(16);
        QString query = QString("INSERT INTO raspredelenie(id_zanyatosti,id_prep,asp_dokt_chasov) VALUES (%1,%2,%3)")
                .arg(chosenClassID)
                .arg(chosenTeacherID)
                .arg(val);
        runner->tryQuery(query);
        runner->tryQuery(QString("UPDATE zanyatost SET asp_dokt_chasov = asp_dokt_chasov - %1 WHERE id_zapisi = %2").arg(val).arg(chosenClassID));
    }
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
        classesTextEditHeader->setVisible(1);
        teachersTextEditHeader->setVisible(1);
    }
}

void MainWindowView::doubleClickClassUpdate(const QModelIndex index)
{
    savedClassIndex = index;
    chosenClass->setText(classModelReference->data(index).toString());
    chosenClassID = classModelReference->data(classModelReference->index(index.row(),0,QModelIndex())).toInt();
    QString query = QString("Select id_zapisi, lekcii_chasov,seminary_chasov,lab_chasov,kontrol_chasov,konsultacii_chasov,"
                            "zachet_chasov,ekzamen_chasov,kursovie_chasov,ucheb_praktika_chasov,proizv_praktika_chasov,preddipl_praktika_chasov,vkl_chasov,obz_lekcii_chasov,gek_chasov,nirs_chasov,"
                            "asp_dokt_chasov,semestr from zanyatost where id_zapisi = %1").arg(chosenClassID);
    runner->tryQuery(query,0,1);
    totalHoursLeftList = myList;
    currentSemester = totalHoursLeftList.value(17);
    updateDataForSelectedClass();

    val1->setTop(totalHoursLeftList.value(1));
    val2->setTop(totalHoursLeftList.value(2));
    val3->setTop(totalHoursLeftList.value(3));
    val4->setTop(totalHoursLeftList.value(4));
    val5->setTop(totalHoursLeftList.value(5));
    val6->setTop(totalHoursLeftList.value(6));
    val7->setTop(totalHoursLeftList.value(7));
    val8->setTop(totalHoursLeftList.value(8));
    val9->setTop(totalHoursLeftList.value(9));
    val10->setTop(totalHoursLeftList.value(10));
    val11->setTop(totalHoursLeftList.value(11));
    val12->setTop(totalHoursLeftList.value(12));
    val13->setTop(totalHoursLeftList.value(13));
    val14->setTop(totalHoursLeftList.value(14));
    val15->setTop(totalHoursLeftList.value(15));
    val16->setTop(totalHoursLeftList.value(16));

    lecLabelAdd->setText("Часы лекций: "+QString::number(totalHoursLeftList.value(1)));
    semLabelAdd->setText("Часы семинаров: "+QString::number(totalHoursLeftList.value(2)));
    labLabelAdd->setText("Часы лабораторных: "+QString::number(totalHoursLeftList.value(3)));
    contLabelAdd->setText("Часы контрольных: "+QString::number(totalHoursLeftList.value(4)));
    consLabelAdd->setText("Часы консультаций: "+QString::number(totalHoursLeftList.value(5)));
    zachLabelAdd->setText("Часы зачета: "+QString::number(totalHoursLeftList.value(6)));
    examLabelAdd->setText("Часы экзамена: "+QString::number(totalHoursLeftList.value(7)));
    kursLabelAdd->setText("Часы курсовых: "+QString::number(totalHoursLeftList.value(8)));
    uchPrLabelAdd->setText("Часы учебной практики: "+QString::number(totalHoursLeftList.value(9)));
    proizvPrLabelAdd->setText("Часы произв. практики: "+QString::number(totalHoursLeftList.value(10)));
    predPrLabelAdd->setText("Часы преддипл. практики: "+QString::number(totalHoursLeftList.value(11)));
    vklLabelAdd->setText("Часы вкл: "+QString::number(totalHoursLeftList.value(12)));
    obzLabelAdd->setText("Часы обз. лекций: "+QString::number(totalHoursLeftList.value(13)));
    gekLabelAdd->setText("Часы ГЭК: "+QString::number(totalHoursLeftList.value(14)));
    nirsLabelAdd->setText("Часы НИРС: "+QString::number(totalHoursLeftList.value(15)));
    aspLabelAdd->setText("Часы асп/докторских: "+QString::number(totalHoursLeftList.value(16)));



    MainWindowView::checkFields();
}

void MainWindowView::doubleClickTeacherUpdate(const QModelIndex index)
{
    savedTeacherIndex = index;
    chosenTeacher->setText(teacherModelReference->data(index).toString());
    chosenTeacherID = teacherModelReference->data(teacherModelReference->index(index.row(),0,QModelIndex())).toInt();
//    QString query = QString("SELECT "
//                            "tab.id_prep,"
//                            "normativy.lekcii_chasov - tab.A,"
//                            "normativy.seminary_chasov - tab.B,"
//                            "normativy.lab_chasov - tab.C,"
//                            "normativy.kontrol_chasov - tab.D,"
//                            "normativy.lekcii_chasov - tab.E,"
//                            "normativy.zachet_chasov - tab.F,"
//                            "normativy.ekzamen_chasov - tab.G,"
//                            "normativy.kursovie_chasov - tab.H,"
//                            "normativy.ucheb_praktika_chasov - tab.I,"
//                            "normativy.proizv_praktika_chasov - tab.J,"
//                            "normativy.preddipl_praktika_chasov - tab.K,"
//                            "normativy.vkl_chasov - tab.L,"
//                            "normativy.obz_lekcii_chasov - tab.M,"
//                            "normativy.gek_chasov - tab.N,"
//                            "normativy.nirs_chasov - tab.O,"
//                            "normativy.asp_dokt_chasov - tab.P "
//                            "FROM normativy,"
//                            "(SELECT "
//                            "raspredelenie.id_prep,"
//                            "sum(raspredelenie.lekcii_chasov) as A,"
//                            "sum(raspredelenie.seminary_chasov) as B,"
//                            "sum(raspredelenie.lab_chasov) as C,"
//                            "sum(raspredelenie.kontrol_chasov) as D,"
//                            "sum(raspredelenie.konsultacii_chasov) as E,"
//                            "sum(raspredelenie.zachet_chasov) as F,"
//                            "sum(raspredelenie.ekzamen_chasov) as G,"
//                            "sum(raspredelenie.kursovie_chasov) as H,"
//                            "sum(raspredelenie.ucheb_praktika_chasov) as I,"
//                            "sum(raspredelenie.proizv_praktika_chasov) as J,"
//                            "sum(raspredelenie.preddipl_praktika_chasov) as K,"
//                            "sum(raspredelenie.vkl_chasov) as L,"
//                            "sum(raspredelenie.obz_lekcii_chasov) as M,"
//                            "sum(raspredelenie.gek_chasov) as N,"
//                            "sum(raspredelenie.nirs_chasov) as O,"
//                            "sum(raspredelenie.asp_dokt_chasov) as P FROM raspredelenie WHERE id_prep = %1 group by id_prep) as tab;").arg(chosenTeacherID);

    updateDataForSelectedClass();

    MainWindowView::checkFields();
}

void MainWindowView::updateDataForSelectedClass()
{
    QString query = QString("SELECT "
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
                            "sum(raspredelenie.asp_dokt_chasov) as P "
                            "FROM raspredelenie "
                            "left join zanyatost on raspredelenie.id_zanyatosti = zanyatost.id_zapisi "
                            "WHERE id_prep = %1 and zanyatost.semestr = %2 group by id_prep;").arg(chosenTeacherID).arg(currentSemester);
    runner->tryQuery(query,0,1);
    totalHoursDistributedList = myList;

    query = QString("SELECT "
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
                            "sum(raspredelenie.asp_dokt_chasov) as P "
                            "FROM raspredelenie "
                            "left join zanyatost on raspredelenie.id_zanyatosti = zanyatost.id_zapisi "
                            "WHERE id_prep = %1 AND raspredelenie.id_zanyatosti = %2 group by id_prep;").arg(chosenTeacherID).arg(chosenClassID);
    runner->tryQuery(query,0,1);
    hoursDistributedForSelectedList = myList;


    lecLabelRemove->setText(QString("Часы лекций: %1(%2)").arg(totalHoursDistributedList.value(1)).arg(hoursDistributedForSelectedList.value(1)));
    semLabelRemove->setText(QString("Часы семинаров: %1(%2)").arg(totalHoursDistributedList.value(2)).arg(hoursDistributedForSelectedList.value(2)));
    labLabelRemove->setText(QString("Часы лабораторных: %1(%2)").arg(totalHoursDistributedList.value(3)).arg(hoursDistributedForSelectedList.value(3)));
    contLabelRemove->setText(QString("Часы контрольных: %1(%2)").arg(totalHoursDistributedList.value(4)).arg(hoursDistributedForSelectedList.value(4)));
    consLabelRemove->setText(QString("Часы консультаций: %1(%2)").arg(totalHoursDistributedList.value(5)).arg(hoursDistributedForSelectedList.value(5)));
    zachLabelRemove->setText(QString("Часы зачета: %1(%2)").arg(totalHoursDistributedList.value(6)).arg(hoursDistributedForSelectedList.value(6)));
    examLabelRemove->setText(QString("Часы экзамена: %1(%2)").arg(totalHoursDistributedList.value(7)).arg(hoursDistributedForSelectedList.value(7)));
    kursLabelRemove->setText(QString("Часы курсовых: %1(%2)").arg(totalHoursDistributedList.value(8)).arg(hoursDistributedForSelectedList.value(8)));
    uchPrLabelRemove->setText(QString("Часы учебной практики: %1(%2)").arg(totalHoursDistributedList.value(9)).arg(hoursDistributedForSelectedList.value(9)));
    proizvPrLabelRemove->setText(QString("Часы произв. практики: %1(%2)").arg(totalHoursDistributedList.value(10)).arg(hoursDistributedForSelectedList.value(10)));
    predPrLabelRemove->setText(QString("Часы преддипл. практики: %1(%2)").arg(totalHoursDistributedList.value(11)).arg(hoursDistributedForSelectedList.value(11)));
    vklLabelRemove->setText(QString("Часы вкл: %1(%2)").arg(totalHoursDistributedList.value(12)).arg(hoursDistributedForSelectedList.value(12)));
    obzLabelRemove->setText(QString("Часы обз. лекций: %1(%2)").arg(totalHoursDistributedList.value(13)).arg(hoursDistributedForSelectedList.value(13)));
    gekLabelRemove->setText(QString("Часы ГЭК: %1(%2)").arg(totalHoursDistributedList.value(14)).arg(hoursDistributedForSelectedList.value(14)));
    nirsLabelRemove->setText(QString("Часы НИРС: %1(%2)").arg(totalHoursDistributedList.value(15)).arg(hoursDistributedForSelectedList.value(15)));
    aspLabelRemove->setText(QString("Часы асп/докторских: %1(%2)").arg(totalHoursDistributedList.value(16)).arg(hoursDistributedForSelectedList.value(16)));


}

void MainWindowView::clearParameters()
{
    chosenClass->clear();
    chosenTeacher->clear();
    this->setStyleSheet("QGroupBox#workField {}");
    workField->setEnabled(false);
    classesInfoGroupBox->setVisible(0);
    teachersInfoGroupBox->setVisible(0);
    classesTextEditHeader->setVisible(0);
    teachersTextEditHeader->setVisible(0);
}

void MainWindowView::newFile()
{
    LoadNewFileView *frm = new LoadNewFileView(receivedID);
    frm->show();
}
