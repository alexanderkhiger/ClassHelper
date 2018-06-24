#include "tableeditorview.h"

TableEditorView::TableEditorView(QString uID, QString uName, QString uShortname, QWidget *parent) : QWidget(parent)
{
    facultyAddButtonState = INACTIVE;
    chairAddButtonState = INACTIVE;
    teacherAddButtonState = INACTIVE;
    disciplineAddButtonState = INACTIVE;
    specialtyAddButtonState = INACTIVE;
    streamAddButtonState = INACTIVE;

    receivedUID = uID;
    receivedName = uName;
    receivedShortname = uShortname;
    teModel = new TableEditorModel;
    connect(teModel,SIGNAL(updateError(QSqlError)),this,SLOT(getError(QSqlError)));
    connect(this,SIGNAL(updateError(QSqlError)),this,SLOT(getError(QSqlError)));
    chairRunner = new QueryRunner;
    facultyRunner = new QueryRunner;
    teacherRunner = new QueryRunner;
    disciplineRunner = new QueryRunner;
    specialtyRunner = new QueryRunner;
    streamRunner = new QueryRunner;

    setMinimumSize(300,300);
    setWindowTitle(tr("Работа с таблицами | %1 | %2").arg(uName).arg(uShortname));

    tableTab = new QTabWidget;
    chairWidget = new QWidget;
    facultyWidget = new QWidget;
    teacherWidget = new QWidget;
    disciplineWidget = new QWidget;
    specialtyWidget = new QWidget;
    streamWidget = new QWidget;

    tableTab->addTab(facultyWidget,tr("Факультеты"));
    tableTab->addTab(chairWidget,tr("Кафедры"));
    tableTab->addTab(teacherWidget,tr("Преподаватели"));
    tableTab->addTab(disciplineWidget,tr("Дисциплины"));
    tableTab->addTab(specialtyWidget,tr("Специальности"));
    tableTab->addTab(streamWidget,tr("Потоки"));

    connect(tableTab,SIGNAL(currentChanged(int)),this,SLOT(checkSize(int)));

    getIDTable = new QTableView;
    agree = new QPushButton(tr("ОК"));
    disagree = new QPushButton(tr("Назад"));
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(agree,QDialogButtonBox::AcceptRole);
    buttonBox->addButton(disagree,QDialogButtonBox::RejectRole);
    connect(disagree,SIGNAL(clicked(bool)),this,SLOT(setSmallTablesInvisible()));
    getIDTable->setVisible(0);
    buttonBox->setVisible(0);
    getIDTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    getIDTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    getIDTable->setSelectionMode(QAbstractItemView::SingleSelection);
    getIDTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    getIDTable->verticalHeader()->setVisible(0);
    connect(getIDTable,SIGNAL(clicked(QModelIndex)),this,SLOT(enableGetIDButtons()));

    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(tableTab);
    vLayout->addWidget(getIDTable);
    vLayout->addWidget(buttonBox);

    createChairWidgetUI();
    createFacultyWidgetUI();
    createTeacherWidgetUI();
    createDisciplineWidgetUI();
    createSpecialtyWidgetUI();
    createStreamWidgetUI();

//    facultyTable->viewport()->installEventFilter(this);
//    chairTable->viewport()->installEventFilter(this);
//    teacherTable->viewport()->installEventFilter(this);
//    disciplineTable->viewport()->installEventFilter(this);
//    specialtyTable->viewport()->installEventFilter(this);
//    streamTable->viewport()->installEventFilter(this);

    connect(facultyTable,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(disableFacultyWidgets()));
    connect(chairTable,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(disableChairWidgets()));
    connect(teacherTable,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(disableTeacherWidgets()));
    connect(disciplineTable,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(disableDisciplineWidgets()));
    connect(specialtyTable,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(disableSpecialtyWidgets()));
    connect(streamTable,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(disableStreamWidgets()));
    resizeTable(facultyTable);
}

//bool TableEditorView::eventFilter(QObject *obj, QEvent *event) {

