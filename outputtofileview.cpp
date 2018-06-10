#include "outputtofileview.h"

OutputToFileView::OutputToFileView(QString uID, QWidget *parent) : QWidget(parent)
{
    receivedID = uID;
    teachersModel = new QSqlQueryModel;
    runner = new QueryRunner;
    receivedHtmlData = QString();

    connect(runner,SIGNAL(querySuccessReturnModel(QSqlQueryModel*)),this,SLOT(setTeachersModel(QSqlQueryModel*)));
    connect(runner,SIGNAL(returnHtml(QString)),this,SLOT(getHtmlData(QString)));

    teachersList = new QTableView;

    teachersList->verticalHeader()->setVisible(0);

    teachersList->setDragDropMode(QAbstractItemView::DragDrop);

    teachersList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    teachersList->setSelectionBehavior(QAbstractItemView::SelectRows);
    teachersList->setSelectionMode(QAbstractItemView::SingleSelection);

    teachersList->setDragEnabled(true);

    connect(teachersList,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setChosenTeacher(QModelIndex)));
    connect(teachersList,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(process()));

    runner->tryQuery(QString("Select id_prep as 'ID',concat(familiya, ' ' ,imya,' ' , otchestvo) as 'Имя' from prepodavatel LEFT JOIN kafedra on prepodavatel.id_kafedry=kafedra.id_kafedry "
                             "LEFT JOIN fakultet on kafedra.id_fakulteta=fakultet.id_fakulteta LEFT JOIN universitet on fakultet.id_universiteta=universitet.id_universiteta"
                             " WHERE universitet.id_universiteta = %1").arg(receivedID),1);


#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    myWebView = new QWebView;
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    myWebView = new QWebEngineView;
#endif

    myWebView->setHtml("Выберите преподавателя, чтобы просмотреть отчет");
    chosenTeacherLabel = new QLabel(tr("Выбранный преподаватель"));
    fileSavePathLabel = new QLabel(tr("Выбранный путь"));
    teachersLabel = new QLabel(tr("Список преподавателей"));
    outputFormatLabel = new QLabel(tr("Формат вывода данных"));
    previewLabel = new QLabel(tr("Предпросмотр отчета"));

    chosenTeacherLabel->setAlignment(Qt::AlignCenter);
    fileSavePathLabel->setAlignment(Qt::AlignCenter);
    teachersLabel->setAlignment(Qt::AlignCenter);
    outputFormatLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setAlignment(Qt::AlignCenter);

    teachersList->setColumnHidden(0,true);
    //    teachersList->setColumnHidden(1,true);
    //    teachersList->setColumnHidden(5,true);
    //    teachersList->setColumnHidden(6,true);
    //    teachersList->setColumnHidden(7,true);

    teachersList->resizeColumnsToContents();
    teachersList->resizeRowsToContents();
    teachersList->horizontalHeader()->setStretchLastSection(1);

    chosenTeacher = new QLineEdit;
    chosenTeacher->setReadOnly(1);

    confirmOutput = new QPushButton("Продолжить");
    confirmOutput->setEnabled(0);
    connect(confirmOutput,SIGNAL(clicked(bool)),this,SLOT(saveFile()));

    outputFormat = new QComboBox;
    outputFormat->addItem(tr("Вывод в PDF файл"));
    outputFormat->addItem(tr("Вывод в HTML файл"));
    outputFormat->addItem(tr("Печать"));


    t1VBoxLayout = new QVBoxLayout;
    t2VBoxLayout = new QVBoxLayout;

    chosenYear = new QDateEdit;
    chosenYear->setDisplayFormat("yyyy");
    chosenYear->setDate(QDate(2018,1,1));
    chosenYear->setMaximumDate(QDate(2030,1,1));
    chosenYear->setMinimumDate(QDate(2010,1,1));
    connect(chosenYear,SIGNAL(dateChanged(QDate)),this,SLOT(process()));

    chosenYearLabel = new QLabel(tr("Выбранный год"));

    t1VBoxLayout->addWidget(chosenTeacherLabel);
    t2VBoxLayout->addWidget(chosenTeacher);

    t1VBoxLayout->addWidget(chosenYearLabel);
    t2VBoxLayout->addWidget(chosenYear);

    t1VBoxLayout->addWidget(outputFormatLabel);
    t2VBoxLayout->addWidget(outputFormat);

    internalTopHBoxLayout = new QHBoxLayout;

    internalTopHBoxLayout->addLayout(t1VBoxLayout);
    internalTopHBoxLayout->addLayout(t2VBoxLayout);

    internalRightVBoxLayout = new QVBoxLayout;
    internalRightVBoxLayout->addLayout(internalTopHBoxLayout);
    internalRightVBoxLayout->addWidget(previewLabel);
    internalRightVBoxLayout->addWidget(myWebView);
    internalRightVBoxLayout->addWidget(confirmOutput);

    internalLeftVBoxLayout = new QVBoxLayout;
    internalLeftVBoxLayout->addWidget(teachersLabel);
    internalLeftVBoxLayout->addWidget(teachersList);


    externalHBoxLayout = new QHBoxLayout(this);
    externalHBoxLayout->addLayout(internalLeftVBoxLayout);
    externalHBoxLayout->addLayout(internalRightVBoxLayout);




}

void OutputToFileView::setTeachersModel(QSqlQueryModel *model)
{
    teachersModel = model;
    teachersList->setModel(teachersModel);
}

void OutputToFileView::setChosenTeacher(QModelIndex index)
{
    confirmOutput->setEnabled(0);
    receivedHtmlData = "";
    savedIndex = index;
    chosenTeacher->setText(teachersList->model()->data(index).toString());
}

void OutputToFileView::process()
{

    runner->outputToFile(teachersList->model()->data(teachersList->model()->index(savedIndex.row(),0)).toInt(),receivedID.toInt(),chosenTeacher->text(),chosenYear->text().toInt());

}

int OutputToFileView::saveFile()
{
    if (outputFormat->currentIndex()==0)
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить"),
                                                        "",
                                                        ("PDF (*.pdf)"));
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFileName(filename);
        QTextDocument textdocument;
        textdocument.setHtml(receivedHtmlData);
        textdocument.print(&printer);
    }

    else if (outputFormat->currentIndex()==1)
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить"),
                                                        "",
                                                        ("HTML File (*.html,*.htm)"));
        QFile outputFile(filename);
        outputFile.open(QIODevice::WriteOnly);

        if(!outputFile.isOpen())
        {
            return 1;
        }

        QTextStream outStream(&outputFile);
        outStream << receivedHtmlData;
        outputFile.close();
    }

    else if (outputFormat->currentIndex()==2)
    {
        QPrinter printer;
        QPrintDialog dialog(&printer,this);
        if (dialog.exec())
        {
            QTextDocument textdocument;
            textdocument.setHtml(receivedHtmlData);
            textdocument.print(&printer);
        }

    }

    return 0;

}

void OutputToFileView::getHtmlData(QString htmlData)
{
    receivedHtmlData = htmlData;
    myWebView->setHtml(htmlData);
    confirmOutput->setEnabled(1);
}
