#include "podcastepisode.h"

unsigned int PodcastEpisode::IDCounter = 0;

const QString &PodcastEpisode::getTitle() const
{
    return title;
}

void PodcastEpisode::setTitle(const QString &newTitle)
{
    title = newTitle;
}

const QString &PodcastEpisode::getLink() const
{
    return link;
}

void PodcastEpisode::setLink(const QString &newLink)
{
    link = newLink;
}

const QString &PodcastEpisode::getDescription() const
{
    return description;
}

void PodcastEpisode::setDescription(const QString &newDescription)
{
    description = newDescription;
}

const QString &PodcastEpisode::getMP3Url() const
{
    return MP3Url;
}

void PodcastEpisode::setMP3Url(const QString &newMP3Url)
{
    MP3Url = newMP3Url;
}

const QString &PodcastEpisode::getWebUrl() const
{
    return webUrl;
}

void PodcastEpisode::setWebUrl(const QString &newWebUrl)
{
    webUrl = newWebUrl;
}

const QString &PodcastEpisode::getDate() const
{
    return date;
}

void PodcastEpisode::setDate(const QString &newDate)
{
    date = newDate;
}

const QString &PodcastEpisode::getRssSource() const
{
    return rssSource;
}

void PodcastEpisode::setRssSource(const QString &newRssSource)
{
    rssSource = newRssSource;
}

unsigned int PodcastEpisode::getID() const
{
    return ID;
}

void PodcastEpisode::setID(unsigned int newID)
{
    ID = newID;
}

PodcastEpisode::PodcastEpisode(QString& title, QString& link, QString& description, QString& MP3Url,
                               QString& webUrl, QString& date, QString& rssSource)
    : title(title), link(link), description(description), MP3Url(MP3Url),
      webUrl(webUrl), date(date), rssSource(rssSource)
{

}