//    if (event->type() == QEvent::MouseButtonDblClick) {
//        QMouseEvent * mouseEvent = static_cast <QMouseEvent *> (event);
//        if (mouseEvent -> button() == Qt::LeftButton) {
//            if (obj == facultyTable->viewport())
//            {
//                disableFacultyWidgets();
//            }
//            else if (obj == chairTable->viewport())
//            {
//                disableChairWidgets();
//            }
//            else if (obj == teacherTable->viewport())
//            {
//                disableTeacherWidgets();
//            }
//            else if (obj == disciplineTable->viewport())
//            {
//                disableDisciplineWidgets();
//            }
//            else if (obj == specialtyTable->viewport())
//            {
//                disableSpecialtyWidgets();
//            }
//            else if (obj == streamTable->viewport())
//            {
//                disableStreamWidgets();
//            }
//        }
//    }
//    return QWidget::eventFilter(obj, event);
//}

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
    else if (index == 2)
    {
        resizeTable(teacherTable);
    }
    else if (index == 3)
    {
        resizeTable(disciplineTable);
    }
    else if (index == 4)
    {
        resizeTable(specialtyTable);
    }
    else if (index == 5)
    {
        resizeTable(streamTable);
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
    checkSize(tableTab->currentIndex());
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

void TableEditorView::enableDisciplineButtons()
{
    disciplineDeleteButton->setEnabled(1);
}

void TableEditorView::enableSpecialtyButtons()
{
    specialtyDeleteButton->setEnabled(1);
}

void TableEditorView::enableStreamButtons()
{
    streamDeleteButton->setEnabled(1);
}



void TableEditorView::enableGetIDButtons()
{
    agree->setEnabled(1);
}

void TableEditorView::createFacultyWidgetUI()
{
    facultyTable = new CustomTableView;
    facultyTable->setEditTriggers(QAbstractItemView::DoubleClicked);
    //    facultyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
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

    //    facultyItemDelegate = new CustomItemDelegate;
    //    facultyTable->setItemDelegate(facultyItemDelegate);

    connect(facultyAddButton,SIGNAL(clicked(bool)),this,SLOT(changeFacultyAddButtonStyle()));
    connect(facultyConfirmAddition,SIGNAL(clicked(bool)),this,SLOT(facultyAddRecord()));
    connect(facultyDeleteButton,SIGNAL(clicked(bool)),this,SLOT(facultyDeleteRecord()));
    connect(facultyTable,SIGNAL(clicked(QModelIndex)),this,SLOT(enableFacultyButtons()));
    connect(facultyTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(facultyEditRecord()));
    connect(facultyTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(enableWidgets()));
//    connect(facultyTable,&QTableView::doubleClicked,this,&TableEditorView::disableFacultyWidgets);
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
    chairTable = new CustomTableView;
    chairTable->setEditTriggers(QAbstractItemView::DoubleClicked);
    //    chairTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    chairTable->setSelectionMode(QAbstractItemView::SingleSelection);
    chairTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    chairTable->verticalHeader()->setVisible(0);
    chairName = new QLineEdit;
    chairShortname = new QLineEdit;
    chairChooseFaculty = new CustomLineEdit(this);
    chairChooseFaculty->setPlaceholderText(tr("ID факультета"));
    chairChooseFaculty->setToolTip(tr("ID факультета"));

    connect(chairChooseFaculty->chooseButton,SIGNAL(clicked(bool)),this,SLOT(openFacultyList()));

    chairConfirmAddition = new QPushButton(tr("ОК"));
    chairAddButton = new QPushButton(tr("Добавить"));
    chairDeleteButton = new QPushButton(tr("Удалить"));
    chairDeleteButton->setEnabled(0);

    chairName->setPlaceholderText(tr("Название"));
    chairName->setToolTip(tr("Название факультета"));

    chairShortname->setPlaceholderText(tr("Аббревиатура"));
    chairShortname->setToolTip(tr("Аббревиатура факультета"));

    connect(chairAddButton,SIGNAL(clicked(bool)),this,SLOT(changeChairAddButtonStyle()));
    connect(chairConfirmAddition,SIGNAL(clicked(bool)),this,SLOT(chairAddRecord()));
    connect(chairDeleteButton,SIGNAL(clicked(bool)),this,SLOT(chairDeleteRecord()));
    connect(chairTable,SIGNAL(clicked(QModelIndex)),this,SLOT(enableChairButtons()));
    connect(chairTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(chairEditRecord()));
    connect(chairTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(enableWidgets()));
//    connect(chairTable,&QTableView::doubleClicked,this,&TableEditorView::disableChairWidgets);

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
    teacherTable = new CustomTableView;
    teacherTable->setEditTriggers(QAbstractItemView::DoubleClicked);
    //    teacherTable->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    connect(teacherChooseChair->chooseButton,SIGNAL(clicked(bool)),this,SLOT(openChairList()));

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

    connect(teacherAddButton,SIGNAL(clicked(bool)),this,SLOT(changeTeacherAddButtonStyle()));
    connect(teacherConfirmAddition,SIGNAL(clicked(bool)),this,SLOT(teacherAddRecord()));
    connect(teacherDeleteButton,SIGNAL(clicked(bool)),this,SLOT(teacherDeleteRecord()));
    connect(teacherTable,SIGNAL(clicked(QModelIndex)),this,SLOT(enableTeacherButtons()));
    connect(teacherTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(teacherEditRecord()));
    connect(teacherTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(enableWidgets()));
//    connect(teacherTable,&QTableView::doubleClicked,this,&TableEditorView::disableTeacherWidgets);

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

void TableEditorView::createDisciplineWidgetUI()
{
    disciplineTable = new CustomTableView;
    disciplineTable->setEditTriggers(QAbstractItemView::DoubleClicked);
    //    chairTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    disciplineTable->setSelectionMode(QAbstractItemView::SingleSelection);
    disciplineTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    disciplineTable->verticalHeader()->setVisible(0);
    disciplineName = new QLineEdit;
    disciplineConfirmAddition = new QPushButton(tr("ОК"));
    disciplineAddButton = new QPushButton(tr("Добавить"));
    disciplineDeleteButton = new QPushButton(tr("Удалить"));
    disciplineDeleteButton->setEnabled(0);

    disciplineName->setPlaceholderText(tr("Название"));
    disciplineName->setToolTip(tr("Название дисциплины"));

    connect(disciplineAddButton,SIGNAL(clicked(bool)),this,SLOT(changeDisciplineAddButtonStyle()));
    connect(disciplineConfirmAddition,SIGNAL(clicked(bool)),this,SLOT(disciplineAddRecord()));
    connect(disciplineDeleteButton,SIGNAL(clicked(bool)),this,SLOT(disciplineDeleteRecord()));

    connect(disciplineTable,SIGNAL(clicked(QModelIndex)),this,SLOT(enableDisciplineButtons()));
    connect(disciplineTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(disciplineEditRecord()));
    connect(disciplineTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(enableWidgets()));
//    connect(disciplineTable,&QTableView::doubleClicked,this,&TableEditorView::disableChairWidgets);

    disciplineConfirmAddition->setVisible(0);
    disciplineName->setVisible(0);

    disciplineTopHLayout = new QHBoxLayout;
    disciplineTopHLayout->addWidget(disciplineName);
    disciplineTopHLayout->addWidget(disciplineConfirmAddition);

    disciplineBotHLayout = new QHBoxLayout;
    disciplineBotHLayout->addWidget(disciplineAddButton);
    disciplineBotHLayout->addWidget(disciplineDeleteButton);

    disciplineVLayout = new QVBoxLayout;
    disciplineVLayout->addWidget(disciplineTable);
    disciplineVLayout->addLayout(disciplineTopHLayout);
    disciplineVLayout->addLayout(disciplineBotHLayout);

    disciplineWidget->setLayout(disciplineVLayout);

    getDisciplineModel();
}

void TableEditorView::createSpecialtyWidgetUI()
{
    specialtyTable = new CustomTableView;
    specialtyTable->setEditTriggers(QAbstractItemView::DoubleClicked);
    //    chairTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    specialtyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    specialtyTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    specialtyTable->verticalHeader()->setVisible(0);
    specialtyName = new QLineEdit;
    specialtyChooseFaculty = new CustomLineEdit(this);
    specialtyChooseFaculty->setPlaceholderText(tr("ID факультета"));
    specialtyChooseFaculty->setToolTip(tr("ID факультета"));

    connect(specialtyChooseFaculty->chooseButton,SIGNAL(clicked(bool)),this,SLOT(openFacultyList()));

    specialtyConfirmAddition = new QPushButton(tr("ОК"));
    specialtyAddButton = new QPushButton(tr("Добавить"));
    specialtyDeleteButton = new QPushButton(tr("Удалить"));
    specialtyDeleteButton->setEnabled(0);

    specialtyName->setPlaceholderText(tr("Название"));
    specialtyName->setToolTip(tr("Название специальности"));

    connect(specialtyAddButton,SIGNAL(clicked(bool)),this,SLOT(changeSpecialtyAddButtonStyle()));
    connect(specialtyConfirmAddition,SIGNAL(clicked(bool)),this,SLOT(specialtyAddRecord()));
    connect(specialtyDeleteButton,SIGNAL(clicked(bool)),this,SLOT(specialtyDeleteRecord()));
    connect(specialtyTable,SIGNAL(clicked(QModelIndex)),this,SLOT(enableSpecialtyButtons()));
    connect(specialtyTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(specialtyEditRecord()));
    connect(specialtyTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(enableWidgets()));
//    connect(chairTable,&QTableView::doubleClicked,this,&TableEditorView::disableChairWidgets);

    specialtyConfirmAddition->setVisible(0);
    specialtyName->setVisible(0);
    specialtyChooseFaculty->setVisible(0);

    specialtyTopHLayout = new QHBoxLayout;
    specialtyTopHLayout->addWidget(specialtyName);
    specialtyTopHLayout->addWidget(specialtyChooseFaculty);
    specialtyTopHLayout->addWidget(specialtyConfirmAddition);

    specialtyBotHLayout = new QHBoxLayout;
    specialtyBotHLayout->addWidget(specialtyAddButton);
    specialtyBotHLayout->addWidget(specialtyDeleteButton);

    specialtyVLayout = new QVBoxLayout;
    specialtyVLayout->addWidget(specialtyTable);
    specialtyVLayout->addLayout(specialtyTopHLayout);
    specialtyVLayout->addLayout(specialtyBotHLayout);
    specialtyWidget->setLayout(specialtyVLayout);

    getSpecialtyModel();
}

void TableEditorView::createStreamWidgetUI()
{
    streamTable = new CustomTableView;
    streamTable->setEditTriggers(QAbstractItemView::DoubleClicked);
    //    teacherTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    streamTable->setSelectionMode(QAbstractItemView::SingleSelection);
    streamTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    streamTable->verticalHeader()->setVisible(0);


    streamGroups = new QLineEdit;
    streamSubgroups = new QLineEdit;
    streamStudents = new QLineEdit;
    streamName = new QLineEdit;
    streamShortName = new QLineEdit;
    streamYear = new QLineEdit;

    streamChooseSpecialty = new CustomLineEdit(this);
    connect(streamChooseSpecialty->chooseButton,SIGNAL(clicked(bool)),this,SLOT(openSpecialtyList()));

    streamConfirmAddition = new QPushButton(tr("ОК"));
    streamAddButton = new QPushButton(tr("Добавить"));
    streamDeleteButton = new QPushButton(tr("Удалить"));
    streamDeleteButton->setEnabled(0);

    streamGroups->setPlaceholderText(tr("Кол-во групп"));
    streamGroups->setToolTip(tr("Количество групп"));

    streamSubgroups->setPlaceholderText(tr("Кол-во подгрупп"));
    streamSubgroups->setToolTip(tr("Количество подгрупп"));

    streamStudents->setPlaceholderText(tr("Кол-во студентов"));
    streamStudents->setToolTip(tr("Количество студентов"));

    streamName->setPlaceholderText(tr("Название потока"));
    streamName->setToolTip(tr("Название потока"));

    streamShortName->setPlaceholderText(tr("Аббревиатура"));
    streamShortName->setToolTip(tr("Аббревиатура потока"));

    streamChooseSpecialty->setPlaceholderText(tr("ID специальности"));
    streamChooseSpecialty->setToolTip(tr("ID специальности"));

    streamYear->setPlaceholderText(tr("Курс"));
    streamYear->setToolTip(tr("Курс"));

    connect(streamAddButton,SIGNAL(clicked(bool)),this,SLOT(changeStreamAddButtonStyle()));
    connect(streamConfirmAddition,SIGNAL(clicked(bool)),this,SLOT(streamAddRecord()));
    connect(streamDeleteButton,SIGNAL(clicked(bool)),this,SLOT(streamDeleteRecord()));
    connect(streamTable,SIGNAL(clicked(QModelIndex)),this,SLOT(enableStreamButtons()));
    connect(streamTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(streamEditRecord()));
    connect(streamTable->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(enableWidgets()));
//    connect(teacherTable,&QTableView::doubleClicked,this,&TableEditorView::disableTeacherWidgets);

    streamConfirmAddition->setVisible(0);
    streamGroups->setVisible(0);
    streamSubgroups->setVisible(0);
    streamStudents->setVisible(0);
    streamName->setVisible(0);
    streamShortName->setVisible(0);
    streamChooseSpecialty->setVisible(0);
    streamYear->setVisible(0);

    streamTopHLayout = new QHBoxLayout;
    streamTopHLayout->addWidget(streamName);
    streamTopHLayout->addWidget(streamShortName);
    streamTopHLayout->addWidget(streamChooseSpecialty);
    streamTopHLayout->addWidget(streamYear);
    streamTopHLayout->addWidget(streamConfirmAddition);

    streamMidHLayout = new QHBoxLayout;
    streamMidHLayout->addWidget(streamStudents);
    streamMidHLayout->addWidget(streamGroups);
    streamMidHLayout->addWidget(streamSubgroups);

    streamBotHLayout = new QHBoxLayout;
    streamBotHLayout->addWidget(streamAddButton);
    streamBotHLayout->addWidget(streamDeleteButton);

    streamVLayout = new QVBoxLayout;
    streamVLayout->addWidget(streamTable);
    streamVLayout->addLayout(streamTopHLayout);
    streamVLayout->addLayout(streamMidHLayout);
    streamVLayout->addLayout(streamBotHLayout);
    streamWidget->setLayout(streamVLayout);

    getStreamModel();
}


void TableEditorView::changeFacultyAddButtonStyle()
{
    facultyName->clear();
    facultyShortname->clear();
    if (facultyAddButtonState == INACTIVE)
    {
        facultyName->setVisible(1);
        facultyShortname->setVisible(1);
        facultyConfirmAddition->setVisible(1);
        facultyAddButtonState = ACTIVE;
        facultyAddButton->setStyleSheet("background-color:gray; border-color:black; color:white");
    }
    else
    {
        facultyName->setVisible(0);
        facultyShortname->setVisible(0);
        facultyConfirmAddition->setVisible(0);
        facultyAddButtonState = INACTIVE;
        facultyAddButton->setStyleSheet("");
    }

}

void TableEditorView::changeChairAddButtonStyle()
{
    chairName->clear();
    chairShortname->clear();
    chairChooseFaculty->clear();
    if (chairAddButtonState == INACTIVE)
    {
        chairName->setVisible(1);
        chairShortname->setVisible(1);
        chairConfirmAddition->setVisible(1);
        chairChooseFaculty->setVisible(1);
        chairAddButtonState = ACTIVE;
        chairAddButton->setStyleSheet("background-color:gray; border-color:black; color:white");
    }
    else
    {
        chairName->setVisible(0);
        chairShortname->setVisible(0);
        chairConfirmAddition->setVisible(0);
        chairChooseFaculty->setVisible(0);
        chairAddButtonState = INACTIVE;
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

    if (teacherAddButtonState == INACTIVE)
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
        teacherAddButtonState = ACTIVE;
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
        teacherAddButtonState = INACTIVE;
        teacherAddButton->setStyleSheet("");

    }

}


void TableEditorView::changeStreamAddButtonStyle()
{
    streamGroups->clear();
    streamSubgroups->clear();
    streamStudents->clear();
    streamName->clear();
    streamShortName->clear();
    streamYear->clear();

    if (streamAddButtonState == INACTIVE)
    {
        streamConfirmAddition->setVisible(1);
        streamGroups->setVisible(1);
        streamSubgroups->setVisible(1);
        streamStudents->setVisible(1);
        streamName->setVisible(1);
        streamShortName->setVisible(1);
        streamChooseSpecialty->setVisible(1);
        streamYear->setVisible(1);
        streamAddButtonState = ACTIVE;
        streamAddButton->setStyleSheet("background-color:gray; border-color:black; color:white");
    }
    else
    {
        streamConfirmAddition->setVisible(0);
        streamGroups->setVisible(0);
        streamSubgroups->setVisible(0);
        streamStudents->setVisible(0);
        streamName->setVisible(0);
        streamShortName->setVisible(0);
        streamChooseSpecialty->setVisible(0);
        streamYear->setVisible(0);
        streamAddButtonState = INACTIVE;
        streamAddButton->setStyleSheet("");

    }

}


void TableEditorView::changeDisciplineAddButtonStyle()
{
    disciplineName->clear();
    if (disciplineAddButtonState == INACTIVE)
    {
        disciplineName->setVisible(1);
        disciplineConfirmAddition->setVisible(1);
        disciplineAddButtonState = ACTIVE;
        disciplineAddButton->setStyleSheet("background-color:gray; border-color:black; color:white");
    }
    else
    {
        disciplineName->setVisible(0);
        disciplineConfirmAddition->setVisible(0);
        disciplineAddButtonState = INACTIVE;
        disciplineAddButton->setStyleSheet("");
    }
}

void TableEditorView::changeSpecialtyAddButtonStyle()
{
    specialtyName->clear();
    specialtyChooseFaculty->clear();
    if (specialtyAddButtonState == INACTIVE)
    {
        specialtyName->setVisible(1);
        specialtyConfirmAddition->setVisible(1);
        specialtyChooseFaculty->setVisible(1);
        specialtyAddButtonState = ACTIVE;
        specialtyAddButton->setStyleSheet("background-color:gray; border-color:black; color:white");
    }
    else
    {
        specialtyName->setVisible(0);
        specialtyConfirmAddition->setVisible(0);
        specialtyChooseFaculty->setVisible(0);
        specialtyAddButtonState = INACTIVE;
        specialtyAddButton->setStyleSheet("");
    }
}

void TableEditorView::openFacultyList()
{
    setSmallTablesVisible();
    getIDTable->setModel(facultyModelReference);
    resizeTable(getIDTable);
    connect(agree,SIGNAL(clicked(bool)),this,SLOT(getFacultyID()));

}

void TableEditorView::openChairList()
{
    setSmallTablesVisible();
    getIDTable->setModel(chairModelReference);
    resizeTable(getIDTable);
    connect(agree,SIGNAL(clicked(bool)),this,SLOT(getChairID()));
}

void TableEditorView::openSpecialtyList()
{
    setSmallTablesVisible();
    getIDTable->setModel(specialtyModelReference);
    resizeTable(getIDTable);
    connect(agree,SIGNAL(clicked(bool)),this,SLOT(getSpecialtyID()));
}

void TableEditorView::getFacultyID()
{
    QModelIndex idIndex = getIDTable->model()->index(getIDTable->currentIndex().row(),0,QModelIndex());
    QString idData = getIDTable->model()->data(idIndex).toString();
    if (tableTab->currentIndex()==1) chairChooseFaculty->setText(idData);
    else if (tableTab->currentIndex()==4) specialtyChooseFaculty->setText(idData);
    setSmallTablesInvisible();
    disconnect(agree,SIGNAL(clicked(bool)),this,SLOT(getFacultyID()));
}


void TableEditorView::getChairID()
{
    QModelIndex idIndex = getIDTable->model()->index(getIDTable->currentIndex().row(),0,QModelIndex());
    QString idData = getIDTable->model()->data(idIndex).toString();
    teacherChooseChair->setText(idData);
    setSmallTablesInvisible();
    disconnect(agree,SIGNAL(clicked(bool)),this,SLOT(getChairID()));
}

void TableEditorView::getSpecialtyID()
{
    QModelIndex idIndex = getIDTable->model()->index(getIDTable->currentIndex().row(),0,QModelIndex());
    QString idData = getIDTable->model()->data(idIndex).toString();
    streamChooseSpecialty->setText(idData);
    setSmallTablesInvisible();
    disconnect(agree,SIGNAL(clicked(bool)),this,SLOT(getSpecialtyID()));
}




void TableEditorView::getFacultyModel()
{
    connect(facultyRunner,SIGNAL(returnTableModel(QSqlTableModel*)),this,SLOT(setFacultyModel(QSqlTableModel*)));
    facultyRunner->tryTableModel("fakultet");
}


void TableEditorView::setFacultyModel(QSqlTableModel *model)
{
    facultyModelReference = model;
    //    facultyModelReference->setFilter(QString("id_universiteta = '%1'").arg(receivedUID));
    facultyModelReference->setHeaderData(0,Qt::Horizontal,tr("ID факультета"));
    facultyModelReference->setHeaderData(1,Qt::Horizontal,tr("Название"));
    facultyModelReference->setHeaderData(2,Qt::Horizontal,tr("Аббревиатура"));
    facultyModelReference->setHeaderData(3,Qt::Horizontal,tr("ID университета"));
    facultyTable->setModel(model);
    connect(facultyTable->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(changedFromData(QItemSelection)));
    connect(facultyTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(changedToData(QModelIndex)));
}

void TableEditorView::getChairModel()
{
    connect(chairRunner,SIGNAL(returnTableModel(QSqlTableModel*)),this,SLOT(setChairModel(QSqlTableModel*)));
    chairRunner->tryTableModel("kafedra");
}


void TableEditorView::setChairModel(QSqlTableModel *model)
{
    chairModelReference = model;
    chairModelReference->setHeaderData(0,Qt::Horizontal,tr("ID кафедры"));
    chairModelReference->setHeaderData(1,Qt::Horizontal,tr("Название"));
    chairModelReference->setHeaderData(2,Qt::Horizontal,tr("Аббревиатура"));
    chairModelReference->setHeaderData(3,Qt::Horizontal,tr("ID факультета"));
    chairTable->setModel(model);
    connect(chairTable->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(changedFromData(QItemSelection)));
    connect(chairTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(changedToData(QModelIndex)));
}

void TableEditorView::getTeacherModel()
{
    connect(teacherRunner,SIGNAL(returnTableModel(QSqlTableModel*)),this,SLOT(setTeacherModel(QSqlTableModel*)));
    teacherRunner->tryTableModel("prepodavatel");
}


void TableEditorView::setTeacherModel(QSqlTableModel *model)
{
    teacherModelReference = model;
    teacherModelReference->setHeaderData(0,Qt::Horizontal,tr("ID преподавателя"));
    teacherModelReference->setHeaderData(1,Qt::Horizontal,tr("Фамилия"));
    teacherModelReference->setHeaderData(2,Qt::Horizontal,tr("Имя"));
    teacherModelReference->setHeaderData(3,Qt::Horizontal,tr("Отчество"));
    teacherModelReference->setHeaderData(4,Qt::Horizontal,tr("ID кафедры"));
    teacherModelReference->setHeaderData(5,Qt::Horizontal,tr("Должность"));
    teacherModelReference->setHeaderData(6,Qt::Horizontal,tr("Ученая степень"));
    teacherModelReference->setHeaderData(7,Qt::Horizontal,tr("Ученое звание"));
    teacherTable->setModel(model);
    connect(teacherTable->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(changedFromData(QItemSelection)));
    connect(teacherTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(changedToData(QModelIndex)));
}


void TableEditorView::getStreamModel()
{
    connect(streamRunner,SIGNAL(returnTableModel(QSqlTableModel*)),this,SLOT(setStreamModel(QSqlTableModel*)));
    streamRunner->tryTableModel("potok");
}


void TableEditorView::setStreamModel(QSqlTableModel *model)
{
    streamModelReference = model;
    streamModelReference->setHeaderData(0,Qt::Horizontal,tr("ID потока"));
    streamModelReference->setHeaderData(1,Qt::Horizontal,tr("Название потока"));
    streamModelReference->setHeaderData(2,Qt::Horizontal,tr("Аббревиатура"));
    streamModelReference->setHeaderData(3,Qt::Horizontal,tr("Курс"));
    streamModelReference->setHeaderData(4,Qt::Horizontal,tr("ID специальности"));
    streamModelReference->setHeaderData(5,Qt::Horizontal,tr("Кол-во студентов"));
    streamModelReference->setHeaderData(6,Qt::Horizontal,tr("Кол-во групп"));
    streamModelReference->setHeaderData(7,Qt::Horizontal,tr("Кол-во подгрупп"));
    streamTable->setModel(model);
    connect(streamTable->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(changedFromData(QItemSelection)));
    connect(streamTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(changedToData(QModelIndex)));
}



void TableEditorView::getDisciplineModel()
{
    connect(disciplineRunner,SIGNAL(returnTableModel(QSqlTableModel*)),this,SLOT(setDisciplineModel(QSqlTableModel*)));
    disciplineRunner->tryTableModel("disciplina");
}

void TableEditorView::setDisciplineModel(QSqlTableModel *model)
{
    disciplineModelReference = model;
    disciplineModelReference->setHeaderData(0,Qt::Horizontal,tr("ID дисциплины"));
    disciplineModelReference->setHeaderData(1,Qt::Horizontal,tr("Название"));
    disciplineTable->setModel(model);
    connect(disciplineTable->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(changedFromData(QItemSelection)));
    connect(disciplineTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(changedToData(QModelIndex)));
}

void TableEditorView::getSpecialtyModel()
{
    connect(specialtyRunner,SIGNAL(returnTableModel(QSqlTableModel*)),this,SLOT(setSpecialtyModel(QSqlTableModel*)));
    specialtyRunner->tryTableModel("specialnost");
}

void TableEditorView::setSpecialtyModel(QSqlTableModel *model)
{
    specialtyModelReference = model;
    specialtyModelReference->setHeaderData(0,Qt::Horizontal,tr("ID специальности"));
    specialtyModelReference->setHeaderData(1,Qt::Horizontal,tr("Название"));
    specialtyModelReference->setHeaderData(2,Qt::Horizontal,tr("ID факультета"));
    specialtyTable->setModel(model);
    connect(specialtyTable->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(changedFromData(QItemSelection)));
    connect(specialtyTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(changedToData(QModelIndex)));
}

void TableEditorView::changedFromData(const QItemSelection &selected)
{
    if (tableTab->currentIndex() == 0)
    {
        facultyBeforeEditing = facultyTable->model()->data(selected.indexes()[0]).toString();
        facultyAfterEditing = facultyTable->model()->data(selected.indexes()[0]).toString();
    }
    else if (tableTab->currentIndex() == 1)
    {
        chairBeforeEditing = chairTable->model()->data(selected.indexes()[0]).toString();
        chairAfterEditing = chairTable->model()->data(selected.indexes()[0]).toString();
    }
    else if (tableTab->currentIndex() == 2)
    {
        teacherBeforeEditing = teacherTable->model()->data(selected.indexes()[0]).toString();
        teacherAfterEditing = teacherTable->model()->data(selected.indexes()[0]).toString();
    }
    else if (tableTab->currentIndex() == 3)
    {
        disciplineBeforeEditing = disciplineTable->model()->data(selected.indexes()[0]).toString();
        disciplineAfterEditing = disciplineTable->model()->data(selected.indexes()[0]).toString();
    }
    else if (tableTab->currentIndex() == 4)
    {
        specialtyBeforeEditing = specialtyTable->model()->data(selected.indexes()[0]).toString();
        specialtyAfterEditing = specialtyTable->model()->data(selected.indexes()[0]).toString();
    }
    else if (tableTab->currentIndex() == 5)
    {
        streamBeforeEditing = streamTable->model()->data(selected.indexes()[0]).toString();
        streamAfterEditing = streamTable->model()->data(selected.indexes()[0]).toString();
    }

}

void TableEditorView::changedToData(const QModelIndex &bIndex)
{
    if (tableTab->currentIndex() == 0)
    {
        facultyAfterEditing = facultyTable->model()->data(bIndex).toString();
    }
    else if (tableTab->currentIndex() == 1)
    {
        chairAfterEditing = chairTable->model()->data(bIndex).toString();
    }
    else if (tableTab->currentIndex() == 2)
    {
        teacherAfterEditing = teacherTable->model()->data(bIndex).toString();
    }
    else if (tableTab->currentIndex() == 3)
    {
        disciplineAfterEditing = disciplineTable->model()->data(bIndex).toString();
    }
    else if (tableTab->currentIndex() == 4)
    {
        specialtyAfterEditing = specialtyTable->model()->data(bIndex).toString();
    }
    else if (tableTab->currentIndex() == 5)
    {
        streamAfterEditing = streamTable->model()->data(bIndex).toString();
    }

}

void TableEditorView::disableFacultyWidgets()
{
    tableTab->setTabEnabled(1,0);
    tableTab->setTabEnabled(2,0);
    tableTab->setTabEnabled(3,0);
    tableTab->setTabEnabled(4,0);
    tableTab->setTabEnabled(5,0);
    facultyAddButton->setVisible(0);
    facultyDeleteButton->setVisible(0);
    facultyName->setVisible(0);
    facultyShortname->setVisible(0);
    facultyConfirmAddition->setVisible(0);

}

void TableEditorView::disableChairWidgets()
{
    tableTab->setTabEnabled(0,0);
    tableTab->setTabEnabled(2,0);
    tableTab->setTabEnabled(3,0);
    tableTab->setTabEnabled(4,0);
    tableTab->setTabEnabled(5,0);
    chairAddButton->setVisible(0);
    chairDeleteButton->setVisible(0);
    chairName->setVisible(0);
    chairShortname->setVisible(0);
    chairConfirmAddition->setVisible(0);
    chairChooseFaculty->setVisible(0);
}

void TableEditorView::disableTeacherWidgets()
{
    teacherAddButton->setVisible(0);
    teacherDeleteButton->setVisible(0);
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
    tableTab->setTabEnabled(0,0);
    tableTab->setTabEnabled(1,0);
    tableTab->setTabEnabled(3,0);
    tableTab->setTabEnabled(4,0);
    tableTab->setTabEnabled(5,0);
}


void TableEditorView::disableDisciplineWidgets()
{

    disciplineAddButton->setVisible(0);
    disciplineDeleteButton->setVisible(0);
    disciplineName->setVisible(0);
    disciplineConfirmAddition->setVisible(0);
    tableTab->setTabEnabled(0,0);
    tableTab->setTabEnabled(1,0);
    tableTab->setTabEnabled(2,0);
    tableTab->setTabEnabled(4,0);
    tableTab->setTabEnabled(5,0);
}

void TableEditorView::disableSpecialtyWidgets()
{
    tableTab->setTabEnabled(0,0);
    tableTab->setTabEnabled(1,0);
    tableTab->setTabEnabled(2,0);
    tableTab->setTabEnabled(3,0);
    tableTab->setTabEnabled(5,0);
    specialtyAddButton->setVisible(0);
    specialtyDeleteButton->setVisible(0);
    specialtyName->setVisible(0);
    specialtyConfirmAddition->setVisible(0);
    specialtyChooseFaculty->setVisible(0);
}

void TableEditorView::disableStreamWidgets()
{
    streamAddButton->setVisible(0);
    streamDeleteButton->setVisible(0);
    streamConfirmAddition->setVisible(0);
    streamName->setVisible(0);
    streamShortName->setVisible(0);
    streamStudents->setVisible(0);
    streamGroups->setVisible(0);
    streamSubgroups->setVisible(0);
    streamChooseSpecialty->setVisible(0);
    streamYear->setVisible(0);
    tableTab->setTabEnabled(0,0);
    tableTab->setTabEnabled(1,0);
    tableTab->setTabEnabled(2,0);
    tableTab->setTabEnabled(3,0);
    tableTab->setTabEnabled(4,0);
}

void TableEditorView::clearAll()
{
    chairName->clear();
    chairShortname->clear();
    chairChooseFaculty->clear();

    facultyName->clear();
    facultyShortname->clear();

    teacherFirstName->clear();
    teacherMiddleName->clear();
    teacherSurname->clear();
    teacherTitle->setCurrentIndex(0);
    teacherDegree->setCurrentIndex(0);
    teacherPost->clear();
    teacherChooseChair->clear();

    streamGroups->clear();
    streamSubgroups->clear();
    streamStudents->clear();
    streamYear->clear();
    streamName->clear();
    streamShortName->clear();
    streamChooseSpecialty->clear();

    specialtyName->clear();
    specialtyChooseFaculty->clear();

    disciplineName->clear();
}

void TableEditorView::enableWidgets()
{
    facultyAddButton->setVisible(1);
    facultyDeleteButton->setVisible(1);
    changeFacultyAddButtonStyle();
    changeFacultyAddButtonStyle();

    chairAddButton->setVisible(1);
    chairDeleteButton->setVisible(1);
    changeChairAddButtonStyle();
    changeChairAddButtonStyle();

    teacherAddButton->setVisible(1);
    teacherDeleteButton->setVisible(1);
    changeTeacherAddButtonStyle();
    changeTeacherAddButtonStyle();

    disciplineAddButton->setVisible(1);
    disciplineDeleteButton->setVisible(1);
    changeDisciplineAddButtonStyle();
    changeDisciplineAddButtonStyle();

    specialtyAddButton->setVisible(1);
    specialtyDeleteButton->setVisible(1);
    changeSpecialtyAddButtonStyle();
    changeSpecialtyAddButtonStyle();

    streamAddButton->setVisible(1);
    streamDeleteButton->setVisible(1);
    changeStreamAddButtonStyle();
    changeStreamAddButtonStyle();

    tableTab->setTabEnabled(0,1);
    tableTab->setTabEnabled(1,1);
    tableTab->setTabEnabled(2,1);
    tableTab->setTabEnabled(3,1);
    tableTab->setTabEnabled(4,1);
    tableTab->setTabEnabled(5,1);
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
    teModel->updateFacultyModel(facultyModelReference,TableEditorModel::fINSERT,facultyTable->currentIndex().row(),facultyName->text(),facultyShortname->text(),receivedUID);
    facultyName->clear();
    facultyShortname->clear();
    facultyDeleteButton->setEnabled(0);

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
        teModel->updateFacultyModel(facultyModelReference,TableEditorModel::fDELETE,facultyTable->currentIndex().row());
        facultyDeleteButton->setEnabled(0);
    }

}

void TableEditorView::facultyEditRecord()
{
    if (facultyBeforeEditing == facultyAfterEditing)
    {
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Изменить: %1 на %2 ?").arg(facultyBeforeEditing).arg(facultyAfterEditing)), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        facultyModelReference->revertAll();
        facultyAfterEditing = facultyBeforeEditing;
        return;
    }
    else
    {
        int check = facultyModelReference->submitAll();
        if (!check)
        {
            emit updateError(facultyModelReference->lastError().text());
            facultyModelReference->revertAll();
            facultyAfterEditing = facultyBeforeEditing;
            return;
        }
        resizeTable(facultyTable);
        facultyDeleteButton->setEnabled(0);
        enableWidgets();
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

    if (chairName->text() == "" || chairShortname->text() == "")
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
    teModel->updateFacultyModel(chairModelReference,TableEditorModel::fINSERT,chairTable->currentIndex().row(),chairName->text(),chairShortname->text(),chairChooseFaculty->text());
    chairName->clear();
    chairShortname->clear();
    chairChooseFaculty->clear();
    chairDeleteButton->setEnabled(0);
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
        teModel->updateFacultyModel(chairModelReference,TableEditorModel::fDELETE,chairTable->currentIndex().row());
        chairDeleteButton->setEnabled(0);
    }

}

void TableEditorView::chairEditRecord()
{
    if (chairBeforeEditing == chairAfterEditing)
    {
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Изменить: %1 на %2 ?").arg(chairBeforeEditing).arg(chairAfterEditing)), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        chairModelReference->revertAll();
        chairAfterEditing = chairBeforeEditing;
        return;
    }
    else
    {
        int check = chairModelReference->submitAll();
        if (!check)
        {
            emit updateError(chairModelReference->lastError().text());
            chairModelReference->revertAll();
            chairAfterEditing = chairBeforeEditing;
            return;
        }
        resizeTable(chairTable);
        chairDeleteButton->setEnabled(0);
        enableWidgets();
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
    teModel->updateTeacherModel(teacherModelReference,TableEditorModel::fINSERT,teacherTable->currentIndex().row(),teacherSurname->text(),teacherFirstName->text(),teacherMiddleName->text(),teacherChooseChair->text(),teacherPost->text(),teacherDegree->currentText(),teacherTitle->currentText());
    teacherFirstName->clear();
    teacherMiddleName->clear();
    teacherSurname->clear();
    teacherTitle->setCurrentIndex(0);
    teacherDegree->setCurrentIndex(0);
    teacherPost->clear();
    teacherChooseChair->clear();
    teacherDeleteButton->setEnabled(0);
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
        teModel->updateTeacherModel(teacherModelReference,TableEditorModel::fDELETE,teacherTable->currentIndex().row());
        teacherDeleteButton->setEnabled(0);
    }

}

void TableEditorView::teacherEditRecord()
{
    if (teacherBeforeEditing == teacherAfterEditing)
    {
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Изменить: %1 на %2 ?").arg(teacherBeforeEditing).arg(teacherAfterEditing)), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        teacherModelReference->revertAll();
        teacherAfterEditing = teacherBeforeEditing;
        return;
    }
    else
    {
        int check = teacherModelReference->submitAll();
        if (!check)
        {
            emit updateError(teacherModelReference->lastError().text());
            teacherModelReference->revertAll();
            teacherAfterEditing = teacherBeforeEditing;
            return;
        }
        resizeTable(teacherTable);
        teacherDeleteButton->setEnabled(0);
        enableWidgets();
    }
}

void TableEditorView::disciplineAddRecord()
{
    if (disciplineName->text() == "")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Не все поля заполнены. Все равно добавить запись?")), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    if (disciplineModelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    teModel->updateDisciplineModel(disciplineModelReference,TableEditorModel::fINSERT,disciplineTable->currentIndex().row(),disciplineName->text());
    disciplineName->clear();
    disciplineDeleteButton->setEnabled(0);
}

void TableEditorView::disciplineDeleteRecord()
{
    if (disciplineModelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    QModelIndex idIndex = disciplineTable->model()->index(disciplineTable->currentIndex().row(),0,QModelIndex());
    QModelIndex nameIndex = disciplineTable->model()->index(disciplineTable->currentIndex().row(),1,QModelIndex());

    QString idData = disciplineTable->model()->data(idIndex).toString();
    QString nameData = disciplineTable->model()->data(nameIndex).toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Удаление"), QString(tr("Удалить запись %1 | %2?")).arg(idData).arg(nameData), QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        teModel->updateDisciplineModel(disciplineModelReference,TableEditorModel::fDELETE,disciplineTable->currentIndex().row());
        disciplineDeleteButton->setEnabled(0);
    }
}

void TableEditorView::disciplineEditRecord()
{
    if (disciplineBeforeEditing == disciplineAfterEditing)
    {
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Изменить: %1 на %2 ?").arg(disciplineBeforeEditing).arg(disciplineAfterEditing)), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        disciplineModelReference->revertAll();
        disciplineAfterEditing = disciplineBeforeEditing;
        return;
    }
    else
    {
        int check = disciplineModelReference->submitAll();
        if (!check)
        {
            emit updateError(disciplineModelReference->lastError().text());
            disciplineModelReference->revertAll();
            disciplineAfterEditing = disciplineBeforeEditing;
            return;
        }
        resizeTable(disciplineTable);
        disciplineDeleteButton->setEnabled(0);
        enableWidgets();
    }
}

void TableEditorView::specialtyAddRecord()
{
    if (specialtyChooseFaculty->text() == "")
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Поле ID факультета обязательно для заполнения"),QMessageBox::Ok);
        return;
    }

    if (specialtyName->text() == "")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Не все поля заполнены. Все равно добавить запись?")), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    if (specialtyModelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    teModel->updateSpecialtyModel(specialtyModelReference,TableEditorModel::fINSERT,specialtyTable->currentIndex().row(),specialtyName->text(),specialtyChooseFaculty->text());
    specialtyName->clear();
    specialtyChooseFaculty->clear();
    specialtyDeleteButton->setEnabled(0);
}

