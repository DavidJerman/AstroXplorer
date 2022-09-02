#include "cfgloader.h"

std::map<std::string, std::string> CfgLoader::cfg = std::map<std::string, std::string> ();

CfgLoader::CfgLoader()
{

}

bool CfgLoader::loadConfig(std::string fileName) {
    std::fstream iStream (fileName);

    if (iStream.bad()) return false;

    if (!cfg.empty()) cfg.clear();

    std::string line;
    while (std::getline(iStream, line)) {
        std::string name, value;
        name = line.substr(0, line.find("="));
        value = line.substr(line.find("=") + 1);
        if (!name.empty() && !value.empty())
            cfg.insert(std::pair<std::string, std::string> (name, value));
    }

    if (cfg.empty()) return false;
    return true;
}

const std::map<std::string, std::string>& CfgLoader::getConfig() {
    return cfg;
}
