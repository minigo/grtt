#ifndef KEY_AUTHENTICATOR_H
#define KEY_AUTHENTICATOR_H

#include "Authenticator.h"
#include <QtCore/QString>

namespace qtredmine {

//!
//! \brief API key authenticator
//! Adds an "X-Redmine-API-Key" header with API key.
//!
class KeyAuthenticator : public Authenticator
{
public:
    //! \brief Constructor
    //! \param apiKey Redmine API key
    //! \param parent Parent QObject
    KeyAuthenticator (const QByteArray &apiKey, QObject* parent = nullptr);

    //! \brief Destructor
    virtual ~KeyAuthenticator() {}

    //! \brief Add authentication header to request
    //! \param request Request to add header to
    virtual void addAuthentication (QNetworkRequest* request) override;

private:
    /// Redmine API Key
    QByteArray _apiKey;
};

} // qtredmine

#endif // KEY_AUTHENTICATOR_H
