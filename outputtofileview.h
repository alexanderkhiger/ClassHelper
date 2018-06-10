#ifndef OUTPUTTOFILEVIEW_H
#define OUTPUTTOFILEVIEW_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QHeaderView>
#include <QSqlQueryModel>
#include "queryrunner.h"
#include <QFile>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QLabel>
#include <QtGlobal>
#include <QTextStream>
#include <QDateEdit>


#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    #include <QWebEngineView>
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    #include <QWebView>
#endif



class OutputToFileView : public QWidget
{
    Q_OBJECT
public:
    explicit OutputToFileView(QString uID, QWidget *parent = 0);

public slots:

    void setTeachersModel(QSqlQueryModel *model);
    void setChosenTeacher(QModelIndex index);
    void process();
    int saveFile();
    void getHtmlData(QString htmlData);

private:

    QModelIndex savedIndex;
    QTableView *teachersList;
    QLineEdit *chosenTeacher;
    QDateEdit *chosenYear;
    QPushButton *confirmOutput;
    QComboBox *outputFormat;
    QString receivedHtmlData;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        QWebView *myWebView;
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QWebEngineView *myWebView;
#endif

    QVBoxLayout *internalRightVBoxLayout;
    QVBoxLayout *internalLeftVBoxLayout;
    QHBoxLayout *externalHBoxLayout;
    QHBoxLayout *internalTopHBoxLayout;
    QVBoxLayout *t1VBoxLayout;
    QVBoxLayout *t2VBoxLayout;

    QSqlQueryModel *teachersModel;
    QueryRunner *runner;
    QString receivedID;

    QLabel *chosenTeacherLabel;
    QLabel *fileSavePathLabel;
    QLabel *teachersLabel;
    QLabel *outputFormatLabel;
    QLabel *previewLabel;
    QLabel *chosenYearLabel;

};

#endif // OUTPUTTOFILEVIEW_H
