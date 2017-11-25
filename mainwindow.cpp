#include <QtWidgets>
#include "mainwindow.h"
#include "droparea.h"
#include "loadnewfile.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumHeight(500);
    dropArea = new DropArea;

    dropArea->setFixedHeight(75);

    connect(dropArea, &DropArea::changed, this, &MainWindow::updateParameters);

    connect(dropArea, &DropArea::clearParameters, this, &MainWindow::clearParameters);

    clearButton = new QPushButton(tr("Очистить"));
    quitButton = new QPushButton(tr("Выход"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(clearButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(quitButton, &QAbstractButton::pressed, this, &QWidget::close);
    connect(clearButton, &QAbstractButton::pressed, dropArea, &DropArea::clear);

    teachersList = new QListWidget;
    classesList = new QListWidget;

    connect(classesList, &QListWidget::itemDoubleClicked, this, &MainWindow::doubleClickClassUpdate);
    connect(teachersList, &QListWidget::itemDoubleClicked, this, &MainWindow::doubleClickTeacherUpdate);

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

    QMenu *fileMenu = new QMenu(tr("Файл"));
    QMenu *editMenu = new QMenu(tr("Правка"));
    QMenu *toolsMenu = new QMenu(tr("Инструменты"));
    QMenu *helpMenu = new QMenu(tr("Справка"));

    this->menuBar()->addMenu(fileMenu);
    this->menuBar()->addMenu(editMenu);
    this->menuBar()->addMenu(toolsMenu);
    this->menuBar()->addMenu(helpMenu);

    QAction *newAct = new QAction(tr("Загрузить файл данных"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Загрузить новый файл данных"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);

    setWindowTitle(tr("Time Tracker"));
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
    LoadNewFile *frm = new LoadNewFile;
    frm->show();
}
