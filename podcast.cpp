#include "podcast.h"

unsigned int Podcast::IDCounter = 0;

const QString &Podcast::getTitle() const {
    return title;
}

void Podcast::setTitle(const QString &newTitle) {
    title = newTitle;
}

const QString &Podcast::getDescription() const {
    return description;
}

void Podcast::setDescription(const QString &newDescription) {
    description = newDescription;
}

const QString &Podcast::getLink() const {
    return link;
}

void Podcast::setLink(const QString &newLink) {
    link = newLink;
}

const QString &Podcast::getLanguage() const {
    return language;
}

void Podcast::setLanguage(const QString &newLanguage) {
    language = newLanguage;
}

const QUrl Podcast::getImageUrl() const {
    return QUrl(imageUrl);
}

void Podcast::setImageUrl(const QString &newImageUrl) {
    imageUrl = newImageUrl;
}

unsigned int Podcast::getID() const {
    return ID;
}

void Podcast::setID(unsigned int newID) {
    ID = newID;
}

const QUrl Podcast::getSelfUrl() const
{
    return {selfUrl};
}

void Podcast::setSelfUrl(const QString &newSelfUrl)
{
    selfUrl = newSelfUrl;
}

Podcast::Podcast(const QString &title, const QString &description, const QString &link,
                 const QString &language, const QString &imageUrl, const QString& selfUrl)
        : title(title), description(description), link(link),
          language(language), imageUrl(imageUrl), selfUrl(selfUrl) {
    IDCounter++;
}

Podcast::Podcast() : ID(IDCounter) {
    IDCounter++;
}

Podcast::~Podcast() {
    clearEpisodes();
}

void Podcast::addEpisode(PodcastEpisode *episode) {
    episodes.push_back(episode);
}

void Podcast::clearEpisodes() {
    for (auto &episode: episodes) delete episode;
    episodes.clear();
}

const std::vector<PodcastEpisode *> &Podcast::getEpisodes() const {
    return episodes;
}
