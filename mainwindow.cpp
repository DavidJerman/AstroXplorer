#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "cfgloader.h"
#include "apihandler.h"
#include "imagemanipulation.h"

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
    fetchAPIData(APIHandler::getAPOD_API_Request_URL(APOD_URL, API_KEY), "apod_json");

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

    // TODO: Set max values for spinboxes etc.

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
    label->setScaledContents(true);
    label->show();
    label->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::fetchAPIData(QUrl url, QString origin) {
    updateStatus("Fetching data for " + origin + "...");

    QNetworkRequest request;
    request.setUrl(url);
    auto res = manager->get(request);
    res->setProperty("origin", origin);
}

void MainWindow::onRequestFinished(QNetworkReply *reply) {
    auto origin = reply->property("origin");
    // APOD
    if (origin == "apod_json") updateWelcomeData(reply);
    else if (origin == "apod_image") updateWelcomeImage(reply);

    // Rover imagery
    else if (origin == "C_FHAZ") C_FHAZ_SetImages(reply);
    else if (origin == "C_FHAZ_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->C_FHAZ_List);

    else if (origin == "C_RHAZ") C_RHAZ_SetImages(reply);
    else if (origin == "C_RHAZ_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->C_RHAZ_List);

    else if (origin == "C_MAST") C_MAST_SetImages(reply);
    else if (origin == "C_MAST_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->C_MAST_List);

    else if (origin == "C_CHEMCAM") C_CHEMCAM_SetImages(reply);
    else if (origin == "C_CHEMCAM_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->C_CHEMCAM_List);

    else if (origin == "C_MAHLI") C_MAHLI_SetImages(reply);
    else if (origin == "C_MAHLI_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->C_MAHLI_List);

    else if (origin == "C_MARDI") C_MARDI_SetImages(reply);
    else if (origin == "C_MARDI_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->C_MARDI_List);

    else if (origin == "C_NAVCAM") C_NAVCAM_SetImages(reply);
    else if (origin == "C_NAVCAM_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->C_NAVCAM_List);

    else if (origin == "O_FHAZ") C_FHAZ_SetImages(reply);
    else if (origin == "O_FHAZ_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->C_FHAZ_List);

    else if (origin == "O_RHAZ") O_RHAZ_SetImages(reply);
    else if (origin == "O_RHAZ_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->O_RHAZ_List);

    else if (origin == "O_NAVCAM") O_NAVCAM_SetImages(reply);
    else if (origin == "O_NAVCAM_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->O_NAVCAM_List);

    else if (origin == "O_PANCAM") O_PANCAM_SetImages(reply);
    else if (origin == "O_PANCAM_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->O_PANCAM_List);

    else if (origin == "O_MINITES") O_MINITES_SetImages(reply);
    else if (origin == "O_MINITES_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->O_MINITES_List);

    else if (origin == "S_FHAZ") S_FHAZ_SetImages(reply);
    else if (origin == "S_FHAZ_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->S_FHAZ_List);

    else if (origin == "S_RHAZ") S_RHAZ_SetImages(reply);
    else if (origin == "S_RHAZ_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->S_RHAZ_List);

    else if (origin == "S_NAVCAM") S_NAVCAM_SetImages(reply);
    else if (origin == "S_NAVCAM_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->S_NAVCAM_List);

    else if (origin == "S_PANCAM") S_PANCAM_SetImages(reply);
    else if (origin == "S_PANCAM_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->S_PANCAM_List);

    else if (origin == "S_MINITES") S_MINITES_SetImages(reply);
    else if (origin == "S_MINITES_Photo") MarsRoverCamera_AddImageToContainer(reply, ui->S_MINITES_List);
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

        fetchAPIData(QUrl(parsedData.find("url").value().toString()), "apod_image");
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
    delete ui;
    delete manager;
}

// Mars Rover Imagery
void MainWindow::MarsRoverCamera_SetImages(QNetworkReply* reply, QString origin) {
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
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label->setFixedHeight(SIZE);
    label->setFixedWidth(p.width() * (SIZE * 1.0 / p.width()));
    label->setMargin(5);

    // UI element property change
    item->setSizeHint(QSize(SIZE + 5, SIZE));
    list->addItem(item);
    list->setItemWidget(item, label);

    // Delete old data
    reply->deleteLater();

    // Update status
    updateStatus("Photo fetched!");
}

// Placeholder
void MainWindow::MarsRoverCamera_AddImageToContainer() {

}

// Rover-camera specific functions
void MainWindow::C_FHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "C_FHAZ_Photo");
}

void MainWindow::C_RHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "C_RHAZ_Photo");
}

void MainWindow::C_MAST_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "C_MAST_Photo");
}

void MainWindow::C_CHEMCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "C_CHEMCAM_Photo");
}

void MainWindow::C_MAHLI_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "C_MAHLI_Photo");
}

void MainWindow::C_MARDI_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "C_MARDI_Photo");
}

void MainWindow::C_NAVCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "C_NAVCAM_Photo");
}

void MainWindow::O_FHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "O_FHAZ_Photo");
}

void MainWindow::O_RHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "O_RHAZ_Photo");
}

void MainWindow::O_NAVCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "O_NAVCAM_Photo");
}

void MainWindow::O_PANCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "O_PANCAM_Photo");
}

void MainWindow::O_MINITES_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "O_MINITES_Photo");
}

void MainWindow::S_FHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "S_FHAZ_Photo");
}

void MainWindow::S_RHAZ_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "S_RHAZ_Photo");
}

void MainWindow::S_NAVCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "S_NAVCAM_Photo");
}

void MainWindow::S_PANCAM_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "S_PANCAM_Photo");
}

