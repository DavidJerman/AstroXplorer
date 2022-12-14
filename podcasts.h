#ifndef PODCASTS_H
#define PODCASTS_H

#include <QtXml>
#include <QFile>
#include <string>
#include <vector>
#include <fstream>
#include "podcast.h"

class Podcasts {
private:
    static std::vector<QDomDocument *> podcastDOMs;
    static std::vector<Podcast *> podcasts;
    static std::vector<QString> favEpisodes;
public:
    Podcasts() = default;

    static bool addPodcastFromXML(std::string fileName);

    static bool addPodcastFromXML(QString fileName);

    static bool loadPodcasts();

    static void clearPodcastDOMs();

    static void clearPodcasts();

    static bool loadPodcastsFromSourceFolder(QString folder);

    static const std::vector<Podcast *> &getPodcasts();

    static Podcast *getPodcastById(unsigned int ID);

    static PodcastEpisode *getEpisodeById(unsigned int ID);

    static QDomDocument* XMLDataToDom(QByteArray& data);

    static Podcast* DomToPodcast(QDomDocument* dom);

    static bool setFavouriteEpisode(const PodcastEpisode&);

    static bool isFavouriteEpisode(const PodcastEpisode&);

    static void saveFavEpisodes(const QString& fileName);

    static void loadFavEpisodes(const QString &fileName);

    static bool matches(const QString& text, const PodcastEpisode* const episode);

    static bool matches(const QString& text, const Podcast* const podcast);

private:
    static bool matches(std::string s1, std::string s2);
};

#endif // PODCASTS_H
