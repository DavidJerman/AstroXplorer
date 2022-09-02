#ifndef PODCASTS_H
#define PODCASTS_H

#include <QtXml>
#include <QFile>
#include <string>
#include <vector>
#include "podcast.h"

class Podcasts {
private:
    static std::vector<QDomDocument *> podcastDOMs;
    static std::vector<Podcast *> podcasts;
public:
    Podcasts();

    static bool addPodcastFromXML(std::string fileName);

    static bool addPodcastFromXML(QString fileName);

    static bool loadPodcasts();

    static void clearPodcastDOMs();

    static void clearPodcasts();

    static bool loadPodcastsFromSourceFolder(QString folder);

    static const std::vector<Podcast *> &getPodcasts();

    static Podcast *getPodcastById(unsigned int ID);

    static PodcastEpisode *getEpisodeById(unsigned int ID);
};

#endif // PODCASTS_H
