#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "cfgloader.h"
#include "apihandler.h"
#include "imagemanipulation.h"
#include "enums.h"

typedef ORIGIN O;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , CORNER_RADIUS(25)
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

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestFinished(QNetworkReply*)));

    updateStatus("Fetching welcome image...");
    fetchAPIData(APIHandler::getAPOD_API_Request_URL(APOD_URL, API_KEY), O::APOD_JSON);

    // Certain UI properties
    ui->WelcomeImageLabel->setScaledContents(false);
    ui->WelcomeImageExplanationTextBrowser->setOpenLinks(true);
    ui->WelcomeImageExplanationTextBrowser->setOpenExternalLinks(true);

    // Center the checkboxes
    ui->FHAZ_Curiosity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->FHAZ_Opportunity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->FHAZ_Spirit->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->RHAZ_Curiosity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->RHAZ_Opportunity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->RHAZ_Spirit->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->MAST_Curiosity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->CHEMCAM_Curiosity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->MAHLI_Curiosity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->MARDI_Curiosity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->NAVCAM_Curiosity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->NAVCAM_Opportunity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->NAVCAM_Spirit->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->PANCAM_Opportunity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->PANCAM_Spirit->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->MINITES_Opportunity->setStyleSheet("margin-left:50%; margin-right:50%;");
    ui->MINITES_Spirit->setStyleSheet("margin-left:50%; margin-right:50%;");

    // Connect all lists
    QObject::connect(ui->C_FHAZ_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->C_RHAZ_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->C_MAST_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->C_CHEMCAM_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->C_MAHLI_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->C_MARDI_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->C_NAVCAM_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->O_FHAZ_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->O_RHAZ_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->O_NAVCAM_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->O_PANCAM_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->O_MINITES_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->S_FHAZ_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->S_RHAZ_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->S_NAVCAM_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->S_PANCAM_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));
    QObject::connect(ui->S_MINITES_List, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(imagePopUp(QListWidgetItem*)));

    // Mars rover imagery
    ui->O_FHAZ_List->setVerticalScrollMode(QListWidget::ScrollPerPixel);

    // Rover manifest data setup
    fetchAPIData(APIHandler::getMarsRoverManifest_API_Request_URL(config.find("mars_rover_url")->second,
                                                                  config.find("api_key")->second,
                                                                  "curiosity"),
                 O::CURIOSITY);
    fetchAPIData(APIHandler::getMarsRoverManifest_API_Request_URL(config.find("mars_rover_url")->second,
                                                                  config.find("api_key")->second,
                                                                  "opportunity"),
                 O::OPPORTUNITY);
    fetchAPIData(APIHandler::getMarsRoverManifest_API_Request_URL(config.find("mars_rover_url")->second,
                                                                  config.find("api_key")->second,
                                                                  "spirit"),
                 O::SPIRIT);

    // Other
}

void MainWindow::updateStatus(QString msg) {
    ui->StatusLabel->setText(msg);
}

