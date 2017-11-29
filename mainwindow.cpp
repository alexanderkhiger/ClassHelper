#include "mainwindow.h"

MainWindow::MainWindow(QString uID, QString uName, QString uShortname, QWidget *ref, QWidget *parent) : QMainWindow(parent)
{
    parentReference = ref;
    receivedID = uID;
    receivedName = uName;
    receivedShortname = uShortname;

    createUI();
    connect(dropArea, &DropArea::changed, this, &MainWindow::updateParameters);
    connect(dropArea, &DropArea::clearParameters, this, &MainWindow::clearParameters);
    connect(clearButton, &QAbstractButton::pressed, dropArea, &DropArea::clear);
    connect(backButton,&QAbstractButton::pressed,this,&MainWindow::back);
    connect(classesList, &QListWidget::itemDoubleClicked, this, &MainWindow::doubleClickClassUpdate);
    connect(teachersList, &QListWidget::itemDoubleClicked, this, &MainWindow::doubleClickTeacherUpdate);
    connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);

    TableEditorView *frm = new TableEditorView(receivedID,receivedName,receivedShortname);
    frm->show();
}

void MainWindow::createUI()
{
    setMinimumHeight(500);
    dropArea = new DropArea;
    dropArea->setFixedHeight(75);

    clearButton = new QPushButton(tr("Очистить"));
    backButton = new QPushButton(tr("Назад"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(clearButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(backButton, QDialogButtonBox::RejectRole);

    teachersList = new QListWidget;
    classesList = new QListWidget;

    teachersList->setDragEnabled(true);
    classesList->setDragEnabled(true);

    teachersList->setFixedWidth(150);
    classesList->setFixedWidth(150);

    teachersList->setObjectName("teachersList");
    classesList->setObjectName("classesList");

    classesList->addItem("Физика");
    classesList->addItem("Математика");
    classesList->addItem("Химия");
    classesList->addItem("Русский язык");
    classesList->addItem("Английский язык");

    teachersList->addItem("Бубнов А.П.");
    teachersList->addItem("Соколова Е.С.");
    teachersList->addItem("Кротов И.Д.");
    teachersList->addItem("Русаков С.С.");
    teachersList->addItem("Коломойский П.И.");

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

    internalHLayout->addWidget(chosenClass);
    internalHLayout->addWidget(chosenTeacher);

    internalMiddleVLayout->addWidget(dropArea);
    internalMiddleVLayout->addLayout(internalHLayout);
    internalMiddleVLayout->addWidget(workField);

    internalLeftVLayout->addWidget(classesHeader);
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

void MainWindow::back()
{
    parentReference->show();
    QWidget::close();
}

void MainWindow::updateParameters(const QObject *myObject, const QMimeData *mimeData)
{

    QByteArray encoded = mimeData->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;
        listViewData = roleDataMap[0].toString();
    }


    if (myObject->objectName()=="classesList")
    {
        chosenClass->setText(listViewData);
    }
    else if (myObject->objectName()=="teachersList")
    {
        chosenTeacher->setText(listViewData);
    }
    else
        return;

    MainWindow::checkFields();

}

void MainWindow::checkFields()
{
    if ((chosenClass->text() != "") && (chosenTeacher->text() != ""))
    {
        this->setStyleSheet("QGroupBox#workField {background-color: white; border-radius: 3px; border: 2px solid gray}");
        workField->setEnabled(true);
    }
}

void MainWindow::doubleClickClassUpdate(const QListWidgetItem *myItem)
{
    chosenClass->setText(myItem->text());
    MainWindow::checkFields();
}

void MainWindow::doubleClickTeacherUpdate(const QListWidgetItem *myItem)
{
    chosenTeacher->setText(myItem->text());
    MainWindow::checkFields();
}

void MainWindow::clearParameters()
{
    chosenClass->clear();
    chosenTeacher->clear();
    this->setStyleSheet("QGroupBox#workField {}");
    workField->setEnabled(false);
}

void MainWindow::newFile()
{
    LoadNewFileView *frm = new LoadNewFileView;
    frm->show();
}
