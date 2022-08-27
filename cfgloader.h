#ifndef CFGLOADER_H
#define CFGLOADER_H

#include <fstream>
#include <map>
#include <string>

class CfgLoader
{
private:
    static std::map<std::string, std::string> cfg;
public:
    CfgLoader();

    static bool loadConfig(std::string);
    static std::map<std::string, std::string> getConfig();
};

#endif // CFGLOADER_H
