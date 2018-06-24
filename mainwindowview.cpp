#include "mainwindowview.h"

MainWindowView::MainWindowView(QString uID, QString uName, QString uShortname, QWidget *ref, QWidget *parent) : QMainWindow(parent)
{
    parentReference = ref;
    receivedID = uID;
    receivedName = uName;
    receivedShortname = uShortname;
    createUI();
}

void MainWindowView::createUI()
{
    leftBar = new QToolBar;
    leftBar->setMovable(false);
    leftBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QToolButton *distributionTool = new QToolButton;
    QToolButton *tableEditorTool = new QToolButton;
    QToolButton *outputTool = new QToolButton;
    QToolButton *newFileTool = new QToolButton;

    newFileTool->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    newFileTool->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    newFileTool->setText(tr("Обработка"));
    newFileTool->setFixedSize(70,60);
    newFileTool->setToolTip(tr("Обработка исходного файла"));
    connect(newFileTool,SIGNAL(clicked(bool)),this,SLOT(setLoadNewFileAsCentral()));

    distributionTool->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    distributionTool->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    distributionTool->setText(tr("Нагрузка"));
    distributionTool->setFixedSize(70,60);
    distributionTool->setToolTip(tr("Распределение нагрузки по преподавателям"));
    connect(distributionTool,SIGNAL(clicked(bool)),this,SLOT(setWorkFieldAsCentral()));

    tableEditorTool->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tableEditorTool->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    tableEditorTool->setText(tr("Редактор"));
    tableEditorTool->setFixedSize(70,60);
    tableEditorTool->setToolTip(tr("Редактор таблиц (словарей)"));
    connect(tableEditorTool,SIGNAL(clicked(bool)),this,SLOT(setTableEditorAsCentral()));

    outputTool->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    outputTool->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    outputTool->setText(tr("Отчеты"));
    outputTool->setFixedSize(70,60);
    outputTool->setToolTip(tr("Получение отчета"));
    connect(outputTool,SIGNAL(clicked(bool)),this,SLOT(setOutputAsCentral()));

    leftBar->addWidget(distributionTool);
    leftBar->addWidget(tableEditorTool);
    leftBar->addWidget(outputTool);
    leftBar->addWidget(newFileTool);

    this->addToolBar(Qt::LeftToolBarArea,leftBar);

    topBar = new QToolBar;
    topBar->setMovable(false);

    QLabel *spacing = new QLabel;
    spacing->setText(QDate::currentDate().toString("dd.MM.yyyy"));
    spacing->setAlignment(Qt::AlignCenter);
    spacing->setFixedSize(69,32);
    topBar->addWidget(spacing);
    topBar->addSeparator();

    clearTool = new QToolButton;
    clearTool->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
    clearTool->setToolTip(tr("Очистить поля"));

    changeSemesterTool = new QToolButton;
    changeSemesterTool->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    changeSemesterTool->setToolTip(tr("Переключить семестр"));

    topBar->addWidget(clearTool);
    topBar->addWidget(changeSemesterTool);

    this->addToolBar(Qt::TopToolBarArea,topBar);

    setMinimumHeight(500);

    fileMenu = new QMenu(tr("Меню"));

    this->menuBar()->addMenu(fileMenu);

//    newFileAction = new QAction(tr("Загрузить файл данных"), this);
//    newFileAction->setShortcuts(QKeySequence::New);
//    newFileAction->setStatusTip(tr("Загрузить новый файл данных"));
//    fileMenu->addAction(newFileAction);
//    fileMenu->addSeparator();
//    connect(newFileAction, SIGNAL(triggered(bool)), this, SLOT(setLoadNewFileAsCentral()));

    backToUniversityList = new QAction(tr("Выбор университета"), this);
    backToUniversityList->setStatusTip(tr("Назад к выбору университета"));
    fileMenu->addAction(backToUniversityList);
    fileMenu->addSeparator();

    closeAction = new QAction(tr("Выход"), this);
    fileMenu->addAction(closeAction);

    connect(backToUniversityList,SIGNAL(triggered(bool)),this,SLOT(back()));
    connect(closeAction,SIGNAL(triggered(bool)),this,SLOT(close()));

    setWorkFieldAsCentral();

    setWindowTitle(QString(tr("Time Tracker | Название университета: %1 | Аббревиатура: %2 | Выбранный семестр: %3 ")).arg(receivedName).arg(receivedShortname).arg(1));
}

void MainWindowView::setTableEditorAsCentral()
{
    if (centralWidget()->objectName()!="tableEditor")
    {
        tableEditor = new TableEditorView(receivedID,receivedName,receivedShortname);
        tableEditor->setObjectName("tableEditor");
        this->setCentralWidget(tableEditor);
        changeSemesterTool->setEnabled(false);
        clearTool->setEnabled(true);
        connect(clearTool,SIGNAL(clicked(bool)),tableEditor,SLOT(clearAll()));
    }
}

void MainWindowView::setWorkFieldAsCentral()
{
    if (centralWidget() == 0 || centralWidget()->objectName()!="workfield")
    {
        distributionWidget = new DistributionView(receivedID,receivedName,receivedShortname);
        distributionWidget->setObjectName("workfield");
        this->setCentralWidget(distributionWidget);
        changeSemesterTool->setEnabled(true);
        connect(clearTool,SIGNAL(clicked(bool)),distributionWidget,SLOT(clearParameters()));
        connect(changeSemesterTool,SIGNAL(clicked(bool)),distributionWidget,SLOT(changeCurrentSemester()));
        connect(distributionWidget,SIGNAL(semesterChanged(int)),this,SLOT(changeWindowTitle(int)));
        clearTool->setEnabled(true);
    }
}

void MainWindowView::setOutputAsCentral()
{
    if (centralWidget()->objectName()!="outputToFile")
    {
        outFile = new OutputToFileView(receivedID);
        outFile->setObjectName("outputToFile");
        this->setCentralWidget(outFile);
        changeSemesterTool->setEnabled(false);
        clearTool->setEnabled(false);
    }
}

void MainWindowView::setLoadNewFileAsCentral()
{
    if (centralWidget()->objectName()!="loadNewFile")
    {
        loadNewFile = new LoadNewFileView(receivedID);
        loadNewFile->setObjectName("loadNewFile");
        this->setCentralWidget(loadNewFile);
        changeSemesterTool->setEnabled(false);
        connect(clearTool,SIGNAL(clicked(bool)),loadNewFile,SLOT(clear()));
        connect(loadNewFile,SIGNAL(startedProcessing()),this,SLOT(hideToolbars()));
        connect(loadNewFile,SIGNAL(finishedProcessing()),this,SLOT(showToolbars()));
        connect(loadNewFile,SIGNAL(destroyed(QObject*)),this,SLOT(showToolbars()));
        clearTool->setEnabled(true);
    }
}

void MainWindowView::hideToolbars() {
    leftBar->setVisible(0);
    topBar->setVisible(0);
    menuBar()->setVisible(0);
}

void MainWindowView::showToolbars() {
    leftBar->setVisible(1);
    topBar->setVisible(1);
    menuBar()->setVisible(1);
}

void MainWindowView::back()
{
    parentReference->show();
    QWidget::close();
}

void MainWindowView::changeWindowTitle(int semester)
{
    setWindowTitle(QString(tr("Time Tracker | Название университета: %1 | Аббревиатура: %2 | Выбранный семестр: %3 ")).arg(receivedName).arg(receivedShortname).arg(semester));
}
