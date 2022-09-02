#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <string>
#include <QUrl>
#include <map>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>

#include "enums.h"

class APIHandler
{
public:
    APIHandler();

    static QUrl getAPOD_API_Request_URL(const std::string &APOD_URL, const std::string &API_KEY);

    static QJsonObject parseJSON(const std::string data);
    static QJsonObject parseJSON(const QString data);
    static QJsonObject parseJSON(const QByteArray &data);

    static std::string dateToString(QDate date);

    static QUrl getMarsRoverImagerySols_API_Request_URL(const std::string MARS_ROVER_URL,
                                                        const std::string API_KEY,
                                                        ORIGIN rover,
                                                        ORIGIN camera,
                                                        const std::string sol);
    static QUrl getMarsRoverImagerySols_API_Request_URL(const std::string MARS_ROVER_URL,
                                                        const std::string API_KEY,
                                                        const std::string rover,
                                                        const std::string camera,
                                                        const std::string sol);
    static QUrl getMarsRoverImageryEarthDate_API_Request_URL(const std::string MARS_ROVER_URL,
                                                             const std::string API_KEY,
                                                             ORIGIN rover,
                                                             ORIGIN camera,
                                                             const std::string date);
    static QUrl getMarsRoverManifest_API_Request_URL(const std::string MARS_ROVER_URL,
                                                     const std::string API_KEY,
                                                     ORIGIN rover);
};

#endif // APIHANDLER_H
