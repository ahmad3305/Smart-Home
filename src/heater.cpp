#include "../include/heater.h"
#include <fstream>
#include <vector>
#include <string>

Heater::Heater() : Device(), temperature(22) {}

int Heater::getTemperature() const { return temperature; }

void Heater::setTemperature(int t) {
    temperature = t;
    Heater::updateDeviceFile();
}

void Heater::updateDeviceFile(const std::string& heaterFile) const {
    std::ifstream in(heaterFile);
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
    std::ofstream out(heaterFile, std::ios::trunc);
    for (const auto& l : lines)
        if (!l.empty()) out << l << std::endl;
}

std::map<std::string, Heater*> Heater::loadHeaterFeaturesFromFile(const std::string& filename) {
    std::map<std::string, Heater*> heaters;
    std::ifstream fin(filename);
    std::string line, deviceId, deviceName;
    int temperature = 0;
    while (std::getline(fin, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.rfind("deviceId:", 0) == 0) {
            if (!deviceId.empty()) {
                Heater* heater = new Heater(deviceId, deviceName);
                heater->setTemperature(temperature);
                heaters[deviceId] = heater;
            }
            deviceId = line.substr(9);
            deviceId.erase(0, deviceId.find_first_not_of(" \t\r\n"));
            deviceName = ""; // Optionally parse name if present
            temperature = 0;
        }
        else if (line.rfind("temperature:", 0) == 0) {
            std::string tempStr = line.substr(12);
            tempStr.erase(0, tempStr.find_first_not_of(" \t\r\n"));
            temperature = std::stoi(tempStr);
        }
    }
    if (!deviceId.empty()) {
        Heater* heater = new Heater(deviceId, deviceName);
        heater->setTemperature(temperature);
        heaters[deviceId] = heater;
    }
    return heaters;
}

void Heater::addToFile() {
    std::ofstream fout("data/heater.txt", std::ios::app);
    fout << "deviceId: " << getDeviceId() << "\n"
        << "temperature: 25" << "\n";
    fout.close();
}