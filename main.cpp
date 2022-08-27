#include "mainwindow.h"

#include <QApplication>

#include <cfgloader.h>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    auto set = CfgLoader::loadConfig("config");
    auto res = CfgLoader::getConfig();
    for (const auto &key: res) {
        std::cout << "Key: " << key.first << " value: " << key.second << std::endl;
    }

    return a.exec();
}
