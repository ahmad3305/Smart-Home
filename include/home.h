#ifndef HOME_H
#define HOME_H

#include <string>
#include <map>

class Room;

class Home {
    std::string homeId;
    std::string homeName;
    std::string ownerid;
    Room** rooms;  
    int roomCount;
    int roomCapacity;
public:
    Home();
    Home(const std::string& homeId, const std::string& homeName, const std::string& ownerId);
    ~Home();

    std::string getHomeId() const;
    std::string getHomeName() const;
	std::string getOwnerID() const;

    void addRoom(Room* room);
    void removeRoom(const std::string& roomId);
    Room* getRoom(const std::string& roomId) const;
    int getRoomCount() const;
    Room** getRooms() const;
    void clearRooms();

    static void loadHomesFromFile(const std::string& filename, std::map<std::string, Home*>& allHomes);
};

#endif