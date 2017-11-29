#ifndef LOADNEWFILEVIEW_H
#define LOADNEWFILEVIEW_H

#include <QWidget>
#include <QtWidgets>
#include <QSqlDatabase>
#include <QDebug>

#include "loadnewfilemodel.h"

class QString;
class QPushButton;
class QLineEdit;
class QLabel;
class QTextEdit;
class QHBoxLayout;
class QVBoxLayout;
class QProgressBar;
class QFileDialog;
class QDialogButtonBox;
class QMessageBox;


class LoadNewFileView : public QWidget
{
    Q_OBJECT
public:
    explicit LoadNewFileView(QWidget *parent = nullptr);

signals:

public slots:
    void chooseFile();
    void clear();
    void startProcessing();
    void finishProcessing();
    void getInformation(const QString info);
    void getProgress(const int percentage);

private:

    QString directory = "";
    QString buffer = "";
    int errorCount = 0;

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
