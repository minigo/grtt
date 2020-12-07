#ifndef PROJECTLISTWIDGET_H
#define PROJECTLISTWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>

namespace Ui {
    class ProjectListWidget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget (QWidget *parent = nullptr)
        : QWidget (parent)
    {
        _vl = new QVBoxLayout (this);
        _vl->setContentsMargins (0, 0, 0, 0);
        _vl->setSpacing (0);
        setLayout (_vl);
    }

    QVBoxLayout *_vl {nullptr};
};

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
    Widget *_w {nullptr};

};

#endif // PROJECTLISTWIDGET_H