void MainWindow::S_MINITES_SetImages(QNetworkReply* reply)
{
    MarsRoverCamera_SetImages(reply, "S_MINITES_Photo");
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
                 "S_RHAZ");
}


void MainWindow::on_S_RHAZ_DATE_Button_clicked()
{

}


void MainWindow::on_S_PANCAM_SOLS_Button_clicked()
{
    ui->S_PANCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "spirit",
                                                                     "PANCAM",
                                                                     std::to_string(ui->S_PANCAM_Sols->value())),
                 "S_PANCAM");
}


void MainWindow::on_S_PANCAM_DATE_Button_clicked()
{

}


void MainWindow::on_S_NAVCAM_SOLS_Button_clicked()
{
    ui->S_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "spirit",
                                                                     "NAVCAM",
                                                                     std::to_string(ui->S_NAVCAM_Sols->value())),
                 "O_NAVCAM");
}


void MainWindow::on_S_NAVCAM_DATE_Button_clicked()
{

}


void MainWindow::on_S_MINITES_SOLS_Button_clicked()
{
    ui->S_MINITES_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "spirit",
                                                                     "MINITES",
                                                                     std::to_string(ui->O_MINITES_Sols->value())),
                 "O_MINITES");
}


void MainWindow::on_S_MINITES_DATE_Button_clicked()
{

}


void MainWindow::on_S_FHAZ_SOLS_Button_clicked()
{
    ui->S_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "spirit",
                                                                     "FHAZ",
                                                                     std::to_string(ui->S_FHAZ_Sols->value())),
                 "O_FHAZ");
}


void MainWindow::on_S_FHAZ_DATE_Button_clicked()
{

}


void MainWindow::on_O_RHAZ_SOLS_Button_clicked()
{
    ui->O_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "opportunity",
                                                                     "RHAZ",
                                                                     std::to_string(ui->O_RHAZ_Sols->value())),
                 "O_RHAZ");
}


void MainWindow::on_O_RHAZ_DATE_Button_clicked()
{

}

void MainWindow::on_O_PANCAM_SOLS_Button_clicked()
{
    ui->O_PANCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "opportunity",
                                                                     "PANCAM",
                                                                     std::to_string(ui->O_PANCAM_Sols->value())),
                 "O_PANCAM");
}


void MainWindow::on_O_PANCAM_DATE_Button_clicked()
{

}


void MainWindow::on_O_NAVCAM_SOLS_Button_clicked()
{
    ui->O_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "opportunity",
                                                                     "NAVCAM",
                                                                     std::to_string(ui->O_NAVCAM_Sols->value())),
                 "O_NAVCAM");
}


void MainWindow::on_O_NAVCAM_DATE_Button_clicked()
{

}


void MainWindow::on_O_MINITES_SOLS_Button_clicked()
{
    ui->O_MINITES_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "opportunity",
                                                                     "MINITEST",
                                                                     std::to_string(ui->O_MINITES_Sols->value())),
                 "O_MINITES");
}


void MainWindow::on_O_MINITES_DATE_Button_clicked()
{

}


void MainWindow::on_O_FHAZ_SOLS_Button_clicked()
{
    ui->O_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "opportunity",
                                                                     "FHAZ",
                                                                     std::to_string(ui->O_FHAZ_Sols->value())),
                 "O_FHAZ");
}


void MainWindow::on_O_FHAZ_DATE_Button_clicked()
{

}


void MainWindow::on_C_NAVCAM_SOLS_Button_clicked()
{
    ui->C_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "NAVCAM",
                                                                     std::to_string(ui->C_NAVCAM_Sols->value())),
                 "C_NAVCAM");
}


void MainWindow::on_C_NAVCAM_DATE_Button_clicked()
{

}


void MainWindow::on_C_MAST_SOLS_Button_clicked()
{
    ui->C_MAST_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "MAST",
                                                                     std::to_string(ui->C_MAST_Sols->value())),
                 "C_MAST");
}


void MainWindow::on_C_MAST_DATE_Button_clicked()
{

}


void MainWindow::on_C_MARDI_SOLS_Button_clicked()
{
    ui->C_MARDI_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "MARDI",
                                                                     std::to_string(ui->C_MARDI_Sols->value())),
                 "C_MARDI");
}


void MainWindow::on_C_MARDI_DATE_Button_clicked()
{

}


void MainWindow::on_C_MAHLI_SOLS_Button_clicked()
{
    ui->C_MAHLI_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "MAHLI",
                                                                     std::to_string(ui->C_MAHLI_Sols->value())),
                 "C_MAHLI");
}


void MainWindow::on_C_MAHLI_DATE_Button_clicked()
{

}


void MainWindow::on_C_FHAZ_SOLS_Button_clicked()
{
    ui->C_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "FHAZ",
                                                                     std::to_string(ui->C_FHAZ_Sols->value())),
                 "C_FHAZ");
}


void MainWindow::on_C_FHAZ_DATE_Button_clicked()
{

}


void MainWindow::on_C_CHEMCAM_SOLS_Button_clicked()
{
    ui->C_CHEMCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "CHEMCAM",
                                                                     std::to_string(ui->C_CHEMCAM_Sols->value())),
                 "C_CHEMCAM");
}


void MainWindow::on_C_CHEMCAM_DATE_Button_clicked()
{

}


void MainWindow::on_C_RHAZ_SOLS_Button_clicked()
{
    ui->C_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     "curiosity",
                                                                     "RHAZ",
                                                                     std::to_string(ui->C_RHAZ_Sols->value())),
                 "C_RHAZ");
}

void MainWindow::on_C_RHAZ_DATE_Button_clicked()
{

}
