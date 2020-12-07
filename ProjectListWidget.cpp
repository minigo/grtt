#include "ProjectListWidget.h"
#include "ui_ProjectListWidget.h"
#include "ProjectWidget.h"

#include <QtCore/QTimer>

#include "qtredmine/SimpleRedmineClient.h"
using namespace qtredmine;

ProjectListWidget::ProjectListWidget (QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::ProjectListWidget)
{
    ui->setupUi (this);
    _w = new Widget (this);
    ui->_scrollArea->setWidget (_w);

    QTimer::singleShot (0, this, &ProjectListWidget::slotInit);
}

ProjectListWidget::~ProjectListWidget ()
{
    delete ui;
}

void ProjectListWidget::slotInit ()
{
    SimpleRedmineClient::_instance->retrieveProjects ([this]( Projects projects, RedmineError redmineError, QStringList errors)
    {
        if (redmineError != RedmineError::NO_ERR) {
            qDebug () << errors;
            return;
        }

        for (const auto& project : projects) {
            auto project_widget = new ProjectWidget (project, this);
            _w->_vl->addWidget (project_widget);
        }
        _w->_vl->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    } );
}
