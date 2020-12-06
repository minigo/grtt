#include "KeyAuthenticator.h"
#include "Logging.h"
#include "PasswordAuthenticator.h"
#include "RedmineClient.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace qtredmine;

RedmineClient::RedmineClient( QObject* parent )
    : QObject( parent )
{
    ENTER();
    RETURN();
}

RedmineClient::RedmineClient( const QString& url, QObject* parent )
    : RedmineClient( parent )
{
    ENTER()(url);

    setUrl( url );

    RETURN();
}

RedmineClient::RedmineClient( const QString& url, const QString& apiKey, const bool checkSsl,
                              QObject* parent )
    : RedmineClient( url, parent )
{
    ENTER()(url)(apiKey)(checkSsl);

    setCheckSsl( checkSsl );
    setUrl( url );
    setAuthenticator( apiKey );

    RETURN();
}

RedmineClient::RedmineClient( const QString& url, const QString& login, const QString& password,
                              const bool checkSsl, QObject* parent )
    : RedmineClient( url, parent )
{
    ENTER()(url)(login)(password)(checkSsl);

    setCheckSsl( checkSsl );
    setUrl( url );
    setAuthenticator( login, password );

    RETURN();
}

void
RedmineClient::init()
{
    ENTER();

    reconnect();
    emit initialised();

    RETURN();
}

void
RedmineClient::reconnect()
{
    ENTER();

    // Possibly delete old QNetworkAccessManager object
    if( _nma )
        _nma->deleteLater();

    // Create QNetworkAccessManager object
    _nma = new QNetworkAccessManager( this );

    // When a reqest to the network access manager has finished, call this->replyFinished()
    connect( _nma, &QNetworkAccessManager::finished, this, &RedmineClient::replyFinished );

    // Handle SSL errors
    connect( _nma, &QNetworkAccessManager::sslErrors, this, &RedmineClient::handleSslErrors );

    // Handle network accessibility change
    connect( _nma, &QNetworkAccessManager::networkAccessibleChanged,
             [&](QNetworkAccessManager::NetworkAccessibility accessible)
    {
        ENTER();
        emit networkAccessibleChanged( accessible );
        RETURN();
    } );

    RETURN();
}

QString
RedmineClient::getUrl() const
{
    ENTER();
    RETURN( _url );
}

void
RedmineClient::handleSslErrors( QNetworkReply* reply, const QList<QSslError>& errors )
{
    ENTER()(reply)(errors);

    if( !checkSsl_ )
        reply->ignoreSslErrors();

    RETURN();
}

void
RedmineClient::setAuthenticator( const QString& apiKey )
{
    ENTER()(apiKey);

    if( apiKey == authApiKey_ )
        RETURN();

    authApiKey_ = apiKey;

    auth_ = new KeyAuthenticator( apiKey.toLatin1(), this );

    if( !_url.isEmpty() )
        init();

    RETURN();
}

void
RedmineClient::setAuthenticator( const QString& login, const QString& password )
{
    ENTER()(login)(password);

    if( login == authLogin_ && password == authPassword_ )
        RETURN();

    authLogin_ = login;
    authPassword_ = password;

    auth_ = new PasswordAuthenticator( login, password, this );

    if( !_url.isEmpty() )
        init();

    RETURN();
}

void
RedmineClient::setCheckSsl(bool checkSsl )
{
    ENTER()(checkSsl);

    if( checkSsl == checkSsl_ )
        RETURN();

    checkSsl_ = checkSsl;

    if( auth_ && !_url.isEmpty() )
        init();

    RETURN();
}

void
RedmineClient::setUrl( const QString& url )
{
    ENTER()(url);

    if( url == _url )
        RETURN();

    _url = url;

    if( auth_ )
        init();

    RETURN();
}

void
RedmineClient::setUserAgent( const QByteArray& userAgent )
{
    ENTER()(userAgent);

    _userAgent = userAgent;

    RETURN();
}

