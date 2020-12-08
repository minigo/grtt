#include "ProjectWidget.h"
#include "ui_ProjectWidget.h"

ProjectWidget::ProjectWidget (const Project &project, QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::ProjectWidget)
{
    ui->setupUi (this);

    _id = project._id;
    ui->_lableProjectName->setText (project._name);
    //ui->_groupBox->setStyleSheet ("QGroupBox { background: yellow; border: 1px solid black; border-radius: 4px; }");
    //ui->_groupBox->setStyleSheet ("QGroupBox { background: yellow }");
}

ProjectWidget::~ProjectWidget ()
{
    delete ui;
}

QString ProjectWidget::name () const {
    return ui->_lableProjectName->text ();
}

void ProjectWidget::mouseReleaseEvent (QMouseEvent *event)
{
    emit signalSelected (_id);
    QWidget::mouseReleaseEvent (event);
}

