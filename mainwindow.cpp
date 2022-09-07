#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "cfgloader.h"
#include "apihandler.h"
#include "imagemanipulation.h"
#include "enums.h"
#include "podcasts.h"
#include "epic.h"

#include <sstream>
#include <QFontDatabase>

#include <QApplication>
#include <QCheckBox>

typedef ORIGIN O;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), CORNER_RADIUS(25) {
    ui->setupUi(this);

    // Media
    mediaPlayer = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    mediaPlayer->setAudioOutput(audioOutput);\
    audioOutput->setVolume(100);

    QObject::connect(mediaPlayer, SIGNAL(durationChanged(qint64)),
                     this, SLOT(onDurationChanged(qint64)));

    QObject::connect(mediaPlayer, SIGNAL(positionChanged(qint64)),
                     this, SLOT(onPositionChanged(qint64)));

    QObject::connect(mediaPlayer, SIGNAL(playbackStateChanged(QMediaPlayer::PlaybackState)),
                     this, SLOT(onPlaybackStateChanged(QMediaPlayer::PlaybackState)));

    // Load config
    auto response = CfgLoader::loadConfig("config");
    if (!response)
        return;
    this->config = CfgLoader::getConfig();

    response = CfgLoader::loadConfig("fontCfg");
    if (!response)
        return;
    this->fontCfg = CfgLoader::getConfig();

    // Load podcasts
    updateStatus("Loading podcasts...");
    Podcasts::loadPodcastsFromSourceFolder(QString::fromStdString(config.find("podcast_sources_path")->second));
    updatePodcastsList();
    Podcasts::loadFavEpisodes(QString::fromStdString(config.find("podcast_fav_episode_path")->second));

    // This part handles requests
    const auto _API_KEY = config.find("api_key");
    if (_API_KEY == config.end()) return;
    const auto API_KEY = _API_KEY->second;

    const auto _APOD_URL = config.find("apod_url");
    if (_APOD_URL == config.end()) return;
    const auto APOD_URL = _APOD_URL->second;

    manager = new QNetworkAccessManager(this);
    QObject::connect(manager, SIGNAL(finished(QNetworkReply * )), this, SLOT(onRequestFinished(QNetworkReply * )));

    updateStatus("Fetching welcome image...");
    fetchAPIData(APIHandler::getAPOD_API_Request_URL(APOD_URL, API_KEY), O::APOD_JSON);

    // EPIC
    on_EPICImageTypeComboBox_currentIndexChanged(0);
    EPIC::reset();

    // AutoPlay Timer
    timer = new QTimer(this);
    timer->setInterval(400);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(on_EPICTimerTimeout()));
    timer->start();

    // Certain UI properties
    ui->WelcomeImageLabel->setScaledContents(false);
    ui->EPICImageLabel->setScaledContents(true);
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
    QObject::connect(ui->C_FHAZ_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->C_RHAZ_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->C_MAST_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->C_CHEMCAM_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->C_MAHLI_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->C_MARDI_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->C_NAVCAM_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->O_FHAZ_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->O_RHAZ_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->O_NAVCAM_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->O_PANCAM_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->O_MINITES_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->S_FHAZ_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->S_RHAZ_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->S_NAVCAM_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->S_PANCAM_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));
    QObject::connect(ui->S_MINITES_List, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(imagePopUp(QListWidgetItem * )));

    // Mars rover imagery
    ui->O_FHAZ_List->setVerticalScrollMode(QListWidget::ScrollPerPixel);

    // Rover manifest data setup
    fetchAPIData(APIHandler::getMarsRoverManifest_API_Request_URL(config.find("mars_rover_url")->second,
                                                                  config.find("api_key")->second,
                                                                  O::CURIOSITY),
                 O::CURIOSITY);
    fetchAPIData(APIHandler::getMarsRoverManifest_API_Request_URL(config.find("mars_rover_url")->second,
                                                                  config.find("api_key")->second,
                                                                  O::OPPORTUNITY),
                 O::OPPORTUNITY);
    fetchAPIData(APIHandler::getMarsRoverManifest_API_Request_URL(config.find("mars_rover_url")->second,
                                                                  config.find("api_key")->second,
                                                                  O::SPIRIT),
                 O::SPIRIT);

    // Welcome image scaling
    ui->WelcomeImageLabel->setScaledContents(true);

    // Podcasts
    QObject::connect(ui->PodcastSelectorList, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(populateEpisodesList(QListWidgetItem * )));
    QObject::connect(ui->EpisodeSelectorList, SIGNAL(itemClicked(QListWidgetItem * )),
                     this, SLOT(playEpisode(QListWidgetItem * )));
    ui->EpisodeDateLabel->setMargin(5);
    ui->EpisodeSelectorList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->PodcastSelectorList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

const QString MainWindow::getCfgValueQ(const std::string key) const {
    return QString::fromStdString(config.find(key)->second);
}

const std::string MainWindow::getCfgValue(const std::string key) const {
    return config.find(key)->second;
}

const QString MainWindow::getFontQ(const std::string key) const {
    return QString::fromStdString(fontCfg.find(key)->second);
}

const std::string MainWindow::getFont(const std::string key) const {
    return fontCfg.find(key)->second;
}

const unsigned int MainWindow::getFSize(const std::string key) const {
    return std::stoi(fontCfg.find(key)->second);
}

void MainWindow::updateStatus(QString msg) {
    ui->StatusLabel->setText(msg);
}

