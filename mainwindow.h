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

#include "enums.h"
#include "podcastepisode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRequestFinished(QNetworkReply *reply);
    void saveDataToFile(const QString filePath, const QByteArray& data);
    QString getValidFileName(QString fileName);
    QString getValidFileName(std::string fileName);

    // Rover imagery
    void on_O_FHAZ_SOLS_Button_clicked();
    void on_O_FHAZ_DATE_Button_clicked();
    void on_O_RHAZ_SOLS_Button_clicked();
    void on_O_RHAZ_DATE_Button_clicked();
    void on_S_RHAZ_DATE_Button_clicked();
    void on_S_RHAZ_SOLS_Button_clicked();
    void on_S_PANCAM_SOLS_Button_clicked();
    void on_S_PANCAM_DATE_Button_clicked();
    void on_S_NAVCAM_SOLS_Button_clicked();
    void on_S_NAVCAM_DATE_Button_clicked();
    void on_S_MINITES_SOLS_Button_clicked();
    void on_S_MINITES_DATE_Button_clicked();
    void on_S_FHAZ_SOLS_Button_clicked();
    void on_S_FHAZ_DATE_Button_clicked();
    void on_O_PANCAM_SOLS_Button_clicked();
    void on_O_PANCAM_DATE_Button_clicked();
    void on_O_NAVCAM_SOLS_Button_clicked();
    void on_O_NAVCAM_DATE_Button_clicked();
    void on_O_MINITES_SOLS_Button_clicked();
    void on_O_MINITES_DATE_Button_clicked();
    void on_C_NAVCAM_SOLS_Button_clicked();
    void on_C_NAVCAM_DATE_Button_clicked();
    void on_C_MAST_SOLS_Button_clicked();
    void on_C_MAST_DATE_Button_clicked();
    void on_C_MARDI_SOLS_Button_clicked();
    void on_C_MARDI_DATE_Button_clicked();
    void on_C_MAHLI_SOLS_Button_clicked();
    void on_C_MAHLI_DATE_Button_clicked();
    void on_C_FHAZ_SOLS_Button_clicked();
    void on_C_FHAZ_DATE_Button_clicked();
    void on_C_CHEMCAM_SOLS_Button_clicked();
    void on_C_CHEMCAM_DATE_Button_clicked();
    void on_C_RHAZ_SOLS_Button_clicked();
    void on_C_RHAZ_DATE_Button_clicked();

private:
    // Rover imagery
    // GeneralisedFunctions
    void MarsRoverCamera_SetImages(QNetworkReply* reply, ORIGIN origin);
    void MarsRoverCamera_AddImageToContainer(QNetworkReply* reply, QListWidget *list);

    // Tab specific functions
    void C_FHAZ_SetImages(QNetworkReply* reply);
    void C_RHAZ_SetImages(QNetworkReply* reply);
    void C_MAST_SetImages(QNetworkReply* reply);
    void C_CHEMCAM_SetImages(QNetworkReply* reply);
    void C_MAHLI_SetImages(QNetworkReply* reply);
    void C_MARDI_SetImages(QNetworkReply* reply);
    void C_NAVCAM_SetImages(QNetworkReply* reply);

    void O_FHAZ_SetImages(QNetworkReply* reply);
    void O_RHAZ_SetImages(QNetworkReply* reply);
    void O_NAVCAM_SetImages(QNetworkReply* reply);
    void O_PANCAM_SetImages(QNetworkReply* reply);
    void O_MINITES_SetImages(QNetworkReply* reply);

    void S_FHAZ_SetImages(QNetworkReply* reply);
    void S_RHAZ_SetImages(QNetworkReply* reply);
    void S_NAVCAM_SetImages(QNetworkReply* reply);
    void S_PANCAM_SetImages(QNetworkReply* reply);
    void S_MINITES_SetImages(QNetworkReply* reply);

    // Limit rover imagery input widget range
    void limitRoverImageryInputWidgetRanges(ORIGIN origin, QString maxSol, QString maxDate, QString landingDate);
    void limitCameraInputWidgetRanges(QSpinBox* solsWidget, QString& maxSol, QDateEdit* dateWidget,
                                      QString& maxDate, QString& landingDate);

    // Rover manifest
    void updateRoverManifest(QNetworkReply* reply, QListWidget* list, ORIGIN origin, QLabel* imageLabel);

private:
    void updateWelcomeImage(QNetworkReply* reply);
    void updateWelcomeData(QNetworkReply *reply);
    void resizeWelcomeImage();

    // Resize handler
    void resizeEvent(QResizeEvent* event);

    void updateWelcomeVideo(const QUrl &videoUrl);
    void setWelcomeImageInformation(QJsonObject &jsonObj);

    void fetchAPIData(QUrl url, ORIGIN origin);
    void fetchPodcastData(QUrl url, QString origin, QLabel* imageLabel);

    void updateStatus(QString msg);
    void popUpDialog(QString msg);

    // Podcasts
    void updatePodcastsList();
    void clearPodcastsList();
    void clearEpisodesList();
    void resetAudioControlsPane();
    void resetAudio();
    void playEpisode(PodcastEpisode*);
    void playNextEpisode(PodcastEpisode*);
    void playPrevEpisode(PodcastEpisode*);

private slots:
    void populateEpisodesList(QListWidgetItem* item);

    void playEpisode(QListWidgetItem* item);

    void onDurationChanged(qint64 duration);

    void onPositionChanged(qint64 position);

    void onPlaybackStateChanged(QMediaPlayer::PlaybackState);

    void imagePopUp(QListWidgetItem*);

    void on_LoadPodcastsButton_clicked();

    void on_AudioProgressBar_sliderPressed();

    void on_AudioProgressBar_sliderReleased();

    void on_PausePlayButton_clicked();

    void on_SkiptimeBackButton_clicked();

    void on_SkipTimeForwardButton_clicked();

    void on_NextEpButton_clicked();

    void on_PreviousEpButton_clicked();

private:
    std::map<std::string, std::string> config;
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;

    bool AudioProgressBarLocked {false};
    PodcastEpisode* episode {nullptr};

    const int CORNER_RADIUS;
};
#endif // MAINWINDOW_H
