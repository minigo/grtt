#ifndef PROJECTLISTWIDGET_H
#define PROJECTLISTWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include "ProjectWidget.h"

namespace Ui {
    class ProjectListWidget;
}

//!
//! \brief The WidgetScroll class
//!
class WidgetScroll : public QWidget
{
    Q_OBJECT

public:
    WidgetScroll (QWidget *parent = nullptr);
    virtual ~WidgetScroll ();

public slots:
    void addProjectWidget (ProjectWidget *wid);
    void removeAllProjectWidgets ();
    void slotFiltered (const QString &text);

protected:
    QVBoxLayout *_vl {nullptr};
};

//!
//! \brief The ProjectListWidget class
//!
class ProjectListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectListWidget (QWidget *parent = nullptr);
    virtual ~ProjectListWidget ();

signals:
    void signalSelected (int id);

protected slots:
    void slotReload ();

private:
    Ui::ProjectListWidget *ui {nullptr};
    WidgetScroll *_w {nullptr};

};

#endif // PROJECTLISTWIDGET_H