void MainWindow::popUpDialog(QString msg) {
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

void MainWindow::imagePopUp(QListWidgetItem *item) {
    // Im just not sure, how to handle potential memory leaks here
    auto label = new QLabel();
    auto widget = dynamic_cast<QLabel *> (item->listWidget()->itemWidget(item));
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
    res->setProperty("data_source", "normal");
}

void MainWindow::fetchPodcastData(QUrl url, QString origin, QLabel *imageLabel, unsigned int SIZE) {
    updateStatus("Fetching data for " + origin + "...");

    QNetworkRequest request;
    request.setUrl(url);
    auto res = manager->get(request);
    res->setProperty("origin", origin);
    res->setProperty("data_source", "podcast");
    res->setProperty("image_label", QVariant::fromValue(imageLabel));
    res->setProperty("size", QVariant::fromValue(SIZE));
}

void MainWindow::fetchImages(QUrl url, QString origin, int sol, QString rover, QString camera) {
    updateStatus("Downloading data for " + origin + "...");

    QNetworkRequest request;
    request.setUrl(url);
    auto res = manager->get(request);
    res->setProperty("origin", origin);
    res->setProperty("data_source", "download");
    res->setProperty("sol", sol);
    res->setProperty("rover", rover);
    res->setProperty("camera", camera);
}

void MainWindow::fetchImage(QUrl url, QString filePath) {
    QNetworkRequest request;
    request.setUrl(url);
    auto res = manager->get(request);
    res->setProperty("data_source", "image_download");
    res->setProperty("file_path", filePath);
}

void MainWindow::fetchEPICJson(QUrl url, ORIGIN origin, QString type) {
    updateStatus("Downloading data for " + E::eToQs(origin) + "...");
    QNetworkRequest request;
    request.setUrl(url);
    auto res = manager->get(request);
    res->setProperty("origin", origin);
    res->setProperty("data_source", "epic");
    res->setProperty("type", type);
}

void MainWindow::fetchEPICImage(QUrl url, ORIGIN origin, QString title, QString date, QString caption, QString version, unsigned int count,
                                double lat, double lon,
                                double dscovrX, double dscovrY, double dscovrZ,
                                double lunarX, double lunarY, double lunarZ,
                                double sunX, double sunY, double sunZ) {
    updateStatus("Fetching data for " + E::eToQs(origin) + "...");
    QNetworkRequest request;
    request.setUrl(url);
    auto res = manager->get(request);
    res->setProperty("origin", origin);
    res->setProperty("data_source", "epic");
    res->setProperty("title", title);
    res->setProperty("date", date);
    res->setProperty("caption", caption);
    res->setProperty("version", version);
    res->setProperty("count", count);
    res->setProperty("lat", lat);
    res->setProperty("lon", lon);
    res->setProperty("dscovrX", dscovrX);
    res->setProperty("dscovrY", dscovrY);
    res->setProperty("dscovrZ", dscovrZ);
    res->setProperty("lunarX", lunarX);
    res->setProperty("lunarY", lunarY);
    res->setProperty("lunarZ", lunarZ);
    res->setProperty("sunX", sunX);
    res->setProperty("sunY", sunY);
    res->setProperty("sunZ", sunZ);
}

void MainWindow::onRequestFinished(QNetworkReply *reply) {
    auto dataSource = reply->property("data_source").value<QString>();
    if (dataSource == "normal") {
        auto origin = reply->property("origin").value<ORIGIN>();
        switch (origin) {
            // APOD
            case O::APOD_JSON:
                updateWelcomeData(reply);
                break;
            case O::APOD_IMAGE:
                updateWelcomeImage(reply);
                break;
                // Rover imagery
            case O::C_FHAZ:
                C_FHAZ_SetImages(reply);
                break;
            case O::C_FHAZ_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->C_FHAZ_List);
                break;
            case O::C_RHAZ:
                C_RHAZ_SetImages(reply);
                break;
            case O::C_RHAZ_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->C_RHAZ_List);
                break;
            case O::C_MAST:
                C_MAST_SetImages(reply);
                break;
            case O::C_MAST_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->C_MAST_List);
                break;
            case O::C_CHEMCAM:
                C_CHEMCAM_SetImages(reply);
                break;
            case O::C_CHEMCAM_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->C_CHEMCAM_List);
                break;
            case O::C_MAHLI:
                C_MAHLI_SetImages(reply);
                break;
            case O::C_MAHLI_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->C_MAHLI_List);
                break;
            case O::C_MARDI:
                C_MARDI_SetImages(reply);
                break;
            case O::C_MARDI_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->C_MARDI_List);
                break;
            case O::C_NAVCAM:
                C_NAVCAM_SetImages(reply);
                break;
            case O::C_NAVCAM_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->C_NAVCAM_List);
                break;
            case O::O_FHAZ:
                C_FHAZ_SetImages(reply);
                break;
            case O::O_FHAZ_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->C_FHAZ_List);
                break;
            case O::O_RHAZ:
                O_RHAZ_SetImages(reply);
                break;
            case O::O_RHAZ_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->O_RHAZ_List);
                break;
            case O::O_NAVCAM:
                O_NAVCAM_SetImages(reply);
                break;
            case O::O_NAVCAM_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->O_NAVCAM_List);
                break;
            case O::O_PANCAM:
                O_PANCAM_SetImages(reply);
                break;
            case O::O_PANCAM_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->O_PANCAM_List);
                break;
            case O::O_MINITES:
                O_MINITES_SetImages(reply);
                break;
            case O::O_MINITES_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->O_MINITES_List);
                break;
            case O::S_FHAZ:
                S_FHAZ_SetImages(reply);
                break;
            case O::S_FHAZ_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->S_FHAZ_List);
                break;
            case O::S_RHAZ:
                S_RHAZ_SetImages(reply);
                break;
            case O::S_RHAZ_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->S_RHAZ_List);
                break;
            case O::S_NAVCAM:
                S_NAVCAM_SetImages(reply);
                break;
            case O::S_NAVCAM_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->S_NAVCAM_List);
                break;
            case O::S_PANCAM:
                S_PANCAM_SetImages(reply);
                break;
            case O::S_PANCAM_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->S_PANCAM_List);
                break;
            case O::S_MINITES:
                S_MINITES_SetImages(reply);
                break;
            case O::S_MINITES_P:
                MarsRoverCamera_AddImageToContainer(reply, ui->S_MINITES_List);
                break;
                // Rover manifest
            case O::CURIOSITY:
                updateRoverManifest(reply, ui->C_RoverManifestList, O::CURIOSITY, ui->C_RoverImageLabel);
                break;
            case O::OPPORTUNITY:
                updateRoverManifest(reply, ui->O_RoverManifestList, O::OPPORTUNITY, ui->O_RoverImageLabel);
                break;
            case O::SPIRIT:
                updateRoverManifest(reply, ui->S_RoverManifestList, O::SPIRIT, ui->S_RoverImageLabel);
                break;
            default:
                reply->deleteLater();
        }

    } else if (dataSource == "podcast") {
        auto origin = reply->property("origin").value<QString>();
        auto imageLabel = reply->property("image_label").value<QLabel *>();
        auto SIZE = reply->property("size").value<unsigned int>();
        auto filePath =
                QString::fromStdString(config.find("podcast_data_path")->second) + getValidFileName(origin) + ".jpg";

        saveDataToFile(filePath, reply->readAll());

        QPixmap p(filePath);
        p = p.scaled(SIZE, SIZE);
        ImageManipulation::roundEdges(p, 20);
        imageLabel->setPixmap(p);

        updateStatus("Thumbnail(s) download!");

    } else if (dataSource == "download") {
        auto answer = reply->readAll();
        auto parsedData = APIHandler::parseJSON(answer);
        auto rover = reply->property("rover").toString();
        auto camera = reply->property("camera").toString();
        auto sol = reply->property("sol").toInt();
        if (parsedData.isEmpty()) {
            // popUpDialog("Failed to get any data from the API");
            updateStatus("Failed to get any data from the API!");
        }
        auto photos = parsedData.find("photos")->toArray();
        if (photos.isEmpty()) {
            // popUpDialog("No photos were found for this query");
            updateStatus("No photos were found for this query");
        } else {
            updateStatus("Fetching images...");

            int c = 0;
            for (const auto &photo: photos)
                downloadImage(photo.toObject().value("img_src").toString(), rover, camera, sol, c++);
        }

    } else if (dataSource == "image_download") {
        auto fileName = reply->property("file_path").toString();

        QFile file;
        file.setFileName(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.close();
        }

        updateStatus("Downladed " + fileName);
        qApp->processEvents();

    } else if (dataSource == "rss_download") {
        updateStatus("RSS fetched!");
        auto answer = reply->readAll();
        auto podcast = Podcasts::DomToPodcast(Podcasts::XMLDataToDom(answer));
        auto fileName = config.find("podcast_sources_path")->second + getValidFileName(podcast->getTitle()).toStdString() + ".rss";
        QFile file;
        file.setFileName(QString::fromStdString(fileName));
        if (!file.open(QIODevice::WriteOnly)) return;
        file.write(answer);
        file.close();
        delete podcast;

    } else if (dataSource == "epic") {

        auto answer = reply->readAll();
        auto origin = reply->property("origin").value<ORIGIN>();
        if (origin == O::EPIC_JSON) {
            auto data = "{\"item\":" + answer + "}";
            auto parsedJson = APIHandler::parseJSON(data);
            auto count = parsedJson.find("item")->toArray().count();
            if (count == 0) {
                updateStatus("No data found for this day!");
                setEPICWidgetsState(true);
                return;
            }
            EPICDownloadCount = 0;
            EPIC::clear();
            for (const auto& item: parsedJson.find("item")->toArray()) {
                auto obj = item.toObject();
                auto date = QDate::fromString(QString::fromStdString(obj.find("date")->toString().toStdString().substr(0, 10)), "yyyy-MM-dd");
                auto fileName = obj.find("image")->toString();
                auto url = APIHandler::getEPICImage_Request_URL(QString::fromStdString(config.find("api_key")->second),
                                                                QString::fromStdString(config.find("epic_image_url")->second),
                                                                date,
                                                                reply->property("type").toString(),
                                                                fileName,
                                                                "png");
                auto centroidCoordsObj = obj.find("centroid_coordinates")->toObject();
                auto dscovrCoordsObj = obj.find("dscovr_j2000_position")->toObject();
                auto lunarCoordsObj = obj.find("lunar_j2000_position")->toObject();
                auto sunCoordsObj = obj.find("sun_j2000_position")->toObject();
                // auto
                fetchEPICImage(url,
                               O::EPIC_IMAGE,
                               obj.find("identifier")->toString(),
                               date.toString(),
                               obj.find("caption")->toString(),
                               obj.find("version")->toString(),
                               count,
                               centroidCoordsObj.find("lat")->toDouble(), centroidCoordsObj.find("lon")->toDouble(),
                               dscovrCoordsObj.find("x")->toDouble(), dscovrCoordsObj.find("y")->toDouble(), dscovrCoordsObj.find("z")->toDouble(),
                               lunarCoordsObj.find("x")->toDouble(), lunarCoordsObj.find("y")->toDouble(), lunarCoordsObj.find("z")->toDouble(),
                               sunCoordsObj.find("x")->toDouble(), sunCoordsObj.find("y")->toDouble(), sunCoordsObj.find("z")->toDouble());
            }
        } else if (origin == O::EPIC_IMAGE) {
            QString title, date, caption, version, coord;
            title = reply->property("title").toString();
            date = reply->property("date").toString();
            caption = reply->property("caption").toString();
            version = reply->property("version").toString();
            unsigned int ct = reply->property("count").toInt();

            auto image = new EPICImage(title, date, caption, version, coord,
                                       {reply->property("lon").toDouble(), reply->property("lat").toDouble()},
                                       {reply->property("dscovrX").toDouble(), reply->property("dscovrY").toDouble(), reply->property("dscovrZ").toDouble()},
                                       {reply->property("lunarX").toDouble(), reply->property("lunarY").toDouble(), reply->property("lunarZ").toDouble()},
                                       {reply->property("sunX").toDouble(), reply->property("sunY").toDouble(), reply->property("sunZ").toDouble()});
            auto pixmap = new QPixmap();
            pixmap->loadFromData(answer);
            ImageManipulation::roundEdges(*pixmap, CORNER_RADIUS);
            image->setPixmap(pixmap);
            EPIC::addImage(image);
            EPICDownloadCount++;
            if (EPICDownloadCount == ct) {
                updateStatus("EPIC Download finished!");
                EPICDownloadCount = 0;
                EPIC::reset();
                setEPICWidgetsState(true);
                updateEPICImageInformation(0);
            }
        } else if (origin == O::EPIC_DATA) {
            EPIC::clearDates();
            auto data = "{\"item\":" + answer + "}";
            auto parsedJson = APIHandler::parseJSON(data);
            for (const auto& item: parsedJson.find("item")->toArray()) {
                auto obj = item.toObject();
                EPIC::addDate(new QDate(QDate::fromString(QString::fromStdString(obj.find("date")->toString().toStdString()), "yyyy-MM-dd")));
            }
            EPIC::sortDates();
            auto max = EPIC::getMaxDateWhenSorted();
            auto min = EPIC::getMinDateWhenSorted();
            updateEPICDataConstraints(max, min);
            setEPICWidgetsState(true);
            updateStatus("New EPIC constraints set!");
        }
    }
}

