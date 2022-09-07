#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QPixmap>
#include <QFile>
#include <QListWidget>
#include <QMessageBox>

#include <QSpinBox>
#include <QDateEdit>
#include <QListWidgetItem>

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>

#include <QTimer>

#include "enums.h"
#include "podcastepisode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    bool appVariablesReset();

    bool appSetup();

    bool appNetworkContentSetup();

private slots:
    // Requests and data
    void onRequestFinished(QNetworkReply * reply);

private:
    static void saveDataToFile(QString filePath, const QByteArray &data);

    static QString getValidFileName(QString fileName);

    static QString getValidFileName(std::string fileName);

private slots:
    // Rover imagery
    const void on_O_FHAZ_SOLS_Button_clicked() const;

    const void on_O_FHAZ_DATE_Button_clicked() const;

    const void on_O_RHAZ_SOLS_Button_clicked() const;

    const void on_O_RHAZ_DATE_Button_clicked() const;

    const void on_S_RHAZ_DATE_Button_clicked() const;

    const void on_S_RHAZ_SOLS_Button_clicked() const;

    const void on_S_PANCAM_SOLS_Button_clicked() const;

    const void on_S_PANCAM_DATE_Button_clicked() const;

    const void on_S_NAVCAM_SOLS_Button_clicked() const;

    const void on_S_NAVCAM_DATE_Button_clicked() const;

    const void on_S_MINITES_SOLS_Button_clicked() const;

    const void on_S_MINITES_DATE_Button_clicked() const;

    const void on_S_FHAZ_SOLS_Button_clicked() const;

    const void on_S_FHAZ_DATE_Button_clicked() const;

    const void on_O_PANCAM_SOLS_Button_clicked() const;

    const void on_O_PANCAM_DATE_Button_clicked() const;

    const void on_O_NAVCAM_SOLS_Button_clicked() const;

    const void on_O_NAVCAM_DATE_Button_clicked() const;

    const void on_O_MINITES_SOLS_Button_clicked() const;

    const void on_O_MINITES_DATE_Button_clicked() const;

    const void on_C_NAVCAM_SOLS_Button_clicked() const;

    const void on_C_NAVCAM_DATE_Button_clicked() const;

    const void on_C_MAST_SOLS_Button_clicked() const;

    const void on_C_MAST_DATE_Button_clicked() const;

    const void on_C_MARDI_SOLS_Button_clicked() const;

    const void on_C_MARDI_DATE_Button_clicked() const;

    const void on_C_MAHLI_SOLS_Button_clicked() const;

    const void on_C_MAHLI_DATE_Button_clicked() const;

    const void on_C_FHAZ_SOLS_Button_clicked() const;

    const void on_C_FHAZ_DATE_Button_clicked() const;

    const void on_C_CHEMCAM_SOLS_Button_clicked() const;

    const void on_C_CHEMCAM_DATE_Button_clicked() const;

    const void on_C_RHAZ_SOLS_Button_clicked() const;

    const void on_C_RHAZ_DATE_Button_clicked() const;

private:
    // Rover imagery
    // GeneralisedFunctions
    const void MarsRoverCamera_SetImages(QNetworkReply *reply, ORIGIN origin) const;

    const void MarsRoverCamera_AddImageToContainer(QNetworkReply *reply, QListWidget *list) const;

    // Tab specific functions
    const void C_FHAZ_SetImages(QNetworkReply *reply) const;

    const void C_RHAZ_SetImages(QNetworkReply *reply) const;

    const void C_MAST_SetImages(QNetworkReply *reply) const;

    const void C_CHEMCAM_SetImages(QNetworkReply *reply) const;

    const void C_MAHLI_SetImages(QNetworkReply *reply) const;

    const void C_MARDI_SetImages(QNetworkReply *reply) const;

    const void C_NAVCAM_SetImages(QNetworkReply *reply) const;

    const void O_FHAZ_SetImages(QNetworkReply *reply) const;

    const void O_RHAZ_SetImages(QNetworkReply *reply) const;

    const void O_NAVCAM_SetImages(QNetworkReply *reply) const;

    const void O_PANCAM_SetImages(QNetworkReply *reply) const;

    const void O_MINITES_SetImages(QNetworkReply *reply) const;

    const void S_FHAZ_SetImages(QNetworkReply *reply) const;

    const void S_RHAZ_SetImages(QNetworkReply *reply) const;

    const void S_NAVCAM_SetImages(QNetworkReply *reply) const;

    const void S_PANCAM_SetImages(QNetworkReply *reply) const;

    const void S_MINITES_SetImages(QNetworkReply *reply) const;

    // Limit rover imagery input widget range
    const void limitRoverImageryInputWidgetRanges(ORIGIN origin, QString maxSol, QString maxDate, QString landingDate) const;

    static void limitCameraInputWidgetRanges(QSpinBox *solsWidget, const QString &maxSol, QDateEdit *dateWidget,
                                             const QString &maxDate, const QString &landingDate);

    // Rover manifest
    const void updateRoverManifest(QNetworkReply *reply, QListWidget *list, ORIGIN origin, QLabel *imageLabel) const;

