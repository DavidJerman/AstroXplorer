#include "podcasts.h"

std::vector<QDomDocument*> Podcasts::podcastDOMs;

Podcasts::Podcasts()
{

}

bool Podcasts::addPodcastFromXML(std::string fileName) {
    return addPodcastFromXML(QString::fromStdString(fileName));
}

bool Podcasts::addPodcastFromXML(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return false;
    auto doc = new QDomDocument();
    doc->setContent(&file);
    podcastDOMs.push_back(doc);
    file.close();
    return true;
}

void Podcasts::clearPodcasts() {
    for (auto &e: podcastDOMs) delete e;
    podcastDOMs.clear();
}

bool Podcasts::loadPodcasts() {
    if (podcastDOMs.empty()) return false;

    return true;
}
