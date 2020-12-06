#include "AuthWidget.h"
#include "ui_AuthWidget.h"

#include <QtCore/QSettings>

AuthWidget::AuthWidget (QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::AuthWidget)
{
    ui->setupUi (this);

    connect (ui->_editUser, &QLineEdit::textChanged, this, &AuthWidget::slotUpdateLoginButton);
    connect (ui->_editPassword, &QLineEdit::textChanged, this, &AuthWidget::slotUpdateLoginButton);
    connect (ui->_editRedmineUrl, &QLineEdit::textChanged, this, &AuthWidget::slotUpdateLoginButton);
    connect (ui->_buttonLogin, &QPushButton::clicked, this, &AuthWidget::slotLoginClicked);

    QSettings settings;
    ui->_checkRemember->setChecked (settings.value ("redmine connection/remember").toBool ());
    if (ui->_checkRemember->isChecked ()) {
        ui->_editUser->setText (settings.value ("redmine connection/user", "").toString ());
        ui->_editPassword->setText (settings.value ("redmine connection/password", "").toString ());
        ui->_editRedmineUrl->setText (settings.value ("redmine connection/url", "").toString ());
    }
}

AuthWidget::~AuthWidget ()
{
    delete ui;
}

void AuthWidget::slotUpdateLoginButton ()
{
    if (ui->_editUser->text ().isEmpty () ||
            ui->_editRedmineUrl->text ().isEmpty () ||
            ui->_editPassword->text ().isEmpty ()) {
        ui->_buttonLogin->setEnabled (false);
    } else {
        ui->_buttonLogin->setEnabled (true);
    }
}

void AuthWidget::slotLoginClicked ()
{
    if (!SimpleRedmineClient::_instance)
        new SimpleRedmineClient (ui->_editRedmineUrl->text ());

    SimpleRedmineClient::_instance->setAuthenticator (ui->_editUser->text (), ui->_editPassword->text ());
    SimpleRedmineClient::_instance->retrieveCurrentUser
            ([this]( User /*user*/, RedmineError redmineError, const QStringList &errors)
    {
        if (redmineError != RedmineError::NO_ERR) {
            qCritical () << "[AuthWidget][slotLoginClicked]" << errors;
            return;
        }

        QSettings settings;
        settings.setValue ("redmine connection/remember", ui->_checkRemember->isChecked ());
        if (ui->_checkRemember->isChecked ()) {
            settings.setValue ("redmine connection/user", ui->_editUser->text ());
            settings.setValue ("redmine connection/password", ui->_editPassword->text ());
            settings.setValue ("redmine connection/url", ui->_editRedmineUrl->text ());
        } else {
            settings.setValue ("redmine connection/user", "");
            settings.setValue ("redmine connection/password", "");
            settings.setValue ("redmine connection/url", "");
        }

        emit loginSuccess ();
    } );
}
