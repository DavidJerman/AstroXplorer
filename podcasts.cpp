#include "podcasts.h"

std::vector<QDomDocument *> Podcasts::podcastDOMs;
std::vector<Podcast *> Podcasts::podcasts;
std::vector<QString> Podcasts::favEpisodes;

bool Podcasts::addPodcastFromXML(std::string fileName) {
    return addPodcastFromXML(QString::fromStdString(std::move(fileName)));
}

bool Podcasts::addPodcastFromXML(QString fileName) {
    QFile file(std::move(fileName));
    if (!file.open(QIODevice::ReadOnly)) return false;
    auto doc = new QDomDocument();
    doc->setContent(&file);
    if (!doc->hasChildNodes()) return false;
    podcastDOMs.push_back(doc);
    file.close();
    return true;
}

QDomDocument* Podcasts::XMLDataToDom(QByteArray& data) {
    auto dom = new QDomDocument();
    dom->setContent(data);
    if (!dom->hasChildNodes()) return nullptr;
    return dom;
}

Podcast* Podcasts::DomToPodcast(QDomDocument* dom) {

    auto root = dom->documentElement();
    if (root.tagName() != "rss") return nullptr;
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
        } else if (item.tagName() == "atom:link" && item.hasAttribute("rel") && item.attribute("rel") == "self") {
            podcast->setSelfUrl(item.attribute("href"));
        }
        item = item.nextSibling().toElement();
    }

    return podcast;
}

bool Podcasts::loadPodcastsFromSourceFolder(QString folder) {
    clearPodcasts();
    clearPodcastDOMs();
    QDir dir(folder);
    auto files = dir.entryList();
    for (const auto &file: files) addPodcastFromXML(std::move(folder) + file);
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

        auto podcast = DomToPodcast(dom);

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

bool Podcasts::setFavouriteEpisode(const PodcastEpisode& episode) {
    if (std::find(favEpisodes.begin(), favEpisodes.end(), episode.getMP3Url()) == favEpisodes.end()) {
        favEpisodes.push_back(episode.getMP3Url());
        return true;
    } else
        favEpisodes.erase(std::find(favEpisodes.begin(), favEpisodes.end(), episode.getMP3Url()));
    return false;
}

bool Podcasts::isFavouriteEpisode(const PodcastEpisode& episode) {
    return std::find(favEpisodes.begin(), favEpisodes.end(), episode.getMP3Url()) != favEpisodes.end();
}

void Podcasts::saveFavEpisodes(const QString& fileName) {
    std::ofstream stream (fileName.toStdString());
    if (!stream.is_open()) return;
    for (const auto &episode: favEpisodes)
        stream << episode.toStdString() << "\n";
    stream.close();
}

void Podcasts::loadFavEpisodes(const QString& fileName) {
    std::ifstream stream (fileName.toStdString());
    if (!stream.is_open()) return;
    favEpisodes.clear();
    std::string s;
    while (std::getline(stream, s)) {
        if (s.length() <= 0) continue;
        favEpisodes.push_back(QString::fromStdString(s));
    }
    stream.close();
}

bool Podcasts::matches(const QString& text, const PodcastEpisode* const episode) {
    auto s1 = text.toStdString();
    auto s2 = episode->getTitle().toStdString();
    return matches(std::move(s1), std::move(s2));
}

bool Podcasts::matches(const QString& text, const Podcast* const podcast) {
    auto s1 = text.toStdString();
    auto s2 = podcast->getTitle().toStdString();
    return matches(std::move(s1), std::move(s2));
}

bool Podcasts::matches(std::string pattern, std::string text) {
    for (auto &c: pattern) if (std::isupper(c)) c = std::tolower(c);
    for (auto &c: text) if (std::isupper(c)) c = std::tolower(c);
    if (text.length() < pattern.length()) return false;
    if (pattern.length() == 0 || text.length() == 0) return false;
    // Sunday algorithm

    // Generate BCH table
    std::vector<int> BCH (256, pattern.length() + 1);
    for (int i = 0; i < pattern.length(); i++) BCH[(unsigned char)pattern[i]] = (int)pattern.length() - i;

    // Search
    for (int i = 0; i < text.length();) {
        bool match = true;
        for (int j = 0; j < pattern.length(); j++) {
            if ((unsigned char)text[i + j] != (unsigned char)pattern[j]) {
                i += BCH[(unsigned char)text[i + pattern.length()]];
                match = false;
                break;
            }
        }
        if (match) {
            return true;
            i += (int)pattern.length();
        }
    }

    return false;
}
