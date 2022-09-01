#ifndef PODCAST_H
#define PODCAST_H

#include "podcastepisode.h"
#include <vector>

class Podcast
{
private:
    std::vector<PodcastEpisode*> episodes;
    QString title, description, link, language, imageUrl;
public:
    Podcast(QString& title, QString& description, QString& link,
            QString& language, QString& imageUrl);
    Podcast() = default;

    ~Podcast();

    void addEpisode(PodcastEpisode* episode);

    void clearEpisodes();

    const std::vector<PodcastEpisode*>& getEpisodes() const;

    const QString &getTitle() const;
    void setTitle(const QString &newTitle);
    const QString &getDescription() const;
    void setDescription(const QString &newDescription);
    const QString &getLink() const;
    void setLink(const QString &newLink);
    const QString &getLanguage() const;
    void setLanguage(const QString &newLanguage);
    const QString &getImageUrl() const;
    void setImageUrl(const QString &newImageUrl);
};

#endif // PODCAST_H