void MainWindow::popUpDialog(QString msg) {
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

void MainWindow::imagePopUp(QListWidgetItem* item) {
    // Im just not sure, how to handle potential memory leaks here
    auto label = new QLabel();
    auto widget = dynamic_cast<QLabel*> (item->listWidget()->itemWidget(item));
    label->setPixmap(widget->pixmap());
    label->show();
    label->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::fetchAPIData(QUrl url, ORIGIN origin) {
    updateStatus("Fetching data for " + E::eToQs(origin) + "...");

    QNetworkRequest request;
    request.setUrl(url);
    auto res = manager->get(request);
    res->setProperty("origin", origin);
}

void MainWindow::onRequestFinished(QNetworkReply *reply) {
    auto origin = reply->property("origin").value<ORIGIN>();
    switch (origin) {
        // APOD
        case O::APOD_JSON: updateWelcomeData(reply); break;
        case O::APOD_IMAGE: updateWelcomeImage(reply); break;
        // Rover imagery
        case O::C_FHAZ: C_FHAZ_SetImages(reply); break;
        case O::C_FHAZ_P: MarsRoverCamera_AddImageToContainer(reply, ui->C_FHAZ_List); break;
        case O::C_RHAZ: C_RHAZ_SetImages(reply); break;
        case O::C_RHAZ_P: MarsRoverCamera_AddImageToContainer(reply, ui->C_RHAZ_List); break;
        case O::C_MAST: C_MAST_SetImages(reply); break;
        case O::C_MAST_P: MarsRoverCamera_AddImageToContainer(reply, ui->C_MAST_List); break;
        case O::C_CHEMCAM: C_CHEMCAM_SetImages(reply); break;
        case O::C_CHEMCAM_P: MarsRoverCamera_AddImageToContainer(reply, ui->C_CHEMCAM_List); break;
        case O::C_MAHLI: C_MAHLI_SetImages(reply); break;
        case O::C_MAHLI_P: MarsRoverCamera_AddImageToContainer(reply, ui->C_MAHLI_List); break;
        case O::C_MARDI: C_MARDI_SetImages(reply); break;
        case O::C_MARDI_P: MarsRoverCamera_AddImageToContainer(reply, ui->C_MARDI_List); break;
        case O::C_NAVCAM: C_NAVCAM_SetImages(reply); break;
        case O::C_NAVCAM_P: MarsRoverCamera_AddImageToContainer(reply, ui->C_NAVCAM_List); break;
        case O::O_FHAZ: C_FHAZ_SetImages(reply); break;
        case O::O_FHAZ_P: MarsRoverCamera_AddImageToContainer(reply, ui->C_FHAZ_List); break;
        case O::O_RHAZ: O_RHAZ_SetImages(reply); break;
        case O::O_RHAZ_P: MarsRoverCamera_AddImageToContainer(reply, ui->O_RHAZ_List); break;
        case O::O_NAVCAM: O_NAVCAM_SetImages(reply); break;
        case O::O_NAVCAM_P: MarsRoverCamera_AddImageToContainer(reply, ui->O_NAVCAM_List); break;
        case O::O_PANCAM: O_PANCAM_SetImages(reply); break;
        case O::O_PANCAM_P: MarsRoverCamera_AddImageToContainer(reply, ui->O_PANCAM_List); break;
        case O::O_MINITES: O_MINITES_SetImages(reply); break;
        case O::O_MINITES_P: MarsRoverCamera_AddImageToContainer(reply, ui->O_MINITES_List); break;
        case O::S_FHAZ: S_FHAZ_SetImages(reply); break;
        case O::S_FHAZ_P: MarsRoverCamera_AddImageToContainer(reply, ui->S_FHAZ_List); break;
        case O::S_RHAZ: S_RHAZ_SetImages(reply); break;
        case O::S_RHAZ_P: MarsRoverCamera_AddImageToContainer(reply, ui->S_RHAZ_List); break;
        case O::S_NAVCAM: S_NAVCAM_SetImages(reply); break;
        case O::S_NAVCAM_P: MarsRoverCamera_AddImageToContainer(reply, ui->S_NAVCAM_List); break;
        case O::S_PANCAM: S_PANCAM_SetImages(reply); break;
        case O::S_PANCAM_P: MarsRoverCamera_AddImageToContainer(reply, ui->S_PANCAM_List); break;
        case O::S_MINITES: S_MINITES_SetImages(reply); break;
        case O::S_MINITES_P: MarsRoverCamera_AddImageToContainer(reply, ui->S_MINITES_List); break;
        // Rover manifest
        case O::CURIOSITY: updateRoverManifest(reply, ui->C_RoverManifestList, O::CURIOSITY, ui->C_RoverImageLabel); break;
        case O::OPPORTUNITY: updateRoverManifest(reply, ui->O_RoverManifestList, O::OPPORTUNITY, ui->O_RoverImageLabel); break;
        case O::SPIRIT: updateRoverManifest(reply, ui->S_RoverManifestList, O::SPIRIT, ui->S_RoverImageLabel); break;
        default: reply->deleteLater();
    }
}

void MainWindow::updateWelcomeData(QNetworkReply* reply) {

    updateStatus("Fetching image of the day...");

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

        fetchAPIData(QUrl(parsedData.find("url").value().toString()), O::APOD_IMAGE);
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

    reply->deleteLater();
}

void MainWindow::updateWelcomeImage(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    auto answer = reply->readAll();
    if (answer.isEmpty()) {
        updateStatus("Could not obtain APOD image!");
        return;
    }

    // Saves the file
    auto filePath = config.find("welcome_image_path")->second;

    QFile file;
    file.setFileName(QString::fromStdString(filePath));
    file.open(QIODevice::WriteOnly);

    file.write(answer);
    file.close();

    // Sets the image to label
    QPixmap pic(QString::fromStdString(filePath));
    ImageManipulation::roundEdges(pic, CORNER_RADIUS);
    ui->WelcomeImageLabel->setPixmap(pic);

    // Delete old data
    reply->deleteLater();

    updateStatus("Welcome image fetched!");
}

void MainWindow::updateWelcomeVideo(const QUrl &videoUrl) {
    // Since WebView is deprecated, this will just load the latest image
    // Sets the image to label
    auto filePath = config.find("welcome_image_path")->second;

    QPixmap pic(QString::fromStdString(filePath));
    ImageManipulation::roundEdges(pic, CORNER_RADIUS);
    ui->WelcomeImageLabel->setPixmap(pic);
    return;
}

void MainWindow::setWelcomeImageInformation(QJsonObject &jsonObj) {
    // Sets all the information provided by the APOD API
    auto title = jsonObj.find("title")->toString(),
         explanation = jsonObj.find("explanation")->toString(),
         date = jsonObj.find("date")->toString();

    ui->WelcomeImageTitleTextEdit->setText(title);
    ui->WelcomeImageExplanationTextBrowser->append(explanation);
    ui->WelcomeImageDateTextEdit->setText(date);
}

// On window resize, change image aspect ration - TODO
void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
}

MainWindow::~MainWindow()
{
    ui->C_RoverManifestList->clear();
    ui->O_RoverManifestList->clear();
    ui->S_RoverManifestList->clear();
    delete ui;
    delete manager;
}

// Mars Rover Imagery
void MainWindow::MarsRoverCamera_SetImages(QNetworkReply* reply, ORIGIN origin) {
    auto answer = reply->readAll();
    auto parsedData = APIHandler::parseJSON(answer);
    if (parsedData.isEmpty()) {
        popUpDialog("Failed to get any data from the API");
        updateStatus("Failed to get any data from the API!");
    }
    auto photos = parsedData.find("photos")->toArray();
    if (photos.isEmpty()) {
        popUpDialog("No photos were found for this query");
        updateStatus("No photos were found for this query");
    }
    else {
        updateStatus("Fetching images...");

        for (const auto &photo: photos)
            fetchAPIData(photo.toObject().value("img_src").toString(), origin);
    }

    reply->deleteLater();
}

void MainWindow::MarsRoverCamera_AddImageToContainer(QNetworkReply* reply, QListWidget* list) {
    auto answer = reply->readAll();
    auto item = new QListWidgetItem("");
    auto label = new QLabel();
    QPixmap p;
    p.loadFromData(answer);
    label->setPixmap(p);

    constexpr int SIZE = 240;

    // Set size
    label->setScaledContents(true);
    label->setFixedHeight(SIZE);
    label->setFixedWidth(p.width() * ((double)SIZE / p.height()));
    label->setMargin(5);

    item->setSizeHint(QSize(p.width() * ((double)SIZE / p.height()), SIZE));
    list->addItem(item);
    list->setItemWidget(item, label);

    reply->deleteLater();

    // Update status
    updateStatus("Photo fetched!");
}

// Rover-camera specific functions
void MainWindow::C_FHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::C_FHAZ_P);
}

