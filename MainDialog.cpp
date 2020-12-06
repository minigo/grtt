#include "MainDialog.h"
#include "ui_MainDialog.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

MainDialog::MainDialog (QWidget *parent)
    : QDialog (parent)
{
    resize (400, 640);

    QHBoxLayout *ml = new QHBoxLayout ();
    ml->setContentsMargins (0, 0, 0, 0);
    setLayout (ml);

    _authWidget = new AuthWidget (this);
    connect (_authWidget, &AuthWidget::loginSuccess, this, [this]()
    {
        delete _authWidget;
        _authWidget = nullptr;

        _projectsWidget = new ProjectListWidget (this);
        layout ()->addWidget (_projectsWidget);

    });
    ml->addWidget (_authWidget);

    //    _redmine = new SimpleRedmineClient ("http://192.168.88.186", "250dd4902e347cc4c9b549b3930d7253b3d789c5", false);
    //    _redmine->setAuthenticator ("user", "Windfr0m@ss");

    //    _redmine->retrieveProjects ([]( Projects projects, RedmineError redmineError, QStringList errors)
    //        {
    //        if (redmineError != RedmineError::NO_ERR)
    //        {
    //            qDebug () << errors;
    //            return;
    //        }

    //        for(const auto& project : projects)
    //            qDebug () << project._name;

    //        } );
}

MainDialog::~MainDialog()
{
    //delete ui;
}

