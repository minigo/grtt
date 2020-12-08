#include "IssuesWidget.h"
#include "ui_IssuesWidget.h"
#include <QtCore/QTimer>

#include "qtredmine/SimpleRedmineClient.h"
using namespace qtredmine;

IssuesWidget::IssuesWidget (int id, QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::IssuesWidget)
    , _id (id)
{
    ui->setupUi (this);
    QTimer::singleShot (0, this, &IssuesWidget::slotReload);
}

IssuesWidget::~IssuesWidget ()
{
    delete ui;
}

void IssuesWidget::slotReload ()
{
    SimpleRedmineClient::_instance->retrieveIssues
            ([this](Issues issues, RedmineError redmineError, const QStringList &errors)
    {
        if (redmineError != RedmineError::NO_ERR) {
            qDebug () << errors;
            return;
        }

        for (const Issue &issue : issues)
        {
            if (issue.project._id != _id)
                continue;
            else
                qDebug () << issue.subject;
        }
    } );
}
