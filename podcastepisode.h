#ifndef PODCASTEPISODE_H
#define PODCASTEPISODE_H

#include <QString>

struct PodcastEpisode
{
private:
    QString title, link, description, MP3Url, webUrl, date, rssSource;
    static unsigned int IDCounter;
    unsigned int ID;
public:
    PodcastEpisode(QString& title, QString& link, QString& description, QString& MP3Url,
                   QString& webUrl, QString& date, QString& rssSource);
    PodcastEpisode() = default;


    const QString &getTitle() const;
    void setTitle(const QString &newTitle);
    const QString &getLink() const;
    void setLink(const QString &newLink);
    const QString &getDescription() const;
    void setDescription(const QString &newDescription);
    const QString &getMP3Url() const;
    void setMP3Url(const QString &newMP3Url);
    const QString &getWebUrl() const;
    void setWebUrl(const QString &newWebUrl);
    const QString &getDate() const;
    void setDate(const QString &newDate);
    const QString &getRssSource() const;
    void setRssSource(const QString &newRssSource);

    unsigned int getID() const;
    void setID(unsigned int newID);
};

#endif // PODCASTEPISODE_H
