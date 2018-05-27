#include "loadnewfileview.h"

LoadNewFileView::LoadNewFileView(QString uID, QWidget *parent) : QWidget(parent)
{
    directory = "";
    buffer = "";
    errorCount = 0;
    receivedID = uID;
    createUI();
    connect(chooseFileButton, SIGNAL(pressed()), this, SLOT(chooseFile()));
    connect(startButton, SIGNAL(pressed()), this, SLOT(startProcessing()));
}

void LoadNewFileView::createUI()
{
    this->setMinimumHeight(250);
    this->resize(300,250);

    skipAllCheck = new QCheckBox;
    skipAllCheck->setText(tr("Добавить все, как есть"));

    chooseFileButton = new QPushButton(tr("Выбрать файл"));
    startButton = new QPushButton(tr("Начать обработку"));

    startButton->setEnabled(false);

    chooseFileButton->setMinimumWidth(150);
    startButton->setMinimumWidth(150);

    chooseFileButton->setMaximumWidth(150);
    startButton->setMaximumWidth(150);

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

    middleHLayout->addWidget(skipAllCheck);

    middleHLayout->addWidget(processingProgress);
    bottomHLayout->addWidget(errorCounterLabel);
    bottomHLayout->addWidget(errorCounterLE);
    vLayout->addLayout(topHLayout);
    vLayout->addLayout(middleHLayout);
    vLayout->addWidget(actionsLog);
    vLayout->addLayout(bottomHLayout);

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

void LoadNewFileView::finishProcessing(double expectedTotal, double countedTotal)
{
    actionsLog->append(tr("Ожидаемое количество часов: %1").arg(expectedTotal));
    actionsLog->append(tr("Обработанное количество часов: %1").arg(countedTotal));
    delete processor;
    QMessageBox::StandardButton infoMsg;
    infoMsg = QMessageBox::information(this,tr("Оповещение"),tr("Обработка завершена!"),QMessageBox::Ok);
    chooseFileButton->setEnabled(1);
    startButton->setEnabled(1);
    skipAllCheck->setEnabled(1);
}
void LoadNewFileView::startProcessing()
{
    processor = new LoadNewFileModel(receivedID,skipAllCheck->isChecked(),this);

    disconnect(processor,SIGNAL(processingFinished(double,double)),this,SLOT(finishProcessing(double,double)));
    disconnect(processor,SIGNAL(sendInformation(QString)),this,SLOT(getInformation(QString)));
    disconnect(processor,SIGNAL(sendProgress(int)),this,SLOT(getProgress(int)));
    connect(processor,SIGNAL(processingFinished(double,double)),this,SLOT(finishProcessing(double,double)));
    connect(processor,SIGNAL(sendInformation(QString)),this,SLOT(getInformation(QString)));
    connect(processor,SIGNAL(sendProgress(int)),this,SLOT(getProgress(int)));
    connect(processor->runner,SIGNAL(queryError(QSqlError)),this,SLOT(getError(QSqlError)));

    errorCount=0;
    actionsLog->clear();
    chooseFileButton->setEnabled(0);
    startButton->setEnabled(0);
    skipAllCheck->setEnabled(0);
    errorCounterLE->setText(QString::number(errorCount));

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

void LoadNewFileView::getError(QSqlError error)
{
    actionsLog->append(error.text());
    errorCount++;
    errorCounterLE->setText(QString::number(errorCount));

//    QMessageBox::StandardButton errorMsg;
//    errorMsg = QMessageBox::critical(this,tr("Ошибка"),error.text(),QMessageBox::Ok);
}
