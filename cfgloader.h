#ifndef CFGLOADER_H
#define CFGLOADER_H

#include <fstream>
#include <map>
#include <string>
#include <QString>

class CfgLoader {
private:
    static std::map<std::string, std::string> cfg;
public:
    CfgLoader() = default;

    static bool loadConfig(std::string);

    static const std::map<std::string, std::string> &getConfig();
};

#endif // CFGLOADER_H
