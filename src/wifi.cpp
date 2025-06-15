#include "../include/wifi.h"
#include <fstream>
#include <vector>
#include <string>

Wifi::Wifi() : Device(), ssid("default_ssid") {}

std::string Wifi::getSSID() const { return ssid; }

void Wifi::setSSID(const std::string& s) {
    ssid = s;
    updateDeviceFile();
}

void Wifi::updateDeviceFile(const std::string& wifiFile) const {
    std::ifstream in(wifiFile);
    std::vector<std::string> lines;
    std::string line;
    bool found = false;
    while (getline(in, line)) {
        if (line.find("deviceId:") == 0) {
            std::string id = line.substr(9);
            id.erase(0, id.find_first_not_of(" \t"));
            if (id == deviceId) {
                lines.push_back("deviceId: " + deviceId);
                lines.push_back("ssid: " + ssid);
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
        lines.push_back("ssid: " + ssid);
    }
    std::ofstream out(wifiFile, std::ios::trunc);
    for (const auto& l : lines)
        if (!l.empty()) out << l << std::endl;
}

std::map<std::string, Wifi*> Wifi::loadWifiFeaturesFromFile(const std::string& filename) {
    std::map<std::string, Wifi*> wifis;
    std::ifstream fin(filename);
    std::string line, deviceId, deviceName, ssid;
    while (std::getline(fin, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.rfind("deviceId:", 0) == 0) {
            if (!deviceId.empty()) {
                Wifi* wifi = new Wifi(deviceId, deviceName);
                wifi->setSSID(ssid);
                wifis[deviceId] = wifi;
            }
            deviceId = line.substr(9);
            deviceId.erase(0, deviceId.find_first_not_of(" \t\r\n"));
            deviceName = "";
            ssid = "";
        }
        else if (line.rfind("ssid:", 0) == 0) {
            ssid = line.substr(5);
            ssid.erase(0, ssid.find_first_not_of(" \t\r\n"));
        }
    }
    if (!deviceId.empty()) {
        Wifi* wifi = new Wifi(deviceId, deviceName);
        wifi->setSSID(ssid);
        wifis[deviceId] = wifi;
    }
    return wifis;
}

void Wifi::addToFile() {
    std::ofstream fout("../data/wifi.txt", std::ios::app);
    fout << "deviceId: " << getDeviceId() << "\n"
        << "ssid: " << getSSID() << "\n";
    fout.close();
}