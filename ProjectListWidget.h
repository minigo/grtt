#ifndef PROJECTLISTWIDGET_H
#define PROJECTLISTWIDGET_H

#include <QtWidgets/QWidget>

namespace Ui {
    class ProjectListWidget;
}

class ProjectListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectListWidget (QWidget *parent = nullptr);
    virtual ~ProjectListWidget ();

protected slots:
    void slotInit ();

private:
    Ui::ProjectListWidget *ui {nullptr};
};

#endif // PROJECTLISTWIDGET_H
