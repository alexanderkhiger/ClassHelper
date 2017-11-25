#ifndef AUTH_H
#define AUTH_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <mainwindow.h>

class QPushButton;
class QDialogButtonBox;
class QLineEdit;
class QLabel;
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;

class Auth : public QWidget
{
    Q_OBJECT

public:
    Auth();

private:
    QSqlDatabase db;
    QPushButton *authButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QLineEdit *hostnameField;
    QLineEdit *databaseField;
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLabel *hostnameLabel;
    QLabel *databaseLabel;
    QTextEdit *actionsLog;
    QVBoxLayout *externalVLayout;
    QVBoxLayout *internalLeftVLayout;
    QVBoxLayout *internalRightVLayout;
    QHBoxLayout *internalHLayout;

private slots:
    void tryAuth();

};

#endif // AUTH_H
