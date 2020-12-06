#ifndef AUTHWIDGET_H
#define AUTHWIDGET_H

#include "qtredmine/SimpleRedmineClient.h"
using namespace qtredmine;
#include <QtWidgets/QWidget>

namespace Ui {
    class AuthWidget;
}

class AuthWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AuthWidget (QWidget *parent = nullptr);
    virtual ~AuthWidget();

signals:
    void loginSuccess ();

protected slots:
    void slotUpdateLoginButton ();
    void slotLoginClicked ();

private:
    Ui::AuthWidget *ui {nullptr};
};

#endif // AUTHWIDGET_H