QNetworkReply*
RedmineClient::sendRequest (const QString& resource, JsonCb callback,
                            const QNetworkAccessManager::Operation mode,
                            const QString& queryParams, const QByteArray& postData)
{
    //
    // Initial checks
    //

    if (!_nma) {
        qCritical () << "[RedmineClient][sendRequest] Network manager not yet initialised";
        return nullptr;
    }
    if (resource.isEmpty ()) {
        qCritical () << "[RedmineClient][sendRequest] No resource specified";
        return nullptr;
    }
    if (mode == QNetworkAccessManager::GetOperation && !callback) {
        qCritical () << "[RedmineClient][sendRequest] No callback specified for HTTP GET mode";
        return nullptr;
    }

    //
    // Build the Redmine REST URL
    //

    QUrl url = _url + "/" + resource + ".json?" + queryParams;

    if (!url.isValid ()) {
        qCritical () << "[RedmineClient][sendRequest] Invalid URL";
        return nullptr;
    }
    else
        qDebug () << "[RedmineClient][sendRequest] Using URL:" << url;

    //
    // Build the network request
    //

    QNetworkRequest request (url);
    request.setRawHeader ("User-Agent",          _userAgent);
    request.setRawHeader ("X-Custom-User-Agent", _userAgent);
    request.setRawHeader ("Content-Type",        "application/json");
    request.setRawHeader ("Content-Length",      QByteArray::number (postData.size ()));
    auth_->addAuthentication (&request);

    //
    // Perform the network action
    //

    QNetworkReply *reply {nullptr};

    switch (mode)
    {
    case QNetworkAccessManager::GetOperation:
        reply = _nma->get (request);
        break;

    case QNetworkAccessManager::PostOperation:
        reply = _nma->post (request, postData);
        break;

    case QNetworkAccessManager::PutOperation:
        reply = _nma->put (request, postData);
        break;

    case QNetworkAccessManager::DeleteOperation:
        reply = _nma->deleteResource (request);
        break;
    default:
        qWarning () << "[RedmineClient][sendRequest] Unknown operation";
        return nullptr;
    }

    if (reply && callback)
        callbacks_[reply] = callback;

    return reply;
}

void
RedmineClient::replyFinished( QNetworkReply* reply )
{
    ENTER()(reply);

    // Search for callback function
    if( reply && callbacks_.contains(reply) )
    {
        QByteArray data_raw = reply->readAll();
        QJsonDocument data_json = QJsonDocument::fromJson( data_raw );

        JsonCb callback = callbacks_[reply];
        callback( reply, &data_json );

        callbacks_.remove( reply );
    }

    reply->deleteLater();

    RETURN();
}

void
getResMode( const int id, QString& resource, QNetworkAccessManager::Operation& mode )
{
    ENTER()(id)(resource)(mode);

    if( id == NULL_ID )
        mode = QNetworkAccessManager::PostOperation;
    else
    {
        resource.append("/").append(QString::number(id));
        mode = QNetworkAccessManager::PutOperation;
    }

    RETURN();
}

