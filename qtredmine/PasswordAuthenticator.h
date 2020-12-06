#ifndef PASSWORD_AUTHENTICATOR_H
#define PASSWORD_AUTHENTICATOR_H

#include "Authenticator.h"
#include <QtCore/QString>

namespace qtredmine {

//!
//! @brief Basic login and password authenticator
//!
//! Adds an "Authorization" header using HTTP Basic authentication.
//!
class PasswordAuthenticator : public Authenticator
{
public:
    //! \brief Constructor
    //!
    //! \param login    Redmine login
    //! \param password Redmine password
    //! \param parent   Parent QObject
    PasswordAuthenticator (const QString &login, const QString &password, QObject* parent = nullptr);

    //! \brief Destructor
    virtual ~PasswordAuthenticator() {}

    //! \brief Add authentication header to request
    //! \param request Request to add header to
    virtual void addAuthentication( QNetworkRequest* request ) override;

private:
    QString _login;
    QString _password;
};

} // qtredmine

#endif // PASSWORD_AUTHENTICATOR_H