void MainWindow::saveDataToFile(const QString filePath, const QByteArray &data) {
    QFile file;
    file.setFileName(filePath);
    file.open(QIODevice::WriteOnly);

    file.write(data);
    file.close();
}

QString MainWindow::getValidFileName(std::string fileName) {
    std::string illegal = "#%&{}\\$!'\":@<>*?/+` |=";
    std::stringstream stream;
    for (const auto &c: fileName) if (illegal.find(c) == std::string::npos) stream << c;
    return QString::fromStdString(stream.str());
}


QString MainWindow::getValidFileName(QString fileName) {
    return getValidFileName(fileName.toStdString());
}

void MainWindow::updateWelcomeData(QNetworkReply *reply) {

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

        ui->WelcomeImageExplanationTextBrowser->append(QString::fromStdString(
                std::string("A video of the day was found, but cannot be displayed. Here is the link to it: ")
                + "<a href=\""
                + parsedData.find("url")->toString().toStdString()
                + "\">YouTube</a><br>"));

    }

    setWelcomeImageInformation(parsedData);

    reply->deleteLater();
}

void MainWindow::updateWelcomeImage(QNetworkReply *reply) {
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
    auto filePath = QString::fromStdString(config.find("welcome_image_path")->second);
    saveDataToFile(filePath, answer);

    // Sets the image to label
    QPixmap pic(filePath);
    ImageManipulation::roundEdges(pic, CORNER_RADIUS);
    ui->WelcomeImageLabel->setPixmap(pic);
    ui->WelcomeImageLabel->update();

    // Delete old data
    reply->deleteLater();

    updateStatus("Welcome image fetched!");

    resizeWelcomeImage();
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

    ui->WelcomeImageTitleLabel->setText(title);
    ui->WelcomeImageExplanationTextBrowser->append(explanation);
    ui->WelcomeImageDateLabel->setText(date);
}

// On window resize, change image aspect ration - TODO
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    resizeWelcomeImage();
    updateEPICImage();
}

void MainWindow::resizeWelcomeImage() {
    auto maxW = ui->WelcomeImageDateLabel->width() - 18;
    auto maxH = ui->Tabs->height() - ui->WelcomeImageInfoFrame->height() - 70;

    auto w = ui->WelcomeImageLabel->pixmap().width();
    auto h = ui->WelcomeImageLabel->pixmap().height();

    auto wP = (double) w / (double) maxW;
    auto hP = (double) h / (double) maxH;

    auto max = std::max(wP, hP);

    if (max > 1) {
        ui->WelcomeImageLabel->setMaximumHeight(h / max);
        ui->WelcomeImageLabel->setMaximumWidth(w / max);
    }
}

MainWindow::~MainWindow() {
    delete ui;
    delete manager;
    delete mediaPlayer;
    delete timer;
}

// Mars Rover Imagery
void MainWindow::MarsRoverCamera_SetImages(QNetworkReply *reply, ORIGIN origin) {
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
    } else {
        updateStatus("Fetching images...");

        for (const auto &photo: photos)
            fetchAPIData(photo.toObject().value("img_src").toString(), origin);
    }

    reply->deleteLater();
}

void MainWindow::MarsRoverCamera_AddImageToContainer(QNetworkReply *reply, QListWidget *list) {
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
    label->setFixedWidth(p.width() * ((double) SIZE / p.height()));
    label->setMargin(5);

    item->setSizeHint(QSize(p.width() * ((double) SIZE / p.height()), SIZE));
    list->addItem(item);
    list->setItemWidget(item, label);

    reply->deleteLater();

    // Update status
    updateStatus("Photo(s) fetched!");
}

// Rover-camera specific functions
void MainWindow::C_FHAZ_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::C_FHAZ_P);
}

void MainWindow::C_RHAZ_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::C_RHAZ_P);
}

void MainWindow::C_MAST_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::C_MAST_P);
}

void MainWindow::C_CHEMCAM_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::C_CHEMCAM_P);
}

void MainWindow::C_MAHLI_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::C_MAHLI_P);
}

void MainWindow::C_MARDI_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::C_MARDI_P);
}

void MainWindow::C_NAVCAM_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::C_NAVCAM_P);
}

void MainWindow::O_FHAZ_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::O_FHAZ_P);
}

void MainWindow::O_RHAZ_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::O_RHAZ_P);
}

void MainWindow::O_NAVCAM_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::O_NAVCAM_P);
}

void MainWindow::O_PANCAM_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::O_PANCAM_P);
}

void MainWindow::O_MINITES_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::O_MINITES_P);
}

void MainWindow::S_FHAZ_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::S_FHAZ_P);
}

void MainWindow::S_RHAZ_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::S_RHAZ_P);
}

void MainWindow::S_NAVCAM_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::S_NAVCAM_P);
}

void MainWindow::S_PANCAM_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::S_PANCAM_P);
}

void MainWindow::S_MINITES_SetImages(QNetworkReply *reply) {
    MarsRoverCamera_SetImages(reply, O::S_MINITES_P);
}