void MainWindow::C_RHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::C_RHAZ_P);
}

void MainWindow::C_MAST_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::C_MAST_P);
}

void MainWindow::C_CHEMCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::C_CHEMCAM_P);
}

void MainWindow::C_MAHLI_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::C_MAHLI_P);
}

void MainWindow::C_MARDI_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::C_MARDI_P);
}

void MainWindow::C_NAVCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::C_NAVCAM_P);
}

void MainWindow::O_FHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::O_FHAZ_P);
}

void MainWindow::O_RHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::O_RHAZ_P);
}

void MainWindow::O_NAVCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::O_NAVCAM_P);
}

void MainWindow::O_PANCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::O_PANCAM_P);
}

void MainWindow::O_MINITES_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::O_MINITES_P);
}

void MainWindow::S_FHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::S_FHAZ_P);
}

void MainWindow::S_RHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::S_RHAZ_P);
}

void MainWindow::S_NAVCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::S_NAVCAM_P);
}

void MainWindow::S_PANCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::S_PANCAM_P);
}

void MainWindow::S_MINITES_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, O::S_MINITES_P);
}


// Rover camera on click events
void MainWindow::on_S_RHAZ_SOLS_Button_clicked()
{
    ui->S_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "spirit",
                                                                     "RHAZ",
                                                                     std::to_string(ui->S_RHAZ_Sols->value())),
                 O::S_RHAZ);
}


