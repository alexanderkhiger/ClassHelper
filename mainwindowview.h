#ifndef MAINWINDOWVIEW_H
#define MAINWINDOWVIEW_H

#include <QMainWindow>
#include "tableeditorview.h"
#include "droparea.h"
#include "loadnewfileview.h"
#include <QSqlTableModel>
#include <QCheckBox>
#include "queryrunner.h"
#include <QTextEdit>
#include <QHeaderView>
#include <QTableView>
#include <QModelIndex>
#include "customdoublevalidator.h"
#include "outputtofileview.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QMimeData>
#include <QPushButton>
#include <QTableWidget>
#include "droparea.h"
#include <QListWidget>
#include <QString>
#include <QGroupBox>
#include <QWidget>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QListView>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QToolButton>
#include <QToolBar>
#include <customquerymodel.h>
#include <QComboBox>
#include <distributionview.h>
#include <QDate>

class MainWindowView : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindowView(QString uID, QString uName, QString uShortname, QWidget *ref, QWidget *parent = 0);

public slots:

    void setLoadNewFileAsCentral();
    void back();
    void setTableEditorAsCentral();
    void setWorkFieldAsCentral();
    void setOutputAsCentral();
    void changeWindowTitle(int semester);
    void hideToolbars();
    void showToolbars();

private:

    QString receivedID;
    QString receivedName;
    QString receivedShortname;

    QToolBar *leftBar;
    QToolBar *topBar;
    QToolButton *clearTool;
    QToolButton *changeSemesterTool;

    QWidget *parentReference;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QAction *newFileAction;

    QAction *backToUniversityList;
    QAction *closeAction;

    DistributionView *distributionWidget;
    TableEditorView *tableEditor;
    LoadNewFileView *loadNewFile;
    OutputToFileView *outFile;

private slots:
    void createUI();

};

#endif // MAINWINDOW_H
