#ifndef ENERGYUSAGE_H
#define ENERGYUSAGE_H

#include <string>
#include <chrono>

class EnergyUsage {
public:
    std::string deviceId;
    std::string usageId;
    double energyRate;

    std::chrono::system_clock::time_point onTimestamp;
    std::chrono::system_clock::time_point offTimestamp;

    double energyConsumed;
    double cost;

public:
    EnergyUsage();
    EnergyUsage(const std::string& usageId, const std::string& deviceId);

    void setOnTimestamp();
    void setOffTimestamp();
    void setOnTimestamp(const std::chrono::system_clock::time_point& tp);
    void setOffTimestamp(const std::chrono::system_clock::time_point& tp);

    void calculateEnergyConsumed(double ratePerHour);
    void calculateCost(double pricePerUnit);

    std::string getUsageId() const;
    std::string getDeviceId() const;
    double getEnergyConsumed() const;
    double getCost() const;
    std::chrono::system_clock::time_point getOnTimestamp() const;
    std::chrono::system_clock::time_point getOffTimestamp() const;

    static std::string formatTime(const std::chrono::system_clock::time_point& tp);
    static double totalEnergyUsed(const std::string& deviceId, const std::string& usageFile = "data/energyUsages.txt");
    static double totalCost(const std::string& deviceId, const std::string& usageFile = "data/energyUsages.txt");
    static std::vector<EnergyUsage> getAllUsages(const std::string& usageFile);


};

#endif