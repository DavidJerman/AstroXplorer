#include "apihandler.h"

QUrl APIHandler::getAPOD_API_Request_URL(const std::string &APOD_URL, const std::string &API_KEY) {
    std::string api_url =  APOD_URL + "?api_key=" + API_KEY;
    return QUrl(QString::fromStdString(api_url));
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