// Rover camera on click events
void MainWindow::on_S_RHAZ_SOLS_Button_clicked() {
    ui->S_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::SPIRIT,
                                                                     O::RHAZ,
                                                                     std::to_string(ui->S_RHAZ_Sols->value())),
                 O::S_RHAZ);
}


void MainWindow::on_S_RHAZ_DATE_Button_clicked() {
    ui->S_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::SPIRIT,
                                                                          O::RHAZ,
                                                                          APIHandler::dateToString(
                                                                                  ui->S_RHAZ_Date->date())),
                 O::S_RHAZ);
}


void MainWindow::on_S_PANCAM_SOLS_Button_clicked() {
    ui->S_PANCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::SPIRIT,
                                                                     O::PANCAM,
                                                                     std::to_string(ui->S_PANCAM_Sols->value())),
                 O::S_PANCAM);
}


void MainWindow::on_S_PANCAM_DATE_Button_clicked() {
    ui->S_PANCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::SPIRIT,
                                                                          O::PANCAM,
                                                                          APIHandler::dateToString(
                                                                                  ui->S_PANCAM_Date->date())),
                 O::S_PANCAM);
}


void MainWindow::on_S_NAVCAM_SOLS_Button_clicked() {
    ui->S_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::SPIRIT,
                                                                     O::NAVCAM,
                                                                     std::to_string(ui->S_NAVCAM_Sols->value())),
                 O::S_NAVCAM);
}


void MainWindow::on_S_NAVCAM_DATE_Button_clicked() {
    ui->S_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::SPIRIT,
                                                                          O::NAVCAM,
                                                                          APIHandler::dateToString(
                                                                                  ui->S_NAVCAM_Date->date())),
                 O::S_NAVCAM);
}


void MainWindow::on_S_MINITES_SOLS_Button_clicked() {
    ui->S_MINITES_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::SPIRIT,
                                                                     O::MINITES,
                                                                     std::to_string(ui->O_MINITES_Sols->value())),
                 O::S_MINITES);
}


void MainWindow::on_S_MINITES_DATE_Button_clicked() {
    ui->S_MINITES_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::SPIRIT,
                                                                          O::MINITES,
                                                                          APIHandler::dateToString(
                                                                                  ui->S_MINITES_Date->date())),
                 O::S_MINITES);
}


void MainWindow::on_S_FHAZ_SOLS_Button_clicked() {
    ui->S_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::SPIRIT,
                                                                     O::FHAZ,
                                                                     std::to_string(ui->S_FHAZ_Sols->value())),
                 O::S_FHAZ);
}


void MainWindow::on_S_FHAZ_DATE_Button_clicked() {
    ui->S_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::SPIRIT,
                                                                          O::FHAZ,
                                                                          APIHandler::dateToString(
                                                                                  ui->S_FHAZ_Date->date())),
                 O::S_FHAZ);
}


void MainWindow::on_O_RHAZ_SOLS_Button_clicked() {
    ui->O_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::OPPORTUNITY,
                                                                     O::RHAZ,
                                                                     std::to_string(ui->O_RHAZ_Sols->value())),
                 O::O_RHAZ);
}


void MainWindow::on_O_RHAZ_DATE_Button_clicked() {
    ui->O_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::OPPORTUNITY,
                                                                          O::RHAZ,
                                                                          APIHandler::dateToString(
                                                                                  ui->O_RHAZ_Date->date())),
                 O::O_RHAZ);
}

void MainWindow::on_O_PANCAM_SOLS_Button_clicked() {
    ui->O_PANCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::OPPORTUNITY,
                                                                     O::PANCAM,
                                                                     std::to_string(ui->O_PANCAM_Sols->value())),
                 O::O_PANCAM);
}


void MainWindow::on_O_PANCAM_DATE_Button_clicked() {
    ui->O_PANCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::OPPORTUNITY,
                                                                          O::PANCAM,
                                                                          APIHandler::dateToString(
                                                                                  ui->O_PANCAM_Date->date())),
                 O::O_PANCAM);
}


void MainWindow::on_O_NAVCAM_SOLS_Button_clicked() {
    ui->O_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::OPPORTUNITY,
                                                                     O::NAVCAM,
                                                                     std::to_string(ui->O_NAVCAM_Sols->value())),
                 O::O_NAVCAM);
}


void MainWindow::on_O_NAVCAM_DATE_Button_clicked() {
    ui->O_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::OPPORTUNITY,
                                                                          O::NAVCAM,
                                                                          APIHandler::dateToString(
                                                                                  ui->O_NAVCAM_Date->date())),
                 O::O_NAVCAM);
}


void MainWindow::on_O_MINITES_SOLS_Button_clicked() {
    ui->O_MINITES_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::OPPORTUNITY,
                                                                     O::MINITES,
                                                                     std::to_string(ui->O_MINITES_Sols->value())),
                 O::O_MINITES);
}


void MainWindow::on_O_MINITES_DATE_Button_clicked() {
    ui->O_MINITES_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::OPPORTUNITY,
                                                                          O::MINITES,
                                                                          APIHandler::dateToString(
                                                                                  ui->O_MINITES_Date->date())),
                 O::O_MINITES);
}


void MainWindow::on_O_FHAZ_SOLS_Button_clicked() {
    ui->O_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::OPPORTUNITY,
                                                                     O::FHAZ,
                                                                     std::to_string(ui->O_FHAZ_Sols->value())),
                 O::O_FHAZ);
}


void MainWindow::on_O_FHAZ_DATE_Button_clicked() {
    ui->O_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::OPPORTUNITY,
                                                                          O::FHAZ,
                                                                          APIHandler::dateToString(
                                                                                  ui->O_FHAZ_Date->date())),
                 O::O_FHAZ);
}


void MainWindow::on_C_NAVCAM_SOLS_Button_clicked() {
    ui->C_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::CURIOSITY,
                                                                     O::NAVCAM,
                                                                     std::to_string(ui->C_NAVCAM_Sols->value())),
                 O::C_NAVCAM);
}


void MainWindow::on_C_NAVCAM_DATE_Button_clicked() {
    ui->C_NAVCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::CURIOSITY,
                                                                          O::NAVCAM,
                                                                          APIHandler::dateToString(
                                                                                  ui->C_NAVCAM_Date->date())),
                 O::C_NAVCAM);
}


void MainWindow::on_C_MAST_SOLS_Button_clicked() {
    ui->C_MAST_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::CURIOSITY,
                                                                     O::MAST,
                                                                     std::to_string(ui->C_MAST_Sols->value())),
                 O::C_MAST);
}


void MainWindow::on_C_MAST_DATE_Button_clicked() {
    ui->C_MAST_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::CURIOSITY,
                                                                          O::MAST,
                                                                          APIHandler::dateToString(
                                                                                  ui->C_MAST_Date->date())),
                 O::C_MAST);
}


void MainWindow::on_C_MARDI_SOLS_Button_clicked() {
    ui->C_MARDI_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::CURIOSITY,
                                                                     O::MARDI,
                                                                     std::to_string(ui->C_MARDI_Sols->value())),
                 O::C_MARDI);
}


void MainWindow::on_C_MARDI_DATE_Button_clicked() {
    ui->C_MARDI_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::CURIOSITY,
                                                                          O::MARDI,
                                                                          APIHandler::dateToString(
                                                                                  ui->C_MARDI_Date->date())),
                 O::C_MARDI);
}


void MainWindow::on_C_MAHLI_SOLS_Button_clicked() {
    ui->C_MAHLI_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::CURIOSITY,
                                                                     O::MAHLI,
                                                                     std::to_string(ui->C_MAHLI_Sols->value())),
                 O::C_MAHLI);
}


void MainWindow::on_C_MAHLI_DATE_Button_clicked() {
    ui->C_MAHLI_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::CURIOSITY,
                                                                          O::MAHLI,
                                                                          APIHandler::dateToString(
                                                                                  ui->C_MAHLI_Date->date())),
                 O::C_MAHLI);
}


