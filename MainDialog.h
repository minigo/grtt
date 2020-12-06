#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "AuthWidget.h"
#include "ProjectListWidget.h"

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog (QWidget *parent = nullptr);
    virtual ~MainDialog ();

private:
    AuthWidget *_authWidget {nullptr};
    ProjectListWidget *_projectsWidget {nullptr};
};
#endif // MAINDIALOG_H
