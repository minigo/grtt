#include "ProjectListWidget.h"
#include "ui_ProjectListWidget.h"

#include <QtCore/QTimer>

#include "qtredmine/SimpleRedmineClient.h"
using namespace qtredmine;

ProjectListWidget::ProjectListWidget (QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::ProjectListWidget)
{
    ui->setupUi (this);
    _w = new WidgetScroll (this);
    ui->_scrollArea->setWidget (_w);

    connect (ui->_buttonReload, &QPushButton::clicked, this, &ProjectListWidget::slotReload);
    connect (ui->_editFilter, &QLineEdit::textChanged, _w, &WidgetScroll::slotFiltered);

    QTimer::singleShot (0, this, &ProjectListWidget::slotReload);
}

ProjectListWidget::~ProjectListWidget ()
{
    delete ui;
}

void ProjectListWidget::slotReload ()
{
    _w->removeAllProjectWidgets ();

    SimpleRedmineClient::_instance->retrieveProjects ([this]( Projects projects, RedmineError redmineError, QStringList errors)
    {
        if (redmineError != RedmineError::NO_ERR) {
            qDebug () << errors;
            return;
        }

        for (const auto& project : projects) {
            auto wid = new ProjectWidget (project, this);
            connect (wid, &ProjectWidget::signalSelected, this, &ProjectListWidget::signalSelected);
            _w->addProjectWidget (wid);
        }
    } );
}

//-----------------------------------------------------------------

WidgetScroll::WidgetScroll (QWidget *parent)
    : QWidget (parent)
{
    _vl = new QVBoxLayout (this);
    _vl->setContentsMargins (0, 0, 0, 0);
    _vl->setSpacing (0);
    _vl->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    setLayout (_vl);
}

WidgetScroll::~WidgetScroll ()
{}

void WidgetScroll::addProjectWidget (ProjectWidget *wid)
{
    if (wid)
        _vl->insertWidget (_vl->count () - 1, wid);
}

void WidgetScroll::removeAllProjectWidgets ()
{
    if (_vl->count () == 1)
        return;

    while (_vl->count () != 1) {
        if (dynamic_cast<ProjectWidget*> (_vl->itemAt (0)->widget ()))
            delete _vl->itemAt (0)->widget ();
    }
}

void WidgetScroll::slotFiltered (const QString &text)
{
    if (text.isEmpty ())
    {
        for (int i = 0; i < _vl->count (); ++i)
        {
            if (dynamic_cast<ProjectWidget*> (_vl->itemAt (i)->widget ()))
                _vl->itemAt (i)->widget ()->setVisible (true);
        }
    }
    else
    {
        for (int i = 0; i < _vl->count (); ++i)
        {
            if (dynamic_cast<ProjectWidget*> (_vl->itemAt (i)->widget ()))
            {
                if (dynamic_cast<ProjectWidget*> (_vl->itemAt (i)->widget ())->name ().toLower ().contains (text.toLower ()))
                    _vl->itemAt (i)->widget ()->setVisible (true);
                else
                    _vl->itemAt (i)->widget ()->setVisible (false);
            }
        }
    }
}