void MainWindow::on_C_FHAZ_SOLS_Button_clicked() {
    ui->C_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::CURIOSITY,
                                                                     O::FHAZ,
                                                                     std::to_string(ui->C_FHAZ_Sols->value())),
                 O::C_FHAZ);
}


void MainWindow::on_C_FHAZ_DATE_Button_clicked() {
    ui->C_FHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::CURIOSITY,
                                                                          O::FHAZ,
                                                                          APIHandler::dateToString(
                                                                                  ui->C_FHAZ_Date->date())),
                 O::C_FHAZ);
}


void MainWindow::on_C_CHEMCAM_SOLS_Button_clicked() {
    ui->C_CHEMCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::CURIOSITY,
                                                                     O::CHEMCAM,
                                                                     std::to_string(ui->C_CHEMCAM_Sols->value())),
                 O::C_CHEMCAM);
}


void MainWindow::on_C_CHEMCAM_DATE_Button_clicked() {
    ui->C_CHEMCAM_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::CURIOSITY,
                                                                          O::CHEMCAM,
                                                                          APIHandler::dateToString(
                                                                                  ui->C_CHEMCAM_Date->date())),
                 O::C_CHEMCAM);
}


void MainWindow::on_C_RHAZ_SOLS_Button_clicked() {
    ui->C_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImagerySols_API_Request_URL(config.find("mars_rover_url")->second,
                                                                     config.find("api_key")->second,
                                                                     O::CURIOSITY,
                                                                     O::RHAZ,
                                                                     std::to_string(ui->C_RHAZ_Sols->value())),
                 O::C_RHAZ);
}

void MainWindow::on_C_RHAZ_DATE_Button_clicked() {
    ui->C_RHAZ_List->clear();
    fetchAPIData(APIHandler::getMarsRoverImageryEarthDate_API_Request_URL(config.find("mars_rover_url")->second,
                                                                          config.find("api_key")->second,
                                                                          O::CURIOSITY,
                                                                          O::RHAZ,
                                                                          APIHandler::dateToString(
                                                                                  ui->C_RHAZ_Date->date())),
                 O::C_RHAZ);
}

void
MainWindow::limitCameraInputWidgetRanges(QSpinBox *solsWidget, QString &maxSol, QDateEdit *dateWidget, QString &maxDate,
                                         QString &landingDate) {
    solsWidget->setMinimum(0);
    solsWidget->setMaximum(maxSol.toInt());
    dateWidget->setMinimumDate(QDate::fromString(landingDate, "yyyy-MM-dd"));
    dateWidget->setMaximumDate(QDate::fromString(maxDate, "yyyy-MM-dd"));
}

void
MainWindow::limitRoverImageryInputWidgetRanges(ORIGIN origin, QString maxSol, QString maxDate, QString landingDate) {
    switch (origin) {
        case O::CURIOSITY: {
            limitCameraInputWidgetRanges(ui->C_FHAZ_Sols, maxSol, ui->C_FHAZ_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->C_RHAZ_Sols, maxSol, ui->C_RHAZ_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->C_MAST_Sols, maxSol, ui->C_MAST_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->C_CHEMCAM_Sols, maxSol, ui->C_CHEMCAM_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->C_MAHLI_Sols, maxSol, ui->C_MAHLI_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->C_MARDI_Sols, maxSol, ui->C_MARDI_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->C_NAVCAM_Sols, maxSol, ui->C_NAVCAM_Date, maxDate, landingDate);
        };
            break;

        case O::OPPORTUNITY: {
            limitCameraInputWidgetRanges(ui->O_FHAZ_Sols, maxSol, ui->O_FHAZ_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->O_RHAZ_Sols, maxSol, ui->O_RHAZ_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->O_NAVCAM_Sols, maxSol, ui->O_NAVCAM_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->O_PANCAM_Sols, maxSol, ui->O_PANCAM_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->O_MINITES_Sols, maxSol, ui->O_MINITES_Date, maxDate, landingDate);
        };
            break;

        case O::SPIRIT: {
            limitCameraInputWidgetRanges(ui->S_FHAZ_Sols, maxSol, ui->S_FHAZ_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->S_RHAZ_Sols, maxSol, ui->S_RHAZ_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->S_NAVCAM_Sols, maxSol, ui->S_NAVCAM_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->S_PANCAM_Sols, maxSol, ui->S_PANCAM_Date, maxDate, landingDate);
            limitCameraInputWidgetRanges(ui->S_MINITES_Sols, maxSol, ui->S_MINITES_Date, maxDate, landingDate);
        };
            break;

        default:
            return;
    }
}

void MainWindow::updateRoverManifest(QNetworkReply *reply, QListWidget *list, ORIGIN origin, QLabel *imageLabel) {

    // Set ranges on rover imagery input widgets
    QString maxSol, maxDate, landingDate;

    // Rover image
    const int MAX_SIZE = 550;
    std::string filePath = config.find("mars_rover_images_path")->second + E::eToS(origin) + ".jpg";
    QPixmap pic(QString::fromStdString(filePath));
    ImageManipulation::roundEdges(pic, 35);
    imageLabel->setPixmap(pic);

    imageLabel->setScaledContents(true);
    imageLabel->setMaximumHeight(MAX_SIZE);
    imageLabel->setMaximumWidth(MAX_SIZE);

    // Manifest data
    auto parsedData = APIHandler::parseJSON(reply->readAll());
    auto parsedObj = parsedData.value("rover").toObject();

    for (const auto &key: parsedObj.keys()) {

        auto item = parsedObj.value(key);
        QLabel *kLabel = new QLabel();
        kLabel->setFont(
                QFont(QFontDatabase::font(getFontQ("default_font"), getFontQ("bold"), getFSize("primary_text_size"))));
        QLabel *vLabel = new QLabel();
        vLabel->setFont(QFont(QFontDatabase::font(getFontQ("default_font"), getFontQ("normal"),
                                                  getFSize("primary_text_size"))));
        QFrame *frame = new QFrame();
        QHBoxLayout *layout = new QHBoxLayout();
        frame->setLayout(layout);
        layout->addWidget(kLabel);
        layout->addWidget(vLabel);

        QListWidgetItem *i = new QListWidgetItem("");

        // auto _size = item.size();
        if (item.isArray()) {
            // Cameras
            auto k = key.toStdString();
            if (k != "cameras") break;
            for (auto &c: k) c = toupper(c);
            kLabel->setText(QString::fromStdString(k));
            std::stringstream stream;
            bool _f = true;
            for (const auto j: item.toArray()) {
                if (!_f) stream << "\n";
                else _f = false;
                auto obj = j.toObject();
                auto v = obj.value("full_name").toString().toStdString();;
                stream << v;
            }
            vLabel->setText(QString::fromStdString(stream.str()));
            vLabel->setWordWrap(true);
        } else {
            // Other data
            auto k = key.toStdString();
            std::string v;
            if (item.isString()) v = item.toString().toStdString();
            else v = std::to_string(item.toInt());
            if (k == "landing_date") landingDate = QString::fromStdString(v);
            else if (k == "max_sol") maxSol = QString::fromStdString(v);
            else if (k == "max_date") maxDate = QString::fromStdString(v);

            for (auto &c: k) c = toupper(c);
            kLabel->setText(QString::fromStdString(k));
            vLabel->setText(QString::fromStdString(v));
        }

        i->setSizeHint(vLabel->sizeHint() + QSize(10, 24));
        list->addItem(i);
        list->setItemWidget(i, frame);
        qApp->processEvents();
    }

    limitRoverImageryInputWidgetRanges(origin, maxSol, maxDate, landingDate);

    reply->deleteLater();
}

void MainWindow::on_UpdatePodcastsButton_clicked()
{
    updateStatus("Updating podcasts from web...");
    updateLocalPodcats();
    // TODO: Update GUI on finish
}

