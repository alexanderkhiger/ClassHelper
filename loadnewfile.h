#ifndef LOADNEWFILE_H
#define LOADNEWFILE_H

#include <QWidget>

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
class LoadNewFileLogic;

class LoadNewFile : public QWidget
{
    Q_OBJECT
public:
    explicit LoadNewFile(QWidget *parent = nullptr);
    friend class LoadNewFileLogic;

signals:

public slots:
    void chooseFile();
    void clear();

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


};

#endif // LOADNEWFILE_H
