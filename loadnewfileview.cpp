#include "loadnewfileview.h"

LoadNewFileView::LoadNewFileView(QString uID, QWidget *parent) : QWidget(parent)
{
    directory = "";
    buffer = "";
    errorCount = 0;
    receivedID = uID;
    createUI();
    connect(chooseFileButton, SIGNAL(pressed()), this, SLOT(chooseFile()));
    connect(exitButton, SIGNAL(pressed()), this, SLOT(close()));
    connect(clearButton, SIGNAL(pressed()), this, SLOT(clear()));
    connect(startButton, SIGNAL(pressed()), this, SLOT(startProcessing()));
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
    QMessageBox::StandardButton infoMsg;
    infoMsg = QMessageBox::information(this,tr("Оповещение"),tr("Обработка завершена!"),QMessageBox::Ok);
}
void LoadNewFileView::startProcessing()
{
    processor = new LoadNewFileModel(receivedID,this);

    disconnect(processor,SIGNAL(processingFinished()),this,SLOT(finishProcessing()));
    disconnect(processor,SIGNAL(sendInformation(QString)),this,SLOT(getInformation(QString)));
    disconnect(processor,SIGNAL(sendProgress(int)),this,SLOT(getProgress(int)));
    connect(processor,SIGNAL(processingFinished()),this,SLOT(finishProcessing()));
    connect(processor,SIGNAL(sendInformation(QString)),this,SLOT(getInformation(QString)));
    connect(processor,SIGNAL(sendProgress(int)),this,SLOT(getProgress(int)));
    processor->processData(directory);
//    processor->convertRtf(directory);
}

void LoadNewFileView::getInformation(const QString info)
{
    actionsLog->append(info);
}

void LoadNewFileView::getProgress(const int percentage)
{
    processingProgress->setValue(percentage);
}