void MainWindow::on_S_RHAZ_DATE_Button_clicked()
{
    ui->S_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "spirit",
                                                                          "RHAZ",
                                                                          APIHandler::dateToString(ui->S_RHAZ_Date->date())),
                 O::S_RHAZ);
}


void MainWindow::on_S_PANCAM_SOLS_Button_clicked()
{
    ui->S_PANCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "spirit",
                                                                     "PANCAM",
                                                                     std::to_string(ui->S_PANCAM_Sols->value())),
                 O::S_PANCAM);
}


void MainWindow::on_S_PANCAM_DATE_Button_clicked()
{
    ui->S_PANCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "spirit",
                                                                          "PANCAM",
                                                                          APIHandler::dateToString(ui->S_PANCAM_Date->date())),
                 O::S_PANCAM);
}


void MainWindow::on_S_NAVCAM_SOLS_Button_clicked()
{
    ui->S_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "spirit",
                                                                     "NAVCAM",
                                                                     std::to_string(ui->S_NAVCAM_Sols->value())),
                 O::S_NAVCAM);
}


void MainWindow::on_S_NAVCAM_DATE_Button_clicked()
{
    ui->S_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "spirit",
                                                                          "NAVCAM",
                                                                          APIHandler::dateToString(ui->S_NAVCAM_Date->date())),
                 O::S_NAVCAM);
}


void MainWindow::on_S_MINITES_SOLS_Button_clicked()
{
    ui->S_MINITES_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "spirit",
                                                                     "MINITES",
                                                                     std::to_string(ui->O_MINITES_Sols->value())),
                 O::S_MINITES);
}


void MainWindow::on_S_MINITES_DATE_Button_clicked()
{
    ui->S_MINITES_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "spirit",
                                                                          "MINITES",
                                                                          APIHandler::dateToString(ui->S_MINITES_Date->date())),
                 O::S_MINITES);
}


void MainWindow::on_S_FHAZ_SOLS_Button_clicked()
{
    ui->S_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "spirit",
                                                                     "FHAZ",
                                                                     std::to_string(ui->S_FHAZ_Sols->value())),
                 O::S_FHAZ);
}


void MainWindow::on_S_FHAZ_DATE_Button_clicked()
{
    ui->S_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "spirit",
                                                                          "FHAZ",
                                                                          APIHandler::dateToString(ui->S_FHAZ_Date->date())),
                 O::S_FHAZ);
}


void MainWindow::on_O_RHAZ_SOLS_Button_clicked()
{
    ui->O_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "opportunity",
                                                                     "RHAZ",
                                                                     std::to_string(ui->O_RHAZ_Sols->value())),
                 O::O_RHAZ);
}


