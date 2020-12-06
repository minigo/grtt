#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QtCore/QObject>

class QNetworkRequest;

namespace qtredmine {

//!
//! \brief Authenticator interface
//! Handle the authentication for a given network request.
//!
class Authenticator : public QObject
{
public:
    //! \brief Constructor
    //! \param parent Parent QObject
    Authenticator (QObject* parent = nullptr)
        : QObject (parent) {}

    //! \brief Destructor
    virtual ~Authenticator () {}

    //! \brief Add authentication header to request
    //! \param request Request to add header to
    virtual void addAuthentication (QNetworkRequest* request) = 0;
};

} // qtredmine

#endif // AUTHENTICATOR_H
