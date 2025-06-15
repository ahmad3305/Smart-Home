#include "../include/light.h"
#include <fstream>
#include <vector>
#include <string>

Light::Light() : Device(), brightness(100) {}


int Light::getBrightness() const { return brightness; }

void Light::setBrightness(int b) {
    brightness = b;
    updateDeviceFile();
}

void Light::updateDeviceFile(const std::string& lightFile) const {
    std::ifstream in(lightFile);
    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    while (getline(in, line)) {
        if (line.find("deviceId:") == 0) {
            std::string id = line.substr(9);
            id.erase(0, id.find_first_not_of(" \t"));
            if (id == deviceId) {
                lines.push_back("deviceId: " + deviceId);
                lines.push_back("brightness: " + std::to_string(brightness));
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
        lines.push_back("brightness: " + std::to_string(brightness));
    }

    std::ofstream out(lightFile, std::ios::trunc);
    for (const auto& l : lines)
        if (!l.empty()) out << l << std::endl;
}


std::map<std::string, Light*> Light::loadLightFeaturesFromFile(const std::string& filename) {
    std::map<std::string, Light*> lights;
    std::ifstream fin(filename);
    std::string line, deviceId, deviceName;
    int brightness = 0;
    while (std::getline(fin, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.rfind("deviceId:", 0) == 0) {
            if (!deviceId.empty()) {
                Light* light = new Light(deviceId, deviceName);
                light->setBrightness(brightness);
                lights[deviceId] = light;
            }
            deviceId = line.substr(9);
            deviceId.erase(0, deviceId.find_first_not_of(" \t\r\n"));
            deviceName = "";
            brightness = 0;
        }
        else if (line.rfind("brightness:", 0) == 0) {
            std::string bStr = line.substr(11);
            bStr.erase(0, bStr.find_first_not_of(" \t\r\n"));
            brightness = std::stoi(bStr);
        }
    }
    if (!deviceId.empty()) {
        Light* light = new Light(deviceId, deviceName);
        light->setBrightness(brightness);
        lights[deviceId] = light;
    }
    return lights;
}

void Light::addToFile() {
    std::ofstream fout("../data/light.txt", std::ios::app);
    fout << "deviceId: " << getDeviceId() << "\n"
        << "brightness: 30" << "\n";
    fout.close();
}