void TableEditorView::specialtyDeleteRecord()
{
    if (specialtyModelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    QModelIndex idIndex = specialtyTable->model()->index(specialtyTable->currentIndex().row(),0,QModelIndex());
    QModelIndex nameIndex = specialtyTable->model()->index(specialtyTable->currentIndex().row(),1,QModelIndex());
    QModelIndex facultyIDIndex = specialtyTable->model()->index(specialtyTable->currentIndex().row(),2,QModelIndex());

    QString idData = specialtyTable->model()->data(idIndex).toString();
    QString nameData = specialtyTable->model()->data(nameIndex).toString();
    QString facultyIDData = specialtyTable->model()->data(facultyIDIndex).toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Удаление"), QString(tr("Удалить запись %1 | %2 | %3?")).arg(idData).arg(nameData).arg(facultyIDData), QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        teModel->updateSpecialtyModel(specialtyModelReference,TableEditorModel::fDELETE,specialtyTable->currentIndex().row());
        specialtyDeleteButton->setEnabled(0);
    }
}

void TableEditorView::specialtyEditRecord()
{
    if (specialtyBeforeEditing == specialtyAfterEditing)
    {
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Изменить: %1 на %2 ?").arg(specialtyBeforeEditing).arg(specialtyAfterEditing)), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        specialtyModelReference->revertAll();
        specialtyAfterEditing = specialtyBeforeEditing;
        return;
    }
    else
    {
        int check = specialtyModelReference->submitAll();
        if (!check)
        {
            emit updateError(specialtyModelReference->lastError().text());
            specialtyModelReference->revertAll();
            specialtyAfterEditing = specialtyBeforeEditing;
            return;
        }
        resizeTable(specialtyTable);
        specialtyDeleteButton->setEnabled(0);
        enableWidgets();
    }
}


