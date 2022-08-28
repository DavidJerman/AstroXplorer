#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QPixmap>

#include <QFile>

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
    void updateWelcomeImage(QNetworkReply* reply);
    void updateWelcomeData(QNetworkReply *reply);

private:
    // Resize handler
    void resizeEvent(QResizeEvent* event);

    void updateWelcomeVideo(const QUrl &videoUrl);
    void setWelcomeImageInformation(QJsonObject &jsonObj);

private:
    std::map<std::string, std::string> config;
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;

    const int CORNER_RADIUS;
};
#endif // MAINWINDOW_H