void MainWindow::updateLocalPodcats() {
    clearPodcastsList();
    clearEpisodesList();
    Podcasts::loadPodcastsFromSourceFolder(QString::fromStdString(config.find("podcast_sources_path")->second));
    QDir dir;
    auto path = QString::fromStdString(config.find("podcast_sources_path")->second);
    dir.setPath(path);
    for (const auto& podcast: Podcasts::getPodcasts()) {
        auto selfUrl = podcast->getSelfUrl();
        QNetworkRequest request;
        request.setUrl(selfUrl);
        auto res = manager->get(request);
        res->setProperty("data_source", "rss_download");
    }
    for (const auto& file: dir.entryList()) {
        QFile f (path + file);
        f.remove();
    }
}

void MainWindow::clearPodcastsList() {
    ui->PodcastSelectorList->clear();
}

void MainWindow::clearEpisodesList() {
    ui->EpisodeSelectorList->clear();
}

void MainWindow::updatePodcastsList() {

    updateStatus("Loading podcasts...");
    qApp->processEvents();

    clearPodcastsList();
    clearEpisodesList();

    for (const auto &podcast: Podcasts::getPodcasts()) {
        // Create the frame with all the labels
        auto mainLayout = new QHBoxLayout();
        QFrame *mainFrame = new QFrame();
        mainFrame->setLayout(mainLayout);
        mainFrame->setProperty("ID", podcast->getID());

        auto informationLayout = new QVBoxLayout();
        QFrame *informationFrame = new QFrame();
        informationFrame->setLayout(informationLayout);

        const int SIZE = 220;
        QLabel *imageLabel = new QLabel();
        imageLabel->setText("Loading image...");
        imageLabel->setScaledContents(true);
        imageLabel->setFixedWidth(SIZE);
        imageLabel->setFixedHeight(SIZE);

        mainLayout->addWidget(imageLabel);

        QLabel *titleLabel = new QLabel();
        QTextEdit *descriptionTextEdit = new QTextEdit();
        // TODO: Make it an actual working link
        QLabel *linkLabel = new QLabel();
        QLabel *languageLabel = new QLabel();

        titleLabel->setText(podcast->getTitle());
        descriptionTextEdit->insertPlainText(podcast->getDescription());
        linkLabel->setText("Link: " + podcast->getLink());
        languageLabel->setText("Language: " + podcast->getLanguage());

        titleLabel->setFont(
                QFont(QFontDatabase::font(getFontQ("default_font"), getFontQ("bold"), getFSize("primary_title_size"))));
        descriptionTextEdit->setReadOnly(true);
        descriptionTextEdit->setFrameStyle(0);
        descriptionTextEdit->setFont(QFont(QFontDatabase::font(getFontQ("default_font"), getFontQ("normal"),
                                                               getFSize("primary_text_size"))));
        descriptionTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        descriptionTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        linkLabel->setFont(QFont(QFontDatabase::font(getFontQ("default_font"), getFontQ("normal"),
                                                     getFSize("primary_text_small_size"))));
        languageLabel->setFont(QFont(QFontDatabase::font(getFontQ("default_font"), getFontQ("normal"),
                                                         getFSize("primary_text_small_size"))));

        informationLayout->addWidget(titleLabel);
        informationLayout->addWidget(descriptionTextEdit);
        informationLayout->addWidget(linkLabel);
        informationLayout->addWidget(languageLabel);

        mainLayout->addWidget(informationFrame);

        QListWidgetItem *podcastItem = new QListWidgetItem("");
        podcastItem->setSizeHint(QSize(SIZE * 2, SIZE + 20));

        ui->PodcastSelectorList->addItem(podcastItem);
        ui->PodcastSelectorList->setItemWidget(podcastItem, mainFrame);

        auto filePath = QString::fromStdString(config.find("podcast_data_path")->second) +
                        getValidFileName(podcast->getTitle()) + ".jpg";
        QFile file;
        file.setFileName(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QPixmap p(filePath);
            p = p.scaled(SIZE, SIZE);
            ImageManipulation::roundEdges(p, 20);
            imageLabel->setPixmap(p);
            updateStatus("Cached thumbnail(s) found!");
        } else {
            fetchPodcastData(podcast->getImageUrl(), podcast->getTitle(), imageLabel, SIZE);
            updateStatus("Downloading thumbnails...");
            qApp->processEvents();
        }
    }

    updateStatus("Podcasts loaded!");

}

void MainWindow::populateEpisodesList(QListWidgetItem *item, bool fav) {

    clearEpisodesList();

    unsigned int ID;
    if (item) {
        auto widget = dynamic_cast<QFrame *> (item->listWidget()->itemWidget(item));
        ID = widget->property("ID").toInt();
        PID = ID;
        FavoriteEpisode = false;
    } else {
        ID = PID;
        FavoriteEpisode = true;
    }

    auto podcast = Podcasts::getPodcastById(ID);

    const int SIZE = 140;
    auto filePath =
            QString::fromStdString(config.find("podcast_data_path")->second) + getValidFileName(podcast->getTitle()) +
            ".jpg";
    QFile file;
    QPixmap p;
    file.setFileName(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        p = QPixmap(filePath);
        p = p.scaled(SIZE, SIZE);
        ImageManipulation::roundEdges(p, 20);

        updateStatus("Cached thumbnail(s) found!");
    } else {
        popUpDialog("Podcast data is corrupt, please reload the podcasts!");
        return;
    }

    updateStatus("Loading epsiodes...");
    qApp->processEvents();

    for (const auto &episode: podcast->getEpisodes()) {

        if (fav)
            if (!Podcasts::isFavouriteEpisode(*episode))
                continue;

        auto mainLayout = new QHBoxLayout();
        QFrame *mainFrame = new QFrame();
        mainFrame->setLayout(mainLayout);
        mainFrame->setProperty("ID", episode->getID());

        auto informationLayout = new QVBoxLayout();
        QFrame *informationFrame = new QFrame();
        informationFrame->setLayout(informationLayout);

        QLabel *imageLabel = new QLabel();
        imageLabel->setText("Loading image...");
        imageLabel->setScaledContents(true);
        imageLabel->setFixedWidth(SIZE);
        imageLabel->setFixedHeight(SIZE);

        mainLayout->addWidget(imageLabel);

        QLabel *titleLabel = new QLabel();
        QTextEdit *descriptionTextEdit = new QTextEdit();
        QLabel *dateLabel = new QLabel();
        // TODO: Move the URL inside title as a href

        titleLabel->setText("<a style=\"color: black; text-decoration: none;\"href=\"" + episode->getWebUrl() + "\">" +
                            episode->getTitle() + "</a>");
        descriptionTextEdit->setText(episode->getDescription());
        dateLabel->setText("Date: " + episode->getDate());

        titleLabel->setTextFormat(Qt::RichText);
        titleLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        titleLabel->setOpenExternalLinks(true);
        titleLabel->setFont(QFont(QFontDatabase::font(getFontQ("default_font"), getFontQ("bold"),
                                                      getFSize("secondary_title_size"))));

        descriptionTextEdit->setReadOnly(true);
        descriptionTextEdit->setFrameStyle(0);
        descriptionTextEdit->setFont(QFont(QFontDatabase::font(getFontQ("default_font"), getFontQ("normal"),
                                                               getFSize("secondary_text_size"))));
        descriptionTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        descriptionTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        informationLayout->addWidget(titleLabel);
        informationLayout->addWidget(descriptionTextEdit);
        informationLayout->addWidget(dateLabel);

        mainLayout->addWidget(informationFrame);

        QListWidgetItem *episodeItem = new QListWidgetItem("");
        episodeItem->setSizeHint(QSize(SIZE * 2, SIZE + 18));

        ui->EpisodeSelectorList->addItem(episodeItem);
        ui->EpisodeSelectorList->setItemWidget(episodeItem, mainFrame);

        imageLabel->setPixmap(p);

        qApp->processEvents();
    }

    updateStatus("Episodes loaded!");

}

void MainWindow::playEpisode(QListWidgetItem *item) {
    auto widget = dynamic_cast<QFrame *> (item->listWidget()->itemWidget(item));
    auto ID = widget->property("ID").toInt();
    auto episode = Podcasts::getEpisodeById(ID);

    playEpisode(episode);
}

