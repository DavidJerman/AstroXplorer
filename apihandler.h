#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <string>
#include <QUrl>
#include <map>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class APIHandler
{
public:
    APIHandler();

    static QUrl getAPOD_API_Request_URL(const std::string &APOD_URL, const std::string &API_KEY);

    static QJsonObject parseJSON(const std::string data);
    static QJsonObject parseJSON(const QString data);
};

#endif // APIHANDLER_H
