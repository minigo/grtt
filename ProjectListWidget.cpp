#include "ProjectListWidget.h"
#include "ui_ProjectListWidget.h"
#include "ProjectWidget.h"

#include <QtCore/QTimer>
#include <QtWidgets/QVBoxLayout>

#include "qtredmine/SimpleRedmineClient.h"
using namespace qtredmine;

ProjectListWidget::ProjectListWidget (QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::ProjectListWidget)
{
    ui->setupUi (this);
    auto l = new QVBoxLayout ();
    l->setContentsMargins (0, 0, 0, 0);
    setLayout (l);

    QTimer::singleShot (0, this, &ProjectListWidget::slotInit);
}

ProjectListWidget::~ProjectListWidget ()
{
    delete ui;
}

void ProjectListWidget::slotInit ()
{
    //QObjectList objects_list = children ();

    SimpleRedmineClient::_instance->retrieveProjects ([this]( Projects projects, RedmineError redmineError, QStringList errors)
    {
        if (redmineError != RedmineError::NO_ERR) {
            qDebug () << errors;
            return;
        }

        for (const auto& project : projects) {
            auto project_widget = new ProjectWidget (project, this);
            layout ()->addWidget (project_widget);
        }
        layout ()->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    } );
}
