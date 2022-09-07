#ifndef PODCAST_H
#define PODCAST_H

#include "podcastepisode.h"
#include <vector>
#include <QUrl>

class Podcast {
private:
    std::vector<PodcastEpisode *> episodes;
    QString title, description, link, language, imageUrl, selfUrl;
    static unsigned int IDCounter;
    unsigned int ID;
public:
    Podcast(const QString &title, const QString &description, const QString &link,
            const QString &language, const QString &imageUrl, const QString& selfUrl);

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

    const QUrl getSelfUrl() const;

    void setSelfUrl(const QString &newSelfUrl);
};

#endif // PODCAST_H
