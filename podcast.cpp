#include "podcast.h"

const QString &Podcast::getTitle() const
{
    return title;
}

void Podcast::setTitle(const QString &newTitle)
{
    title = newTitle;
}

const QString &Podcast::getDescription() const
{
    return description;
}

void Podcast::setDescription(const QString &newDescription)
{
    description = newDescription;
}

const QString &Podcast::getLink() const
{
    return link;
}

void Podcast::setLink(const QString &newLink)
{
    link = newLink;
}

const QString &Podcast::getLanguage() const
{
    return language;
}

void Podcast::setLanguage(const QString &newLanguage)
{
    language = newLanguage;
}

const QString &Podcast::getImageUrl() const
{
    return imageUrl;
}

void Podcast::setImageUrl(const QString &newImageUrl)
{
    imageUrl = newImageUrl;
}

Podcast::Podcast(QString& title, QString& description, QString& link,
                 QString& language, QString& imageUrl)
    : title(title), description(description), link(link),
      language(language), imageUrl(imageUrl)
{

}

Podcast::~Podcast() {
    clearEpisodes();
}

void Podcast::addEpisode(PodcastEpisode* episode) {
    episodes.push_back(episode);
}

void Podcast::clearEpisodes() {
    for (auto &episode: episodes) delete episode;
    episodes.clear();
}

const std::vector<PodcastEpisode*>& Podcast::getEpisodes() const {
    return episodes;
}
