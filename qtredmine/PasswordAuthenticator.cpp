#include "Logging.h"
#include "PasswordAuthenticator.h"

#include <QNetworkRequest>

using namespace qtredmine;

PasswordAuthenticator::PasswordAuthenticator (const QString &login, const QString &password, QObject* parent)
    : Authenticator (parent)
    , _login (login)
    , _password (password)
{}

void PasswordAuthenticator::addAuthentication (QNetworkRequest* request)
{
    QByteArray auth = QByteArray (QString ("%1:%2").arg (_login, _password).toLatin1 ().toBase64 ());
    request->setRawHeader ("Authorization", "Basic " + auth);
}
