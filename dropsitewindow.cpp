#include <QtWidgets>
#include "droparea.h"
#include "dropsitewindow.h"

DropSiteWindow::DropSiteWindow()
{
    dropArea = new DropArea;

    dropArea->setFixedHeight(75);

    connect(dropArea, &DropArea::changed,
            this, &DropSiteWindow::updateParameters);

    connect(dropArea, &DropArea::clearParameters,
            this, &DropSiteWindow::clearParameters);

    clearButton = new QPushButton(tr("Очистить"));
    quitButton = new QPushButton(tr("Выход"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(clearButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(quitButton, &QAbstractButton::pressed, this, &QWidget::close);
    connect(clearButton, &QAbstractButton::pressed, dropArea, &DropArea::clear);

    teachersList = new QListWidget;
    classesList = new QListWidget;

    connect(classesList,&QListWidget::itemDoubleClicked,this,&DropSiteWindow::doubleClickClassUpdate);
    connect(teachersList,&QListWidget::itemDoubleClicked,this,&DropSiteWindow::doubleClickTeacherUpdate);

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


    QVBoxLayout *externalVLayout = new QVBoxLayout(this);
    QVBoxLayout *internalMiddleVLayout = new QVBoxLayout;
    QVBoxLayout *internalLeftVLayout = new QVBoxLayout;
    QVBoxLayout *internalRightVLayout = new QVBoxLayout;
    QHBoxLayout *externalHLayout = new QHBoxLayout;
    QHBoxLayout *internalHLayout = new QHBoxLayout;


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

    setWindowTitle(tr("Time Tracker"));
    setMinimumHeight(500);
}


void DropSiteWindow::updateParameters(const QObject *myObject, const QMimeData *mimeData)
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
    else return;

    DropSiteWindow::checkFields();

}

void DropSiteWindow::checkFields()
{
    if ((chosenClass->text()!="")&&(chosenTeacher->text()!=""))
    {
        this->setStyleSheet("QGroupBox#workField {background-color: white; border-radius: 3px; border: 2px solid gray}");
        workField->setEnabled(true);
    }
}

void DropSiteWindow::doubleClickClassUpdate(const QListWidgetItem *myItem)
{
    chosenClass->setText(myItem->text());
    DropSiteWindow::checkFields();
}

void DropSiteWindow::doubleClickTeacherUpdate(const QListWidgetItem *myItem)
{
    chosenTeacher->setText(myItem->text());
    DropSiteWindow::checkFields();
}

void DropSiteWindow::clearParameters()
{
    chosenClass->clear();
    chosenTeacher->clear();
    this->setStyleSheet("QGroupBox#workField {}");
    workField->setEnabled(false);
}
