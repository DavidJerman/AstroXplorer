#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "cfgloader.h"
#include "apihandler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Web setup
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);

    // Load config
    auto response = CfgLoader::loadConfig("config");
    if (!response)
        return;
    this->config = CfgLoader::getConfig();

    // This part handles requests
    const auto _API_KEY = config.find("api_key");
    if (_API_KEY == config.end()) return;
    const auto API_KEY = _API_KEY->second;

    const auto _APOD_URL = config.find("apod_url");
    if (_APOD_URL == config.end()) return;
    const auto APOD_URL = _APOD_URL->second;

    manager = new QNetworkAccessManager(this);

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateWelcomeData(QNetworkReply*)));

    QNetworkRequest request;
    request.setUrl(APIHandler::getAPOD_API_Request_URL(APOD_URL, API_KEY));
    manager->get(request);
}

void MainWindow::updateWelcomeData(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString answer = reply->readAll();

    auto parsedData = APIHandler::parseJSON(answer);

    // This assumes the API always works correctly
    // Image
    if (parsedData.find("media_type").value().toString() == "image") {
        QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateWelcomeImage(QNetworkReply*)));

        QNetworkRequest request;
        request.setUrl(QUrl(parsedData.find("url").value().toString()));
        manager->get(request);
    }
    // Video
    if (parsedData.find("media_type").value().toString() == "video") {
        updateWelcomeVideo(QUrl(parsedData.find("url")->toString()));
    }
}

void MainWindow::updateWelcomeImage(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    auto answer = reply->readAll();

    qDebug() << answer;
}

void MainWindow::updateWelcomeVideo(const QUrl &videoUrl) {

    return;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
}

