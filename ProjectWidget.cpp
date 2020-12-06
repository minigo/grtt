#include "ProjectWidget.h"
#include "ui_ProjectWidget.h"

ProjectWidget::ProjectWidget (const Project &project, QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::ProjectWidget)
{
    ui->setupUi (this);

    _id = project._id;
    ui->_lableProjectName->setText (project._name);
}

ProjectWidget::~ProjectWidget ()
{
    delete ui;
}
