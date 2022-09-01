#ifndef PODCASTS_H
#define PODCASTS_H

#include <QtXml>
#include <QFile>
#include <string>
#include <vector>

class Podcasts
{
private:
    static std::vector<QDomDocument*> podcastDOMs;
public:
    Podcasts();

    static bool addPodcastFromXML(std::string fileName);
    static bool addPodcastFromXML(QString fileName);

    static bool loadPodcasts();

    static void clearPodcasts();
};

#endif // PODCASTS_H
