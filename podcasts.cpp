#include "podcasts.h"

std::vector<QDomDocument *> Podcasts::podcastDOMs;
std::vector<Podcast *> Podcasts::podcasts;

Podcasts::Podcasts() {

}

bool Podcasts::addPodcastFromXML(std::string fileName) {
    return addPodcastFromXML(QString::fromStdString(fileName));
}

bool Podcasts::addPodcastFromXML(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return false;
    auto doc = new QDomDocument();
    doc->setContent(&file);
    if (!doc->hasChildNodes()) return false;
    podcastDOMs.push_back(doc);
    file.close();
    return true;
}

bool Podcasts::loadPodcastsFromSourceFolder(QString folder) {
    clearPodcasts();
    clearPodcastDOMs();
    QDir dir(folder);
    auto files = dir.entryList();
    for (const auto &file: files) addPodcastFromXML(folder + file);
    return loadPodcasts();
}

void Podcasts::clearPodcastDOMs() {
    for (auto &e: podcastDOMs) delete e;
    podcastDOMs.clear();
}

void Podcasts::clearPodcasts() {
    for (auto &e: podcasts) delete e;
    podcasts.clear();
}

const std::vector<Podcast *> &Podcasts::getPodcasts() {
    return podcasts;
}

bool Podcasts::loadPodcasts() {
    if (podcastDOMs.empty()) return false;

    for (const auto &dom: podcastDOMs) {

        auto root = dom->documentElement();
        if (root.tagName() != "rss") continue;
        auto podcastEl = root.firstChild().toElement();

        // Data about the podcast
        Podcast *podcast = new Podcast();

        auto item = podcastEl.firstChild().toElement();
        while (!item.isNull()) {
            // Get info about the podcast
            if (item.tagName() == "title") podcast->setTitle(item.text());
            else if (item.tagName() == "description") podcast->setDescription(item.text());
            else if (item.tagName() == "link") podcast->setLink(item.text());
            else if (item.tagName() == "language") podcast->setLanguage(item.text());
            else if (item.tagName() == "image") {
                auto subItem = item.firstChild().toElement();
                while (!subItem.isNull()) {
                    if (subItem.tagName() == "url") {
                        podcast->setImageUrl(subItem.text());
                        break;
                    }
                    subItem = subItem.nextSibling().toElement();
                }
            } else if (item.tagName() == "item") {
                // Get info on the episode
                PodcastEpisode *episode = new PodcastEpisode(podcast->getID());
                auto subItem = item.firstChild().toElement();
                while (!subItem.isNull()) {
                    if (subItem.tagName() == "title") episode->setTitle(subItem.text());
                    else if (subItem.tagName() == "link") episode->setLink(subItem.text());
                    else if (subItem.tagName() == "description") episode->setDescription(subItem.text());
                    else if (subItem.tagName() == "enclosure") episode->setMP3Url(subItem.attribute("url"));
                    else if (subItem.tagName() == "guid") episode->setWebUrl(subItem.text());
                    else if (subItem.tagName() == "pubDate") episode->setDate(subItem.text());
                    else if (subItem.tagName() == "source") episode->setRssSource(subItem.attribute("url"));
                    subItem = subItem.nextSibling().toElement();
                }
                podcast->addEpisode(episode);
            }
            item = item.nextSibling().toElement();
        }

        podcasts.push_back(podcast);
    }

    return true;
}

Podcast *Podcasts::getPodcastById(unsigned int ID) {
    for (auto &podcast: podcasts)
        if (podcast->getID() == ID)
            return podcast;
    return {};
}

PodcastEpisode *Podcasts::getEpisodeById(unsigned int ID) {
    for (const auto &podcast: podcasts)
        for (const auto &episode: podcast->getEpisodes())
            if (episode->getID() == ID)
                return episode;
    return {};

}
