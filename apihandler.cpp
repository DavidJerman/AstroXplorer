#include "apihandler.h"

QUrl APIHandler::getAPOD_API_Request_URL(const std::string &APOD_URL, const std::string &API_KEY) {
    std::string api_url =  APOD_URL + "?api_key=" + API_KEY;
    return {QString::fromStdString(api_url)};
}

QJsonObject APIHandler::parseJSON(const QString data) {
    auto jsonDocument = QJsonDocument::fromJson(data.toUtf8());
    if (jsonDocument.isEmpty()) return {};

    return jsonDocument.object();
}

QJsonObject APIHandler::parseJSON(const std::string data) {
    return parseJSON(QString::fromStdString(data));
}

QJsonObject APIHandler::parseJSON(const QByteArray &data) {
    auto jsonDocument = QJsonDocument::fromJson(data);
    if (jsonDocument.isEmpty()) return {};

    return jsonDocument.object();
}

std::string APIHandler::dateToString(QDate date) {
    return std::to_string(date.year()) + "-" + std::to_string(date.month()) + "-" + std::to_string(date.day());
}

QUrl APIHandler::getMarsRoverImagerySols_API_Request_URL(const std::string MARS_ROVER_URL,
                                                         const std::string API_KEY,
                                                         const std::string rover,
                                                         const std::string camera,
                                                         const std::string sol)
{
    std::string api_url = MARS_ROVER_URL + rover + "/photos" + "?camera=" + camera + "&sol=" + sol + "&api_key=" + API_KEY;
    return {QString::fromStdString(api_url)};
}

QUrl APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(const std::string MARS_ROVER_URL,
                                                              const std::string API_KEY,
                                                              const std::string rover,
                                                              const std::string camera,
                                                              const std::string date)
{
    std::string api_url = MARS_ROVER_URL + rover + "/photos" + "?camera=" + camera + "&earth_date=" + date + "&api_key=" + API_KEY;
    return {QString::fromStdString(api_url)};
}

QUrl APIHandler::getMarsRoverManifest_API_Request_URL(const std::string MARS_ROVER_URL,
                                                      const std::string API_KEY,
                                                      const std::string rover) {
    std::string api_url = MARS_ROVER_URL + rover + "?api_key=" + API_KEY;
    return {QString::fromStdString(api_url)};
}
