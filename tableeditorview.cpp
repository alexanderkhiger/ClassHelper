#include "tableeditorview.h"

TableEditorView::TableEditorView(QString uID, QString uName, QString uShortname, QWidget *parent) : QWidget(parent)
{

    receivedUID = uID;
    receivedName = uName;
    receivedShortname = uShortname;
    teModel = new TableEditorModel;
    connect(teModel,SIGNAL(updateError(QSqlError)),this,SLOT(getError(QSqlError)));
    connect(this,SIGNAL(updateError(QSqlError)),this,SLOT(getError(QSqlError)));
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

    facultyTable->viewport()->installEventFilter(this);
    chairTable->viewport()->installEventFilter(this);
    teacherTable->viewport()->installEventFilter(this);

    resizeTable(facultyTable);
}

bool TableEditorView::eventFilter(QObject *obj, QEvent *event) {

    if (event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent * mouseEvent = static_cast <QMouseEvent *> (event);
        if (mouseEvent -> button() == Qt::LeftButton) {
            if (obj == facultyTable->viewport())
            {
                disableFacultyWidgets();
            }
            else if (obj == chairTable->viewport())
            {
                disableChairWidgets();
            }
            else if (obj == teacherTable->viewport())
            {
                disableTeacherWidgets();
            }
        }
    }
    return QWidget::eventFilter(obj, event);
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
    connect(agree,SIGNAL(clicked(bool)),this,SLOT(getFacultyID()));

}

void TableEditorView::openChairList()
{
    setSmallTablesVisible();
    getIDTable->setModel(chairModelReference);
    resizeTable(getIDTable);
    connect(agree,SIGNAL(clicked(bool)),this,SLOT(getChairID()));
}

void TableEditorView::getFacultyID()
{
    QModelIndex idIndex = getIDTable->model()->index(getIDTable->currentIndex().row(),0,QModelIndex());
    QString idData = getIDTable->model()->data(idIndex).toString();
    chairChooseFaculty->setText(idData);
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
    connect(facultyTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(changedToData(QModelIndex)));
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
    connect(chairTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(changedToData(QModelIndex)));
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
    connect(teacherTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(changedToData(QModelIndex)));
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
}

void TableEditorView::disableFacultyWidgets()
{
    tableTab->setTabEnabled(1,0);
    tableTab->setTabEnabled(2,0);
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

    tableTab->setTabEnabled(0,1);
    tableTab->setTabEnabled(1,1);
    tableTab->setTabEnabled(2,1);
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
        teModel->updateFacultyModel(facultyModelReference,TableEditorModel::operationType::fDELETE,facultyTable->currentIndex().row());
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
        teModel->updateFacultyModel(chairModelReference,TableEditorModel::operationType::fDELETE,chairTable->currentIndex().row());
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
    teModel->updateTeacherModel(teacherModelReference,TableEditorModel::operationType::fINSERT,teacherTable->currentIndex().row(),teacherSurname->text(),teacherFirstName->text(),teacherMiddleName->text(),teacherChooseChair->text(),teacherPost->text(),teacherDegree->currentText(),teacherTitle->currentText());
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
        teModel->updateTeacherModel(teacherModelReference,TableEditorModel::operationType::fDELETE,teacherTable->currentIndex().row());
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

