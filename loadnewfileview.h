#ifndef LOADNEWFILEVIEW_H
#define LOADNEWFILEVIEW_H

#include <QWidget>
#include <QDebug>

#include "loadnewfilemodel.h"

#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QMessageBox>


class LoadNewFileView : public QWidget
{
    Q_OBJECT
public:
    explicit LoadNewFileView(QString uID, QWidget *parent = 0);

signals:

public slots:
    void chooseFile();
    void clear();
    void startProcessing();
    void finishProcessing();
    void getInformation(const QString info);
    void getProgress(const int percentage);
    void getError(QSqlError error);

private:

    QString directory;
    QString buffer;
    int errorCount;
    QString receivedID;
    QPushButton *chooseFileButton;
    QPushButton *startButton;
    QPushButton *exitButton;
    QPushButton *clearButton;
    QDialogButtonBox *buttonBox;

    QLineEdit *chosenFile;
    QLineEdit *errorCounterLE;

    QLabel *errorCounterLabel;

    QTextEdit *actionsLog;

    QHBoxLayout *topHLayout;
    QHBoxLayout *middleHLayout;
    QHBoxLayout *bottomHLayout;
    QVBoxLayout *vLayout;

    QProgressBar *processingProgress;

    QMessageBox *msg;

    LoadNewFileModel *processor;

private slots:
    void createUI();

};

#endif // LOADNEWFILE_H
