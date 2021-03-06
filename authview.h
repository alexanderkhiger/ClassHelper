#ifndef AUTHVIEW_H
#define AUTHVIEW_H

#include <QWidget>

#include <queryrunner.h>
#include <universityview.h>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

class AuthView : public QWidget
{
    Q_OBJECT

public:
    AuthView();

private:
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
    QueryRunner *runner;

public slots:
    void tryAuth();
    void authSuccess();
    void authError(const QString error);

private slots:
    void createUI();

};

#endif // AUTH_H
