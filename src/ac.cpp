#include "../include/ac.h"
#include <fstream>
#include <vector>
#include <string>

AC::AC() : Device(), temperature(24) {}


void AC::setTemperature(int t) {
    temperature = t;
    AC::updateDeviceFile();
}


void AC::updateDeviceFile(const std::string& acFile) const {
    std::ifstream in(acFile);
    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    while (getline(in, line)) {
        if (line.find("deviceId:") == 0) {
            std::string id = line.substr(9);
            id.erase(0, id.find_first_not_of(" \t"));
            if (id == deviceId) {
                lines.push_back("deviceId: " + deviceId);
                lines.push_back("temperature: " + std::to_string(temperature));
                found = true;
                getline(in, line);
                continue;
            }
        }
        lines.push_back(line);
    }
    in.close();

    if (!found) {
        lines.push_back("deviceId: " + deviceId);
        lines.push_back("temperature: " + std::to_string(temperature));
    }

    std::ofstream out(acFile, std::ios::trunc);
    for (const auto& l : lines)
        if (!l.empty()) out << l << std::endl;
}

std::map<std::string, AC*> AC::loadACFeaturesFromFile(const std::string& filename) {
    std::map<std::string, AC*> acs;
    std::ifstream fin(filename);
    std::string line, deviceId, deviceName;
    int temperature = 0;
    while (std::getline(fin, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.rfind("deviceId:", 0) == 0) {
            if (!deviceId.empty()) {
                AC* ac = new AC(deviceId, deviceName);
                ac->setTemperature(temperature);
                acs[deviceId] = ac;
            }
            deviceId = line.substr(9);
            deviceId.erase(0, deviceId.find_first_not_of(" \t\r\n"));
            deviceName = "";
            temperature = 0;
        }
        else if (line.rfind("temperature:", 0) == 0) {
            std::string tempStr = line.substr(12);
            tempStr.erase(0, tempStr.find_first_not_of(" \t\r\n"));
            temperature = std::stoi(tempStr);
        }
    }
    if (!deviceId.empty()) {
        AC* ac = new AC(deviceId, deviceName);
        ac->setTemperature(temperature);
        acs[deviceId] = ac;
    }
    return acs;
}

void AC::addToFile() {
    std::ofstream fout("data/ac.txt", std::ios::app);
    fout << "deviceId: " << getDeviceId() << "\n"
        << "temperature: 20" << "\n";
    fout.close();
}