void MainWindow::on_O_RHAZ_DATE_Button_clicked()
{
    ui->O_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "opportunity",
                                                                          "RHAZ",
                                                                          APIHandler::dateToString(ui->O_RHAZ_Date->date())),
                 O::O_RHAZ);
}

void MainWindow::on_O_PANCAM_SOLS_Button_clicked()
{
    ui->O_PANCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "opportunity",
                                                                     "PANCAM",
                                                                     std::to_string(ui->O_PANCAM_Sols->value())),
                 O::O_PANCAM);
}


void MainWindow::on_O_PANCAM_DATE_Button_clicked()
{
    ui->O_PANCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "opportunity",
                                                                          "PANCAM",
                                                                          APIHandler::dateToString(ui->O_PANCAM_Date->date())),
                 O::O_PANCAM);
}


void MainWindow::on_O_NAVCAM_SOLS_Button_clicked()
{
    ui->O_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "opportunity",
                                                                     "NAVCAM",
                                                                     std::to_string(ui->O_NAVCAM_Sols->value())),
                 O::O_NAVCAM);
}


void MainWindow::on_O_NAVCAM_DATE_Button_clicked()
{
    ui->O_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "opportunity",
                                                                          "NAVCAM",
                                                                          APIHandler::dateToString(ui->O_NAVCAM_Date->date())),
                 O::O_NAVCAM);
}


void MainWindow::on_O_MINITES_SOLS_Button_clicked()
{
    ui->O_MINITES_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "opportunity",
                                                                     "MINITEST",
                                                                     std::to_string(ui->O_MINITES_Sols->value())),
                 O::O_MINITES);
}


void MainWindow::on_O_MINITES_DATE_Button_clicked()
{
    ui->O_MINITES_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "opportunity",
                                                                          "MINITES",
                                                                          APIHandler::dateToString(ui->O_MINITES_Date->date())),
                 O::O_MINITES);
}


void MainWindow::on_O_FHAZ_SOLS_Button_clicked()
{
    ui->O_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "opportunity",
                                                                     "FHAZ",
                                                                     std::to_string(ui->O_FHAZ_Sols->value())),
                 O::O_FHAZ);
}


void MainWindow::on_O_FHAZ_DATE_Button_clicked()
{
    ui->O_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "opportunity",
                                                                          "FHAZ",
                                                                          APIHandler::dateToString(ui->O_FHAZ_Date->date())),
                 O::O_FHAZ);
}


void MainWindow::on_C_NAVCAM_SOLS_Button_clicked()
{
    ui->C_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "NAVCAM",
                                                                     std::to_string(ui->C_NAVCAM_Sols->value())),
                 O::C_NAVCAM);
}


void MainWindow::on_C_NAVCAM_DATE_Button_clicked()
{
    ui->C_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "curiosity",
                                                                          "NAVCAM",
                                                                          APIHandler::dateToString(ui->C_NAVCAM_Date->date())),
                 O::C_NAVCAM);
}


void MainWindow::on_C_MAST_SOLS_Button_clicked()
{
    ui->C_MAST_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "MAST",
                                                                     std::to_string(ui->C_MAST_Sols->value())),
                 O::C_MAST);
}


void MainWindow::on_C_MAST_DATE_Button_clicked()
{
    ui->C_MAST_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "curiosity",
                                                                          "MAST",
                                                                          APIHandler::dateToString(ui->C_MAST_Date->date())),
                 O::C_MAST);
}


void MainWindow::on_C_MARDI_SOLS_Button_clicked()
{
    ui->C_MARDI_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "MARDI",
                                                                     std::to_string(ui->C_MARDI_Sols->value())),
                 O::C_MARDI);
}


void MainWindow::on_C_MARDI_DATE_Button_clicked()
{
    ui->C_MARDI_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "curiosity",
                                                                          "MARDI",
                                                                          APIHandler::dateToString(ui->C_MARDI_Date->date())),
                 O::C_MARDI);
}


