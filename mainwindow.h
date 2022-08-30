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
    void MarsRoverCamera_SetImages(QNetworkReply* reply, QString origin);
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

private:
    void updateWelcomeImage(QNetworkReply* reply);
    void updateWelcomeData(QNetworkReply *reply);

    // Resize handler
    void resizeEvent(QResizeEvent* event);

    void updateWelcomeVideo(const QUrl &videoUrl);
    void setWelcomeImageInformation(QJsonObject &jsonObj);

    void fetchAPIData(QUrl url, QString origin);

    void updateStatus(QString msg);
    void popUpDialog(QString msg);

private slots:
    void imagePopUp(QListWidgetItem*);

private:
    std::map<std::string, std::string> config;
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;

    const int CORNER_RADIUS;
};
#endif // MAINWINDOW_H
