#ifndef PODCASTEPISODE_H
#define PODCASTEPISODE_H

#include <QString>

struct PodcastEpisode {
private:
    QString title, link, description, MP3Url, webUrl, date, rssSource;
    static unsigned int IDCounter;
    unsigned int ID;
    unsigned int PID;
public:
    PodcastEpisode(const QString &title, const QString &link, const QString &description, const QString &MP3Url,
                   const QString &webUrl, const QString &date, const QString &rssSource, unsigned int PID);

    PodcastEpisode(unsigned int PID);

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

    unsigned int getPID() const;
};

#endif // PODCASTEPISODE_H
