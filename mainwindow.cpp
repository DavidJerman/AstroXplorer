#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "cfgloader.h"
#include "apihandler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    // Certain UI properties
    ui->WelcomeImageLabel->setScaledContents(false);
    ui->WelcomeImageExplanationTextBrowser->setOpenLinks(true);
    ui->WelcomeImageExplanationTextBrowser->setOpenExternalLinks(true);
}

void MainWindow::updateWelcomeData(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    auto answer = reply->readAll();

    auto parsedData = APIHandler::parseJSON(answer);

    // This assumes the API always works correctly

    ui->WelcomeImageExplanationTextBrowser->clear();

    // Image
    if (parsedData.find("media_type").value().toString() == "image") {
        // Save json for later use
        auto filePath = config.find("welcome_image_metadata")->second;

        QFile file;
        file.setFileName(QString::fromStdString(filePath));
        file.open(QIODevice::WriteOnly);

        file.write(answer);
        file.close();

        // Bind signal to slot
        QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateWelcomeImage(QNetworkReply*)));

        QNetworkRequest request;
        request.setUrl(QUrl(parsedData.find("url").value().toString()));
        manager->get(request);
    }
    // Video
    if (parsedData.find("media_type").value().toString() == "video") {
        updateWelcomeVideo(QUrl(parsedData.find("url")->toString()));

        auto filePath = config.find("welcome_image_metadata")->second;

        QFile file;
        file.setFileName(QString::fromStdString(filePath));
        file.open(QIODevice::ReadOnly);

        auto jsonData = file.readAll();

        parsedData = APIHandler::parseJSON(jsonData);

        ui->WelcomeImageExplanationTextBrowser->append(QString::fromStdString(std::string("A video of the day was found, but cannot be displayed. Here is the link to it: ")
                                                                              + "<a href=\""
                                                                              + parsedData.find("url")->toString().toStdString()
                                                                              + "\">YouTube</a><br>"));

    }

    setWelcomeImageInformation(parsedData);
}

void MainWindow::updateWelcomeImage(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    auto answer = reply->readAll();

    // Saves the file
    auto filePath = config.find("welcome_image_path")->second;

    QFile file;
    file.setFileName(QString::fromStdString(filePath));
    file.open(QIODevice::WriteOnly);

    file.write(answer);
    file.close();

    // Sets the image to label
    QPixmap pic(QString::fromStdString(filePath));
    ui->WelcomeImageLabel->setPixmap(pic);
}

void MainWindow::updateWelcomeVideo(const QUrl &videoUrl) {
    // Since WebView is deprecated, this will just load the latest image
    // Sets the image to label
    auto filePath = config.find("welcome_image_path")->second;

    QPixmap pic(QString::fromStdString(filePath));
    ui->WelcomeImageLabel->setPixmap(pic);
    return;
}

void MainWindow::setWelcomeImageInformation(QJsonObject &jsonObj) {
    // Sets all the information provided by the APOD API
    auto title = jsonObj.find("title")->toString(),
         explanation = jsonObj.find("explanation")->toString(),
         date = jsonObj.find("date")->toString();

    ui->WelcomeImageTitleLabel->setText(title);
    ui->WelcomeImageExplanationTextBrowser->append(explanation);
    ui->WelcomeImageDateLabel->setText(date);
}

// On window resize, change image aspect ration - TODO
void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
}
