#include "universityview.h"

UniversityView::UniversityView(QWidget *parent) : QWidget(parent)
{
    createUI();
    getModel();
    resizeTable();
    connect(exitButton, &QAbstractButton::pressed, this, &QWidget::close);
    connect(confirmButton, &QAbstractButton::pressed, this, &UniversityView::choiceConfirmed);
    connect(universityTableView,&QTableView::clicked,this,&UniversityView::enableButtons);
    connect(deleteButton,&QPushButton::clicked,this,&UniversityView::deleteRecord);
    connect(addButton,&QPushButton::clicked,this,&UniversityView::changeAddButtonStyle);
    connect(confirmAddition,&QPushButton::clicked,this,&UniversityView::addRecord);
}

void UniversityView::createUI()
{
    runner = new QueryRunner;
    uModel = new UniversityModel;
    workField = new QGroupBox;
    nameField = new QLineEdit;
    nameField->setPlaceholderText(tr("Название"));
    nameField->setToolTip(tr("Название университета"));
    shortnameField = new QLineEdit;
    shortnameField->setPlaceholderText(tr("Аббревиатура"));
    shortnameField->setToolTip(tr("Аббревиатура университета"));
    confirmAddition = new QPushButton(tr("OK"));
    universityTableView = new QTableView;
    universityTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    universityTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    universityTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    universityTableView->verticalHeader()->setVisible(0);
    addButton = new QPushButton(tr("Добавить"));;
    deleteButton = new QPushButton(tr("Удалить"));;
    deleteButton->setEnabled(0);
    confirmButton = new QPushButton(tr("Выбрать университет"));;
    confirmButton->setEnabled(0);
    exitButton = new QPushButton(tr("Выход"));;
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(exitButton, QDialogButtonBox::RejectRole);
    buttonBox->addButton(confirmButton, QDialogButtonBox::AcceptRole);
    internalVLayout = new QVBoxLayout;
    externalVLayout = new QVBoxLayout;
    topHLayout = new QHBoxLayout;
    topHLayout->addWidget(nameField);
    topHLayout->addWidget(shortnameField);
    topHLayout->addWidget(confirmAddition);

    nameField->setVisible(0);
    shortnameField->setVisible(0);
    confirmAddition->setVisible(0);

    botHLayout = new QHBoxLayout;
    botHLayout->addWidget(addButton);
    botHLayout->addWidget(deleteButton);
    internalVLayout->addWidget(universityTableView);
    internalVLayout->addLayout(topHLayout);
    internalVLayout->addLayout(botHLayout);
    workField->setLayout(internalVLayout);
    externalVLayout->addWidget(workField);
    externalVLayout->addWidget(buttonBox);
    this->setLayout(externalVLayout);
    this->setMinimumSize(300,300);
    this->setWindowTitle(tr("Выбор университета"));
}

void UniversityView::resizeTable()
{
    hHeader = universityTableView->horizontalHeader();

    int neededWidth = 0;

    universityTableView->resizeColumnsToContents();
    universityTableView->resizeRowsToContents();

    for (int c = 0; c < universityTableView->horizontalHeader()->count(); c++)
    {
        neededWidth += hHeader->sectionSize(c) + 20;
    }

    if (neededWidth > 300)
        setMinimumSize(neededWidth,300);

    universityTableView->horizontalHeader()->setStretchLastSection(true);

}
void UniversityView::enableButtons()
{
    deleteButton->setEnabled(1);
    confirmButton->setEnabled(1);
}

void UniversityView::getModel()
{
    connect(runner,&QueryRunner::returnTableModel,this,&UniversityView::setModel);
    runner->tryTableModel("universitet");
}


void UniversityView::setModel(QSqlTableModel *model)
{
    modelReference = model;
    universityTableView->setModel(model);
}

void UniversityView::deleteRecord()
{
    QModelIndex idIndex = universityTableView->model()->index(universityTableView->currentIndex().row(),0,QModelIndex());
    QModelIndex nameIndex = universityTableView->model()->index(universityTableView->currentIndex().row(),1,QModelIndex());
    QModelIndex shortnameIndex = universityTableView->model()->index(universityTableView->currentIndex().row(),2,QModelIndex());

    QString idData = universityTableView->model()->data(idIndex).toString();
    QString nameData = universityTableView->model()->data(nameIndex).toString();
    QString shortnameData = universityTableView->model()->data(shortnameIndex).toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Удаление"), QString(tr("Удалить запись %1 | %2 | %3?")).arg(idData).arg(nameData).arg(shortnameData), QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        connect(uModel,&UniversityModel::updateError,this,&UniversityView::getError);
        uModel->updateModel(modelReference,UniversityModel::operationType::uDELETE,universityTableView->currentIndex().row());
    }

}

void UniversityView::addRecord()
{
    if (nameField->text() == "" || shortnameField->text() == "")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Не все поля заполнены. Все равно добавить запись?")), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    connect(uModel,&UniversityModel::updateError,this,&UniversityView::getError);
    uModel->updateModel(modelReference,UniversityModel::operationType::uINSERT,universityTableView->currentIndex().row(),nameField->text(),shortnameField->text());
    nameField->clear();
    shortnameField->clear();
}

void UniversityView::changeAddButtonStyle()
{
    nameField->clear();
    shortnameField->clear();
    if (addButtonState == buttonState::INACTIVE)
    {
        nameField->setVisible(1);
        shortnameField->setVisible(1);
        confirmAddition->setVisible(1);
        addButtonState = buttonState::ACTIVE;
        addButton->setStyleSheet("background-color:gray; border-color:black; color:white");
    }
    else
    {
        nameField->setVisible(0);
        shortnameField->setVisible(0);
        confirmAddition->setVisible(0);
        addButtonState = buttonState::INACTIVE;
        addButton->setStyleSheet("");
    }
}

void UniversityView::getError(QSqlError error)
{
    QMessageBox::StandardButton errorMsg;
    errorMsg = QMessageBox::information(this,tr("Ошибка"),error.text(),QMessageBox::Ok);
}



void UniversityView::choiceConfirmed()
{
    QModelIndex idIndex = universityTableView->model()->index(universityTableView->currentIndex().row(),0,QModelIndex());
    QModelIndex nameIndex = universityTableView->model()->index(universityTableView->currentIndex().row(),1,QModelIndex());
    QModelIndex shortnameIndex = universityTableView->model()->index(universityTableView->currentIndex().row(),2,QModelIndex());

    QString idData = universityTableView->model()->data(idIndex).toString();
    QString nameData = universityTableView->model()->data(nameIndex).toString();
    QString shortnameData = universityTableView->model()->data(shortnameIndex).toString();

    MainWindow *frm = new MainWindow(idData, nameData, shortnameData,this);
    frm->show();
    this->hide();
}