void MainWindow::playEpisode(PodcastEpisode *episode) {
    updateHeartButtonIcon(episode);

    if (!episode) return;

    const unsigned int SIZE = 100;
    auto filePath = QString::fromStdString(config.find("podcast_data_path")->second) +
                    getValidFileName(Podcasts::getPodcastById(episode->getPID())->getTitle()) + ".jpg";
    QFile file;
    QPixmap p;
    file.setFileName(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        p = QPixmap(filePath);
        p = p.scaled(SIZE, SIZE);
        ImageManipulation::roundEdges(p, 20);

        updateStatus("Cached thumbnail(s) found!");
    } else {
        popUpDialog("Podcast data is corrupt, please reload the podcasts!");
        return;
    }

    auto title = episode->getTitle();
    auto date = episode->getDate();
    auto audioUrl = episode->getMP3Url();

    ui->EpisodeThumbnailLabel->setPixmap(p);
    ui->EpisodeTitleTextEdit->setText(title);
    ui->EpisodeDateLabel->setText(date);

    mediaPlayer->stop();
    mediaPlayer->setSource(QUrl(audioUrl));
    mediaPlayer->play();

    this->episode = episode;

    setButtonToPlay(false);

    updateStatus("Playing " + episode->getTitle());
}

void MainWindow::playNextEpisode(PodcastEpisode *currentEpisode) {
    if (!currentEpisode) {
        episode = nullptr;
        return;
    }
    auto nextEpisode = Podcasts::getEpisodeById(currentEpisode->getID() + 1);
    if (nextEpisode) {
        if (currentEpisode->getPID() != nextEpisode->getPID()) {
            episode = nullptr;
            return;
        }
        if (FavoriteEpisode) {
            if (!Podcasts::isFavouriteEpisode(*nextEpisode)) {
                playNextEpisode(nextEpisode);
            } else {
                playEpisode(nextEpisode);
            }
        } else playEpisode(nextEpisode);
    } else episode = nullptr;
}

void MainWindow::playPrevEpisode(PodcastEpisode *currentEpisode) {
    if (!currentEpisode) {
        episode = nullptr;
        return;
    }
    if (currentEpisode->getID() < 1) return;
    auto nextEpisode = Podcasts::getEpisodeById(currentEpisode->getID() - 1);
    if (nextEpisode) {
        if (currentEpisode->getPID() != nextEpisode->getPID()) {
            episode = nullptr;
            return;
        }
        if (FavoriteEpisode) {
            if (!Podcasts::isFavouriteEpisode(*nextEpisode)) {
                playPrevEpisode(nextEpisode);
            } else {
                playEpisode(nextEpisode);
            }
        } else playEpisode(nextEpisode);
    } else episode = nullptr;
}

void MainWindow::onDurationChanged(qint64 duration) {
    duration /= 1000;
    int hours = duration / 3600;
    int minutes = (duration - (hours * 3600)) / 60;
    int seconds = duration % 60;
    std::string time;
    if (hours != 0)
        time = std::to_string(hours) + ":" + (minutes > 9 ? "" : "0") + std::to_string(minutes) + ":" +
               (seconds > 9 ? "" : "0") + std::to_string(seconds);
    else
        time = std::to_string(minutes) + ":" + (seconds > 9 ? "" : "0") + std::to_string(seconds);
    ui->MaxTimeLabel->setText(QString::fromStdString(time));
    ui->AudioProgressBar->setMaximum(duration * 1000);
}

void MainWindow::onPositionChanged(qint64 position) {
    if (!AudioProgressBarLocked)
        ui->AudioProgressBar->setValue(position);
    if (position % 1000) {
        position /= 1000;
        int hours = position / 3600;
        int minutes = (position - (hours * 3600)) / 60;
        int seconds = position % 60;
        std::string time;
        if (hours != 0)
            time = std::to_string(hours) + ":" + (minutes > 9 ? "" : "0") + std::to_string(minutes) + ":" +
                   (seconds > 9 ? "" : "0") + std::to_string(seconds);
        else
            time = std::to_string(minutes) + ":" + (seconds > 9 ? "" : "0") + std::to_string(seconds);
        ui->CurrentTimeLabel->setText(QString::fromStdString(time));
    }
}

void MainWindow::on_AudioProgressBar_sliderPressed() {
    AudioProgressBarLocked = true;
}


void MainWindow::on_AudioProgressBar_sliderReleased() {
    if (mediaPlayer->mediaStatus() == QMediaPlayer::BufferedMedia)
        mediaPlayer->setPosition(ui->AudioProgressBar->sliderPosition());
    AudioProgressBarLocked = false;
}


void MainWindow::on_PausePlayButton_clicked() {
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
        setButtonToPlay(true);
    } else if (mediaPlayer->playbackState() == QMediaPlayer::PausedState) {
        mediaPlayer->play();
        setButtonToPlay(false);
    }

}

void MainWindow::setButtonToPlay(bool state) {
    if (!state)
        ui->PausePlayButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bx-pause-circle.png")));
    else ui->PausePlayButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bx-play-circle.png")));
}

void MainWindow::onPlaybackStateChanged(QMediaPlayer::PlaybackState) {
    if (mediaPlayer->playbackState() == QMediaPlayer::StoppedState &&
        mediaPlayer->mediaStatus() == QMediaPlayer::EndOfMedia) {
        resetAudioControlsPane();
        resetAudio();
        if (AudioAutoPlay)
            playNextEpisode(episode);
    }
}

void MainWindow::resetAudioControlsPane() {
    ui->AudioProgressBar->setMaximum(0);
    ui->AudioProgressBar->setValue(0);
    ui->MaxTimeLabel->setText("0:00");
    ui->CurrentTimeLabel->setText("0:00");
    ui->EpisodeThumbnailLabel->setPixmap({});
    ui->EpisodeDateLabel->setText("");
    ui->EpisodeTitleTextEdit->setText("");
    updateHeartButtonIcon(nullptr);
    setButtonToPlay(true);
}

void MainWindow::resetAudio() {
    mediaPlayer->stop();
}

void MainWindow::on_SkiptimeBackButton_clicked() {
    mediaPlayer->setPosition(std::max((qint64) 0, mediaPlayer->position() - 15000));
}


void MainWindow::on_SkipTimeForwardButton_clicked() {
    mediaPlayer->setPosition(std::min(mediaPlayer->position() + 15000, mediaPlayer->duration()));
}

void MainWindow::on_NextEpButton_clicked() {
    resetAudioControlsPane();
    resetAudio();
    playNextEpisode(episode);
}


void MainWindow::on_PreviousEpButton_clicked() {
    resetAudioControlsPane();
    resetAudio();
    playPrevEpisode(episode);
}


void MainWindow::on_DownloadsDownloadButton_clicked() {
    auto startSol = ui->DownloadsStartSol->value();
    auto endSol = ui->DownloadsEndSol->value();
    for (; startSol <= endSol; startSol++) {
        for (const auto &e: ui->MarsRoverImagesCfgFrame->children()) {
            if (e->isWidgetType()) {
                for (const auto &checkBox: e->findChildren<QCheckBox *>()) {
                    if (checkBox->isChecked()) {
                        downloadImages(checkBox->objectName(), startSol);
                    }
                }
            }
        }
    }
}

void MainWindow::downloadImages(const QString &checkBoxTitle, const unsigned int sol) {
    auto _temp = checkBoxTitle.toStdString();
    auto camera = _temp.substr(0, _temp.find("_"));
    auto rover = _temp.substr(_temp.find("_") + 1);
    for (auto &c: rover) if (std::isupper(c)) c = std::tolower(c);
    for (auto &c: camera) if (std::islower(c)) c = std::toupper(c);
    fetchImages(APIHandler::getMarsRoverImagerySols_API_Request_URL(
                        config.find("mars_rover_url")->second,
                        config.find("api_key")->second,
                        rover,
                        camera,
                        std::to_string(sol)
                ),
                checkBoxTitle,
                sol,
                QString::fromStdString(rover),
                QString::fromStdString(camera));
}

