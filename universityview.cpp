#include "universityview.h"

UniversityView::UniversityView(QWidget *parent) : QWidget(parent)
{
    createUI();
    getModel();
    resizeTable();
    universityTableView->viewport()->installEventFilter(this);

    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(confirmButton, SIGNAL(clicked(bool)), this, SLOT(choiceConfirmed()));
    connect(universityTableView,SIGNAL(clicked(QModelIndex)),this,SLOT(enableButtons()));
    connect(deleteButton,SIGNAL(clicked(bool)),this,SLOT(deleteRecord()));
    connect(addButton,SIGNAL(clicked(bool)),this,SLOT(changeAddButtonStyle()));
    connect(confirmAddition,SIGNAL(clicked(bool)),this,SLOT(addRecord()));
    connect(this,SIGNAL(updateError(QSqlError)),this,SLOT(getError(QSqlError)));
    connect(universityTableView->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(editRecord()));
    connect(universityTableView->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this,SLOT(turnOnButtons()));
}

bool UniversityView::eventFilter(QObject *obj, QEvent *event)
{

    if (event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent * mouseEvent = static_cast <QMouseEvent *> (event);

        if (mouseEvent -> button() == Qt::LeftButton) {
            turnOffButtons();
        }
    }
    return QWidget::eventFilter(obj, event);
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
    universityTableView = new CustomTableView(this);
    universityTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    //    universityTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    universityTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    universityTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    universityTableView->verticalHeader()->setVisible(0);
    connect(uModel,SIGNAL(updateError(QSqlError)),this,SLOT(getError(QSqlError)));
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

void UniversityView::turnOffButtons()
{
    deleteButton->setVisible(0);
    confirmButton->setVisible(0);
    addButton->setVisible(0);
    nameField->setVisible(0);
    shortnameField->setVisible(0);
    confirmAddition->setVisible(0);
}

void UniversityView::turnOnButtons()
{
    deleteButton->setVisible(1);
    confirmButton->setVisible(1);
    addButton->setVisible(1);
    nameField->setVisible(1);
    shortnameField->setVisible(1);
    confirmAddition->setVisible(1);
    changeAddButtonStyle();
    changeAddButtonStyle();
}
void UniversityView::resizeTable()
{
    hHeader = universityTableView->horizontalHeader();
    setMinimumSize(120,300);
    resize(120,300);
    int neededWidth = 0;

    universityTableView->resizeColumnsToContents();
    universityTableView->resizeRowsToContents();

    for (int c = 0; c < universityTableView->horizontalHeader()->count(); c++)
    {
        neededWidth += hHeader->sectionSize(c) + 20;
    }

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
    connect(runner,SIGNAL(returnTableModel(QSqlTableModel*)),this,SLOT(setModel(QSqlTableModel*)));
    runner->tryTableModel("universitet");
}


void UniversityView::setModel(QSqlTableModel *model)
{
    modelReference = model;
    modelReference->setHeaderData(0,Qt::Horizontal,tr("ID университета"));
    modelReference->setHeaderData(1,Qt::Horizontal,tr("Название"));
    modelReference->setHeaderData(2,Qt::Horizontal,tr("Аббревиатура"));
    universityTableView->setModel(model);
    connect(universityTableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(changedFrom(QItemSelection)));
    connect(universityTableView->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(changedTo(QModelIndex)));
}

void UniversityView::deleteRecord()
{
    if (modelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }

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
        uModel->updateModel(modelReference,UniversityModel::operationType::uDELETE,universityTableView->currentIndex().row());
        deleteButton->setEnabled(0);
        confirmButton->setEnabled(0);
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

    if (modelReference == 0)
    {
        QMessageBox::StandardButton errorMsg;
        errorMsg = QMessageBox::information(this,tr("Ошибка"),tr("Модель не загружена. Критическая ошибка"),QMessageBox::Ok);
        return;
    }

    uModel->updateModel(modelReference,UniversityModel::operationType::uINSERT,universityTableView->currentIndex().row(),nameField->text(),shortnameField->text());
    nameField->clear();
    shortnameField->clear();
    deleteButton->setEnabled(0);
    confirmButton->setEnabled(0);
}

void UniversityView::editRecord()
{
    if (universityBeforeUpdate == universityAfterUpdate)
    {
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Подтверджение"), QString(tr("Изменить: %1 на %2 ?").arg(universityBeforeUpdate).arg(universityAfterUpdate)), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        modelReference->revertAll();
        universityAfterUpdate = universityBeforeUpdate;
        return;
    }
    else
    {
        int check = modelReference->submitAll();
        if (!check)
        {
            emit updateError(modelReference->lastError().text());
            modelReference->revertAll();
            universityAfterUpdate = universityBeforeUpdate;
            return;
        }
        resizeTable();
        deleteButton->setEnabled(0);
        confirmButton->setEnabled(0);
        //        QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows;
        //        QModelIndex index = universityTableView->model()->index(0, 0);
        //        universityTableView->selectionModel()->select(index,flags);
    }
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
    errorMsg = QMessageBox::critical(this,tr("Ошибка"),error.text(),QMessageBox::Ok);
}

void UniversityView::changedFrom(const QItemSelection &selected)
{
    universityBeforeUpdate = universityTableView->model()->data(selected.indexes()[0]).toString();
    universityAfterUpdate = universityTableView->model()->data(selected.indexes()[0]).toString();
}

void UniversityView::changedTo(const QModelIndex &bIndex)
{
    universityAfterUpdate = universityTableView->model()->data(bIndex).toString();
}


void UniversityView::choiceConfirmed()
{
    QModelIndex idIndex = universityTableView->model()->index(universityTableView->currentIndex().row(),0,QModelIndex());
    QModelIndex nameIndex = universityTableView->model()->index(universityTableView->currentIndex().row(),1,QModelIndex());
    QModelIndex shortnameIndex = universityTableView->model()->index(universityTableView->currentIndex().row(),2,QModelIndex());

    QString idData = universityTableView->model()->data(idIndex).toString();
    QString nameData = universityTableView->model()->data(nameIndex).toString();
    QString shortnameData = universityTableView->model()->data(shortnameIndex).toString();

    MainWindowView *frm = new MainWindowView(idData, nameData, shortnameData,this);
    frm->show();
    this->hide();
}
