#include <fstream>
#include <sstream>
#include <string>
#include "../include/energyUsage.h"
#include <vector>

EnergyUsage::EnergyUsage()
    : usageId(""), deviceId(""), energyConsumed(0.0), cost(0.0) {
}

EnergyUsage::EnergyUsage(const std::string& usageId, const std::string& deviceId)
    : usageId(usageId), deviceId(deviceId), energyConsumed(0.0), cost(0.0) {
}

void EnergyUsage::setOnTimestamp() {
    onTimestamp = std::chrono::system_clock::now();
}

void EnergyUsage::setOffTimestamp() {
    offTimestamp = std::chrono::system_clock::now();
}

void EnergyUsage::setOnTimestamp(const std::chrono::system_clock::time_point& tp) {
    onTimestamp = tp;
}

void EnergyUsage::setOffTimestamp(const std::chrono::system_clock::time_point& tp) {
    offTimestamp = tp;
}


void EnergyUsage::calculateEnergyConsumed(double ratePerHour) {
    using namespace std::chrono;
    auto duration = offTimestamp - onTimestamp;
    double hours = duration_cast<minutes>(duration).count() / 60.0;
    energyConsumed = hours * ratePerHour;
}

void EnergyUsage::calculateCost(double pricePerUnit) {
    cost = energyConsumed * pricePerUnit;
}

std::string EnergyUsage::getUsageId() const { return usageId; }
std::string EnergyUsage::getDeviceId() const { return deviceId; }
double EnergyUsage::getEnergyConsumed() const { return energyConsumed; }
double EnergyUsage::getCost() const { return cost; }
std::chrono::system_clock::time_point EnergyUsage::getOnTimestamp() const { return onTimestamp; }
std::chrono::system_clock::time_point EnergyUsage::getOffTimestamp() const { return offTimestamp; }

double EnergyUsage::totalEnergyUsed(const std::string& deviceId, const std::string& usageFile) {
    std::ifstream fin(usageFile);
    std::string line;
    double total = 0.0;
    bool isDevice = false;
    while (std::getline(fin, line)) {
        if (line.rfind("deviceId:", 0) == 0) {
            std::string dId = line.substr(9);
            dId.erase(0, dId.find_first_not_of(" \n\r\t"));
            isDevice = (dId == deviceId);
        }
        if (isDevice && line.rfind("energyConsumed:", 0) == 0) {
            double val = std::stod(line.substr(15));
            total += val;
            isDevice = false;
        }
    }
    return total;
}

double EnergyUsage::totalCost(const std::string& deviceId, const std::string& usageFile) {
    std::ifstream fin(usageFile);
    std::string line;
    double total = 0.0;
    bool isDevice = false;
    while (std::getline(fin, line)) {
        if (line.rfind("deviceId:", 0) == 0) {
            std::string dId = line.substr(9);
            dId.erase(0, dId.find_first_not_of(" \n\r\t"));
            isDevice = (dId == deviceId);
        }
        if (isDevice && line.rfind("cost:", 0) == 0) {
            double val = std::stod(line.substr(5));
            total += val;
            isDevice = false;
        }
    }
    return total;
}

static inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \n\r\t");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \n\r\t");
    return s.substr(start, end - start + 1);
}

inline std::chrono::system_clock::time_point parseTimestamp(const std::string& str) {
    if (str.empty())
        return std::chrono::system_clock::time_point{};
    std::tm tm = {};
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        return std::chrono::system_clock::time_point{};
    }
    std::time_t time_tt = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time_tt);
}

std::vector<EnergyUsage> EnergyUsage::getAllUsages(const std::string& usageFile) {
    std::ifstream fin(usageFile);
    std::string line;
    std::vector<EnergyUsage> usages;
    EnergyUsage usage;
    bool inBlock = false;
    while (std::getline(fin, line)) {
        if (line.rfind("usageId:", 0) == 0) {
            if (inBlock) usages.push_back(usage);
            usage = EnergyUsage();
            usage.usageId = trim(line.substr(8));
            inBlock = true;
        }
        else if (inBlock) {
            if (line.rfind("deviceId:", 0) == 0)
                usage.deviceId = trim(line.substr(9));
            else if (line.rfind("onTimestamp:", 0) == 0)
                usage.onTimestamp = parseTimestamp(trim(line.substr(12)));
            else if (line.rfind("offTimestamp:", 0) == 0)
                usage.offTimestamp = parseTimestamp(trim(line.substr(13)));
            else if (line.rfind("energyConsumed:", 0) == 0)
                usage.energyConsumed = std::stod(trim(line.substr(15)));
            else if (line.rfind("cost:", 0) == 0)
                usage.cost = std::stod(trim(line.substr(5)));
        }
    }
    if (inBlock) usages.push_back(usage);
    return usages;
}

std::string EnergyUsage::formatTime(const std::chrono::system_clock::time_point& tp) {
    if (tp == std::chrono::system_clock::time_point{}) return "";
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm local_tm;
    localtime_s(&local_tm, &tt);
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_tm);
    return std::string(buffer);
}
