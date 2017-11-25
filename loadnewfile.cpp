#include <QtWidgets>
#include "loadnewfile.h"
#include <QSqlDatabase>
#include <QDebug>

LoadNewFile::LoadNewFile(QWidget *parent) : QWidget(parent)
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

    connect(chooseFileButton, &QAbstractButton::pressed, this, &LoadNewFile::chooseFile);
    connect(exitButton, &QAbstractButton::pressed, this, &QWidget::close);
    connect(clearButton, &QAbstractButton::pressed, this, &LoadNewFile::clear);

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

void LoadNewFile::chooseFile()
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

void LoadNewFile::clear()
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
