#include "../include/fan.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

int Fan::getSpeed() const { return speed; }

void Fan::setSpeed(int s) {
    speed = s;
    Fan::updateDeviceFile();
}


void Fan::updateDeviceFile(const std::string& fanFile) const {
    std::ifstream in(fanFile);
    std::vector<std::string> lines;
    std::string line;
    bool found = false;
    while (getline(in, line)) {
        if (line.find("deviceId:") == 0) {
            std::string id = line.substr(9);
            id.erase(0, id.find_first_not_of(" \t"));
            if (id == deviceId) {
                lines.push_back("deviceId: " + deviceId);
                lines.push_back("speed: " + std::to_string(speed));
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
        lines.push_back("speed: " + std::to_string(speed));
    }
    std::ofstream out(fanFile, std::ios::trunc);
    for (const auto& l : lines) {
        if (!l.empty()) out << l << std::endl;
    }
}


std::map<std::string, Fan*> Fan::loadFanFeaturesFromFile(const std::string& filename) {
    std::map<std::string, Fan*> fans;
    std::ifstream fin(filename);

    if (!fin) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return fans;
    }

    std::string line, deviceId, deviceName;
    int speed = 0;

    while (std::getline(fin, line)) {
        line = trim_bothh(line);
        if (line.empty()) continue;

        if (line.rfind("deviceId:", 0) == 0) {
            if (!deviceId.empty()) {
                Fan* fan = new Fan(deviceId, deviceName);
                fan->setSpeed(speed);
                fans[deviceId] = fan;

            }

            deviceId = trim_bothh(line.substr(9));
            deviceName = ""; 
            speed = 0;
        }
        else if (line.rfind("speed:", 0) == 0) {
            try {
                speed = std::stoi(trim_bothh(line.substr(6)));
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid speed value in file " << filename << ": " << line << std::endl;
                speed = 0;
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Speed value out of range in file " << filename << ": " << line << std::endl;
                speed = 0;
            }
        }
    }

    if (!deviceId.empty()) {
        Fan* fan = new Fan(deviceId, deviceName);
        fan->setSpeed(speed);
        fans[deviceId] = fan;

    }

    return fans;
}

void Fan::addToFile() {
    std::ofstream fout("data/fan.txt", std::ios::app);
    fout << "deviceId: " << getDeviceId() << "\n"
        << "speed: 3" << "\n";
    fout.close();
}