#include "authview.h"
#include <QtWidgets>

AuthView::AuthView()
{
    createUI();
    connect(quitButton, &QAbstractButton::pressed, this, &QWidget::close);
    connect(authButton, &QAbstractButton::pressed, this, &AuthView::tryAuth);
}

void AuthView::createUI()
{
    this->setMinimumHeight(250);
    this->resize(250,250);

    usernameField = new QLineEdit;
    passwordField = new QLineEdit;
    hostnameField = new QLineEdit;
    databaseField = new QLineEdit;

    usernameField->setText("root");
    passwordField->setText("10011997");
    hostnameField->setText("localhost");
    databaseField->setText("diplom");

    passwordField->setEchoMode(QLineEdit::Password);
    usernameField->setPlaceholderText(tr("Ваш логин"));
    passwordField->setPlaceholderText(tr("Ваш пароль"));
    hostnameField->setPlaceholderText(tr("IP или localhost"));
    databaseField->setPlaceholderText(tr("Название БД"));

    usernameField->setAlignment(Qt::AlignCenter);
    passwordField->setAlignment(Qt::AlignCenter);
    hostnameField->setAlignment(Qt::AlignCenter);
    databaseField->setAlignment(Qt::AlignCenter);

    usernameLabel = new QLabel(tr("Имя пользователя"));
    passwordLabel = new QLabel(tr("Пароль"));
    hostnameLabel = new QLabel(tr("Адрес машины"));
    databaseLabel = new QLabel(tr("База данных"));


    usernameLabel->setFixedWidth(150);
    usernameField->setMinimumWidth(150);

    authButton = new QPushButton(tr("Авторизация"));
    quitButton = new QPushButton(tr("Выход"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(authButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    actionsLog = new QTextEdit;

    actionsLog->setReadOnly(true);

    usernameLabel->setAlignment(Qt::AlignHCenter);
    passwordLabel->setAlignment(Qt::AlignHCenter);
    hostnameLabel->setAlignment(Qt::AlignHCenter);
    databaseLabel->setAlignment(Qt::AlignHCenter);

    externalVLayout = new QVBoxLayout();
    internalLeftVLayout = new QVBoxLayout();
    internalRightVLayout = new QVBoxLayout();
    internalHLayout = new QHBoxLayout();

    this->setLayout(externalVLayout);

    internalLeftVLayout->addWidget(usernameLabel);
    internalLeftVLayout->addWidget(passwordLabel);
    internalLeftVLayout->addWidget(hostnameLabel);
    internalLeftVLayout->addWidget(databaseLabel);

    internalRightVLayout->addWidget(usernameField);
    internalRightVLayout->addWidget(passwordField);
    internalRightVLayout->addWidget(hostnameField);
    internalRightVLayout->addWidget(databaseField);

    internalHLayout->addLayout(internalLeftVLayout);
    internalHLayout->addLayout(internalRightVLayout);

    externalVLayout->addLayout(internalHLayout);
    externalVLayout->addWidget(actionsLog);
    externalVLayout->addWidget(buttonBox);

    setWindowTitle(tr("Авторизация"));
}
void AuthView::authError(const QString error)
{
    actionsLog->setText(error);
}

void AuthView::authSuccess()
{
    delete runner;
    UniversityView *frm = new UniversityView;
    frm->show();
    this->close();
}

void AuthView::tryAuth()
{
    runner = new QueryRunner(this);
    connect(runner,&QueryRunner::authError,this,&AuthView::authError);
    connect(runner,&QueryRunner::authSuccess,this,&AuthView::authSuccess);
    runner->tryAuth(usernameField->text(),passwordField->text(),databaseField->text(),hostnameField->text());
}