void MainWindow::on_C_MAHLI_SOLS_Button_clicked()
{
    ui->C_MAHLI_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "MAHLI",
                                                                     std::to_string(ui->C_MAHLI_Sols->value())),
                 O::C_MAHLI);
}


void MainWindow::on_C_MAHLI_DATE_Button_clicked()
{
    ui->C_MAHLI_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "curiosity",
                                                                          "MAHLI",
                                                                          APIHandler::dateToString(ui->C_MAHLI_Date->date())),
                 O::C_MAHLI);
}


void MainWindow::on_C_FHAZ_SOLS_Button_clicked()
{
    ui->C_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "FHAZ",
                                                                     std::to_string(ui->C_FHAZ_Sols->value())),
                 O::C_FHAZ);
}


void MainWindow::on_C_FHAZ_DATE_Button_clicked()
{
    ui->C_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "curiosity",
                                                                          "FHAZ",
                                                                          APIHandler::dateToString(ui->C_FHAZ_Date->date())),
                 O::C_FHAZ);
}


void MainWindow::on_C_CHEMCAM_SOLS_Button_clicked()
{
    ui->C_CHEMCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "CHEMCAM",
                                                                     std::to_string(ui->C_CHEMCAM_Sols->value())),
                 O::C_CHEMCAM);
}


void MainWindow::on_C_CHEMCAM_DATE_Button_clicked()
{
    ui->C_CHEMCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "curiosity",
                                                                          "CHEMCAM",
                                                                          APIHandler::dateToString(ui->C_CHEMCAM_Date->date())),
                 O::C_CHEMCAM);
}


void MainWindow::on_C_RHAZ_SOLS_Button_clicked()
{
    ui->C_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "RHAZ",
                                                                     std::to_string(ui->C_RHAZ_Sols->value())),
                 O::C_RHAZ);
}

void MainWindow::on_C_RHAZ_DATE_Button_clicked()
{
    ui->C_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          "curiosity",
                                                                          "RHAZ",
                                                                          APIHandler::dateToString(ui->C_RHAZ_Date->date())),
                 O::C_RHAZ);
}

void MainWindow::updateRoverManifest(QNetworkReply* reply, QListWidget* list, ORIGIN origin, QLabel* imageLabel) {
    // Rover image
    const int MAX_SIZE = 550;
    std::string filePath = config.find("mars_rover_images_path")->second + E::eToS(origin) + ".jpg";
    QPixmap pic(QString::fromStdString(filePath));
    imageLabel->setPixmap(pic);

    imageLabel->setScaledContents(true);
    imageLabel->setMaximumHeight(MAX_SIZE);
    imageLabel->setMaximumWidth(MAX_SIZE);

    // Manifest data
    auto parsedData = APIHandler::parseJSON(reply->readAll());
    auto parsedObj = parsedData.value("rover").toObject();

    for (const auto& key: parsedObj.keys()) {
        auto item = parsedObj.value(key);
        // auto _size = item.size();
        if (item.isArray()) {

            auto k = key.toStdString();
            if (k != "cameras") break;
            for (auto &c: k) c = toupper(c);

            QListWidgetItem* i = new QListWidgetItem("");
            i->setText(QString::fromStdString(k));
            list->addItem(i);

            for (const auto j: item.toArray()) {
                auto obj = j.toObject();
                auto v = obj.value("full_name").toString().toStdString();;
                i = new QListWidgetItem();
                i->setText(QString::fromStdString("\t" + v));
                list->addItem(i);
            }

        } else {

            QListWidgetItem* i = new QListWidgetItem("");
            auto k = key.toStdString();
            std::string v;
            if (item.isString()) v = item.toString().toStdString();
            else v = std::to_string(item.toInt());
            for (auto & c: k) c = toupper(c);
            i->setText(QString::fromStdString(k + ": " + v));
            list->addItem(i);
        }
    }

    reply->deleteLater();
}
