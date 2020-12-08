#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtWidgets/QDialog>
#include "AuthWidget.h"
#include "ProjectListWidget.h"
#include "IssuesWidget.h"

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog (QWidget *parent = nullptr);
    virtual ~MainDialog ();

private slots:
    void slotProjectSelected (int id);

private:
    AuthWidget *_authWidget {nullptr};
    ProjectListWidget *_projectsWidget {nullptr};
    IssuesWidget *_issuesWidget {nullptr};
};
#endif // MAINDIALOG_H
