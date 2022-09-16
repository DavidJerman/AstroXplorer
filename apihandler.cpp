#include "apihandler.h"
#include <fstream>
#include <cstdio>

QUrl APIHandler::getAPOD_API_Request_URL(const std::string &APOD_URL, const std::string &API_KEY) {
    std::string api_url = APOD_URL + "?api_key=" + API_KEY;
    return {QString::fromStdString(api_url)};
}

QJsonObject APIHandler::parseJSON(QString data) {
    auto jsonDocument = QJsonDocument::fromJson(data.toUtf8());
    if (jsonDocument.isEmpty()) return {};

    return jsonDocument.object();
}

QJsonObject APIHandler::parseJSON(std::string data) {
    return parseJSON(QString::fromStdString(std::move(data)));
}

QJsonObject APIHandler::parseJSON(const QByteArray &data) {
    auto jsonDocument = QJsonDocument::fromJson(data);
    if (jsonDocument.isEmpty()) return {};

    return jsonDocument.object();
}

std::string APIHandler::dateToString(const QDate& date) {
    return std::to_string(date.year()) + "-" + std::to_string(date.month()) + "-" + std::to_string(date.day());
}

QUrl APIHandler::getMarsRoverImagerySols_API_Request_URL(std::string MARS_ROVER_URL,
                                                         std::string API_KEY,
                                                         ORIGIN rover,
                                                         ORIGIN camera,
                                                         std::string sol) {
    std::string api_url =
            std::move(MARS_ROVER_URL) + E::eToS(rover) + "/photos" + "?camera=" + E::eToS(camera) + "&sol=" + std::move(sol) + "&api_key=" +
            std::move(API_KEY);
    return {QString::fromStdString(std::move(api_url))};
}

QUrl APIHandler::getMarsRoverImagerySols_API_Request_URL(std::string MARS_ROVER_URL,
                                                         std::string API_KEY,
                                                         std::string rover,
                                                         std::string camera,
                                                         std::string sol) {
    std::string api_url =
            std::move(MARS_ROVER_URL) + std::move(rover) + "/photos" + "?camera=" + std::move(camera) + "&sol=" + std::move(sol) + "&api_key=" + std::move(API_KEY);
    return {QString::fromStdString(std::move(api_url))};
}

QUrl APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(std::string MARS_ROVER_URL,
                                                              std::string API_KEY,
                                                              ORIGIN rover,
                                                              ORIGIN camera,
                                                              std::string date) {
    std::string api_url =
            std::move(MARS_ROVER_URL) + E::eToS(rover) + "/photos" + "?camera=" + E::eToS(camera) + "&earth_date=" + std::move(date) +
            "&api_key=" + std::move(API_KEY);
    return {QString::fromStdString(std::move(api_url))};
}

QUrl APIHandler::getMarsRoverManifest_API_Request_URL(std::string MARS_ROVER_URL,
                                                      std::string API_KEY,
                                                      ORIGIN rover) {
    std::string api_url = std::move(MARS_ROVER_URL) + E::eToS(rover) + "?api_key=" + std::move(API_KEY);
    return {QString::fromStdString(std::move(api_url))};
}

QUrl APIHandler::getEPICJson_Request_URL(QString API_KEY,
                                         QString baseUrl,
                                         QDate date,
                                         QString type) {
    return {std::move(baseUrl) + std::move(type) + "/date/" + date.toString("yyyy-MM-dd") + "?api_key=" + std::move(API_KEY)};
}

QUrl APIHandler::getEPICImage_Request_URL(QString API_KEY,
                                          QString baseUrl,
                                          QDate date,
                                          QString type,
                                          QString imageFileName,
                                          QString extension) {
    auto url = std::move(baseUrl) + std::move(type) + "/" + date.toString("yyyy") + "/" + date.toString("MM") + "/" + date.toString("dd") + "/" + extension + "/" + std::move(imageFileName) + "." + extension + "?api_key=" + std::move(API_KEY);
    return {std::move(url)};
}

QUrl APIHandler::getEPICData_Request_URL(QString API_KEY,
                             QString baseUrl,
                             QString type) {
    auto url = std::move(baseUrl) + std::move(type) + "/all?api_key" + std::move(API_KEY);
    return {std::move(url)};
}

void APIHandler::clearXMLFile(QString fileName) {
    clearXMLFile(fileName.toStdString());
}

// Removes any breaks in XML file
void APIHandler::clearXMLFile(std::string fileName) {
    // TODO: Remove
    std::string temp = fileName + ".temp";
    std::ifstream stream (fileName);
    std::ofstream oStream (temp);
    char *c = new char[1];
    char *b = new char[1];
    bool detect = false;
    bool first = true;
    while (!stream.eof()) {
        stream.read(c, 1);
        if (c[0] == '\'') {
            if (b[0] != 'b') {
                stream.read(c, 1);
                stream.read(c, 1);
                if (c[0] == '\'') stream.read(c, 1);
            } else {
                stream.read(b, 1);
                stream.read(c, 1);
            }
        }
        if (!first)
            oStream << b[0];
        else
            first = !first;
        b[0] = c[0];
    }
    delete [] b;
    delete [] c;
    stream.close();
    oStream.close();
    std::remove(fileName.c_str());
    std::rename(temp.c_str(), fileName.c_str());
}
