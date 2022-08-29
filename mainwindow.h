#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QPixmap>

#include <QFile>

#include <QListWidget>

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

    // Rover imagery
    void on_O_FHAZ_SOLS_Button_clicked();

    void on_O_FHAZ_DATE_Button_clicked();

private:
    // Rover imagery
    // GeneralisedFunctions
    void MarsRoverCamera_SetImages(QNetworkReply* reply, QString origin);
    void MarsRoverCamera_AddImageToContainer(QNetworkReply* reply, QListWidget *list); // TODO: Add the actual image container

    // Tab specific functions
    void O_FHAZ_SetImages(QNetworkReply* reply);

private:
    void updateWelcomeImage(QNetworkReply* reply);
    void updateWelcomeData(QNetworkReply *reply);

    // Resize handler
    void resizeEvent(QResizeEvent* event);

    void updateWelcomeVideo(const QUrl &videoUrl);
    void setWelcomeImageInformation(QJsonObject &jsonObj);

    void fetchAPIData(QUrl url, QString origin);

private:
    std::map<std::string, std::string> config;
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;

    const int CORNER_RADIUS;
};
#endif // MAINWINDOW_H