private:
    const void updateWelcomeImage(QNetworkReply *reply) const;

    const void updateWelcomeData(QNetworkReply *reply) const;

    const void resizeWelcomeImage() const;

    // Resize handler
    void resizeEvent(QResizeEvent *event);

    // Other
    const void updateWelcomeVideo(const QUrl &videoUrl) const;

    const void setWelcomeImageInformation(const QJsonObject &jsonObj) const;

    // Fetching data
    const void fetchAPIData(QUrl url, ORIGIN origin) const;

    const void fetchPodcastData(QUrl url, QString origin, QLabel *imageLabel, unsigned int SIZE) const;

    const void fetchImages(QUrl url, QString origin, int sol, QString rover, QString camera) const;

    const void fetchImage(QUrl url, QString filePath) const;

    // Information
    const void updateStatus(QString msg) const;

    const void popUpDialog(QString msg) const;

    // Downloads
    const void downloadImages(const QString &checkBoxTitle, unsigned int sol) const;

    const void downloadImage(const QString &imgSource, const QString &rover, const QString &camera, unsigned int sol,
                       unsigned int ID) const;

    // Podcasts
    const void updatePodcastsList(QString search = "", bool isSearch = false) const;

    const void clearPodcastsList() const;

    const void clearEpisodesList() const;

    const void resetAudioControlsPane() const;

    const void resetAudio() const;

    void playEpisode(PodcastEpisode *);

    void playNextEpisode(PodcastEpisode *);

    void playPrevEpisode(PodcastEpisode *);

    const void setButtonToPlay(bool) const;

    const void updateLocalPodcats() const;

    const void updateHeartButtonIcon(PodcastEpisode*) const;

    // EPIC
    const void fetchEPICJson(QUrl url, ORIGIN origin, QString type) const;

    const void fetchEPICImage(QUrl url, ORIGIN origin, QString title, QString date, QString caption, QString version, const unsigned int count,
                        double lat, double lon,
                        double dscovrX, double dscovrY, double dscovrZ,
                        double lunarX, double lunarY, double lunarZ,
                        double sunX, double sunY, double sunZ) const;

    void setEPICWidgetsState(bool enable);

    const void clearEPICImagesLabel() const;

    const void updateEPICImageInformation(int state) const; // 0 - current, 1 - next, 2 - prev

    const void updateEPICDataConstraints(const QDate* const maxDate, const QDate* const minDate) const;

    const void updateEPICImage() const;

    // CfgLoder extension
    const QString getCfgValueQ(std::string key) const;

    const std::string getCfgValue(std::string key) const;

    const QString getFontQ(std::string key) const;

    const std::string getFont(std::string key) const;

    const unsigned int getFSize(std::string key) const;


private slots:
    void populateEpisodesList(QListWidgetItem * item, bool fav = false, QString search = "", bool isSearch = false);

    void playEpisode(QListWidgetItem *item);

    void onDurationChanged(qint64 duration);

    void onPositionChanged(qint64 position);

    void onPlaybackStateChanged(QMediaPlayer::PlaybackState);

    void imagePopUp(QListWidgetItem *);

    void on_AudioProgressBar_sliderPressed();

    void on_AudioProgressBar_sliderReleased();

    void on_PausePlayButton_clicked();

    void on_SkiptimeBackButton_clicked();

    void on_SkipTimeForwardButton_clicked();

    void on_NextEpButton_clicked();

    void on_PreviousEpButton_clicked();

    void on_DownloadsDownloadButton_clicked();

    void on_AutoPlayButton_clicked();

    void on_HeartButton_clicked();

    void on_UpdatePodcastsButton_clicked();

    void on_LoadPodcastsButton_clicked();

    void on_FavoritesButton_clicked();

    void on_EPICSearchButton_clicked();

    void on_EPICNextImageButton_clicked();

    void on_EPICPrevImageButton_clicked();

    void on_EPICImageTypeComboBox_currentIndexChanged(int index);

    void on_EPICDateSlider_sliderReleased();

    void on_EPICPrevDateButton_clicked();

    void on_EPICNextDateButton_clicked();

    void on_Tabs_currentChanged(int index);

    void on_EPICAutoPlayButton_clicked();

    void on_EPICTimerTimeout();

    void on_EPICAutoPlaySpeedSlider_valueChanged(int value);

    void on_RefreshAppButton_clicked();

    void on_SearchPodcastButton_clicked();

    void on_SearchEpisodeButton_clicked();

private:
    std::map <std::string, std::string> config;
    std::map <std::string, std::string> fontCfg;

    Ui::MainWindow *ui;
    QNetworkAccessManager *manager{nullptr};
    QMediaPlayer *mediaPlayer{nullptr};
    QAudioOutput *audioOutput{nullptr};

    bool AudioProgressBarLocked{false};
    bool AudioAutoPlay{false};
    PodcastEpisode *episode{nullptr};
    unsigned int PID{(unsigned int)(-1)};
    bool FavoriteEpisode{false};
    unsigned int EPICDownloadCount{0};
    bool EPICDownloadLock{true};
    bool EPICAutoPlay{false};
    QTimer* timer{nullptr};

    static constexpr int CORNER_RADIUS {25};
};
#endif // MAINWINDOW_H
