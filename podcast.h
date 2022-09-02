#ifndef PODCAST_H
#define PODCAST_H

#include "podcastepisode.h"
#include <vector>
#include <QUrl>

class Podcast {
private:
    std::vector<PodcastEpisode *> episodes;
    QString title, description, link, language, imageUrl;
    static unsigned int IDCounter;
    unsigned int ID;
public:
    Podcast(QString &title, QString &description, QString &link,
            QString &language, QString &imageUrl);

    Podcast();

    ~Podcast();

    void addEpisode(PodcastEpisode *episode);

    void clearEpisodes();

    const std::vector<PodcastEpisode *> &getEpisodes() const;

    const QString &getTitle() const;

    void setTitle(const QString &newTitle);

    const QString &getDescription() const;

    void setDescription(const QString &newDescription);

    const QString &getLink() const;

    void setLink(const QString &newLink);

    const QString &getLanguage() const;

    void setLanguage(const QString &newLanguage);

    const QUrl getImageUrl() const;

    void setImageUrl(const QString &newImageUrl);

    unsigned int getID() const;

    void setID(unsigned int newID);
};

#endif // PODCAST_H