void TableEditorView::streamAddRecord()
{
    if (streamChooseSpecialty->text() == "")
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Поле ID специальности обязательно для заполнения"),QMessageBox::Ok);
        return;
    }

    if (streamYear->text() == "")
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Поле Курс обязательно для заполнения"),QMessageBox::Ok);
        return;
    }

    if (streamGroups->text() == "" || streamSubgroups->text() == "" || streamStudents->text() == "" || streamName->text() == "" || streamShortName->text() == "")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Не все поля заполнены. Все равно добавить запись?")), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    if (streamModelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    teModel->updateTeacherModel(streamModelReference,TableEditorModel::fINSERT,streamTable->currentIndex().row(),streamName->text(),streamShortName->text(),streamYear->text(),streamChooseSpecialty->text(),streamStudents->text(),streamGroups->text(),streamSubgroups->text());
    streamGroups->clear();
    streamSubgroups->clear();
    streamStudents->clear();
    streamYear->clear();
    streamName->clear();
    streamShortName->clear();
    streamChooseSpecialty->clear();
    streamDeleteButton->setEnabled(0);
}

void TableEditorView::streamDeleteRecord()
{
    if (streamModelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }
    QModelIndex idIndex = streamTable->model()->index(streamTable->currentIndex().row(),0,QModelIndex());
    QModelIndex nameIndex = streamTable->model()->index(streamTable->currentIndex().row(),2,QModelIndex());
    QModelIndex shortNameIndex = streamTable->model()->index(streamTable->currentIndex().row(),1,QModelIndex());
    QModelIndex yearIndex = streamTable->model()->index(streamTable->currentIndex().row(),3,QModelIndex());
    QModelIndex specialtyIDIndex = streamTable->model()->index(streamTable->currentIndex().row(),6,QModelIndex());
    QModelIndex studentsIndex = streamTable->model()->index(streamTable->currentIndex().row(),4,QModelIndex());
    QModelIndex groupsIndex = streamTable->model()->index(streamTable->currentIndex().row(),5,QModelIndex());
    QModelIndex subgroupsIndex = streamTable->model()->index(streamTable->currentIndex().row(),7,QModelIndex());

    QString idData = streamTable->model()->data(idIndex).toString();
    QString nameData = streamTable->model()->data(nameIndex).toString();
    QString shortNameData = streamTable->model()->data(shortNameIndex).toString();
    QString yearData = streamTable->model()->data(yearIndex).toString();
    QString specialtyIDData = streamTable->model()->data(specialtyIDIndex).toString();
    QString studentsData = streamTable->model()->data(studentsIndex).toString();
    QString groupsData = streamTable->model()->data(groupsIndex).toString();
    QString subgroupsData = streamTable->model()->data(subgroupsIndex).toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Удаление"), QString(tr("Удалить запись %1 | %2 | %3 | %4 | %5 | %6 | %7 | %8?")).arg(idData).arg(nameData).arg(shortNameData).arg(yearData).arg(specialtyIDData).arg(studentsData).arg(groupsData).arg(subgroupsData), QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        teModel->updateTeacherModel(streamModelReference,TableEditorModel::fDELETE,streamTable->currentIndex().row());
        streamDeleteButton->setEnabled(0);
    }

}

void TableEditorView::streamEditRecord()
{
    if (streamBeforeEditing == streamAfterEditing)
    {
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Изменить: %1 на %2 ?").arg(streamBeforeEditing).arg(streamAfterEditing)), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        streamModelReference->revertAll();
        streamAfterEditing = streamBeforeEditing;
        return;
    }
    else
    {
        int check = streamModelReference->submitAll();
        if (!check)
        {
            emit updateError(streamModelReference->lastError().text());
            streamModelReference->revertAll();
            streamAfterEditing = streamBeforeEditing;
            return;
        }
        resizeTable(streamTable);
        streamDeleteButton->setEnabled(0);
        enableWidgets();
    }
}


