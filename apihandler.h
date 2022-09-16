#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <string>
#include <QUrl>
#include <map>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include <fstream>

#include "enums.h"

class APIHandler {
public:
    APIHandler() = default;

    static QUrl getAPOD_API_Request_URL(const std::string &APOD_URL, const std::string &API_KEY);

    static QJsonObject parseJSON(std::string data);

    static QJsonObject parseJSON(QString data);

    static QJsonObject parseJSON(const QByteArray &data);

    static std::string dateToString(const QDate& date);

    static QUrl getMarsRoverImagerySols_API_Request_URL(std::string MARS_ROVER_URL,
                                                        std::string API_KEY,
                                                        ORIGIN rover,
                                                        ORIGIN camera,
                                                        std::string sol);

    static QUrl getMarsRoverImagerySols_API_Request_URL(std::string MARS_ROVER_URL,
                                                        std::string API_KEY,
                                                        std::string rover,
                                                        std::string camera,
                                                        std::string sol);

    static QUrl getMarsRoverImageryEarthDate_API_Request_URL(std::string MARS_ROVER_URL,
                                                             std::string API_KEY,
                                                             ORIGIN rover,
                                                             ORIGIN camera,
                                                             std::string date);

    static QUrl getMarsRoverManifest_API_Request_URL(std::string MARS_ROVER_URL,
                                                     std::string API_KEY,
                                                     ORIGIN rover);
    static QUrl getEPICJson_Request_URL(QString API_KEY, QString baseUrl, QDate date, QString type);

    static QUrl getEPICImage_Request_URL(QString API_KEY, QString baseUrl, QDate date, QString type, QString imageFileName, QString extension);

    static QUrl getEPICData_Request_URL(QString API_KEY, QString baseUrl, QString type);

    static void clearXMLFile(QString fileName);

    static void clearXMLFile(std::string fileName);
};

#endif // APIHANDLER_H
