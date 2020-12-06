#include "KeyAuthenticator.h"
#include "Logging.h"

#include <QNetworkRequest>

using namespace qtredmine;

KeyAuthenticator::KeyAuthenticator (const QByteArray &apiKey, QObject* parent)
    : Authenticator (parent)
    , _apiKey (apiKey)
{}

void KeyAuthenticator::addAuthentication (QNetworkRequest* request)
{
    request->setRawHeader ("X-Redmine-API-Key", _apiKey);
}