void
MainWindow::downloadImage(const QString &imgSource, const QString &rover, const QString &camera, const unsigned int sol,
                          const unsigned int ID) {
    auto fileName = config.find("downloads_path")->second + rover.toStdString() + "_" + camera.toStdString()
                    + "_" + std::to_string(sol) + "_" + std::to_string(ID) + ".jpg";
    fetchImage(imgSource, QString::fromStdString(fileName));
}

void MainWindow::on_AutoPlayButton_clicked()
{
    AudioAutoPlay = !AudioAutoPlay;
    if (AudioAutoPlay)
        ui->AutoPlayButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bx-podcast.png")));
    else
        ui->AutoPlayButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bx-headphone.png")));
}

void MainWindow::on_HeartButton_clicked()
{
    if (!episode || !(mediaPlayer->mediaStatus() == QMediaPlayer::BufferedMedia)) return;
    auto toFav = Podcasts::setFavouriteEpisode(*episode);
    if (toFav) ui->HeartButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bxs-heart.png")));
    else ui->HeartButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bx-heart.png")));
    Podcasts::saveFavEpisodes(QString::fromStdString(config.find("podcast_fav_episode_path")->second));
}

void MainWindow::updateHeartButtonIcon(PodcastEpisode* episode) {
    if (!episode || !Podcasts::isFavouriteEpisode(*episode)) ui->HeartButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bx-heart.png")));
    else ui->HeartButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bxs-heart.png")));
}

void MainWindow::on_LoadPodcastsButton_clicked()
{
    updateStatus("Loading local podcasts...");
    Podcasts::loadPodcastsFromSourceFolder(QString::fromStdString(config.find("podcast_sources_path")->second));
    updatePodcastsList();
    updateStatus("Podcasts loaded!");
}

void MainWindow::on_FavoritesButton_clicked()
{
    populateEpisodesList(nullptr, true);
}

void MainWindow::setEPICWidgetsState(bool enabled) {
    ui->EPICSearchButton->setEnabled(enabled);
    ui->EPICDate->setEnabled(enabled);
    ui->EPICDateSlider->setEnabled(enabled);
    ui->EPICImageTypeComboBox->setEnabled(enabled);
    ui->EPICNextDateButton->setEnabled(enabled);
    ui->EPICPrevDateButton->setEnabled(enabled);
    ui->EPICNextImageButton->setEnabled(enabled);
    ui->EPICPrevImageButton->setEnabled(enabled);
    ui->EPICAutoPlayButton->setEnabled(enabled);
    if (EPICAutoPlay) on_EPICAutoPlayButton_clicked();
    EPICDownloadLock = !enabled;
}

void MainWindow::on_EPICSearchButton_clicked()
{
    setEPICWidgetsState(false);
    auto mode = (ui->EPICImageTypeComboBox->currentIndex() == 0 ? O::EPIC_NATURAL : O::EPIC_ENCHANCED);
    fetchEPICJson(APIHandler::getEPICJson_Request_URL(QString::fromStdString(config.find("api_key")->second),
                                                      QString::fromStdString(config.find("epic_json_url")->second),
                                                      ui->EPICDate->date(),
                                                      E::eToQs(mode)),
                  O::EPIC_JSON,
                  E::eToQs(mode));
}

void MainWindow::clearEPICImagesLabel() {
    ui->EPICImageLabel->setPixmap(QPixmap());
}

void MainWindow::updateEPICImageInformation(int state) {
    if (EPICDownloadLock) return;
    clearEPICImagesLabel();
    auto image = (state == 0 ? EPIC::getCurrentImage() : state == 1 ? EPIC::getNextImage() : EPIC::getPrevImage());
    if (!image) return;
    updateEPICImage();
    ui->EPICImageTitleLabel_->setText(image->getTitle());
    ui->EPICImageDescriptionTextEdit->clear();
    ui->EPICImageDescriptionTextEdit->append(image->getCaption());
    ui->EPICImageDateLabel_->setText(image->getDate());
    ui->EPICVersionLabel_->setText(image->getVersion());
    ui->EPICSunToEarthLabel_->setText(image->sunToEarth());
    ui->EPICDistanceToEarth_->setText(image->dscovrToEarth());
    ui->EPICDistanceToSunLabel_->setText(image->dscovrToSun());
    ui->EPICMoonToEarthLabel_->setText(image->moonToEarth());
} // 0 - current, 1 - next, 2 - prev

void MainWindow::updateEPICImage() {
    auto w = ui->EPICImageFrame->frameSize().width() - 96;
    auto h = ui->EPICImageFrame->frameSize().height() - 24;

    auto min = std::min(w, h);

    auto image = EPIC::getCurrentImage();
    if (!image) return;

    ui->EPICImageLabel->setFixedHeight(min);
    ui->EPICImageLabel->setFixedWidth(min);

    ui->EPICImageLabel->setPixmap(*image->getPixmap());
}

void MainWindow::on_EPICNextImageButton_clicked()
{
    updateEPICImageInformation(1);
}


void MainWindow::on_EPICPrevImageButton_clicked()
{
    updateEPICImageInformation(2);
}


void MainWindow::on_EPICImageTypeComboBox_currentIndexChanged(int index)
{
    setEPICWidgetsState(false);
    if (index == 0) {
        ui->EPICImageTypeButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bx-color.png")));
        fetchEPICJson(APIHandler::getEPICData_Request_URL(QString::fromStdString(config.find("api_key")->second),
                                                          QString::fromStdString(config.find("epic_json_url")->second),
                                                          E::eToQs(O::EPIC_NATURAL)),
                      O::EPIC_DATA,
                      E::eToQs(O::EPIC_NATURAL));
    } else {
        ui->EPICImageTypeButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bxs-color.png")));
        fetchEPICJson(APIHandler::getEPICData_Request_URL(QString::fromStdString(config.find("api_key")->second),
                                                          QString::fromStdString(config.find("epic_json_url")->second),
                                                          E::eToQs(O::EPIC_ENCHANCED)),
                      O::EPIC_DATA,
                      E::eToQs(O::EPIC_ENCHANCED));
    }
}

void MainWindow::updateEPICDataConstraints(const QDate* maxDate, const QDate* minDate) {
    ui->EPICDate->setMaximumDate(*maxDate);
    ui->EPICDate->setMinimumDate(*minDate);
    ui->EPICDateSlider->setMaximum(EPIC::getTotalDates() - 1);
}

void MainWindow::on_EPICDateSlider_sliderReleased()
{
    ui->EPICDate->setDate(*EPIC::getDate(ui->EPICDateSlider->value()));
}

void MainWindow::on_EPICPrevDateButton_clicked()
{
    if (ui->EPICDateSlider->value() > 0) ui->EPICDateSlider->setValue(ui->EPICDateSlider->value() - 1);
    on_EPICDateSlider_sliderReleased();
}

void MainWindow::on_EPICNextDateButton_clicked()
{
    if (ui->EPICDateSlider->value() < ui->EPICDateSlider->maximum()) ui->EPICDateSlider->setValue(ui->EPICDateSlider->value() + 1);
    on_EPICDateSlider_sliderReleased();
}

void MainWindow::on_Tabs_currentChanged(int index)
{
    resizeWelcomeImage();
    updateEPICImage();
}


void MainWindow::on_EPICAutoPlayButton_clicked()
{
    EPICAutoPlay = !EPICAutoPlay;
    if (EPICAutoPlay) ui->EPICAutoPlayButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bx-pause-circle.png")));
    else ui->EPICAutoPlayButton->setIcon(QIcon(QString::fromStdString(config.find("icons_path")->second + "bx-play-circle.png")));
}

void MainWindow::on_EPICTimerTimeout() {
    if (EPICAutoPlay) on_EPICNextImageButton_clicked();
}

void MainWindow::on_EPICAutoPlaySpeedSlider_valueChanged(int value)
{
    timer->setInterval(value);
}
