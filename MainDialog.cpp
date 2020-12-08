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
        connect (_projectsWidget, &ProjectListWidget::signalSelected,
                 this, &MainDialog::slotProjectSelected);
        layout ()->addWidget (_projectsWidget);

    });
    ml->addWidget (_authWidget);
}

MainDialog::~MainDialog()
{
    //delete ui;
}

void MainDialog::slotProjectSelected (int id)
{
    if (_projectsWidget) {
        delete _projectsWidget;
        _projectsWidget = nullptr;
    }

    _issuesWidget = new IssuesWidget (id, this);
    layout ()->addWidget (_issuesWidget);
}