void
RedmineClient::sendCustomField(const QJsonDocument& data, JsonCb callback, int id,
                                const QString& parameters )
{
    ENTER()(parameters);

    QString resource = "custom_fields";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendEnumeration( const QString& enumeration, const QJsonDocument& data, JsonCb callback,
                                const int id, const QString& parameters )
{
    ENTER()(enumeration)(parameters);

    QString resource = "enumerations/"+enumeration;
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendIssue(const QJsonDocument& data, JsonCb callback, int id,
                          const QString& parameters )
{
    ENTER()(id)(parameters);

    QString resource = "issues";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendIssueCategory( const QJsonDocument& data, JsonCb callback, const int id,
                                  const QString& parameters )
{
    ENTER()(parameters);

    QString resource = "issue_categories";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendIssuePriority( const QJsonDocument& data, JsonCb callback, const int id,
                                  const QString& parameters )
{
    ENTER()(parameters);

    QString resource = "issue_priorities";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendEnumeration( resource, data, callback, id, parameters );

    RETURN();
}

void
RedmineClient::sendIssueStatus( const QJsonDocument& data, JsonCb callback, const int id,
                                const QString& parameters )
{
    ENTER()(parameters);

    QString resource = "issue_statuses";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendProject( const QJsonDocument& data, JsonCb callback, const int id,
                            const QString& parameters )
{
    ENTER()(parameters);

    QString resource = "projects";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendTimeEntry( const QJsonDocument& data, JsonCb callback, const int id,
                              const QString& parameters )
{
    ENTER()(parameters);

    QString resource = "time_entries";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendTimeEntryActivity( const QJsonDocument& data, JsonCb callback, const int id,
                                      const QString& parameters )
{
    ENTER()(parameters);

    QString resource = "time_entry_activities";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendEnumeration( resource, data, callback, id, parameters );

    RETURN();
}

void
RedmineClient::sendTracker( const QJsonDocument& data, JsonCb callback, const int id,
                            const QString& parameters )
{
    ENTER()(parameters);

    QString resource = "trackers";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::sendUser( const QJsonDocument& data, JsonCb callback, const int id, const QString& parameters )
{
    ENTER()(parameters);

    QString resource = "users";
    QNetworkAccessManager::Operation mode;

    getResMode( id, resource, mode );

    sendRequest( resource, callback, mode, parameters, data.toJson() );

    RETURN();
}

void
RedmineClient::retrieveCustomFields( JsonCb callback, const QString& parameters )
{
    ENTER()(parameters);

    sendRequest( "shared/custom_fields", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveEnumerations( const QString& enumeration, JsonCb callback, const QString& parameters )
{
    ENTER()(enumeration)(parameters);

    sendRequest( "enumerations/"+enumeration, callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveIssues( JsonCb callback, const QString& parameters )
{
    ENTER()(parameters);

    sendRequest( "issues", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveIssueCategories( JsonCb callback, const int projectId, const QString& parameters )
{
    ENTER()(projectId)(parameters);

    sendRequest( QString("projects/%1/issue_categories").arg(projectId), callback,
                 QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveIssuePriorities( JsonCb callback, const QString& parameters )
{
    ENTER()(parameters);

    retrieveEnumerations( "issue_priorities", callback, parameters );

    RETURN();
}

void
RedmineClient::retrieveIssue( JsonCb callback, const int issueId, const QString& parameters )
{
    ENTER()(issueId)(parameters);

    sendRequest( QString("issues/%1").arg(issueId), callback, QNetworkAccessManager::GetOperation,
                 parameters );

    RETURN();
}

void
RedmineClient::retrieveIssueStatuses( JsonCb callback, const QString& parameters )
{
    ENTER()(parameters);

    sendRequest( "issue_statuses", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveMemberships( JsonCb callback, const int projectId, const QString& parameters )
{
    ENTER()(projectId)(parameters);

    sendRequest( QString("projects/%1/memberships").arg(projectId), callback,
                 QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveProject( JsonCb callback, const int projectId, const QString& parameters )
{
    ENTER()(projectId)(parameters);

    sendRequest( QString("projects/%1").arg(projectId), callback, QNetworkAccessManager::GetOperation,
                 QString("%1&include=%2").arg(parameters).arg("enabled_modules,issue_categories,trackers") );

    RETURN();
}

void
RedmineClient::retrieveProjects (JsonCb callback, const QString& parameters)
{
    sendRequest ("projects", callback, QNetworkAccessManager::GetOperation,
                 QString ("%1&include=%2").arg (parameters).arg ("enabled_modules,issue_categories,trackers"));
}

void
RedmineClient::retrieveTimeEntries( JsonCb callback, const QString& parameters )
{
    ENTER()(parameters);

    sendRequest( "time_entries", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveTimeEntryActivities( JsonCb callback, const QString& parameters )
{
    ENTER()(parameters);

    retrieveEnumerations( "time_entry_activities", callback, parameters );

    RETURN();
}

void
RedmineClient::retrieveTrackers( JsonCb callback, const QString& parameters )
{
    ENTER()(parameters);

    sendRequest( "trackers", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveCurrentUser( JsonCb callback, const QString& parameters )
{
    ENTER()(parameters);

    sendRequest( "users/current", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveUsers( JsonCb callback, const QString& parameters )
{
    ENTER()(parameters);

    sendRequest( "users", callback, QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}

void
RedmineClient::retrieveVersions( JsonCb callback, const int projectId, const QString& parameters )
{
    ENTER()(projectId)(parameters);

    sendRequest( QString("projects/%1/versions").arg(projectId), callback,
                 QNetworkAccessManager::GetOperation, parameters );

    RETURN();
}
