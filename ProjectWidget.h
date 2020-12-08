#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include "qtredmine/SimpleRedmineClient.h"
using namespace qtredmine;

#include <QtWidgets/QWidget>

namespace Ui {
    class ProjectWidget;
}

class ProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectWidget (const Project &project, QWidget *parent = nullptr);
    virtual ~ProjectWidget ();

    QString name () const;

signals:
    void signalSelected (int id);

protected:
    void mouseReleaseEvent (QMouseEvent *event) override;

private:
    Ui::ProjectWidget *ui {nullptr};
    int _id {-1};
};

#endif // PROJECTWIDGET_H
