#include <QtWidgets>
#include "universityview.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
UniversityView::UniversityView(QWidget *parent) : QWidget(parent)
{
    createUI();
    connect(exitButton, &QAbstractButton::pressed, this, &QWidget::close);
    connect(confirmButton, &QAbstractButton::pressed, this, &UniversityView::choiceConfirmed);
    connect(universityTableView,&QTableView::clicked,this,&UniversityView::enableButtons);
    getModel();
    universityTableView->setVisible(false);
    universityTableView->resizeColumnsToContents();
    universityTableView->resizeRowsToContents();
    universityTableView->setVisible(true);
    resize(universityTableView->width(),universityTableView->height());

}

void UniversityView::createUI()
{
    runner = new QueryRunner;
    workField = new QGroupBox;
    universityTableView = new QTableView;
    universityTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    universityTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    universityTableView->resize(100,100);
    qDebug() << universityTableView->size();с
    addButton = new QPushButton(tr("Добавить"));;
    deleteButton = new QPushButton(tr("Удалить"));;
    deleteButton->setEnabled(0);
    confirmButton = new QPushButton(tr("ОК"));;
    confirmButton->setEnabled(0);
    exitButton = new QPushButton(tr("Выход"));;
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(exitButton, QDialogButtonBox::RejectRole);
    buttonBox->addButton(confirmButton, QDialogButtonBox::AcceptRole);
    internalVLayout = new QVBoxLayout;
    externalVLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout;
    hLayout->addWidget(addButton);
    hLayout->addWidget(deleteButton);
    internalVLayout->addWidget(universityTableView);
    internalVLayout->addLayout(hLayout);
    workField->setLayout(internalVLayout);
    externalVLayout->addWidget(workField);
    externalVLayout->addWidget(buttonBox);
    this->setLayout(externalVLayout);
    this->setMinimumSize(300,300);
    this->setWindowTitle("Выбор университета");
}

void UniversityView::enableButtons()
{
    deleteButton->setEnabled(1);
    confirmButton->setEnabled(1);
}

void UniversityView::getModel()
{
    connect(runner,&QueryRunner::querySuccessReturnModel,this,&UniversityView::setModel);
    runner->tryQuery("SELECT * FROM universitet;",1);
}

void UniversityView::setModel(QSqlQueryModel *model)
{
    universityTableView->setModel(model);
}

void UniversityView::deleteRecord()
{

}

void UniversityView::addRecord()
{

}

void UniversityView::choiceConfirmed()
{
//    universityTableView->selectionModel()->currentIndex().row();
}
