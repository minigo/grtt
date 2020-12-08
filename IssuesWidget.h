#ifndef ISSUEWIDGET_H
#define ISSUEWIDGET_H

#include <QtWidgets/QWidget>

namespace Ui {
    class IssuesWidget;
}

class IssuesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IssuesWidget (int id, QWidget *parent = nullptr);
    virtual ~IssuesWidget();

protected slots:
    void slotReload ();

private:
    Ui::IssuesWidget *ui {nullptr};
    int _id {-1};
};

#endif // ISSUEWIDGET_H
