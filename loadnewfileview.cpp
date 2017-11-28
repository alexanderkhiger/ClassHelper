#include <QtWidgets>
#include "loadnewfileview.h"
#include <QSqlDatabase>
#include <QDebug>

LoadNewFileView::LoadNewFileView(QWidget *parent) : QWidget(parent)
{
    createUI();
    connect(chooseFileButton, &QAbstractButton::pressed, this, &LoadNewFileView::chooseFile);
    connect(exitButton, &QAbstractButton::pressed, this, &QWidget::close);
    connect(clearButton, &QAbstractButton::pressed, this, &LoadNewFileView::clear);
    connect(startButton, &QAbstractButton::pressed, this, &LoadNewFileView::startProcessing);
}

void LoadNewFileView::createUI()
{
    this->setMinimumHeight(250);
    this->resize(300,250);

    chooseFileButton = new QPushButton(tr("Выбрать файл"));
    startButton = new QPushButton(tr("Начать обработку"));
    exitButton = new QPushButton(tr("Назад"));
    clearButton = new QPushButton(tr("Очистить"));
    buttonBox = new QDialogButtonBox;

    buttonBox->addButton(clearButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(exitButton, QDialogButtonBox::RejectRole);

    startButton->setEnabled(false);

    chooseFileButton->setMinimumWidth(150);
    startButton->setMinimumWidth(150);

    chosenFile = new QLineEdit;
    chosenFile->setReadOnly(true);
    processingProgress = new QProgressBar;

    chosenFile->setMinimumWidth(150);
    processingProgress->setMinimumWidth(150);

    errorCounterLE = new QLineEdit;
    errorCounterLE->setReadOnly(true);

    errorCounterLabel = new QLabel(tr("Количество ошибок"));

    errorCounterLabel->setMinimumWidth(150);
    errorCounterLabel->setAlignment(Qt::AlignCenter);

    actionsLog = new QTextEdit;
    actionsLog->setReadOnly(true);

    topHLayout = new QHBoxLayout;
    middleHLayout = new QHBoxLayout;
    bottomHLayout = new QHBoxLayout;
    vLayout = new QVBoxLayout;

    topHLayout->addWidget(chooseFileButton);
    topHLayout->addWidget(chosenFile);
    middleHLayout->addWidget(startButton);
    middleHLayout->addWidget(processingProgress);
    bottomHLayout->addWidget(errorCounterLabel);
    bottomHLayout->addWidget(errorCounterLE);
    vLayout->addLayout(topHLayout);
    vLayout->addLayout(middleHLayout);
    vLayout->addWidget(actionsLog);
    vLayout->addLayout(bottomHLayout);
    vLayout->addWidget(buttonBox);

    this->setLayout(vLayout);

    this->setWindowTitle(tr("Добавление файла"));
}

void LoadNewFileView::chooseFile()
{
    buffer = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "C:/", tr("Text files (*.txt *.rtf)"));
    if (buffer != "")
        directory = buffer;
    chosenFile->setText(directory);
    if (chosenFile->text()!="")
        startButton->setEnabled(true);
    else
        startButton->setEnabled(false);
}

void LoadNewFileView::clear()
{
    chosenFile->clear();
    directory = "";
    startButton->setEnabled(false);
    actionsLog->clear();
    errorCounterLE->clear();
    //    qDebug() << QSqlDatabase::database("MyConnection").isOpen();
    //    QSqlDatabase::database("MyConnection",false).close();
    //    QSqlDatabase::removeDatabase("MyConnection");
    //    qDebug() << QSqlDatabase::database("MyConnection").isOpen();
}

void LoadNewFileView::finishProcessing()
{
    delete processor;
    msg = new QMessageBox;
    msg->setText("Processing finished!");
    msg->setStandardButtons(QMessageBox::Ok);
    msg->exec();
}
void LoadNewFileView::startProcessing()
{
    processor = new LoadNewFileLogic(this);
    connect(processor,&LoadNewFileLogic::processingFinished,this,&LoadNewFileView::finishProcessing);
    connect(processor,&LoadNewFileLogic::sendInformation,this,&LoadNewFileView::getInformation);
    connect(processor,&LoadNewFileLogic::sendProgress,this,&LoadNewFileView::getProgress);
    processor->processData(directory);
}

void LoadNewFileView::getInformation(const QString info)
{
    actionsLog->append(info);
}

void LoadNewFileView::getProgress(const int percentage)
{
    processingProgress->setValue(percentage);
}
